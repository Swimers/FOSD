#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
剔除 C/C++ 注释与字面量后检查括号是否配平 —— 在没有编译器的环境里做冒烟测试。
用法: python tools/check_balance.py <文件...>
      不给参数则检查仓库里改动频繁的一批源文件。
"""

import io
import os
import sys

BS, DQ, SQ = chr(92), chr(34), chr(39)

DEFAULT_FILES = [
    "src/main.cpp", "src/fosdplugin.cpp",
    "src/app/conncect/connectwindow.cpp",
    "src/app/mainwindow/oldmainwindow.cpp", "src/app/mainwindow/oldmainwindow.h",
    "src/app/mainwindow/rainbowmainwindow.cpp",
    "src/app/disp2d/disp2dwindow.cpp",
    "src/app/disp3d/disp3dwindow.cpp", "src/app/disp3d/disp3dkeypointedit.cpp",
    "src/app/disp3d/disp3dkeypointselect.cpp", "src/app/disp3d/trackingpointdatatable.cpp",
    "src/core/model/disp3dmodel.cpp", "src/core/model/disp3dmodel.h",
    "src/core/model/disp3dbackground.cpp", "src/core/model/disp3dinteractor.cpp",
    "src/core/model/trackingpoint.cpp", "lib/CustomUi/switchcontrol.cpp",
]


def strip(src):
    out, i, n = [], 0, len(src)
    while i < n:
        c = src[i]
        if c == '/' and i + 1 < n and src[i + 1] == '/':
            while i < n and src[i] != '\n':
                i += 1
        elif c == '/' and i + 1 < n and src[i + 1] == '*':
            i += 2
            while i + 1 < n and not (src[i] == '*' and src[i + 1] == '/'):
                i += 1
            i += 2
        elif c == DQ or c == SQ:
            q, i = c, i + 1
            while i < n:
                if src[i] == BS:
                    i += 2
                    continue
                if src[i] == q:
                    i += 1
                    break
                i += 1
        else:
            out.append(c)
            i += 1
    return ''.join(out)


def main(argv):
    files = argv[1:] or DEFAULT_FILES
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    bad = 0
    print("%-46s %5s %5s %5s %5s" % ("文件", "{", "}", "(", ")"))
    for f in files:
        p = f if os.path.isabs(f) else os.path.join(root, f)
        if not os.path.exists(p):
            print("SKIP %s (不存在)" % f)
            continue
        t = strip(io.open(p, encoding='utf-8', errors='ignore').read())
        b1, b2, p1, p2 = t.count('{'), t.count('}'), t.count('('), t.count(')')
        flag = "" if (b1 == b2 and p1 == p2) else "   <-- 不平衡"
        if flag:
            bad += 1
        print("%-46s %5d %5d %5d %5d%s" % (f, b1, b2, p1, p2, flag))
    print("\n不平衡文件数: %d" % bad)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
