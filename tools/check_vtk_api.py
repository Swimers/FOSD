#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
校验源码中调用的 VTK 方法在其头文件里确实存在。

VTK 的方法大多由宏生成，宏里写的是**属性名**而不是方法名：
    vtkSetMacro(Color, double)          -> SetColor / GetColor
    vtkSetVector3Macro(Color, double)   -> SetColor(r,g,b) / SetColor(double[3])
    vtkGetObjectMacro(XAxisShaftProperty, vtkProperty) -> GetXAxisShaftProperty
    vtkBooleanMacro(EdgeVisibility, vtkTypeBool)       -> SetEdgeVisibility / EdgeVisibilityOn / EdgeVisibilityOff
所以不能直接搜方法名，要先把 Set/Get/On/Off/Is 前缀剥掉得到属性名再匹配。
"""

import io
import os
import re
import sys

VTK_INC = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                       "..", "lib", "VTK", "VTK-release", "include", "vtk-8.2")

# 宏名形如 vtkSetMacro / vtkSetClampMacro / vtkSetVector2Macro / vtkGetObjectMacro ...
MACRO_RE = r'vtk\w*Macro\(\s*%s\s*[,)]'


def load(header):
    p = os.path.join(VTK_INC, header)
    if not os.path.exists(p):
        return None
    return io.open(p, encoding='utf-8', errors='ignore').read()


def prop_names(method):
    """由方法名反推它可能对应的属性名。"""
    names = [method]
    for pre in ("Set", "Get", "Is"):
        if method.startswith(pre) and len(method) > len(pre):
            names.append(method[len(pre):])
            break
    for suf in ("Off", "On"):
        if method.endswith(suf) and len(method) > len(suf):
            names.append(method[:-len(suf)])
            break
    return names


def exists(header, method):
    t = load(header)
    if t is None:
        return None, "头文件不存在"
    # 1) 显式声明的成员函数
    if re.search(r'\b' + re.escape(method) + r'\s*\(', t):
        return True, "显式声明"
    # 2) 宏生成的访问器
    for n in prop_names(method):
        if re.search(MACRO_RE % re.escape(n), t):
            return True, "宏生成(%s)" % n
    return False, "未找到"


# (头文件, [方法名...])  —— 仅列出本次改造新增/改动的调用
CHECKS = [
    ("vtkProperty.h", ["SetColor", "SetAmbient", "SetOpacity", "SetLineWidth",
                       "SetEdgeColor", "SetEdgeVisibility", "LightingOff"]),
    ("vtkTextProperty.h", ["SetColor", "SetFontSize", "SetFontFamilyToCourier",
                           "SetBold", "ShadowOff", "SetOpacity", "SetJustificationToLeft"]),
    ("vtkAxesActor.h", ["GetXAxisShaftProperty", "GetYAxisShaftProperty", "GetZAxisShaftProperty",
                        "GetXAxisTipProperty", "GetYAxisTipProperty", "GetZAxisTipProperty",
                        "GetXAxisCaptionActor2D"]),
    ("vtkLookupTable.h", ["SetNumberOfColors", "SetHueRange", "Build"]),
    ("vtkActor.h", ["GetProperty", "SetMapper", "SetPosition", "SetScale"]),
    ("vtkSphereSource.h", ["SetRadius", "SetPhiResolution", "SetThetaResolution"]),
    ("vtkCamera.h", ["SetFocalPoint", "SetPosition"]),
]


def main():
    if not os.path.isdir(VTK_INC):
        print("找不到 VTK 头文件目录: %s" % VTK_INC)
        return 2
    bad = 0
    for header, methods in CHECKS:
        for m in methods:
            ok, how = exists(header, m)
            if ok is None:
                print("SKIP %-22s %-28s %s" % (header, m, how))
                continue
            if not ok:
                bad += 1
            print("%-4s %-22s %-28s %s" % ("OK" if ok else "FAIL", header, m, how))
    print("\n不存在的调用: %d" % bad)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
