@echo off
setlocal
rem All filesystem operations and validation live in one PowerShell script.
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0build_installer.ps1" %*
exit /b %errorlevel%
