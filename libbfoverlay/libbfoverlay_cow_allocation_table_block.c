/*
 * Copy-on-write (COW) allocation table block functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libbfoverlay_cow_allocation_table_block.h"
#include "libbfoverlay_definitions.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libcnotify.h"

#include "bfoverlay_cow_allocation_table_block.h"

/* Creates COW allocation table block
 * Make sure the value cow_allocation_table_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_block_initialize(
     libbfoverlay_cow_allocation_table_block_t **cow_allocation_table_block,
     size32_t block_size,
     libcerror_error_t **error )
{
	static char *function    = "libbfoverlay_cow_allocation_table_block_initialize";
	size_t number_of_entries = 0;

	if( cow_allocation_table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table block.",
		 function );

		return( -1 );
	}
	if( *cow_allocation_table_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid COW allocation table block value already set.",
		 function );

		return( -1 );
	}
	if( (size_t) block_size < sizeof( bfoverlay_cow_allocation_table_block_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
	number_of_entries = ( (size_t) block_size - sizeof( bfoverlay_cow_allocation_table_block_header_t ) ) / sizeof( bfoverlay_cow_allocation_table_block_entry_t );

	if( ( number_of_entries == 0 )
	 || ( number_of_entries > (size_t) INT_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		return( -1 );
	}
	*cow_allocation_table_block = memory_allocate_structure(
	                               libbfoverlay_cow_allocation_table_block_t );

	if( *cow_allocation_table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create COW allocation table block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *cow_allocation_table_block,
	     0,
	     sizeof( libbfoverlay_cow_allocation_table_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW allocation table block.",
		 function );

		goto on_error;
	}
	( *cow_allocation_table_block )->number_of_entries = (int) number_of_entries;

	return( 1 );

on_error:
	if( *cow_allocation_table_block != NULL )
	{
		memory_free(
		 *cow_allocation_table_block );

		*cow_allocation_table_block = NULL;
	}
	return( -1 );
}

/* Frees COW allocation table block
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_block_free(
     libbfoverlay_cow_allocation_table_block_t **cow_allocation_table_block,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_allocation_table_block_free";

	if( cow_allocation_table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table block.",
		 function );

		return( -1 );
	}
	if( *cow_allocation_table_block != NULL )
	{
		memory_free(
		 *cow_allocation_table_block );

		*cow_allocation_table_block = NULL;
	}
	return( 1 );
}

/* Retrieves a specific block number
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_block_get_block_number_by_index(
     libbfoverlay_cow_allocation_table_block_t *cow_allocation_table_block,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     int entry_index,
     uint32_t *block_number,
     libcerror_error_t **error )
{
	uint8_t cow_allocation_table_block_entry_data[ sizeof( bfoverlay_cow_allocation_table_block_entry_t ) ];

	static char *function      = "libbfoverlay_cow_allocation_table_block_get_block_number_by_index";
	ssize_t read_count         = 0;
	uint32_t safe_block_number = 0;

	if( cow_allocation_table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table block.",
		 function );

		return( -1 );
	}
	if( ( entry_index < 0 )
	 || ( entry_index >= cow_allocation_table_block->number_of_entries ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
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
	file_offset += entry_index * sizeof( bfoverlay_cow_allocation_table_block_entry_t );

	read_count = libbfio_pool_read_buffer_at_offset(
	             file_io_pool,
	             file_io_pool_entry,
	             cow_allocation_table_block_entry_data,
	             sizeof( bfoverlay_cow_allocation_table_block_entry_t ),
	             file_offset,
	             error );

	if( read_count != (ssize_t) sizeof( bfoverlay_cow_allocation_table_block_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COW allocation table block entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 entry_index,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 cow_allocation_table_block_entry_data,
	 safe_block_number );

	*block_number = safe_block_number;

	return( 1 );
}

/* Sets a specific block number
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_block_set_block_number_by_index(
     libbfoverlay_cow_allocation_table_block_t *cow_allocation_table_block,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     int entry_index,
     uint32_t block_number,
     uint8_t write_header,
     libcerror_error_t **error )
{
	uint8_t cow_allocation_table_block_entry_data[ sizeof( bfoverlay_cow_allocation_table_block_entry_t ) ];
	uint8_t cow_allocation_table_block_header_data[ sizeof( bfoverlay_cow_allocation_table_block_header_t ) ];

	static char *function = "libbfoverlay_cow_allocation_table_block_set_block_number_by_index";
	ssize_t write_count   = 0;

	if( cow_allocation_table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW allocation table block.",
		 function );

		return( -1 );
	}
	if( ( entry_index < 0 )
	 || ( entry_index >= cow_allocation_table_block->number_of_entries ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( write_header != 0 )
	{
		if( memory_copy(
		     cow_allocation_table_block_header_data,
		     "# BFO-COW-AB",
		     12 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy COW allocation table block signature.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     &( cow_allocation_table_block_header_data[ 12 ] ),
		     0,
		     sizeof( bfoverlay_cow_allocation_table_block_header_t ) - 12 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear COW allocation table block header data.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_big_endian(
		 ( (bfoverlay_cow_allocation_table_block_header_t *) cow_allocation_table_block_header_data )->format_version,
		 LIBBFOVERLAY_COW_FILE_FORMAT_VERSION );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: writing COW allocation table block header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 file_offset,
			 file_offset );
		}
#endif
		write_count = libbfio_pool_write_buffer_at_offset(
		              file_io_pool,
		              file_io_pool_entry,
		              cow_allocation_table_block_header_data,
		              sizeof( bfoverlay_cow_allocation_table_block_header_t ),
		              file_offset,
		              error );

		if( write_count != (ssize_t) sizeof( bfoverlay_cow_allocation_table_block_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write COW allocation table block header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 file_offset,
			 file_offset );

			return( -1 );
		}
	}
	byte_stream_copy_from_uint32_big_endian(
	 cow_allocation_table_block_entry_data,
	 block_number );

	file_offset += entry_index * sizeof( bfoverlay_cow_allocation_table_block_entry_t );

	write_count = libbfio_pool_write_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              cow_allocation_table_block_entry_data,
	              sizeof( bfoverlay_cow_allocation_table_block_entry_t ),
	              file_offset,
	              error );

	if( write_count != (ssize_t) sizeof( bfoverlay_cow_allocation_table_block_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write COW allocation table block entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 entry_index,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

