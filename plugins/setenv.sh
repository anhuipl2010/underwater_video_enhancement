#!/bin/sh

# use "source" (or ".") command in order to setup environment variables for current shell

export GST_PLUGIN_PATH=$(eval ../scripts/pluginPathAdd.py \
gstFpsCount/bin/release \
gstMedianFilter/bin/release \
gstUnsharpMask/bin/release \
)

echo "GST_PLUGIN_PATH value:"
echo $GST_PLUGIN_PATH

