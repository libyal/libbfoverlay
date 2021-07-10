/*
 * Copy-on-write (COW) file functions
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

#include "libbfoverlay_cow_allocation_table.h"
#include "libbfoverlay_cow_file.h"
#include "libbfoverlay_cow_file_header.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"

#include "bfoverlay_cow_file_header.h"

/* Creates a COW file
 * Make sure the value cow_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_initialize(
     libbfoverlay_cow_file_t **cow_file,
     size64_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_file_initialize";

	if( cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file.",
		 function );

		return( -1 );
	}
	if( *cow_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid COW file value already set.",
		 function );

		return( -1 );
	}
	*cow_file = memory_allocate_structure(
	             libbfoverlay_cow_file_t );

	if( *cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create COW file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *cow_file,
	     0,
	     sizeof( libbfoverlay_cow_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW file.",
		 function );

		memory_free(
		 *cow_file );

		*cow_file = NULL;

		return( -1 );
	}
	( *cow_file )->data_size = data_size;

	return( 1 );

on_error:
	if( *cow_file != NULL )
	{
		memory_free(
		 *cow_file );

		*cow_file = NULL;
	}
	return( -1 );
}

/* Frees a COW file
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_free(
     libbfoverlay_cow_file_t **cow_file,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_file_free";
	int result            = 1;

	if( cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file.",
		 function );

		return( -1 );
	}
	if( *cow_file != NULL )
	{
		memory_free(
		 *cow_file );

		*cow_file = NULL;
	}
	return( result );
}

/* Opens a COW file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_open_file_io_handle(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libbfoverlay_cow_allocation_table_t *allocation_table = NULL;
	libbfoverlay_cow_file_header_t *file_header           = NULL;
	static char *function                                 = "libbfoverlay_cow_file_open_file_io_handle";
	size64_t file_size                                    = 0;
	uint64_t number_of_blocks                             = 0;

	if( cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_size(
	     file_io_handle,
	     &file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		goto on_error;
	}
	if( libbfoverlay_cow_file_header_initialize(
	     &file_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create COW file header.",
		 function );

		goto on_error;
	}
	if( file_size == 0 )
	{
		file_header->data_size = cow_file->data_size;

		if( libbfoverlay_cow_file_header_write_file_io_handle(
		     file_header,
		     file_io_handle,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write COW file header.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libbfoverlay_cow_file_header_read_file_io_handle(
		     file_header,
		     file_io_handle,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read COW file header.",
			 function );

			goto on_error;
		}
	}
	if( file_header->data_size != cow_file->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid COW file data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( file_header->block_size != 4096 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid COW file block size value out of bounds.",
		 function );

		return( -1 );
	}
	number_of_blocks = (uint64_t) file_header->data_size / file_header->block_size;

	if( ( number_of_blocks % 4096 ) != 0 )
	{
		number_of_blocks += 1;
	}
	if( libbfoverlay_cow_file_header_free(
	     &file_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free COW file header.",
		 function );

		goto on_error;
	}
	if( libbfoverlay_cow_allocation_table_initialize(
	     &allocation_table,
	     number_of_blocks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create COW allocation table.",
		 function );

		goto on_error;
	}
	if( file_size == 0 )
	{
		if( libbfoverlay_cow_allocation_table_write_file_io_handle(
		     allocation_table,
		     file_io_handle,
		     (off64_t) sizeof( bfoverlay_cow_file_header_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write COW allocation table.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libbfoverlay_cow_allocation_table_read_file_io_handle(
		     allocation_table,
		     file_io_handle,
		     (off64_t) sizeof( bfoverlay_cow_file_header_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read COW allocation table.",
			 function );

			goto on_error;
		}
	}
	if( libbfoverlay_cow_allocation_table_free(
	     &allocation_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free COW allocation table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( allocation_table != NULL )
	{
		libbfoverlay_cow_allocation_table_free(
		 &allocation_table,
		 NULL );
	}
	if( file_header != NULL )
	{
		libbfoverlay_cow_file_header_free(
		 &file_header,
		 NULL );
	}
	return( -1 );
}

/* Closes a COW file
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_close(
     libbfoverlay_cow_file_t *cow_file,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_file_close";

	if( cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file.",
		 function );

		return( -1 );
	}
/* TODO implement */
	return( 1 );
}

