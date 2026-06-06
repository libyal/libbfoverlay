#!/bin/sh
# Script to run tests
#
# Version: 20260602

if test -f ${PWD}/libbfoverlay/.libs/libbfoverlay.1.dylib && test -f ./pybfoverlay/.libs/pybfoverlay.so
then
	install_name_tool -change /usr/local/lib/libbfoverlay.1.dylib ${PWD}/libbfoverlay/.libs/libbfoverlay.1.dylib ./pybfoverlay/.libs/pybfoverlay.so
fi

make check $@
RESULT=$?

if test ${RESULT} -ne 0 && test -f tests/test-suite.log
then
	cat tests/test-suite.log
fi
exit ${RESULT}

