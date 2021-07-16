AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBBFOVERLAY_DLL_EXPORT@

lib_LTLIBRARIES = libbfoverlay.la

libbfoverlay_la_SOURCES = \
	bfoverlay_cow_allocation_table_block.h \
	bfoverlay_cow_file_header.h \
	libbfoverlay.c \
	libbfoverlay_codepage.h \
	libbfoverlay_cow_allocation_table_block.c libbfoverlay_cow_allocation_table_block.h \
	libbfoverlay_cow_file.c libbfoverlay_cow_file.h \
	libbfoverlay_cow_file_header.c libbfoverlay_cow_file_header.h \
	libbfoverlay_definitions.h \
	libbfoverlay_descriptor_file.c libbfoverlay_descriptor_file.h \
	libbfoverlay_error.c libbfoverlay_error.h \
	libbfoverlay_extern.h \
	libbfoverlay_handle.c libbfoverlay_handle.h \
	libbfoverlay_layer.c libbfoverlay_layer.h \
	libbfoverlay_libbfio.h \
	libbfoverlay_libcdata.h \
	libbfoverlay_libcerror.h \
	libbfoverlay_libcfile.h \
	libbfoverlay_libclocale.h \
	libbfoverlay_libcnotify.h \
	libbfoverlay_libcthreads.h \
	libbfoverlay_libfvalue.h \
	libbfoverlay_libuna.h \
	libbfoverlay_notify.c libbfoverlay_notify.h \
	libbfoverlay_range.c libbfoverlay_range.h \
	libbfoverlay_support.c libbfoverlay_support.h \
	libbfoverlay_types.h \
	libbfoverlay_unused.h

libbfoverlay_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@PTHREAD_LIBADD@

libbfoverlay_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libbfoverlay_definitions.h.in \
	libbfoverlay.rc \
	libbfoverlay.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libbfoverlay_definitions.h
	/bin/rm -f libbfoverlay.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libbfoverlay ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libbfoverlay_la_SOURCES)

