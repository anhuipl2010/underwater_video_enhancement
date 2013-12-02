@echo off

set script_dir=%~dp0
set plugins_dir=%script_dir%bin
set pathadd=%script_dir%..\pathadd\bin\release\pathadd.exe

for /f %%i in ('%pathadd% GST_PLUGIN_PATH %plugins_dir%') do set GST_PLUGIN_PATH=%%i

set GST_PLUGIN_PATH
