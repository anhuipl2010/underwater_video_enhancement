#!/bin/sh

pkgconfigdir=$1
: ${pkgconfigdir:=/usr/local/lib/pkgconfig}

TEMPLATE_FILE=gstPluginWrap.pc.template
OUT_FILE=${pkgconfigdir}/gstPluginWrap.pc

VERSION=`head -1 VERSION`
CUR_DIR=`pwd`

echo "$(eval "echo \"$(cat $TEMPLATE_FILE)\"")" > $OUT_FILE
echo "$OUT_FILE has been created, now you can run \"pkg-config --cflags gstPluginWrap\" and \"pkg-config --libs gstPluginWrap\""
