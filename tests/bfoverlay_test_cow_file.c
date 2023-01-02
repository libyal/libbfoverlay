/*
 * Library cow_file type test program
 *
 * Copyright (C) 2020-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "../libbfoverlay/libbfoverlay_cow_file.h"

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

/* Tests the libbfoverlay_cow_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_file_initialize(
     void )
{
	libbfoverlay_cow_file_t *cow_file = NULL;
	libcerror_error_t *error          = NULL;
	int result                        = 0;

#if defined( HAVE_BFOVERLAY_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 1;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libbfoverlay_cow_file_initialize(
	          &cow_file,
	          1024,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "cow_file",
	 cow_file );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_cow_file_free(
	          &cow_file,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "cow_file",
	 cow_file );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbfoverlay_cow_file_initialize(
	          NULL,
	          1024,
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

	cow_file = (libbfoverlay_cow_file_t *) 0x12345678UL;

	result = libbfoverlay_cow_file_initialize(
	          &cow_file,
	          1024,
	          &error );

	cow_file = NULL;

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
		/* Test libbfoverlay_cow_file_initialize with malloc failing
		 */
		bfoverlay_test_malloc_attempts_before_fail = test_number;

		result = libbfoverlay_cow_file_initialize(
		          &cow_file,
		          1024,
		          &error );

		if( bfoverlay_test_malloc_attempts_before_fail != -1 )
		{
			bfoverlay_test_malloc_attempts_before_fail = -1;

			if( cow_file != NULL )
			{
				libbfoverlay_cow_file_free(
				 &cow_file,
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
			 "cow_file",
			 cow_file );

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
		/* Test libbfoverlay_cow_file_initialize with memset failing
		 */
		bfoverlay_test_memset_attempts_before_fail = test_number;

		result = libbfoverlay_cow_file_initialize(
		          &cow_file,
		          1024,
		          &error );

		if( bfoverlay_test_memset_attempts_before_fail != -1 )
		{
			bfoverlay_test_memset_attempts_before_fail = -1;

			if( cow_file != NULL )
			{
				libbfoverlay_cow_file_free(
				 &cow_file,
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
			 "cow_file",
			 cow_file );

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
	if( cow_file != NULL )
	{
		libbfoverlay_cow_file_free(
		 &cow_file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_cow_file_free function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_cow_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbfoverlay_cow_file_free(
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
	 "libbfoverlay_cow_file_initialize",
	 bfoverlay_test_cow_file_initialize );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_cow_file_free",
	 bfoverlay_test_cow_file_free );

	/* TODO add tests for libbfoverlay_cow_file_open_file_io_handle */

	/* TODO add tests for libbfoverlay_cow_file_close */

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */
}

