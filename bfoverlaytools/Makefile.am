AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBBFOVERLAY_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	bfoverlaymount

bfoverlaymount_SOURCES = \
	bfoverlaymount.c \
	bfoverlaytools_getopt.c bfoverlaytools_getopt.h \
	bfoverlaytools_i18n.h \
	bfoverlaytools_libbfio.h \
	bfoverlaytools_libcdata.h \
	bfoverlaytools_libcerror.h \
	bfoverlaytools_libclocale.h \
	bfoverlaytools_libcnotify.h \
	bfoverlaytools_libcpath.h \
	bfoverlaytools_libbfoverlay.h \
	bfoverlaytools_libuna.h \
	bfoverlaytools_output.c bfoverlaytools_output.h \
	bfoverlaytools_signal.c bfoverlaytools_signal.h \
	bfoverlaytools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

bfoverlaymount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

splint-local:
	@echo "Running splint on bfoverlaymount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(bfoverlaymount_SOURCES)

