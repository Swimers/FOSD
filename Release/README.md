# 发行文件与数据边界

本仓库以公开源码交流为目的，不再跟踪历史 FOSD 主程序 ZIP 或对应的校验文件。本地发行文件仍保留；历史包已退出 Git 跟踪，不应重新强制添加。

发布新二进制前，需要单独审阅模型、点阵、标定、采集记录、运行配置、调试符号及内嵌资源。源码路径检查通过不代表二进制已经完成保密审查。

保留的 `MPWavelengthSimulator-win64-20261105.zip` 是合成波长发包工具；本次检查未发现其包含识别出的模型、点阵数据。源码位于 `tools/mp-wavelength-simulator`。

构建与预设见 [构建说明](../doc/build-presets.md)，公开前检查及历史重写后的协作说明见 [私有数据边界](../doc/Architectural/private-data-boundary.md)。
