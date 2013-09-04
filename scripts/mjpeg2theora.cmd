@echo off

set inFile=%1
if [%inFile%]==[] (
	echo usage: %~nx0 input_file [output_file] [fps] 1>&2
	exit /B 1
)

set outFile=%2
if [%outFile%]==[] (
	set outFile=%~dp1%~n1.ogg
)

set fps=%3
if [%fps%]==[] (
	set fps=25
)

set tempFile=%outFile%.temp

gst-launch -e filesrc location=%inFile:\=/% ! ^
multipartdemux ! ^
jpegdec ! ^
videorate ! "video/x-raw-yuv, framerate=(fraction)%fps%" ! ^
theoraenc ! ^
oggmux ! ^
filesink location="%tempFile:\=/%"

move /Y "%tempFile%" "%outFile%"
echo conversion from %inFile% to %outFile% is done
