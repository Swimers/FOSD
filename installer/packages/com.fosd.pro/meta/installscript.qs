// FOSD-Pro Installer Script
// Creates shortcuts and registers file associations

function Component()
{
    installer.gainAdminRights();
}

Component.prototype.beginInstallation = function()
{
    component.beginInstallation();
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {

        // Desktop shortcut -> bin\FOSD-Pro.exe
        component.addOperation("CreateShortcut",
            "@TargetDir@/bin/FOSD-Pro.exe",
            "@DesktopDir@/FOSD-Pro.lnk",
            "workingDirectory=@TargetDir@/bin",
            "iconPath=@TargetDir@/bin/FOSD-Pro.exe",
            "iconId=0",
            "description=FOSD Fiber Sensing Host Software"
        );

        // Start Menu shortcut
        component.addOperation("CreateShortcut",
            "@TargetDir@/bin/FOSD-Pro.exe",
            "@StartMenuDir@/FOSD-Pro.lnk",
            "workingDirectory=@TargetDir@/bin",
            "iconPath=@TargetDir@/bin/FOSD-Pro.exe",
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
