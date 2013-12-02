# Check errors only

HEADERS = $(wildcard *.h)

INCLUDES += `pkg-config --cflags gstreamer-0.10`

CPPFLAGS = -DGST_PLUGIN_TYPE=Fake

all:

claen:

check-syntax:
	g++ -fsyntax-only $(CPPFLAGS) $(INCLUDES) $(HEADERS)

install:
	./make_pkgconfig.sh

.PHONY: all clean check-syntax install
