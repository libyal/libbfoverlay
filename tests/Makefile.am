AUTOMAKE_OPTIONS = subdir-objects

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
	@LIBBFOVERLAY_DLL_IMPORT@

TESTS = \
	test_library.sh \
	test_tools.sh

check_SCRIPTS = \
	test_library.sh \
	test_manpage.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	bfoverlay_test_cow_allocation_table_block \
	bfoverlay_test_cow_file \
	bfoverlay_test_cow_file_header \
	bfoverlay_test_descriptor_file \
	bfoverlay_test_error \
	bfoverlay_test_handle \
	bfoverlay_test_layer \
	bfoverlay_test_notify \
	bfoverlay_test_range \
	bfoverlay_test_support \
	bfoverlay_test_tools_output \
	bfoverlay_test_tools_signal

bfoverlay_test_cow_allocation_table_block_SOURCES = \
	bfoverlay_test_cow_allocation_table_block.c \
	bfoverlay_test_functions.c bfoverlay_test_functions.h \
	bfoverlay_test_libbfio.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_memory.c bfoverlay_test_memory.h \
	bfoverlay_test_unused.h

bfoverlay_test_cow_allocation_table_block_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_cow_file_SOURCES = \
	bfoverlay_test_cow_file.c \
	bfoverlay_test_functions.c bfoverlay_test_functions.h \
	bfoverlay_test_libbfio.h \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_memory.c bfoverlay_test_memory.h \
	bfoverlay_test_unused.h

bfoverlay_test_cow_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_cow_file_header_SOURCES = \
	bfoverlay_test_cow_file_header.c \
	bfoverlay_test_functions.c bfoverlay_test_functions.h \
	bfoverlay_test_libbfio.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_memory.c bfoverlay_test_memory.h \
	bfoverlay_test_unused.h

bfoverlay_test_cow_file_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_descriptor_file_SOURCES = \
	bfoverlay_test_descriptor_file.c \
	bfoverlay_test_functions.c bfoverlay_test_functions.h \
	bfoverlay_test_libbfio.h \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_memory.c bfoverlay_test_memory.h \
	bfoverlay_test_unused.h

bfoverlay_test_descriptor_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_error_SOURCES = \
	bfoverlay_test_error.c \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_unused.h

bfoverlay_test_error_LDADD = \
	../libbfoverlay/libbfoverlay.la

bfoverlay_test_handle_SOURCES = \
	bfoverlay_test_functions.c bfoverlay_test_functions.h \
	bfoverlay_test_getopt.c bfoverlay_test_getopt.h \
	bfoverlay_test_handle.c \
	bfoverlay_test_libbfio.h \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_libclocale.h \
	bfoverlay_test_libcnotify.h \
	bfoverlay_test_libuna.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_memory.c bfoverlay_test_memory.h \
	bfoverlay_test_unused.h

bfoverlay_test_handle_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbfoverlay/libbfoverlay.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

bfoverlay_test_layer_SOURCES = \
	bfoverlay_test_layer.c \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_memory.c bfoverlay_test_memory.h \
	bfoverlay_test_unused.h

bfoverlay_test_layer_LDADD = \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_notify_SOURCES = \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_notify.c \
	bfoverlay_test_unused.h

bfoverlay_test_notify_LDADD = \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_range_SOURCES = \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_memory.c bfoverlay_test_memory.h \
	bfoverlay_test_range.c \
	bfoverlay_test_unused.h

bfoverlay_test_range_LDADD = \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_support_SOURCES = \
	bfoverlay_test_libbfoverlay.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_support.c \
	bfoverlay_test_unused.h

bfoverlay_test_support_LDADD = \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_tools_output_SOURCES = \
	../bfoverlaytools/bfoverlaytools_output.c ../bfoverlaytools/bfoverlaytools_output.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_tools_output.c \
	bfoverlay_test_unused.h

bfoverlay_test_tools_output_LDADD = \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

bfoverlay_test_tools_signal_SOURCES = \
	../bfoverlaytools/bfoverlaytools_signal.c ../bfoverlaytools/bfoverlaytools_signal.h \
	bfoverlay_test_libcerror.h \
	bfoverlay_test_macros.h \
	bfoverlay_test_tools_signal.c \
	bfoverlay_test_unused.h

bfoverlay_test_tools_signal_LDADD = \
	../libbfoverlay/libbfoverlay.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

