/*
 * Extent functions
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
#include <memory.h>
#include <types.h>

#include "libbfoverlay_layer.h"
#include "libbfoverlay_libcerror.h"

/* Creates an layer
 * Make sure the value layer is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_layer_initialize(
     libbfoverlay_layer_t **layer,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_layer_initialize";

	if( layer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid layer.",
		 function );

		return( -1 );
	}
	if( *layer != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid layer value already set.",
		 function );

		return( -1 );
	}
	*layer = memory_allocate_structure(
	          libbfoverlay_layer_t );

	if( *layer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create layer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *layer,
	     0,
	     sizeof( libbfoverlay_layer_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear layer.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *layer != NULL )
	{
		memory_free(
		 *layer );

		*layer = NULL;
	}
	return( -1 );
}

/* Frees an layer
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_layer_free(
     libbfoverlay_layer_t **layer,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_layer_free";

	if( layer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid layer.",
		 function );

		return( -1 );
	}
	if( *layer != NULL )
	{
		if( ( *layer )->file_path != NULL )
		{
			memory_free(
			 ( *layer )->file_path );
		}
		memory_free(
		 *layer );

		*layer = NULL;
	}
	return( 1 );
}

