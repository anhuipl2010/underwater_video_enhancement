@echo off

set target=%~1

set pattern=%2
if [%pattern%] == [] (
    set pattern="*"
)

set enhancement_mode=%3
if [%enhancement_mode%] == [] (
    set enhancement_mode=0_0
)

set deinterlacing_mode=%4
if [%deinterlacing_mode%] == [] (
    set deinterlacing_mode=2
)

set user_out_dir=
set out_file_prefix=

set run_script_dir=%~dp0

call %run_script_dir%..\plugins\setpath.cmd
call %run_script_dir%..\plugins\setenv.cmd

call set PATH=%run_script_dir%bin;%%PATH:%run_script_dir%bin;=%%

set batchrun=%run_script_dir%..\batchrun\bin\release\batchrun.exe

call %batchrun% -l ^
-s %run_script_dir%enhancement_4_windows\enhance.cmd ^
-p %pattern% "%target%"
