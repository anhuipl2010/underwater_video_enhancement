#!/bin/sh

if [ "$#" -lt 1 ]; then
  echo "usage: `basename $0` input_file [output_file fps]" >&2
  exit 1
fi

inFile=$1

if ! [ -f "$inFile" ]; then
  echo "\"$inFile\" is not a file" >&2
  exit 1
fi

outFile=$2
: ${outFile:=`dirname "$inFile"`/`basename "${inFile%.*}"`.ogg}

fps=$3
: ${fps:=25}

tempFile=$outFile.temp

gst-launch -e filesrc location="$inFile" ! \
multipartdemux ! \
jpegdec ! \
videorate ! "video/x-raw-yuv, framerate=(fraction)$fps" ! \
theoraenc ! \
oggmux ! \
filesink location="$tempFile"

mv "$tempFile" "$outFile"
echo "`basename "$inFile"` to `basename "$outFile"` conversion complete"
