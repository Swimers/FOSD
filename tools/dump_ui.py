#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""以缩进树的形式打印 .ui 文件里的控件与布局，附带网格位置和几何尺寸。

用法: python tools/dump_ui.py <file.ui> [深度上限]
"""

import sys
import xml.etree.ElementTree as ET


def geo_of(w):
    g = w.find('property[@name="geometry"]/rect')
    if g is None:
        return ""
    return " [%sx%s @%s,%s]" % (g.findtext('width'), g.findtext('height'),
                                g.findtext('x'), g.findtext('y'))


def text_of(w):
    s = w.find('property[@name="text"]/string')
    if s is not None and s.text:
        return ' "%s"' % s.text.replace('\n', ' ')[:18]
    return ""


def dump(node, depth=0, maxd=99):
    if depth > maxd:
        return
    for child in node:
        tag = child.tag
        if tag == 'item':
            r, c = child.get('row'), child.get('column')
            rs, cs = child.get('rowspan'), child.get('colspan')
            pos = "(%s,%s%s%s)" % (r, c, "r"+rs if rs else "", "c"+cs if cs else "")
            for sub in child:
                if sub.tag in ('widget', 'layout', 'spacer'):
                    emit(sub, pos, depth, maxd)
        elif tag in ('widget', 'layout', 'spacer'):
            emit(child, "", depth, maxd)


def emit(x, pos, depth, maxd):
    pad = "  " * depth
    if x.tag == 'widget':
        print("%s%-11s [W] %-24s <%s>%s%s"
              % (pad, pos, x.get('name'), x.get('class'), geo_of(x), text_of(x)))
        dump(x, depth + 1, maxd)
    elif x.tag == 'layout':
        print("%s%-11s {L} %s" % (pad, pos, x.get('class')))
        dump(x, depth + 1, maxd)
    elif x.tag == 'spacer':
        print("%s%-11s (spacer)" % (pad, pos))


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    path = argv[1]
    maxd = int(argv[2]) if len(argv) > 2 else 99
    root = ET.parse(path).getroot()
    w = root.find('widget')
    g = w.find('property[@name="geometry"]/rect')
    if g is not None:
        print("%s : %sx%s\n" % (path, g.findtext('width'), g.findtext('height')))
    print("%-11s %-28s %s" % ("网格位置", "控件", "类型 / 尺寸 / 文本"))
    print("-" * 96)
    dump(w, 0, maxd)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
