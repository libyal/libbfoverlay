/*
 * Library descriptor_file type test program
 *
 * Copyright (C) 2020, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bfoverlay_test_functions.h"
#include "bfoverlay_test_libbfio.h"
#include "bfoverlay_test_libbfoverlay.h"
#include "bfoverlay_test_libcerror.h"
#include "bfoverlay_test_macros.h"
#include "bfoverlay_test_memory.h"
#include "bfoverlay_test_unused.h"

#include "../libbfoverlay/libbfoverlay_descriptor_file.h"

uint8_t bfoverlay_test_descriptor_file_data1[ 138 ] = {
	0x23, 0x20, 0x62, 0x61, 0x73, 0x69, 0x63, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x20, 0x6f, 0x76, 0x65,
	0x72, 0x6c, 0x61, 0x79, 0x20, 0x64, 0x65, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x6f, 0x72, 0x20,
	0x66, 0x69, 0x6c, 0x65, 0x0a, 0x6c, 0x61, 0x79, 0x65, 0x72, 0x3a, 0x20, 0x6f, 0x66, 0x66, 0x73,
	0x65, 0x74, 0x3d, 0x30, 0x20, 0x73, 0x69, 0x7a, 0x65, 0x3d, 0x32, 0x31, 0x34, 0x37, 0x34, 0x38,
	0x33, 0x36, 0x34, 0x38, 0x0a, 0x6c, 0x61, 0x79, 0x65, 0x72, 0x3a, 0x20, 0x6f, 0x66, 0x66, 0x73,
	0x65, 0x74, 0x3d, 0x31, 0x36, 0x37, 0x37, 0x37, 0x32, 0x31, 0x36, 0x20, 0x73, 0x69, 0x7a, 0x65,
	0x3d, 0x31, 0x30, 0x37, 0x33, 0x37, 0x34, 0x31, 0x38, 0x32, 0x34, 0x20, 0x66, 0x69, 0x6c, 0x65,
	0x3d, 0x22, 0x6e, 0x74, 0x66, 0x73, 0x2e, 0x72, 0x61, 0x77, 0x22, 0x20, 0x66, 0x69, 0x6c, 0x65,
	0x5f, 0x6f, 0x66, 0x66, 0x73, 0x65, 0x74, 0x3d, 0x30, 0x0a };

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

/* Tests the libbfoverlay_descriptor_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_descriptor_file_initialize(
     void )
{
	libbfoverlay_descriptor_file_t *descriptor_file = NULL;
	libcerror_error_t *error                        = NULL;
	int result                                      = 0;

#if defined( HAVE_BFOVERLAY_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 1;
	int number_of_memset_fail_tests                 = 1;
	int test_number                                 = 0;
#endif

	/* Test regular cases
	 */
	result = libbfoverlay_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_descriptor_file_free(
	          &descriptor_file,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbfoverlay_descriptor_file_initialize(
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

	descriptor_file = (libbfoverlay_descriptor_file_t *) 0x12345678UL;

	result = libbfoverlay_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	descriptor_file = NULL;

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
		/* Test libbfoverlay_descriptor_file_initialize with malloc failing
		 */
		bfoverlay_test_malloc_attempts_before_fail = test_number;

		result = libbfoverlay_descriptor_file_initialize(
		          &descriptor_file,
		          &error );

		if( bfoverlay_test_malloc_attempts_before_fail != -1 )
		{
			bfoverlay_test_malloc_attempts_before_fail = -1;

			if( descriptor_file != NULL )
			{
				libbfoverlay_descriptor_file_free(
				 &descriptor_file,
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
			 "descriptor_file",
			 descriptor_file );

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
		/* Test libbfoverlay_descriptor_file_initialize with memset failing
		 */
		bfoverlay_test_memset_attempts_before_fail = test_number;

		result = libbfoverlay_descriptor_file_initialize(
		          &descriptor_file,
		          &error );

		if( bfoverlay_test_memset_attempts_before_fail != -1 )
		{
			bfoverlay_test_memset_attempts_before_fail = -1;

			if( descriptor_file != NULL )
			{
				libbfoverlay_descriptor_file_free(
				 &descriptor_file,
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
			 "descriptor_file",
			 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libbfoverlay_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_descriptor_file_free function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_descriptor_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbfoverlay_descriptor_file_free(
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

/* Tests the libbfoverlay_descriptor_file_read_data function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_descriptor_file_read_data(
     void )
{
	libbfoverlay_descriptor_file_t *descriptor_file = NULL;
	libcerror_error_t *error                        = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libbfoverlay_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libbfoverlay_descriptor_file_read_data(
	          descriptor_file,
	          bfoverlay_test_descriptor_file_data1,
	          138,
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
	result = libbfoverlay_descriptor_file_read_data(
	          NULL,
	          bfoverlay_test_descriptor_file_data1,
	          138,
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

	result = libbfoverlay_descriptor_file_read_data(
	          descriptor_file,
	          NULL,
	          138,
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

	result = libbfoverlay_descriptor_file_read_data(
	          descriptor_file,
	          bfoverlay_test_descriptor_file_data1,
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

	result = libbfoverlay_descriptor_file_read_data(
	          descriptor_file,
	          bfoverlay_test_descriptor_file_data1,
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

	/* Clean up
	 */
	result = libbfoverlay_descriptor_file_free(
	          &descriptor_file,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libbfoverlay_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_descriptor_file_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_descriptor_file_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle                = NULL;
	libbfoverlay_descriptor_file_t *descriptor_file = NULL;
	libcerror_error_t *error                        = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libbfoverlay_descriptor_file_initialize(
	          &descriptor_file,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "descriptor_file",
	 descriptor_file );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = bfoverlay_test_open_file_io_handle(
	          &file_io_handle,
	          bfoverlay_test_descriptor_file_data1,
	          138,
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
	result = libbfoverlay_descriptor_file_read_file_io_handle(
	          descriptor_file,
	          file_io_handle,
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
	result = libbfoverlay_descriptor_file_read_file_io_handle(
	          NULL,
	          file_io_handle,
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

	result = libbfoverlay_descriptor_file_read_file_io_handle(
	          descriptor_file,
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
	          bfoverlay_test_descriptor_file_data1,
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

	result = libbfoverlay_descriptor_file_read_file_io_handle(
	          descriptor_file,
	          file_io_handle,
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
/* TODO implement */

	/* Clean up
	 */
	result = libbfoverlay_descriptor_file_free(
	          &descriptor_file,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "descriptor_file",
	 descriptor_file );

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
	if( descriptor_file != NULL )
	{
		libbfoverlay_descriptor_file_free(
		 &descriptor_file,
		 NULL );
	}
	return( 0 );
}

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
	 "libbfoverlay_descriptor_file_initialize",
	 bfoverlay_test_descriptor_file_initialize );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_descriptor_file_free",
	 bfoverlay_test_descriptor_file_free );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_descriptor_file_read_data",
	 bfoverlay_test_descriptor_file_read_data );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_descriptor_file_read_file_io_handle",
	 bfoverlay_test_descriptor_file_read_file_io_handle );

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

