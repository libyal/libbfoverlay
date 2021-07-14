/*
 * Copy-on-write (COW) allocation table functions
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
#include <memory.h>
#include <types.h>

#include "libbfoverlay_cow_allocation_table.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libcnotify.h"

/* Creates COW allocation table
 * Make sure the value cow_allocation_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_initialize(
     libbfoverlay_cow_allocation_table_t **cow_allocation_table,
     off64_t l1_allocation_table_offset,
     uint64_t number_of_blocks,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_allocation_table_initialize";

	if( cow_allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table.",
		 function );

		return( -1 );
	}
	if( *cow_allocation_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid COW allocation table value already set.",
		 function );

		return( -1 );
	}
	if( ( number_of_blocks == 0 )
	 || ( number_of_blocks > ( (uint64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / 8 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of blocks value exceed maximum allocation size.",
		 function );

		return( -1 );
	}
	*cow_allocation_table = memory_allocate_structure(
	                         libbfoverlay_cow_allocation_table_t );

	if( *cow_allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create COW allocation table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *cow_allocation_table,
	     0,
	     sizeof( libbfoverlay_cow_allocation_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW allocation table.",
		 function );

		goto on_error;
	}
	( *cow_allocation_table )->l1_allocation_table_offset = l1_allocation_table_offset;
	( *cow_allocation_table )->number_of_blocks           = number_of_blocks;

	return( 1 );

on_error:
	if( *cow_allocation_table != NULL )
	{
		memory_free(
		 *cow_allocation_table );

		*cow_allocation_table = NULL;
	}
	return( -1 );
}

/* Frees COW allocation table
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_free(
     libbfoverlay_cow_allocation_table_t **cow_allocation_table,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_allocation_table_free";

	if( cow_allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table.",
		 function );

		return( -1 );
	}
	if( *cow_allocation_table != NULL )
	{
		memory_free(
		 *cow_allocation_table );

		*cow_allocation_table = NULL;
	}
	return( 1 );
}

/* Retrieves a specific block number
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_get_block_number_by_index(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     int table_index,
     uint64_t *block_number,
     libcerror_error_t **error )
{
	uint8_t cow_allocation_table_entry_data[ 8 ];

	static char *function      = "libbfoverlay_cow_allocation_table_get_block_number_by_index";
	ssize_t read_count         = 0;
	off64_t file_offset        = 0;
	uint64_t safe_block_number = 0;

	if( cow_allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table.",
		 function );

		return( -1 );
	}
	if( ( table_index < 0 )
	 || ( (uint64_t) table_index >= cow_allocation_table->number_of_blocks ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table index value out of bounds.",
		 function );

		return( -1 );
	}
	if( block_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block number.",
		 function );

		return( -1 );
	}
	file_offset = cow_allocation_table->l1_allocation_table_offset + ( table_index * 8 );

	read_count = libbfio_pool_read_buffer_at_offset(
	             file_io_pool,
	             file_io_pool_entry,
	             cow_allocation_table_entry_data,
	             8,
	             file_offset,
	             error );

	if( read_count != (ssize_t) 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COW allocation table entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 table_index,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	byte_stream_copy_to_uint64_big_endian(
	 cow_allocation_table_entry_data,
	 safe_block_number );

	*block_number = safe_block_number;

	return( 1 );
}

/* Sets a specific block number
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_set_block_number_by_index(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     int table_index,
     uint64_t block_number,
     libcerror_error_t **error )
{
	uint8_t cow_allocation_table_entry_data[ 8 ];

	static char *function = "libbfoverlay_cow_allocation_table_set_block_number_by_index";
	ssize_t write_count   = 0;
	off64_t file_offset   = 0;

	if( cow_allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table.",
		 function );

		return( -1 );
	}
	if( ( table_index < 0 )
	 || ( (uint64_t) table_index >= cow_allocation_table->number_of_blocks ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table index value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint64_big_endian(
	 cow_allocation_table_entry_data,
	 block_number );

	file_offset = cow_allocation_table->l1_allocation_table_offset + ( table_index * 8 );

	write_count = libbfio_pool_write_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              cow_allocation_table_entry_data,
	              8,
	              file_offset,
	              error );

	if( write_count != (ssize_t) 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write COW allocation table entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 table_index,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

