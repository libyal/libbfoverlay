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
	( *cow_file )->data_size  = data_size;
	( *cow_file )->block_size = 4096;

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

/* Opens a COW file
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_open(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	libbfoverlay_cow_file_header_t *file_header = NULL;
	static char *function                       = "libbfoverlay_cow_file_open";
	size64_t file_size                          = 0;
	off64_t first_data_block_offset             = 0;
	uint64_t number_of_blocks                   = 0;

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
	if( cow_file->allocation_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid COW file - allocation table value already set.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_size(
	     file_io_pool,
	     file_io_pool_entry,
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

		if( libbfoverlay_cow_file_header_write_file_io_pool(
		     file_header,
		     file_io_pool,
		     file_io_pool_entry,
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
		if( libbfoverlay_cow_file_header_read_file_io_pool(
		     file_header,
		     file_io_pool,
		     file_io_pool_entry,
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
	if( file_header->block_size != cow_file->block_size )
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

	if( ( number_of_blocks % file_header->block_size ) != 0 )
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
	     &( cow_file->allocation_table ),
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
	if( file_size > 0 )
	{
/* TODO move into libbfoverlay_cow_allocation_table_get_block_number_by_index ? */
		if( libbfoverlay_cow_allocation_table_read_file_io_pool(
		     cow_file->allocation_table,
		     file_io_pool,
		     file_io_pool_entry,
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
	first_data_block_offset = (off64_t) sizeof( bfoverlay_cow_file_header_t ) + ( (off64_t) number_of_blocks * 8 );
	first_data_block_offset = (off64_t) sizeof( bfoverlay_cow_file_header_t ) + ( (off64_t) number_of_blocks * 8 );

	cow_file->first_data_block_number = (uint64_t) first_data_block_offset / cow_file->block_size;
	cow_file->last_data_block_number  = (uint64_t) file_size / cow_file->block_size;

	return( 1 );

on_error:
	if( file_header != NULL )
	{
		libbfoverlay_cow_file_header_free(
		 &file_header,
		 NULL );
	}
	if( cow_file->allocation_table != NULL )
	{
		libbfoverlay_cow_allocation_table_free(
		 &( cow_file->allocation_table ),
		 NULL );
	}
	return( -1 );
}

/* Closes a COW file
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_close(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
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
	if( libbfoverlay_cow_allocation_table_free(
	     &( cow_file->allocation_table ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free COW allocation table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Allocates a new data block for a specific offset
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_allocate_block_for_offset(
     libbfoverlay_cow_file_t *cow_file,
     off64_t offset,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t *file_offset,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_file_allocate_block_for_offset";
	off64_t table_index   = 0;
	uint64_t block_number = 0;

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
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( file_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file offset.",
		 function );

		return( -1 );
	}
	table_index = offset / cow_file->block_size;

	if( libbfoverlay_cow_allocation_table_get_block_number_by_index(
	     cow_file->allocation_table,
	     table_index,
	     &block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve COW block number: %" PRIi64 ".",
		 function,
		 table_index );

		return( -1 );
	}
	if( block_number != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid COW block number: %" PRIu64 " value already set.",
		 function,
		 table_index );

		return( -1 );
	}
	cow_file->last_data_block_number += 1;

	if( libbfoverlay_cow_allocation_table_set_block_number_by_index(
	     cow_file->allocation_table,
	     file_io_pool,
	     file_io_pool_entry,
	     table_index,
	     cow_file->last_data_block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set COW block number: %" PRIi64 ".",
		 function,
		 table_index );

		return( -1 );
	}
	*file_offset = cow_file->last_data_block_number * cow_file->block_size;

	return( 1 );
}

/* Retrieve the data block for a specific offset
 * Returns 1 if successful, 0 if no data block exists or -1 on error
 */
int libbfoverlay_cow_file_get_block_at_offset(
     libbfoverlay_cow_file_t *cow_file,
     off64_t offset,
     off64_t *range_start_offset,
     off64_t *range_end_offset,
     off64_t *file_offset,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_file_get_block_at_offset";
	off64_t table_index   = 0;
	uint64_t block_number = 0;

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
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( range_start_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range start offset.",
		 function );

		return( -1 );
	}
	if( range_end_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range end offset.",
		 function );

		return( -1 );
	}
	if( file_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file offset.",
		 function );

		return( -1 );
	}
	table_index = offset / cow_file->block_size;

	if( libbfoverlay_cow_allocation_table_get_block_number_by_index(
	     cow_file->allocation_table,
	     table_index,
	     &block_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve COW block number: %" PRIi64 ".",
		 function,
		 table_index );

		return( -1 );
	}
	if( ( block_number != 0 )
	 && ( ( block_number < cow_file->first_data_block_number )
	  ||  ( block_number > cow_file->last_data_block_number ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid COW block number: %" PRIu64 " value: %" PRIu64 " out of bounds.",
		 function,
		 table_index,
		 block_number );

		return( -1 );
	}
	*file_offset        = block_number * cow_file->block_size;
	*range_start_offset = (off64_t) table_index * cow_file->block_size;
	*range_end_offset   = ( (off64_t) table_index * cow_file->block_size ) + cow_file->block_size;

	if( block_number == 0 )
	{
		return( 0 );
	}
	return( 1 );
}

