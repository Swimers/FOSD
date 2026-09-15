#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
.ui 布局改动后的静态安全检查。

本机没有 Qt，改不了就编译，所以用两道检查代替：

  1) 控件清单比对 —— 与 git HEAD 版本对比每个 .ui 里
     (class, name) 的集合。重排布局绝不应该让控件凭空消失，
     新增的装饰控件会被单独列出供人工确认。

  2) 自动连接槽比对 —— 扫描全部 C++ 源码里的 `ui->xxx` 与
     `on_xxx_<signal>()`，确认对应控件名在 .ui 中仍然存在。
     这条能抓住"改名后编译通过、但信号静默断开"的问题，
     那是编译期发现不了的。

用法:
    python tools/check_ui.py                # 检查全部 .ui
    python tools/check_ui.py <文件.ui> ...  # 只检查指定文件
"""

import io
import os
import re
import subprocess
import sys
import xml.etree.ElementTree as ET

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Qt 自动连接槽的后缀（on_<objectName>_<signal>）
SIGNALS = ("clicked", "toggled", "triggered", "currentIndexChanged",
           "currentTextChanged", "textChanged", "textEdited", "editingFinished",
           "stateChanged", "valueChanged", "activated", "returnPressed",
           "pressed", "released", "destroyed", "changed")
SIG_ALT = "|".join(SIGNALS)
AUTO_SLOT = re.compile(r'\bon_([A-Za-z_][A-Za-z0-9_]*)_(' + SIG_ALT + r')\s*\(')


def widgets_in(text):
    """返回 {name: class} —— 只取 name 非空的 widget。"""
    out = {}
    try:
        root = ET.fromstring(text)
    except ET.ParseError as e:
        raise RuntimeError("XML 解析失败: %s" % e)
    for w in root.iter('widget'):
        n = w.get('name')
        if n:
            out[n] = w.get('class')
    return out


def head_version(path):
    """取 git HEAD 里的版本；不在 HEAD 中（新文件）返回 None。"""
    rel = os.path.relpath(path, ROOT).replace(os.sep, '/')
    r = subprocess.run(['git', 'show', 'HEAD:%s' % rel],
                       cwd=ROOT, capture_output=True)
    if r.returncode != 0:
        return None
    return r.stdout.decode('utf-8', errors='ignore')


def cpp_index():
    """扫描全部 C++，返回 {控件名: [引用处...]} 与 {自动槽: [(控件名, 文件)]}"""
    ui_refs, auto = {}, {}
    for dirpath, _, files in os.walk(os.path.join(ROOT, 'src')):
        for f in files:
            if not f.endswith(('.cpp', '.h')):
                continue
            p = os.path.join(dirpath, f)
            t = io.open(p, encoding='utf-8', errors='ignore').read()
            for m in re.findall(r'ui->([A-Za-z_][A-Za-z0-9_]*)', t):
                ui_refs.setdefault(m, []).append(os.path.relpath(p, ROOT))
            for name, sig in AUTO_SLOT.findall(t):
                auto.setdefault(name, []).append(
                    "%s (on_%s_%s)" % (os.path.relpath(p, ROOT), name, sig))
    return ui_refs, auto


def main(argv):
    targets = argv[1:]
    if targets:
        uis = [os.path.abspath(t) if os.path.isabs(t) else os.path.join(ROOT, t)
               for t in targets]
    else:
        uis = []
        for dirpath, _, files in os.walk(ROOT):
            if os.sep + 'build' in dirpath or os.sep + 'lib' in dirpath:
                continue
            for f in files:
                if f.endswith('.ui'):
                    uis.append(os.path.join(dirpath, f))
        uis.sort()

    ui_refs, auto = cpp_index()
    total_bad = 0

    for p in uis:
        rel = os.path.relpath(p, ROOT)
        cur_text = io.open(p, encoding='utf-8', errors='ignore').read()
        try:
            cur = widgets_in(cur_text)
        except RuntimeError as e:
            print("[FAIL] %s  %s" % (rel, e))
            total_bad += 1
            continue

        print("=" * 78)
        print("%s   (%d 个控件)" % (rel, len(cur)))

        # ---- 1) 与 HEAD 比对 ----
        old_text = head_version(p)
        if old_text is None:
            print("  (git HEAD 中无此文件，跳过清单比对)")
        else:
            try:
                old = widgets_in(old_text)
            except RuntimeError:
                old = None
            if old is not None:
                gone = sorted(set(old) - set(cur))
                added = sorted(set(cur) - set(old))
                changed = sorted(n for n in set(old) & set(cur)
                                 if old[n] != cur[n])
                if gone:
                    print("  !! 消失的控件 (%d):" % len(gone))
                    for n in gone:
                        print("       - %-26s <%s>" % (n, old[n]))
                    total_bad += 1
                if changed:
                    print("  !! 类型改变 (%d):" % len(changed))
                    for n in changed:
                        print("       ~ %-26s <%s> -> <%s>"
                              % (n, old[n], cur[n]))
                if added:
                    print("  >> 新增控件 (%d) —— 请确认都是纯装饰:" % len(added))
                    for n in added:
                        print("       + %-26s <%s>" % (n, cur[n]))
                if not (gone or changed or added):
                    print("  OK  控件清单与 HEAD 完全一致")

        # ---- 2) C++ 引用完整性 ----
        missing_ui = sorted(n for n in ui_refs if n not in cur)
        # 只在"该文件曾经拥有这个名字"时才报，避免其它文件的控件误报
        if old_text:
            try:
                oldnames = set(widgets_in(old_text))
            except RuntimeError:
                oldnames = set()
            relevant = [n for n in missing_ui if n in oldnames]
            if relevant:
                print("  !! 被 ui-> 引用但已从本文件消失 (%d):" % len(relevant))
                for n in relevant:
                    print("       - %-26s 引用于 %s"
                          % (n, ", ".join(sorted(set(ui_refs[n])))))
                total_bad += 1

            rel_auto = [n for n in auto if n in oldnames and n not in cur]
            if rel_auto:
                print("  !! 自动连接槽对应控件消失 (%d):" % len(rel_auto))
                for n in rel_auto:
                    print("       - %-26s %s"
                          % (n, ", ".join(sorted(set(auto[n])))))
                total_bad += 1

    print("=" * 78)
    if total_bad:
        print("发现 %d 处问题" % total_bad)
    else:
        print("全部通过")
    return 1 if total_bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
