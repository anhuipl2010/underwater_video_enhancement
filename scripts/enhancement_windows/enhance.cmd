@rem Enhance underwater video and encode to H.264

@echo off
setlocal EnableDelayedExpansion

set enhance_script_dir=%~dp0

call %enhance_script_dir%configure.cmd

set in_file=%1
if [%in_file%]==[] (
    echo usage: %~nx0 input_file [output_file] 1>&2
    exit /B 1
)

set out_file=%2
if [%out_file%]==[] (
    if [%user_out_dir%]==[] (
        rem directory of the input file by default
        set out_dir=%~dp1
    ) else (
        set out_dir=!user_out_dir!
    )

    if [%enhancement_mode%]==[0_0] (
        set out_file_suffix=
    ) else (
        set out_file_suffix=-enhanced-!enhancement_mode!
    )

    set out_file=!out_dir!!out_file_prefix!%~n1!out_file_suffix!.mp4
)

set temp_file=%out_file%.temp

@echo on
gst-launch-0.10 -e filesrc location=%in_file:\=/% ! ^
decodebin2 ! ^
videorate ! "video/x-raw-yuv, framerate=%fps%" ! ^
colortuning enabled=%colortuning_enabled% cbexp=%colortuning_cbexp% crexp=%colortuning_crexp% ! ^
deinterlace %deinterlace_options% ! ^
x264enc %x264enc_options% ! ^
mp4mux ! ^
filesink location="%temp_file:\=/%"
@echo off

if %ERRORLEVEL% neq 0 (
	exit /B 1
)

move /Y "%temp_file%" "%out_file%"
echo underwater video enhancement from %in_file% to %out_file% is done
