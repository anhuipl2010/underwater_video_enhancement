@echo off

set script_dir=%~dp0
set plugins_dir=%script_dir%bin

if ["%GST_PLUGIN_PATH%"]==[""] (
    set GST_PLUGIN_PATH=%plugins_dir%
) else (
    call set GST_PLUGIN_PATH=%%GST_PLUGIN_PATH:%plugins_dir%=%%
)

echo GST_PLUGIN_PATH: %GST_PLUGIN_PATH%
