@rem Enhance underwater video and encode to H.264

@echo off

set enhance_script_dir=%~dp0

call %enhance_script_dir%configure.cmd

set in_file=%~1
if "%in_file%"=="" (
    echo usage: %~nx0 input_file [output_file] 1>&2
    exit /b 1
)

set out_file=%~2
if "%out_file%"=="" (
    call:set_out_file "%~n1" "%~dp1" out_file
)

set temp_file=%out_file%.temp

@echo on
gst-launch-0.10 -e ^
filesrc location="%in_file:\=/%" ! ^
decodebin2 ! ^
videorate ! "video/x-raw-yuv, framerate=%fps%" ! ^
colortuning enabled=%colortuning_enabled% ^
cbexp=%colortuning_cbexp% crexp=%colortuning_crexp% ! ^
medfilt enabled=%medfilt_enabled% ^
radius1=%medfilt_radius1% radius2=%medfilt_radius2% radius3=%medfilt_radius3% ! ^
unsharpmask enabled=%unsharpmask_enabled% ^
radius1=%unsharpmask_radius1% amount1=%unsharpmask_amount1% radius2=%unsharpmask_radius2% ^
amount2=%unsharpmask_amount2% radius3=%unsharpmask_radius3% amount3=%unsharpmask_amount3% ! ^
deinterlace %deinterlace_options% ! ^
x264enc %x264enc_options% ! ^
mp4mux ! ^
filesink location="%temp_file:\=/%"
@echo off

if exist "%temp_file%" (
    for %%a in ("%temp_file%") do set temp_file_size=%%~za
    echo file size: %temp_file_size%
    
    if "%temp_file_size%" GTR "0" (
        move /Y "%temp_file%" "%out_file%"
        echo underwater video enhancement completed: from "%in_file%" to "%out_file%"
    ) else (
        rem del "%temp_file%"
        echo underwater video enhancement failed: "%in_file%" 1>&2
        exit /b 1
    )
) else (
    echo underwater video enhancement failed: "%in_file%" 1>&2
    exit /b 1
)

goto:eof

:set_out_file
    setlocal EnableDelayedExpansion
    set fn=%~1
    set dir=%~2

    if [%user_out_dir%]==[] (
        set out_dir=%dir%
    ) else (
        set out_dir=%user_out_dir%
    )

    if [%enhancement_mode%]==[0_0] (
        set out_file_suffix=
    ) else (
        set out_file_suffix=-enhanced-%enhancement_mode%
    )

    set ret_val=!out_dir!!out_file_prefix!!fn!!out_file_suffix!.mp4

    (
        endlocal
        set %~3=%ret_val%
    )
goto:eof
