#!/bin/sh
# plays MJPEG (Motion JPEG) video files

file=$1

fps=$2
: ${fps:=25}

if (test -n "$3") && (test -n "$4"); then
	scale="video/x-raw-yuv,width=$3,height=$4"
else
	scale="video/x-raw-yuv"
fi

gst-launch filesrc location="$file" ! \
multipartdemux ! jpegdec ! \
queue ! \
videorate ! "video/x-raw-yuv, framerate=(fraction)$fps" ! \
queue ! \
videoscale ! $scale ! \
xvimagesink
