@echo off

set setenv_script_dir=%~dp0
set plugins_dir=%setenv_script_dir%bin
set pathadd=%setenv_script_dir%..\pathadd\bin\release\pathadd.exe

for /f %%i in ('%pathadd% GST_PLUGIN_PATH %plugins_dir%') do set GST_PLUGIN_PATH=%%i
