#!/usr/bin/env python

# Usage (on Linux): export GST_PLUGIN_PATH=$(eval <this script> [path to be added in any form]...)

import sys
import os

envVarName = "GST_PLUGIN_PATH"

def main(argv = None):
	if argv is None:
		argv = sys.argv
	
	val = os.environ.get(envVarName)
	if not val:
		paths = []
	else:
		paths = val.split(os.pathsep)
	
	for p in argv[1:]:
		if not os.path.exists(p):
			continue
		if os.path.isfile(p):
			p = os.path.dirname(p)
		p = os.path.abspath(p)
		
		if not p in paths:
			paths.append(p)
	
	print os.pathsep.join(paths)
	return 0

if __name__ == "__main__":
	sys.exit(main())