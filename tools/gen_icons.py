#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
FOSD-Pro 显控台图标集生成脚本
================================================================================
用途
    按统一的「线性轮廓（outline）」风格生成界面图标，输出到 res/icon/。

为什么是 PNG 而不是 SVG
    项目未链接 Qt5Svg（FOSD-Pro.pro / FOSD-plugin.pro / CMakeLists.txt 三处
    构建配置都没有 svg 组件），引入它需要改构建并部署 Qt5Svg.dll 与 qsvg
    插件。本脚本用 Pillow 离线栅格化，零新增依赖、无需改动构建配置。

关键约束：线宽必须按目标显示尺寸反算
    源图是 200×200，但界面上的 iconSize 多为 24×24。
    若在 200 画布上按 2px 画线，缩到 24px 后线条只剩 0.24px，肉眼几乎不可见。
    因此线宽 = 目标线宽 × (画布尺寸 ÷ 显示尺寸)。
       24px 显示、2px 线宽  →  200 画布上画 2 × 200/24 ≈ 17px
       10px 显示、2px 线宽  →   64 画布上画 2 ×  64/10 ≈ 13px

用法
    python tools/gen_icons.py            # 生成全部图标
    python tools/gen_icons.py --preview  # 额外导出 24×24 预览图到 preview_24/
================================================================================
"""

import argparse
import math
import os
import sys

from PIL import Image, ImageDraw

# --------------------------------------------------------------------------
# 调色板 —— 与 data/QSS/ConsoleDark.qss 保持同源
# --------------------------------------------------------------------------
COLOR_LIGHT = (0xD6, 0xE4, 0xF0, 255)   # text-main  深底上的常规描边
COLOR_DARK  = (0x07, 0x0B, 0x12, 255)   # bg-base    冰青填充上的描边（选中态变体）
COLOR_DIM   = (0x6B, 0x82, 0x99, 255)   # text-dim   禁用态
COLOR_ACCENT= (0x00, 0xD4, 0xFF, 255)   # accent     冰青

# 设计空间：所有图标在 24×24 的坐标系里定义，再缩放到目标画布
VIEWBOX = 24.0
STROKE_UNITS = 2.0          # 24×24 下的线宽，即 24px 显示时的 2px
SUPERSAMPLE = 4             # 超采样倍率，用降采样换取抗锯齿

ICON_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "res", "icon")


# --------------------------------------------------------------------------
# 绘图基元
# --------------------------------------------------------------------------
class Canvas:
    """在 24×24 设计坐标系里作画，内部按超采样倍率放大绘制。"""

    def __init__(self, size, color):
        self.size = size
        self.color = color
        self.ss = SUPERSAMPLE
        self.k = size / VIEWBOX          # 设计空间 → 画布像素
        self.img = Image.new("RGBA", (size * self.ss, size * self.ss), (0, 0, 0, 0))
        self.d = ImageDraw.Draw(self.img)
        self.w = STROKE_UNITS * self.k * self.ss   # 线宽（超采样空间）

    def _p(self, pt):
        return (pt[0] * self.k * self.ss, pt[1] * self.k * self.ss)

    def _r(self, v):
        return v * self.k * self.ss

    def _cap(self, pt, r):
        """补一个圆点，模拟 round line cap。"""
        x, y = self._p(pt)
        self.d.ellipse([x - r, y - r, x + r, y + r], fill=self.color)

    def polyline(self, pts, closed=False):
        seq = list(pts) + ([pts[0]] if closed else [])
        xy = [self._p(p) for p in seq]
        self.d.line(xy, fill=self.color, width=int(round(self.w)), joint="curve")
        r = self.w / 2.0
        for p in seq:
            self._cap(p, r)

    def circle(self, cx, cy, r):
        x, y = self._p((cx, cy))
        rr = self._r(r)
        w = int(round(self.w))
        self.d.ellipse([x - rr, y - rr, x + rr, y + rr], outline=self.color, width=w)

    def arc(self, cx, cy, r, start, end):
        """start/end 为角度（0°=3 点钟方向，顺时针）。"""
        x, y = self._p((cx, cy))
        rr = self._r(r)
        w = int(round(self.w))
        self.d.arc([x - rr, y - rr, x + rr, y + rr], start, end, fill=self.color, width=w)
        # 端点补圆，避免与折线衔接处出现缺口
        for a in (start, end):
            rad = math.radians(a)
            self._cap((cx + r * math.cos(rad), cy + r * math.sin(rad)), w / 2.0)

    def dot(self, cx, cy, r):
        x, y = self._p((cx, cy))
        rr = self._r(r)
        self.d.ellipse([x - rr, y - rr, x + rr, y + rr], fill=self.color)

    def finish(self):
        out = self.img.resize((self.size, self.size), Image.LANCZOS)
        return out


# --------------------------------------------------------------------------
# 图标定义（24×24 设计坐标系）
# --------------------------------------------------------------------------
def ic_console(c):
    """调解仪 —— 波形"""
    c.polyline([(2, 12), (6, 12), (9, 5), (13, 19), (16, 12), (22, 12)])


def ic_chart2d(c):
    """二维验证 —— 内含柱状图的方框"""
    c.polyline([(3, 3), (21, 3), (21, 21), (3, 21)], closed=True)
    c.polyline([(7.5, 17), (7.5, 12)])
    c.polyline([(12, 17), (12, 8)])
    c.polyline([(16.5, 17), (16.5, 14.5)])


def ic_cube3d(c):
    """变形场 —— 等轴测立方体"""
    c.polyline([(12, 2.6), (21, 7.3), (21, 16.7), (12, 21.4), (3, 16.7), (3, 7.3)], closed=True)
    c.polyline([(3, 7.3), (12, 12)])
    c.polyline([(21, 7.3), (12, 12)])
    c.polyline([(12, 12), (12, 21.4)])


def ic_power(c):
    """退出系统 —— 电源符号"""
    c.arc(12, 12, 8, 300, 240)
    c.polyline([(12, 3), (12, 12)])


def ic_play(c):
    """启动"""
    c.polyline([(7, 4.5), (19, 12), (7, 19.5)], closed=True)


def ic_stop(c):
    """停止"""
    c.polyline([(6, 6), (18, 6), (18, 18), (6, 18)], closed=True)


def ic_pause(c):
    """暂停"""
    c.polyline([(9, 5), (9, 19)])
    c.polyline([(15, 5), (15, 19)])


def ic_reset(c):
    """复位 —— 循环箭头"""
    c.arc(12, 12, 8, 320, 250)
    # 箭头：在 320° 端点处（右上）画一个人字形
    rad = math.radians(320)
    px, py = 12 + 8 * math.cos(rad), 12 + 8 * math.sin(rad)
    c.polyline([(px - 1.0, py + 4.2), (px, py), (px + 4.4, py + 1.2)])


def ic_model(c):
    """模型 —— 带节点标记的立方体"""
    c.polyline([(12, 2.6), (21, 7.3), (21, 16.7), (12, 21.4), (3, 16.7), (3, 7.3)], closed=True)
    c.polyline([(3, 7.3), (12, 12)])
    c.polyline([(21, 7.3), (12, 12)])
    c.circle(12, 12, 1.7)


def ic_help(c):
    """帮助 —— 问号"""
    c.circle(12, 12, 9)
    c.arc(12, 10, 2.9, 180, 400)
    c.polyline([(12, 12.9), (12, 15)])
    c.dot(12, 17.4, 1.15)


def ic_win_min(c):
    """最小化"""
    c.polyline([(5, 12), (19, 12)])


def ic_win_max(c):
    """最大化"""
    c.polyline([(5, 5), (19, 5), (19, 19), (5, 19)], closed=True)


def ic_win_close(c):
    """关闭"""
    c.polyline([(6, 6), (18, 18)])
    c.polyline([(18, 6), (6, 18)])


def ic_arrow_down(c):
    c.polyline([(5, 9), (12, 16), (19, 9)])


def ic_arrow_up(c):
    c.polyline([(5, 15), (12, 8), (19, 15)])


def ic_check(c):
    c.polyline([(5, 12.5), (10, 17.5), (19, 7)])


# --------------------------------------------------------------------------
# 输出清单
#   (函数,   输出文件名,        画布尺寸, 描边色,       显示尺寸)
# --------------------------------------------------------------------------
ICONS = [
    # 左侧导航（各配一份选中态深色变体，用于冰青填充底）
    (ic_console,   "Menu.png",          200, COLOR_LIGHT, 24),
    (ic_console,   "Menu_on.png",       200, COLOR_DARK,  24),
    (ic_chart2d,   "2D_Plot.png",       200, COLOR_LIGHT, 24),
    (ic_chart2d,   "2D_Plot_on.png",    200, COLOR_DARK,  24),
    (ic_cube3d,    "3D_Plot.png",       200, COLOR_LIGHT, 24),
    (ic_cube3d,    "3D_Plot_on.png",    200, COLOR_DARK,  24),
    (ic_power,     "TurnOff.png",       200, COLOR_LIGHT, 24),
    (ic_power,     "TurnOff_on.png",    200, COLOR_DARK,  24),

    # disp3dwindow 工具栏
    (ic_play,      "Play_icon.png",     200, COLOR_LIGHT, 24),
    (ic_stop,      "Stop_icon.png",     200, COLOR_LIGHT, 24),
    (ic_pause,     "Pause_icon.png",    200, COLOR_LIGHT, 24),
    (ic_reset,     "Reset_icon.png",    200, COLOR_LIGHT, 24),
    (ic_model,     "Model_icon.png",    200, COLOR_LIGHT, 24),
    (ic_help,      "Help_icon.png",     200, COLOR_LIGHT, 24),

    # 窗口控制（替换原先用文字 - 口 X 冒充的方案）
    (ic_win_min,   "Win_Min.png",       200, COLOR_LIGHT, 14),
    (ic_win_max,   "Win_Max.png",       200, COLOR_LIGHT, 14),
    (ic_win_close, "Win_Close.png",     200, COLOR_LIGHT, 14),

    # 小尺寸 UI 元件（由 ConsoleDark.qss 通过 qrc 引用）
    (ic_arrow_down,"arrow_down.png",     64, COLOR_LIGHT, 10),
    (ic_arrow_down,"arrow_down_dim.png", 64, COLOR_DIM,   10),
    (ic_arrow_up,  "arrow_up.png",       64, COLOR_LIGHT, 10),
    (ic_check,     "check.png",          64, COLOR_DARK,  10),
]

# 保留但不再被引用的旧图标（生成后会提示可清理）
STALE = ["Play_icon1.png", "2D_Plot2.png", "UserIcon.png", "UserManage.png",
         "Pasue_icon.png"]


def render(fn, size, color):
    c = Canvas(size, color)
    fn(c)
    return c.finish()


def opaque_ratio(img):
    """非全透明像素占比，用于断言图标没画空也没画满。"""
    alpha = img.getchannel("A")
    hist = alpha.histogram()
    total = img.size[0] * img.size[1]
    return sum(hist[8:]) / float(total)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--preview", action="store_true",
                    help="额外导出 24×24 预览图到 preview_24/ 以便目视自检")
    args = ap.parse_args()

    out_dir = os.path.normpath(ICON_DIR)
    os.makedirs(out_dir, exist_ok=True)

    preview_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "preview_24")
    if args.preview:
        os.makedirs(preview_dir, exist_ok=True)

    print("输出目录: %s\n" % out_dir)
    print("%-22s %-12s %-8s %s" % ("文件", "画布", "描边色", "不透明像素占比"))
    print("-" * 68)

    failures = []
    for fn, name, size, color, disp in ICONS:
        img = render(fn, size, color)
        path = os.path.join(out_dir, name)
        img.save(path, "PNG")

        # ---- 自检断言 ----
        assert img.size == (size, size), "%s 尺寸异常: %s" % (name, img.size)
        assert img.mode == "RGBA", "%s 颜色模式异常: %s" % (name, img.mode)
        ratio = opaque_ratio(img)
        if not (0.005 < ratio < 0.60):
            failures.append((name, ratio))

        tag = "#%02X%02X%02X" % color[:3]
        print("%-22s %-12s %-8s %.3f%%" % (name, "%d×%d" % (size, size), tag, ratio * 100))

        if args.preview and size == 200:
            small = img.resize((disp, disp), Image.LANCZOS)
            small.save(os.path.join(preview_dir, name.replace(".png", "_%d.png" % disp)), "PNG")

    print("-" * 68)
    print("共生成 %d 个图标。" % len(ICONS))

    if failures:
        print("\n[警告] 以下图标不透明像素占比超出预期区间，可能画空或画满：")
        for n, r in failures:
            print("    %s  %.4f%%" % (n, r * 100))
        return 1

    print("\n以下旧图标已不再被引用，确认后可删除：")
    for s in STALE:
        p = os.path.join(out_dir, s)
        if os.path.exists(p):
            print("    res/icon/%s" % s)

    if args.preview:
        print("\n预览图已导出到: %s" % preview_dir)
    return 0


if __name__ == "__main__":
    sys.exit(main())
