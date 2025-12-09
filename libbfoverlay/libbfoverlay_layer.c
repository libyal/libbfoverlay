/*
 * Extent functions
 *
 * Copyright (C) 2020-2025, Joachim Metz <joachim.metz@gmail.com>
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

#include "libbfoverlay_definitions.h"
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
	( *layer )->size        = -1;
	( *layer )->file_offset = -1;

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
		if( ( *layer )->data_file_path != NULL )
		{
			memory_free(
			 ( *layer )->data_file_path );
		}
		memory_free(
		 *layer );

		*layer = NULL;
	}
	return( 1 );
}

/* Sets the data file path
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_layer_set_data_file_path(
     libbfoverlay_layer_t *layer,
     const uint8_t *path,
     size_t path_size,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_layer_set_data_file_path";

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
	if( layer->data_file_path != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid layer - data file path value already set.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	if( ( path_size == 0 )
	 || ( path_size > LIBBFOVERLAY_MAXIMUM_PATH_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid path size value out of bounds.",
		 function );

		goto on_error;
	}
	layer->data_file_path = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * path_size );

	if( layer->data_file_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data file path.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     layer->data_file_path,
	     path,
	     path_size - 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy path.",
		 function );

		goto on_error;
	}
	layer->data_file_path[ path_size - 1 ] = 0;

	layer->data_file_path_size = path_size;

	return( 1 );

on_error:
	if( layer->data_file_path != NULL )
	{
		memory_free(
		 layer->data_file_path );

		layer->data_file_path = NULL;
	}
	layer->data_file_path_size = 0;

	return( -1 );
}

