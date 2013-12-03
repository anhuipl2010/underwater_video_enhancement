@rem Enhancement and re-encoding settings

rem Output video frame rate
if [%fps%]==[] (
    set fps=25/1
)

rem Enhancement mode:
rem 0_0 - original (no changes)
rem 0_1 - weak filtering
rem 0_2 - strong filtering
rem 1_0 - color correction
rem 1_1 - color correction and weak filtering
rem 1_2 - color correction and strong filtering
if [%enhancement_mode%]==[] (
    set enhancement_mode=0_0
)

rem Deinterlacing mode:
rem 0 - auto (auto detection)
rem 1 - interlaced (force deinterlacing)
rem 2 - disabled (run in passthrough mode)
if [%deinterlacing_mode%]==[] (
    set deinterlacing_mode=2
)

rem GStremaer deinterlace plugin options
set deinterlace_options=mode=%deinterlacing_mode% method=greedyh

rem GStreamer x264enc plugin options
set x264enc_options=threads=4 bitrate=16000 bframes=0 pass=qual me=umh subme=6 dct8x8=false ref=4 aud=true key-int-max=25 noise-reduction=1000

set configure_script_dir=%~dp0

call %configure_script_dir%configure_enhancement.cmd
call %configure_script_dir%..\..\plugins\setenv.cmd
call %configure_script_dir%..\..\plugins\setpath.cmd
