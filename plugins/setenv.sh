#!/bin/bash

# use "source" (or ".") command in order to set up 
# environment variable for current shell

script=$(readlink -f $BASH_SOURCE)
script_dir=$(dirname $script)
pathadd=$script_dir/../pathadd/bin/release/pathadd


export GST_PLUGIN_PATH=$(eval $pathadd \
GST_PLUGIN_PATH \
$script_dir/bin \
)

echo "GST_PLUGIN_PATH="$GST_PLUGIN_PATH

