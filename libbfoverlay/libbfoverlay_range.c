/*
 * The range functions
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
#include <memory.h>
#include <types.h>

#include "libbfoverlay_libcdata.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_range.h"

/* Creates a range
 * Make sure the value range is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_range_initialize(
     libbfoverlay_range_t **range,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_range_initialize";

	if( range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range.",
		 function );

		return( -1 );
	}
	if( *range != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid range value already set.",
		 function );

		return( -1 );
	}
	*range = memory_allocate_structure(
	          libbfoverlay_range_t );

	if( *range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create range.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *range,
	     0,
	     sizeof( libbfoverlay_range_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear range.",
		 function );

		goto on_error;
	}
	( *range )->data_file_index = -1;
	( *range )->start_offset    = (off64_t) -1;
	( *range )->end_offset      = (off64_t) -1;

	return( 1 );

on_error:
	if( *range != NULL )
	{
		memory_free(
		 *range );

		*range = NULL;
	}
	return( -1 );
}

/* Frees a range
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_range_free(
     libbfoverlay_range_t **range,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_range_free";

	if( range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range.",
		 function );

		return( -1 );
	}
	if( *range != NULL )
	{
		memory_free(
		 *range );

		*range = NULL;
	}
	return( 1 );
}

/* Compares ranges
 * Returns  LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libbfoverlay_range_compare(
     libbfoverlay_range_t *first_range,
     libbfoverlay_range_t *second_range,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_range_compare";

	if( first_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first range.",
		 function );

		return( -1 );
	}
	if( second_range == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second range.",
		 function );

		return( -1 );
	}
	/* Note that the end offset is the upper bound of the range and and is not considered part of the range
	 */
	if( first_range->end_offset <= second_range->start_offset )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	if( first_range->start_offset >= second_range->end_offset )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

