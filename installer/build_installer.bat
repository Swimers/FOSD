@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: ============================================================
:: FOSD-Pro Installer Build Script
:: Uses Qt Installer Framework
:: ============================================================

:: ---- Configuration (modify to match your environment) ----

set QT_DIR=D:\AppData\Qt\5.12.2\msvc2017_64
set IFW_DIR=D:\AppData\Qt\QtIFW-3.0.6\bin
set PROJECT_DIR=%~dp0..
set RELEASE_DIR=%PROJECT_DIR%\build\QMake\windows\bin\release
set VTK_DLL_DIR=%PROJECT_DIR%\lib\VTK\VTK-release\bin
set INSTALLER_DIR=%~dp0
set OUTPUT_DIR=%INSTALLER_DIR%

:: ---- Pre-flight checks ----
echo ========================================
echo   FOSD-Pro Installer Builder
echo ========================================
echo.

if not exist "%QT_DIR%\bin\windeployqt.exe" (
    echo [ERROR] windeployqt.exe not found
    echo Check QT_DIR in this script: %QT_DIR%
    pause
    exit /b 1
)

if not exist "%IFW_DIR%\binarycreator.exe" (
    echo [ERROR] binarycreator.exe not found
    echo Check IFW_DIR in this script: %IFW_DIR%
    pause
    exit /b 1
)

if not exist "%RELEASE_DIR%\FOSD-Pro.exe" (
    echo [ERROR] FOSD-Pro.exe not found
    echo Build Release first: %RELEASE_DIR%\FOSD-Pro.exe
    pause
    exit /b 1
)

echo [OK] Qt:     %QT_DIR%
echo [OK] IFW:    %IFW_DIR%
echo [OK] Release: %RELEASE_DIR%
echo.

:: ---- Step 1: Collect Qt dependencies ----
echo [1/4] Collecting Qt runtime...
cd /d "%RELEASE_DIR%"

set PACK_DIR=%INSTALLER_DIR%\packages\com.fosd.pro\data
if exist "%PACK_DIR%" rmdir /s /q "%PACK_DIR%"
mkdir "%PACK_DIR%"

"%QT_DIR%\bin\windeployqt.exe" --release --no-translations --no-system-d3d-compiler --no-opengl-sw "%RELEASE_DIR%\FOSD-Pro.exe"

if errorlevel 1 (
    echo [ERROR] windeployqt failed
    pause
    exit /b 1
)
echo    Done

:: ---- Step 2: Copy files to package ----
echo [2/4] Copying files...

xcopy /E /I /Y "%RELEASE_DIR%\*" "%PACK_DIR%\" >nul

if exist "%VTK_DLL_DIR%" (
    echo    Copying VTK DLLs...
    xcopy /E /I /Y "%VTK_DLL_DIR%\*.dll" "%PACK_DIR%\" >nul
) else (
    echo    [WARN] VTK DLL dir not found: %VTK_DLL_DIR%
    echo    If VTK is statically linked, ignore this warning.
    echo.
)

if exist "%SystemRoot%\System32\vcruntime140.dll" (
    echo    Copying VC++ runtime...
    copy /Y "%SystemRoot%\System32\vcruntime140.dll" "%PACK_DIR%\" >nul 2>&1
    copy /Y "%SystemRoot%\System32\vcruntime140_1.dll" "%PACK_DIR%\" >nul 2>&1
    copy /Y "%SystemRoot%\System32\msvcp140.dll" "%PACK_DIR%\" >nul 2>&1
)

if exist "%PACK_DIR%\FOSD-Pro.lib" del /q "%PACK_DIR%\FOSD-Pro.lib"
if exist "%PACK_DIR%\FOSD-Pro.exp" del /q "%PACK_DIR%\FOSD-Pro.exp"

echo    Done

:: ---- Step 3: Count files ----
echo [3/4] Counting files...
set FILE_COUNT=0
for /r "%PACK_DIR%" %%f in (*) do set /a FILE_COUNT+=1
echo    Total: %FILE_COUNT% files

:: ---- Step 4: Build installer ----
echo [4/4] Building installer...
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
pause
