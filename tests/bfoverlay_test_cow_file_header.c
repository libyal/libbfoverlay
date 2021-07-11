/*
 * Library cow_file_header type test program
 *
 * Copyright (C) 2020-2021, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bfoverlay_test_functions.h"
#include "bfoverlay_test_libbfio.h"
#include "bfoverlay_test_libcerror.h"
#include "bfoverlay_test_libbfoverlay.h"
#include "bfoverlay_test_macros.h"
#include "bfoverlay_test_memory.h"
#include "bfoverlay_test_unused.h"

#include "../libbfoverlay/libbfoverlay_cow_file_header.h"

uint8_t bfoverlay_test_cow_file_header_data1[ 64 ] = {
	0x23, 0x20, 0x62, 0x61, 0x73, 0x69, 0x63, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x20, 0x6f, 0x76, 0x65,
	0x72, 0x6c, 0x61, 0x79, 0x20, 0x43, 0x4f, 0x57, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x00, 0x00, 0x00,
	0x01, 0x34, 0x64, 0x16, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

/* Tests the libbfoverlay_cow_file_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_file_header_initialize(
     void )
{
	libbfoverlay_cow_file_header_t *cow_file_header = NULL;
	libcerror_error_t *error                        = NULL;
	int result                                      = 0;

#if defined( HAVE_BFOVERLAY_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 1;
	int number_of_memset_fail_tests                 = 1;
	int test_number                                 = 0;
#endif

	/* Test regular cases
	 */
	result = libbfoverlay_cow_file_header_initialize(
	          &cow_file_header,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "cow_file_header",
	 cow_file_header );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_cow_file_header_free(
	          &cow_file_header,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "cow_file_header",
	 cow_file_header );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbfoverlay_cow_file_header_initialize(
	          NULL,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	cow_file_header = (libbfoverlay_cow_file_header_t *) 0x12345678UL;

	result = libbfoverlay_cow_file_header_initialize(
	          &cow_file_header,
	          &error );

	cow_file_header = NULL;

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BFOVERLAY_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libbfoverlay_cow_file_header_initialize with malloc failing
		 */
		bfoverlay_test_malloc_attempts_before_fail = test_number;

		result = libbfoverlay_cow_file_header_initialize(
		          &cow_file_header,
		          &error );

		if( bfoverlay_test_malloc_attempts_before_fail != -1 )
		{
			bfoverlay_test_malloc_attempts_before_fail = -1;

			if( cow_file_header != NULL )
			{
				libbfoverlay_cow_file_header_free(
				 &cow_file_header,
				 NULL );
			}
		}
		else
		{
			BFOVERLAY_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			BFOVERLAY_TEST_ASSERT_IS_NULL(
			 "cow_file_header",
			 cow_file_header );

			BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libbfoverlay_cow_file_header_initialize with memset failing
		 */
		bfoverlay_test_memset_attempts_before_fail = test_number;

		result = libbfoverlay_cow_file_header_initialize(
		          &cow_file_header,
		          &error );

		if( bfoverlay_test_memset_attempts_before_fail != -1 )
		{
			bfoverlay_test_memset_attempts_before_fail = -1;

			if( cow_file_header != NULL )
			{
				libbfoverlay_cow_file_header_free(
				 &cow_file_header,
				 NULL );
			}
		}
		else
		{
			BFOVERLAY_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			BFOVERLAY_TEST_ASSERT_IS_NULL(
			 "cow_file_header",
			 cow_file_header );

			BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_BFOVERLAY_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( cow_file_header != NULL )
	{
		libbfoverlay_cow_file_header_free(
		 &cow_file_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_cow_file_header_free function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_file_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbfoverlay_cow_file_header_free(
	          NULL,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbfoverlay_cow_file_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_file_header_read_data(
     void )
{
	libbfoverlay_cow_file_header_t *cow_file_header = NULL;
	libcerror_error_t *error                        = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libbfoverlay_cow_file_header_initialize(
	          &cow_file_header,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "cow_file_header",
	 cow_file_header );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libbfoverlay_cow_file_header_read_data(
	          cow_file_header,
	          bfoverlay_test_cow_file_header_data1,
	          64,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	BFOVERLAY_TEST_ASSERT_EQUAL_UINT32(
	 "cow_file_header->format_version",
	 cow_file_header->format_version,
	 20210710 );

	/* Test error cases
	 */
	result = libbfoverlay_cow_file_header_read_data(
	          NULL,
	          bfoverlay_test_cow_file_header_data1,
	          64,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbfoverlay_cow_file_header_read_data(
	          cow_file_header,
	          NULL,
	          64,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbfoverlay_cow_file_header_read_data(
	          cow_file_header,
	          bfoverlay_test_cow_file_header_data1,
	          0,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbfoverlay_cow_file_header_read_data(
	          cow_file_header,
	          bfoverlay_test_cow_file_header_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where signature is invalid
	 */
	bfoverlay_test_cow_file_header_data1[ 0 ] = '!';

	result = libbfoverlay_cow_file_header_read_data(
	          cow_file_header,
	          bfoverlay_test_cow_file_header_data1,
	          64,
	          &error );

	bfoverlay_test_cow_file_header_data1[ 0 ] = '#';

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libbfoverlay_cow_file_header_free(
	          &cow_file_header,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "cow_file_header",
	 cow_file_header );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( cow_file_header != NULL )
	{
		libbfoverlay_cow_file_header_free(
		 &cow_file_header,
		 NULL );
	}
	return( 0 );
}

#ifdef TODO

/* Tests the libbfoverlay_cow_file_header_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_file_header_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle                = NULL;
	libbfoverlay_cow_file_header_t *cow_file_header = NULL;
	libcerror_error_t *error                        = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libbfoverlay_cow_file_header_initialize(
	          &cow_file_header,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "cow_file_header",
	 cow_file_header );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = bfoverlay_test_open_file_io_handle(
	          &file_io_handle,
	          bfoverlay_test_cow_file_header_data1,
	          64,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libbfoverlay_cow_file_header_read_file_io_handle(
	          cow_file_header,
	          file_io_handle,
	          0,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbfoverlay_cow_file_header_read_file_io_handle(
	          NULL,
	          file_io_handle,
	          0,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbfoverlay_cow_file_header_read_file_io_handle(
	          cow_file_header,
	          NULL,
	          0,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up file IO handle
	 */
	result = bfoverlay_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data too small
	 */
	result = bfoverlay_test_open_file_io_handle(
	          &file_io_handle,
	          bfoverlay_test_cow_file_header_data1,
	          8,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_cow_file_header_read_file_io_handle(
	          cow_file_header,
	          file_io_handle,
	          0,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = bfoverlay_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data invalid
	 */
	result = bfoverlay_test_open_file_io_handle(
	          &file_io_handle,
	          bfoverlay_test_cow_file_header_data1,
	          64,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	bfoverlay_test_cow_file_header_data1[ 0 ] = '!';

	result = libbfoverlay_cow_file_header_read_file_io_handle(
	          cow_file_header,
	          file_io_handle,
	          0,
	          &error );

	bfoverlay_test_cow_file_header_data1[ 0 ] = '#';

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = bfoverlay_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libbfoverlay_cow_file_header_free(
	          &cow_file_header,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "cow_file_header",
	 cow_file_header );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( cow_file_header != NULL )
	{
		libbfoverlay_cow_file_header_free(
		 &cow_file_header,
		 NULL );
	}
	return( 0 );
}

#endif /* TODO */

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc BFOVERLAY_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] BFOVERLAY_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc BFOVERLAY_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] BFOVERLAY_TEST_ATTRIBUTE_UNUSED )
#endif
{
	BFOVERLAY_TEST_UNREFERENCED_PARAMETER( argc )
	BFOVERLAY_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_file_header_initialize",
	 bfoverlay_test_cow_file_header_initialize );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_file_header_free",
	 bfoverlay_test_cow_file_header_free );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_file_header_read_data",
	 bfoverlay_test_cow_file_header_read_data );

#ifdef TODO
	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_file_header_read_file_io_handle",
	 bfoverlay_test_cow_file_header_read_file_io_handle );
#endif

	/* TODO add tests for libbfoverlay_cow_file_header_write_file_io_handle */

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */
}

