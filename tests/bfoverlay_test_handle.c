/*
 * Library handle type test program
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "bfoverlay_test_functions.h"
#include "bfoverlay_test_getopt.h"
#include "bfoverlay_test_libbfio.h"
#include "bfoverlay_test_libbfoverlay.h"
#include "bfoverlay_test_libcerror.h"
#include "bfoverlay_test_macros.h"
#include "bfoverlay_test_memory.h"

#include "../libbfoverlay/libbfoverlay_handle.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make bfoverlay_test_handle generate verbose output
#define BFOVERLAY_TEST_HANDLE_VERBOSE
 */

#define BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE	4096

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int bfoverlay_test_handle_open_source(
     libbfoverlay_handle_t **handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "bfoverlay_test_handle_open_source";
	int result            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfoverlay_handle_initialize(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
	result = libbfoverlay_handle_open_file_io_handle(
	          *handle,
	          file_io_handle,
	          LIBBFOVERLAY_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *handle != NULL )
	{
		libbfoverlay_handle_free(
		 handle,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source handle
 * Returns 1 if successful or -1 on error
 */
int bfoverlay_test_handle_close_source(
     libbfoverlay_handle_t **handle,
     libcerror_error_t **error )
{
	static char *function = "bfoverlay_test_handle_close_source";
	int result            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libbfoverlay_handle_close(
	     *handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		result = -1;
	}
	if( libbfoverlay_handle_free(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libbfoverlay_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_initialize(
     void )
{
	libbfoverlay_handle_t *handle   = NULL;
	libcerror_error_t *error        = NULL;
	int result                      = 0;

#if defined( HAVE_BFOVERLAY_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libbfoverlay_handle_initialize(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_handle_free(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbfoverlay_handle_initialize(
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

	handle = (libbfoverlay_handle_t *) 0x12345678UL;

	result = libbfoverlay_handle_initialize(
	          &handle,
	          &error );

	handle = NULL;

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
		/* Test libbfoverlay_handle_initialize with malloc failing
		 */
		bfoverlay_test_malloc_attempts_before_fail = test_number;

		result = libbfoverlay_handle_initialize(
		          &handle,
		          &error );

		if( bfoverlay_test_malloc_attempts_before_fail != -1 )
		{
			bfoverlay_test_malloc_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libbfoverlay_handle_free(
				 &handle,
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
			 "handle",
			 handle );

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
		/* Test libbfoverlay_handle_initialize with memset failing
		 */
		bfoverlay_test_memset_attempts_before_fail = test_number;

		result = libbfoverlay_handle_initialize(
		          &handle,
		          &error );

		if( bfoverlay_test_memset_attempts_before_fail != -1 )
		{
			bfoverlay_test_memset_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libbfoverlay_handle_free(
				 &handle,
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
			 "handle",
			 handle );

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
	if( handle != NULL )
	{
		libbfoverlay_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_handle_free function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbfoverlay_handle_free(
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

/* Tests the libbfoverlay_handle_open function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libbfoverlay_handle_t *handle = NULL;
	libcerror_error_t *error      = NULL;
	int result                    = 0;

	/* Initialize test
	 */
	result = bfoverlay_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_handle_initialize(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libbfoverlay_handle_open(
	          handle,
	          narrow_source,
	          LIBBFOVERLAY_OPEN_READ,
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
	result = libbfoverlay_handle_open(
	          NULL,
	          narrow_source,
	          LIBBFOVERLAY_OPEN_READ,
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

	result = libbfoverlay_handle_open(
	          handle,
	          NULL,
	          LIBBFOVERLAY_OPEN_READ,
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

	result = libbfoverlay_handle_open(
	          handle,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libbfoverlay_handle_open(
	          handle,
	          narrow_source,
	          LIBBFOVERLAY_OPEN_READ,
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
	result = libbfoverlay_handle_free(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

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
	if( handle != NULL )
	{
		libbfoverlay_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libbfoverlay_handle_open_wide function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libbfoverlay_handle_t *handle = NULL;
	libcerror_error_t *error      = NULL;
	int result                    = 0;

	/* Initialize test
	 */
	result = bfoverlay_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_handle_initialize(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libbfoverlay_handle_open_wide(
	          handle,
	          wide_source,
	          LIBBFOVERLAY_OPEN_READ,
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
	result = libbfoverlay_handle_open_wide(
	          NULL,
	          wide_source,
	          LIBBFOVERLAY_OPEN_READ,
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

	result = libbfoverlay_handle_open_wide(
	          handle,
	          NULL,
	          LIBBFOVERLAY_OPEN_READ,
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

	result = libbfoverlay_handle_open_wide(
	          handle,
	          wide_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libbfoverlay_handle_open_wide(
	          handle,
	          wide_source,
	          LIBBFOVERLAY_OPEN_READ,
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
	result = libbfoverlay_handle_free(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

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
	if( handle != NULL )
	{
		libbfoverlay_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libbfoverlay_handle_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libbfoverlay_handle_t *handle    = NULL;
	libcerror_error_t *error         = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
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

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_handle_initialize(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libbfoverlay_handle_open_file_io_handle(
	          handle,
	          file_io_handle,
	          LIBBFOVERLAY_OPEN_READ,
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
	result = libbfoverlay_handle_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBBFOVERLAY_OPEN_READ,
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

	result = libbfoverlay_handle_open_file_io_handle(
	          handle,
	          NULL,
	          LIBBFOVERLAY_OPEN_READ,
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

	result = libbfoverlay_handle_open_file_io_handle(
	          handle,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libbfoverlay_handle_open_file_io_handle(
	          handle,
	          file_io_handle,
	          LIBBFOVERLAY_OPEN_READ,
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
	result = libbfoverlay_handle_free(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

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
	if( handle != NULL )
	{
		libbfoverlay_handle_free(
		 &handle,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_handle_close function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbfoverlay_handle_close(
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

/* Tests the libbfoverlay_handle_open and libbfoverlay_handle_close functions
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_open_close(
     const system_character_t *source )
{
	libbfoverlay_handle_t *handle = NULL;
	libcerror_error_t *error      = NULL;
	int result                    = 0;

	/* Initialize test
	 */
	result = libbfoverlay_handle_initialize(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfoverlay_handle_open_wide(
	          handle,
	          source,
	          LIBBFOVERLAY_OPEN_READ,
	          &error );
#else
	result = libbfoverlay_handle_open(
	          handle,
	          source,
	          LIBBFOVERLAY_OPEN_READ,
	          &error );
#endif

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_handle_close(
	          handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfoverlay_handle_open_wide(
	          handle,
	          source,
	          LIBBFOVERLAY_OPEN_READ,
	          &error );
#else
	result = libbfoverlay_handle_open(
	          handle,
	          source,
	          LIBBFOVERLAY_OPEN_READ,
	          &error );
#endif

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfoverlay_handle_close(
	          handle,
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
	result = libbfoverlay_handle_free(
	          &handle,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

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
	if( handle != NULL )
	{
		libbfoverlay_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbfoverlay_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_signal_abort(
     libbfoverlay_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbfoverlay_handle_signal_abort(
	          handle,
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
	result = libbfoverlay_handle_signal_abort(
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

/* Tests the libbfoverlay_handle_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_read_buffer(
     libbfoverlay_handle_t *handle )
{
	uint8_t buffer[ BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libbfoverlay_handle_get_size(
	          handle,
	          &size,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE;

	if( size < BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libbfoverlay_handle_read_buffer(
	              handle,
	              buffer,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Set offset to size - 8
		 */
		offset = libbfoverlay_handle_seek_offset(
		          handle,
		          -8,
		          SEEK_END,
		          &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) size - 8 );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on size boundary
		 */
		read_count = libbfoverlay_handle_read_buffer(
		              handle,
		              buffer,
		              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
		              &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libbfoverlay_handle_read_buffer(
		              handle,
		              buffer,
		              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
		              &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	remaining_size = size;

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		BFOVERLAY_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		read_size = (size_t) random_number % BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE;

#if defined( BFOVERLAY_TEST_HANDLE_VERBOSE )
		fprintf(
		 stdout,
		 "libbfoverlay_handle_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libbfoverlay_handle_read_buffer(
		              handle,
		              buffer,
		              read_size,
		              &error );

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libbfoverlay_handle_get_offset(
		          handle,
		          &offset,
		          &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_size -= read_count;

		if( remaining_size == 0 )
		{
			offset = libbfoverlay_handle_seek_offset(
			          handle,
			          0,
			          SEEK_SET,
			          &error );

			BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			BFOVERLAY_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			read_offset = 0;

			remaining_size = size;
		}
	}
	/* Reset offset to 0
	 */
	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libbfoverlay_handle_read_buffer(
	              NULL,
	              buffer,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbfoverlay_handle_read_buffer(
	              handle,
	              NULL,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbfoverlay_handle_read_buffer(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BFOVERLAY_TEST_RWLOCK )

	/* Test libbfoverlay_handle_read_buffer with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libbfoverlay_handle_read_buffer(
	              handle,
	              buffer,
	              BFOVERLAY_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libbfoverlay_handle_read_buffer with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libbfoverlay_handle_read_buffer(
	              handle,
	              buffer,
	              BFOVERLAY_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_BFOVERLAY_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbfoverlay_handle_read_buffer_at_offset function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_read_buffer_at_offset(
     libbfoverlay_handle_t *handle )
{
	uint8_t buffer[ BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libbfoverlay_handle_get_size(
	          handle,
	          &size,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE;

	if( size < BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libbfoverlay_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Read buffer on size boundary
		 */
		read_count = libbfoverlay_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
		              size - 8,
		              &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libbfoverlay_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
		              size + 8,
		              &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		BFOVERLAY_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		if( size > 0 )
		{
			read_offset = (off64_t) random_number % size;
		}
		read_size = (size_t) random_number % BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE;

#if defined( BFOVERLAY_TEST_HANDLE_VERBOSE )
		fprintf(
		 stdout,
		 "libbfoverlay_handle_read_buffer_at_offset: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libbfoverlay_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              read_size,
		              read_offset,
		              &error );

		remaining_size = size - read_offset;

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libbfoverlay_handle_get_offset(
		          handle,
		          &offset,
		          &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	read_count = libbfoverlay_handle_read_buffer_at_offset(
	              NULL,
	              buffer,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbfoverlay_handle_read_buffer_at_offset(
	              handle,
	              NULL,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbfoverlay_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbfoverlay_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              -1,
	              &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BFOVERLAY_TEST_RWLOCK )

	/* Test libbfoverlay_handle_read_buffer_at_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libbfoverlay_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libbfoverlay_handle_read_buffer_at_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libbfoverlay_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              BFOVERLAY_TEST_HANDLE_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		BFOVERLAY_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_BFOVERLAY_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbfoverlay_handle_seek_offset function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_seek_offset(
     libbfoverlay_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	off64_t offset           = 0;

	/* Test regular cases
	 */
	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          0,
	          SEEK_END,
	          &error );

	BFOVERLAY_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          1024,
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 1024 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          -512,
	          SEEK_CUR,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 512 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          (off64_t) ( size + 512 ),
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) ( size + 512 ) );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BFOVERLAY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libbfoverlay_handle_seek_offset(
	          NULL,
	          0,
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_SET,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_CUR,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          (off64_t) ( -1 * ( size + 1 ) ),
	          SEEK_END,
	          &error );

	BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BFOVERLAY_TEST_RWLOCK )

	/* Test libbfoverlay_handle_seek_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	if( bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		bfoverlay_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libbfoverlay_handle_seek_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	offset = libbfoverlay_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	if( bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		bfoverlay_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		BFOVERLAY_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_BFOVERLAY_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbfoverlay_handle_get_offset function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_get_offset(
     libbfoverlay_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbfoverlay_handle_get_offset(
	          handle,
	          &offset,
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
	result = libbfoverlay_handle_get_offset(
	          NULL,
	          &offset,
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

	result = libbfoverlay_handle_get_offset(
	          handle,
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

/* Tests the libbfoverlay_handle_get_size function
 * Returns 1 if successful or 0 if not
 */
int bfoverlay_test_handle_get_size(
     libbfoverlay_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbfoverlay_handle_get_size(
	          handle,
	          &size,
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
	result = libbfoverlay_handle_get_size(
	          NULL,
	          &size,
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

	result = libbfoverlay_handle_get_size(
	          handle,
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

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle = NULL;
	libbfoverlay_handle_t *handle    = NULL;
	libcerror_error_t *error         = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int result                       = 0;

	while( ( option = bfoverlay_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( BFOVERLAY_TEST_HANDLE_VERBOSE )
	libbfoverlay_notify_set_verbose(
	 1 );
	libbfoverlay_notify_set_stream(
	 stderr,
	 NULL );
#endif

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_handle_initialize",
	 bfoverlay_test_handle_initialize );

	BFOVERLAY_TEST_RUN(
	 "libbfoverlay_handle_free",
	 bfoverlay_test_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
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

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		BFOVERLAY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BFOVERLAY_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
	if( result != 0 )
	{
		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_open",
		 bfoverlay_test_handle_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_open_wide",
		 bfoverlay_test_handle_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_open_file_io_handle",
		 bfoverlay_test_handle_open_file_io_handle,
		 source );

		BFOVERLAY_TEST_RUN(
		 "libbfoverlay_handle_close",
		 bfoverlay_test_handle_close );

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_open_close",
		 bfoverlay_test_handle_open_close,
		 source );

		/* Initialize handle for tests
		 */
		result = bfoverlay_test_handle_open_source(
		          &handle,
		          file_io_handle,
		          &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BFOVERLAY_TEST_ASSERT_IS_NOT_NULL(
		 "handle",
		 handle );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_signal_abort",
		 bfoverlay_test_handle_signal_abort,
		 handle );

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

		/* TODO: add tests for libbfoverlay_internal_handle_open_read */

		/* TODO: add tests for libbfoverlay_internal_handle_read_buffer */

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_read_buffer",
		 bfoverlay_test_handle_read_buffer,
		 handle );

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_read_buffer_at_offset",
		 bfoverlay_test_handle_read_buffer_at_offset,
		 handle );

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

		/* TODO: add tests for libbfoverlay_internal_handle_write_buffer */

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

		/* TODO: add tests for libbfoverlay_handle_write_buffer */

		/* TODO: add tests for libbfoverlay_handle_write_buffer_at_offset */

#if defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT )

		/* TODO: add tests for libbfoverlay_internal_handle_seek_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBBFOVERLAY_DLL_IMPORT ) */

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_seek_offset",
		 bfoverlay_test_handle_seek_offset,
		 handle );

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_get_offset",
		 bfoverlay_test_handle_get_offset,
		 handle );

		BFOVERLAY_TEST_RUN_WITH_ARGS(
		 "libbfoverlay_handle_get_size",
		 bfoverlay_test_handle_get_size,
		 handle );

		/* Clean up
		 */
		result = bfoverlay_test_handle_close_source(
		          &handle,
		          &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "handle",
		 handle );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( file_io_handle != NULL )
	{
		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		BFOVERLAY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BFOVERLAY_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        BFOVERLAY_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libbfoverlay_handle_free(
		 &handle,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

