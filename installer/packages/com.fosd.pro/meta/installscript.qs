// FOSD-Pro Installer Script
// Creates per-user shortcuts; executable and data directory are siblings.

function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows" && installer.value("SkipShortcuts") !== "true") {

        // Desktop shortcut
        component.addOperation("CreateShortcut",
            "@TargetDir@/FOSD-Pro.exe",
            "@DesktopDir@/FOSD-Pro.lnk",
            "workingDirectory=@TargetDir@",
            "iconPath=@TargetDir@/FOSD-Pro.exe",
            "iconId=0",
            "description=FOSD Fiber Sensing Host Software"
        );

        // Start Menu shortcut
        component.addOperation("CreateShortcut",
            "@TargetDir@/FOSD-Pro.exe",
            "@StartMenuDir@/FOSD-Pro.lnk",
            "workingDirectory=@TargetDir@",
            "iconPath=@TargetDir@/FOSD-Pro.exe",
            "iconId=0",
            "description=FOSD Fiber Sensing Host Software"
        );

        // Start Menu -> Uninstall
        component.addOperation("CreateShortcut",
            "@TargetDir@/FOSD-Pro Maintenance Tool.exe",
            "@StartMenuDir@/Uninstall FOSD-Pro.lnk",
            "workingDirectory=@TargetDir@",
            "description=Uninstall FOSD-Pro"
        );

        // Start Menu -> Data directory
        component.addOperation("CreateShortcut",
            "@TargetDir@/data",
            "@StartMenuDir@/Data Directory.lnk",
            "workingDirectory=@TargetDir@",
            "description=Open FOSD-Pro data folder"
        );
    }
}
