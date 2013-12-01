#!/bin/sh

# use "source" (or ".") command in order to set up environment variables for current shell

export GST_PLUGIN_PATH=$(eval ../scripts/pluginPathAdd.py bin)

echo "GST_PLUGIN_PATH value:"
echo $GST_PLUGIN_PATH

