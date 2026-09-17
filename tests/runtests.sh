#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libbfoverlay/.libs/libbfoverlay.1.dylib" ] && [ -f ./pybfoverlay/.libs/pybfoverlay.so ]
then
    install_name_tool -change /usr/local/lib/libbfoverlay.1.dylib "${PWD}/libbfoverlay/.libs/libbfoverlay.1.dylib" ./pybfoverlay/.libs/pybfoverlay.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

