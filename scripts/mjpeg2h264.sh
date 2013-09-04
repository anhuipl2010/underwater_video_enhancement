#!/bin/sh
# converts video format from MJPEG to H264

if [ "$#" -lt 1 ]; then
  echo "usage: `basename $0` input_file [output_file] [fps]" >&2
  exit 1
fi

inFile=$1

if ! [ -f "$inFile" ]; then
  echo "\"$inFile\" is not a file" >&2
  exit 1
fi

outFile=$2
: ${outFile:=`dirname "$inFile"`/`basename "${inFile%.*}"`.mp4}

fps=$3
: ${fps:=25}

tempFile=$outFile.temp

gst-launch -e filesrc location="$inFile" ! \
multipartdemux ! \
jpegdec ! \
videorate ! "video/x-raw-yuv, framerate=(fraction)$fps" ! \
x264enc pass=5 quantizer=22 speed-preset=4 profile=1 ! \
mp4mux ! \
filesink location="$tempFile"

mv "$tempFile" "$outFile"
echo "`basename "$inFile"` to `basename "$outFile"` conversion complete"

