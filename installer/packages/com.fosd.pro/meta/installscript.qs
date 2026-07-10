// FOSD-Pro 安装脚本
// 负责创建快捷方式、注册环境变量等操作

function Component()
{
    // 构造函数，在安装程序加载组件时调用
    installer.gainAdminRights();
}

// 安装操作执行前
Component.prototype.beginInstallation = function()
{
    component.beginInstallation();
}

// 创建操作（快捷方式等）
Component.prototype.createOperations = function()
{
    // 调用基类的 createOperations
    component.createOperations();

    if (systemInfo.productType === "windows") {
        // ========== 桌面快捷方式 ==========
        component.addOperation("CreateShortcut",
            "@TargetDir@/FOSD-Pro.exe",                    // 目标程序
            "@DesktopDir@/FOSD-Pro.lnk",                   // 快捷方式路径
            "workingDirectory=@TargetDir@",                 // 工作目录
            "iconPath=@TargetDir@/FOSD-Pro.exe",            // 图标来源
            "iconId=0",                                     // 图标索引
            "description=FOSD 光纤传感数据采集与分析上位机"    // 描述
        );

        // ========== 开始菜单快捷方式 ==========
        component.addOperation("CreateShortcut",
            "@TargetDir@/FOSD-Pro.exe",
            "@StartMenuDir@/FOSD-Pro.lnk",
            "workingDirectory=@TargetDir@",
            "iconPath=@TargetDir@/FOSD-Pro.exe",
            "iconId=0",
            "description=FOSD 光纤传感数据采集与分析上位机"
        );

        // ========== 开始菜单 - 卸载快捷方式 ==========
        component.addOperation("CreateShortcut",
            "@TargetDir@/FOSD-Pro Maintenance Tool.exe",
            "@StartMenuDir@/卸载 FOSD-Pro.lnk",
            "workingDirectory=@TargetDir@",
            "iconPath=@TargetDir@/FOSD-Pro Maintenance Tool.exe",
            "iconId=0",
            "description=卸载 FOSD-Pro 及所有组件"
        );

        // ========== 开始菜单 - 数据目录快捷方式 ==========
        component.addOperation("CreateShortcut",
            "@TargetDir@/data",
            "@StartMenuDir@/数据目录.lnk",
            "workingDirectory=@TargetDir@",
            "description=打开 FOSD-Pro 数据目录"
        );
    }
}

// 卸载时清理快捷方式
Component.prototype.forgetClicked = function()
{
    // 用户点击"忘记"时的清理操作
}
