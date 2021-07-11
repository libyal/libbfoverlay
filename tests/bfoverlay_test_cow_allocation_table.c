/*
 * Library cow_allocation_table type test program
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

#include "../libbfoverlay/libbfoverlay_cow_allocation_table.h"

uint8_t bfoverlay_test_cow_allocation_table_data1[ 64 ] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

/* Tests the libbfoverlay_cow_allocation_table_initialize function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_allocation_table_initialize(
     void )
{
	libbfoverlay_cow_allocation_table_t *cow_allocation_table = NULL;
	libcerror_error_t *error                                  = NULL;
	int result                                                = 0;

#if defined( HAVE_BFOVERLAY_TEST_MEMORY )
	int number_of_malloc_fail_tests                           = 1;
	int number_of_memset_fail_tests                           = 1;
	int test_number                                           = 0;
#endif

	/* Test regular cases
	 */
	result = libbfoverlay_cow_allocation_table_initialize(
	          &cow_allocation_table,
	          8,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "cow_allocation_table",
	 cow_allocation_table );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_cow_allocation_table_free(
	          &cow_allocation_table,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "cow_allocation_table",
	 cow_allocation_table );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbfoverlay_cow_allocation_table_initialize(
	          NULL,
	          8,
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

	cow_allocation_table = (libbfoverlay_cow_allocation_table_t *) 0x12345678UL;

	result = libbfoverlay_cow_allocation_table_initialize(
	          &cow_allocation_table,
	          8,
	          &error );

	cow_allocation_table = NULL;

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
		/* Test libbfoverlay_cow_allocation_table_initialize with malloc failing
		 */
		bfoverlay_test_malloc_attempts_before_fail = test_number;

		result = libbfoverlay_cow_allocation_table_initialize(
		          &cow_allocation_table,
		          8,
		          &error );

		if( bfoverlay_test_malloc_attempts_before_fail != -1 )
		{
			bfoverlay_test_malloc_attempts_before_fail = -1;

			if( cow_allocation_table != NULL )
			{
				libbfoverlay_cow_allocation_table_free(
				 &cow_allocation_table,
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
			 "cow_allocation_table",
			 cow_allocation_table );

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
		/* Test libbfoverlay_cow_allocation_table_initialize with memset failing
		 */
		bfoverlay_test_memset_attempts_before_fail = test_number;

		result = libbfoverlay_cow_allocation_table_initialize(
		          &cow_allocation_table,
		          8,
		          &error );

		if( bfoverlay_test_memset_attempts_before_fail != -1 )
		{
			bfoverlay_test_memset_attempts_before_fail = -1;

			if( cow_allocation_table != NULL )
			{
				libbfoverlay_cow_allocation_table_free(
				 &cow_allocation_table,
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
			 "cow_allocation_table",
			 cow_allocation_table );

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
	if( cow_allocation_table != NULL )
	{
		libbfoverlay_cow_allocation_table_free(
		 &cow_allocation_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_cow_allocation_table_free function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_allocation_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbfoverlay_cow_allocation_table_free(
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

/* Tests the libbfoverlay_cow_allocation_table_read_data function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_allocation_table_read_data(
     void )
{
	libbfoverlay_cow_allocation_table_t *cow_allocation_table = NULL;
	libcerror_error_t *error                                  = NULL;
	int result                                                = 0;

	/* Initialize test
	 */
	result = libbfoverlay_cow_allocation_table_initialize(
	          &cow_allocation_table,
	          8,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "cow_allocation_table",
	 cow_allocation_table );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libbfoverlay_cow_allocation_table_read_data(
	          cow_allocation_table,
	          bfoverlay_test_cow_allocation_table_data1,
	          64,
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
	result = libbfoverlay_cow_allocation_table_read_data(
	          NULL,
	          bfoverlay_test_cow_allocation_table_data1,
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

	result = libbfoverlay_cow_allocation_table_read_data(
	          cow_allocation_table,
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

	result = libbfoverlay_cow_allocation_table_read_data(
	          cow_allocation_table,
	          bfoverlay_test_cow_allocation_table_data1,
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

	result = libbfoverlay_cow_allocation_table_read_data(
	          cow_allocation_table,
	          bfoverlay_test_cow_allocation_table_data1,
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

	/* Clean up
	 */
	result = libbfoverlay_cow_allocation_table_free(
	          &cow_allocation_table,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "cow_allocation_table",
	 cow_allocation_table );

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
	if( cow_allocation_table != NULL )
	{
		libbfoverlay_cow_allocation_table_free(
		 &cow_allocation_table,
		 NULL );
	}
	return( 0 );
}

#ifdef TODO

/* Tests the libbfoverlay_cow_allocation_table_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_allocation_table_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle                          = NULL;
	libbfoverlay_cow_allocation_table_t *cow_allocation_table = NULL;
	libcerror_error_t *error                                  = NULL;
	int result                                                = 0;

	/* Initialize test
	 */
	result = libbfoverlay_cow_allocation_table_initialize(
	          &cow_allocation_table,
	          8,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "cow_allocation_table",
	 cow_allocation_table );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = bfoverlay_test_open_file_io_handle(
	          &file_io_handle,
	          bfoverlay_test_cow_allocation_table_data1,
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
	result = libbfoverlay_cow_allocation_table_read_file_io_handle(
	          cow_allocation_table,
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
	result = libbfoverlay_cow_allocation_table_read_file_io_handle(
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

	result = libbfoverlay_cow_allocation_table_read_file_io_handle(
	          cow_allocation_table,
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
	          bfoverlay_test_cow_allocation_table_data1,
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

	result = libbfoverlay_cow_allocation_table_read_file_io_handle(
	          cow_allocation_table,
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

	/* Clean up
	 */
	result = libbfoverlay_cow_allocation_table_free(
	          &cow_allocation_table,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "cow_allocation_table",
	 cow_allocation_table );

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
	if( cow_allocation_table != NULL )
	{
		libbfoverlay_cow_allocation_table_free(
		 &cow_allocation_table,
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
	 "libbfoverlay_cow_allocation_table_initialize",
	 bfoverlay_test_cow_allocation_table_initialize );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_allocation_table_free",
	 bfoverlay_test_cow_allocation_table_free );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_allocation_table_read_data",
	 bfoverlay_test_cow_allocation_table_read_data );

#ifdef TODO
	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_allocation_table_read_file_io_handle",
	 bfoverlay_test_cow_allocation_table_read_file_io_handle );
#endif

	/* TODO add tests for libbfoverlay_cow_allocation_table_write_file_io_handle */

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */
}

