@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: ============================================================
:: FOSD-Pro Installer Build Script
:: Uses Qt Installer Framework
:: ============================================================

:: ---- Configuration ----

set QT_DIR=D:\AppData\Qt\5.12.2\msvc2017_64
set IFW_DIR=D:\AppData\Qt\QtIFW-3.0.6\bin
set PROJECT_DIR=%~dp0..
set RELEASE_DIR=%PROJECT_DIR%\build\QMake\windows\bin\release
set INSTALLER_DIR=%~dp0
set OUTPUT_DIR=%INSTALLER_DIR%

:: VTK DLL search paths (checked in order, first match wins)
set VTK_DLL_SEARCH1=D:\VTK\VTK-release\bin
set VTK_DLL_SEARCH2=%PROJECT_DIR%\lib\VTK\VTK-release\bin
set VTK_DLL_SEARCH3=%PROJECT_DIR%\lib\VTK\VTK-release\lib

:: ---- Pre-flight checks ----
echo ========================================
echo   FOSD-Pro Installer Builder
echo ========================================
echo.

if not exist "%QT_DIR%\bin\windeployqt.exe" (
    echo [ERROR] windeployqt.exe not found
    echo Check QT_DIR: %QT_DIR%
    pause
    exit /b 1
)

if not exist "%IFW_DIR%\binarycreator.exe" (
    echo [ERROR] binarycreator.exe not found
    echo Check IFW_DIR: %IFW_DIR%
    pause
    exit /b 1
)

if not exist "%RELEASE_DIR%\FOSD-Pro.exe" (
    echo [ERROR] FOSD-Pro.exe not found
    echo Build Release first: %RELEASE_DIR%\FOSD-Pro.exe
    pause
    exit /b 1
)

echo [OK] Qt:      %QT_DIR%
echo [OK] IFW:     %IFW_DIR%
echo [OK] Release: %RELEASE_DIR%
echo.

:: ---- Prepare package directory ----
set PACK_DIR=%INSTALLER_DIR%\packages\com.fosd.pro\data
set BIN_DIR=%PACK_DIR%\bin
set APP_DATA_DIR=%PACK_DIR%\data

if exist "%PACK_DIR%" rmdir /s /q "%PACK_DIR%"
mkdir "%BIN_DIR%"
mkdir "%APP_DATA_DIR%"

:: ---- Step 1: Collect Qt DLLs into bin/ ----
echo [1/4] Collecting Qt runtime into bin\...

"%QT_DIR%\bin\windeployqt.exe" ^
    --release ^
    --no-translations ^
    --no-system-d3d-compiler ^
    --no-opengl-sw ^
    --dir "%BIN_DIR%" ^
    "%RELEASE_DIR%\FOSD-Pro.exe"

if errorlevel 1 (
    echo [ERROR] windeployqt failed
    pause
    exit /b 1
)
echo    Done

:: ---- Step 2: Copy VTK DLLs ----
echo [2/4] Searching for VTK DLLs...

set VTK_FOUND=0

:: Search in order
for %%D in ("%VTK_DLL_SEARCH1%" "%VTK_DLL_SEARCH2%" "%VTK_DLL_SEARCH3%") do (
    if exist "%%~D\*.dll" (
        if !VTK_FOUND!==0 (
            echo    Found VTK DLLs in: %%~D
            xcopy /Y "%%~D\*.dll" "%BIN_DIR%\" >nul
            set VTK_FOUND=1
        )
    )
)

if !VTK_FOUND!==0 (
    echo    [INFO] No VTK DLLs found - VTK is likely statically linked
    echo    Searched:
    echo      - %VTK_DLL_SEARCH1%
    echo      - %VTK_DLL_SEARCH2%
    echo      - %VTK_DLL_SEARCH3%
)

:: ---- Step 3: Copy exe, data, and VC++ runtime ----
echo [3/4] Copying application files...

:: Copy exe to bin/
copy /Y "%RELEASE_DIR%\FOSD-Pro.exe" "%BIN_DIR%\" >nul
echo    FOSD-Pro.exe

:: Copy app data (models, config, QSS, calibration, etc.)
if exist "%RELEASE_DIR%\data" (
    xcopy /E /I /Y "%RELEASE_DIR%\data\*" "%APP_DATA_DIR%\" >nul
    echo    data\
)

:: Copy VC++ runtime
if exist "%SystemRoot%\System32\vcruntime140.dll" (
    copy /Y "%SystemRoot%\System32\vcruntime140.dll" "%BIN_DIR%\" >nul 2>&1
    copy /Y "%SystemRoot%\System32\vcruntime140_1.dll" "%BIN_DIR%\" >nul 2>&1
    copy /Y "%SystemRoot%\System32\msvcp140.dll" "%BIN_DIR%\" >nul 2>&1
    echo    VC++ runtime
)

:: Clean up unnecessary files
del /q "%BIN_DIR%\FOSD-Pro.lib" 2>nul
del /q "%BIN_DIR%\FOSD-Pro.exp" 2>nul

echo    Done

:: ---- Step 4: Build installer ----
echo [4/4] Building installer...

:: Count files
set FILE_COUNT=0
for /r "%PACK_DIR%" %%f in (*) do set /a FILE_COUNT+=1
echo    Package: %FILE_COUNT% files

:: Count DLLs
set DLL_COUNT=0
for %%f in ("%BIN_DIR%\*.dll") do set /a DLL_COUNT+=1
echo    DLLs:    %DLL_COUNT%

if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

"%IFW_DIR%\binarycreator.exe" ^
    --offline-only ^
    -c "%INSTALLER_DIR%\config\config.xml" ^
    -p "%INSTALLER_DIR%\packages" ^
    "%OUTPUT_DIR%\FOSD-Pro-Setup.exe"

if errorlevel 1 (
    echo [ERROR] binarycreator failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo   BUILD SUCCESS
echo ========================================
echo.
echo   Output: %OUTPUT_DIR%\FOSD-Pro-Setup.exe
echo.

for %%A in ("%OUTPUT_DIR%\FOSD-Pro-Setup.exe") do (
    set SIZE=%%~zA
    set /a SIZE_MB=!SIZE! / 1048576
    echo   Size: !SIZE_MB! MB
)

echo.
echo   Installed layout:
echo     FOSD-Pro\
echo       bin\          (exe + all DLLs + plugins)
echo       data\         (models, config, QSS, etc.)
echo.
pause
