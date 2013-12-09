@echo off

if defined GSTREAMER_SDK_ROOT_X86 (
    call set PATH=%GSTREAMER_SDK_ROOT_X86%bin;%%PATH:%GSTREAMER_SDK_ROOT_X86%bin;=%%
)
