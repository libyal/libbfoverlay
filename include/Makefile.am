include_HEADERS = \
	libbfoverlay.h

pkginclude_HEADERS = \
	libbfoverlay/codepage.h \
	libbfoverlay/definitions.h \
	libbfoverlay/error.h \
	libbfoverlay/extern.h \
	libbfoverlay/features.h \
	libbfoverlay/types.h

EXTRA_DIST = \
	libbfoverlay.h.in \
	libbfoverlay/definitions.h.in \
	libbfoverlay/features.h.in \
	libbfoverlay/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libbfoverlay.h
	-rm -f libbfoverlay/definitions.h
	-rm -f libbfoverlay/features.h
	-rm -f libbfoverlay/types.h
	-rm -f Makefile

