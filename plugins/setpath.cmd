@echo off

set gst_bin_dir=%GSTREAMER_SDK_ROOT_X86%bin
call set PATH=%%PATH:%gst_bin_dir%;=%%
set PATH=%gst_bin_dir%;%PATH%
