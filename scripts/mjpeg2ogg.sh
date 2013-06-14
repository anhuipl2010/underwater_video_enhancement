#!/bin/sh

inFile=$1

outFile=$2
: ${outFile:=`dirname "$inFile"`/`basename "$inFile"`.ogg}

fps=$3
: ${fps:=25}

gst-launch -e filesrc location="$inFile" ! \
multipartdemux ! \
jpegdec ! \
videorate ! "video/x-raw-yuv, framerate=(fraction)$fps" ! \
theoraenc ! \
oggmux ! \
filesink location="$outFile"
