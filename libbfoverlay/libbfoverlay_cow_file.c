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

#include "libbfoverlay_cow_allocation_table_block.h"
#include "libbfoverlay_cow_file.h"
#include "libbfoverlay_cow_file_header.h"
#include "libbfoverlay_definitions.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libcnotify.h"

#include "bfoverlay_cow_allocation_table_block.h"
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
		if( ( *cow_file )->allocation_table_block != NULL )
		{
			if( libbfoverlay_cow_file_close(
			     *cow_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close COW file.",
				 function );

				result = -1;
			}
		}
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
	if( cow_file->allocation_table_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid COW file - allocation table block value already set.",
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
	if( ( (size_t) file_header->block_size < sizeof( bfoverlay_cow_file_header_t ) )
	 || ( file_header->block_size != cow_file->block_size ) )
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

	if( number_of_blocks > ( (uint64_t) UINT32_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of blocks value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( number_of_blocks % file_header->block_size ) != 0 )
	{
		number_of_blocks += 1;
	}
	cow_file->data_size                  = file_header->data_size;
	cow_file->number_of_allocated_blocks = file_header->number_of_allocated_blocks;
	cow_file->last_data_block_number     = (uint32_t) ( file_size / cow_file->block_size );
	cow_file->l1_allocation_table_offset = (off64_t) sizeof( bfoverlay_cow_file_header_t );

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
	if( libbfoverlay_cow_allocation_table_block_initialize(
	     &( cow_file->allocation_table_block ),
	     cow_file->block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create COW allocation table block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_header != NULL )
	{
		libbfoverlay_cow_file_header_free(
		 &file_header,
		 NULL );
	}
	if( cow_file->allocation_table_block != NULL )
	{
		libbfoverlay_cow_allocation_table_block_free(
		 &( cow_file->allocation_table_block ),
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
	if( libbfoverlay_cow_allocation_table_block_free(
	     &( cow_file->allocation_table_block ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free COW allocation table block.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the data size in the file header
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_set_data_size(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     size64_t data_size,
     libcerror_error_t **error )
{
	uint8_t cow_file_header_data[ 8 ];

	static char *function = "libbfoverlay_cow_file_set_data_size";
	ssize_t write_count   = 0;
	off64_t file_offset   = 0;

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu64 "\n",
		 function,
		 data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_from_uint64_big_endian(
	 cow_file_header_data,
	 data_size );

	file_offset = 16;

	write_count = libbfio_pool_write_buffer_at_offset(
	               file_io_pool,
	               file_io_pool_entry,
	               cow_file_header_data,
	               8,
	               file_offset,
	               error );

	if( write_count != (ssize_t) 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write data size at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	cow_file->data_size = data_size;

	return( 1 );
}

/* Enlarges the allocation table
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_enlarge_allocation_table(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error )
{
	uint8_t *block_data   = NULL;
	static char *function = "libbfoverlay_cow_file_enlarge_allocation_table";
	ssize_t read_count    = 0;
	ssize_t write_count   = 0;
	off64_t file_offset   = 0;

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
	if( cow_file->allocation_table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid COW file - missing allocation table block.",
		 function );

		return( -1 );
	}
	if( ( cow_file->number_of_allocated_blocks == 0 )
	 || ( cow_file->number_of_allocated_blocks > ( (uint32_t) UINT32_MAX / cow_file->allocation_table_block->number_of_entries ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of allocated blocks value out of bounds.",
		 function );

		return( -1 );
	}
	block_data = (uint8_t *) memory_allocate(
	                          cow_file->block_size );

	if( block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     block_data,
	     0,
	     cow_file->block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block data.",
		 function );

		goto on_error;
	}
	/* Read COW file header block with level 1 allocation table
	 */
	read_count = libbfio_pool_read_buffer_at_offset(
	             file_io_pool,
	             file_io_pool_entry,
	             block_data,
	             (size_t) cow_file->block_size,
	             file_offset,
	             error );

	if( read_count != (ssize_t) cow_file->block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	/* Copy the level 1 allocation to a COW level 2 allocation table block
	 */
	if( memory_copy(
	     block_data,
	     "# BFO-COW-AB",
	     12 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy COW allocation table block signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_big_endian(
	 &( block_data[ 12 ] ),
	 LIBBFOVERLAY_COW_FILE_FORMAT_VERSION );

	if( memory_set(
	     &( block_data[ 16 ] ),
	     0,
	     sizeof( bfoverlay_cow_allocation_table_block_header_t ) - 16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW allocation table block header data.",
		 function );

		goto on_error;
	}
	cow_file->last_data_block_number += 1;

	file_offset = (off64_t) cow_file->last_data_block_number * cow_file->block_size;

	/* Write a COW level 2 allocation table block
	 */
	write_count = libbfio_pool_write_buffer_at_offset(
	               file_io_pool,
	               file_io_pool_entry,
	               block_data,
	               (size_t) cow_file->block_size,
	               file_offset,
	               error );

	if( write_count != (ssize_t) cow_file->block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write COW allocation table block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	/* Update the number of allocated blocks and the level 1 allocation table
	 */
	if( memory_set(
	     block_data,
	     0,
	     cow_file->block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block data.",
		 function );

		goto on_error;
	}
	cow_file->number_of_allocated_blocks *= cow_file->allocation_table_block->number_of_entries;

	byte_stream_copy_to_uint32_big_endian(
	 &( block_data[ 28 ] ),
	 cow_file->number_of_allocated_blocks );

	byte_stream_copy_to_uint32_big_endian(
	 &( block_data[ 32 ] ),
	 cow_file->last_data_block_number );

	file_offset = 28;

	write_count = libbfio_pool_write_buffer_at_offset(
	               file_io_pool,
	               file_io_pool_entry,
	               &( block_data[ 28 ] ),
	               cow_file->block_size - 28,
	               file_offset,
	               error );

	if( write_count != (ssize_t) cow_file->block_size - 28 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	memory_free(
	 block_data );

	return( 1 );

on_error:
	if( block_data != NULL )
	{
		memory_free(
		 block_data );
	}
	return( -1 );
}

/* Allocates a new data block for a specific offset
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_allocate_block_for_offset(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t offset,
     off64_t *file_offset,
     libcerror_error_t **error )
{
	uint8_t cow_allocation_table_entry_data[ sizeof( bfoverlay_cow_allocation_table_block_entry_t ) ];

	static char *function     = "libbfoverlay_cow_file_allocate_block_for_offset";
	ssize_t read_count        = 0;
	ssize_t write_count       = 0;
	off64_t safe_file_offset  = 0;
	off64_t table_index       = 0;
	uint32_t block_number     = 0;
	uint32_t blocks_per_entry = 0;
	uint8_t write_header      = 0;
	int depth                 = 0;
	int entry_index           = 0;

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
	if( cow_file->allocation_table_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid COW file - missing allocation table block.",
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

	if( table_index >= (off64_t) cow_file->number_of_allocated_blocks )
	{
		if( libbfoverlay_cow_file_enlarge_allocation_table(
		     cow_file,
		     file_io_pool,
		     file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to enlarge allocation table.",
			 function );

			return( -1 );
		}
	}
	blocks_per_entry = cow_file->number_of_allocated_blocks / cow_file->allocation_table_block->number_of_entries;
	entry_index      = table_index / blocks_per_entry;
	safe_file_offset = cow_file->l1_allocation_table_offset + ( entry_index * sizeof( bfoverlay_cow_allocation_table_block_entry_t ) );

	read_count = libbfio_pool_read_buffer_at_offset(
	             file_io_pool,
	             file_io_pool_entry,
	             cow_allocation_table_entry_data,
	             sizeof( bfoverlay_cow_allocation_table_block_entry_t ),
	             safe_file_offset,
	             error );

	if( read_count != (ssize_t) sizeof( bfoverlay_cow_allocation_table_block_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read level 1 COW allocation table entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 entry_index,
		 safe_file_offset,
		 safe_file_offset );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 cow_allocation_table_entry_data,
	 block_number );

	if( block_number == LIBBFOVERLAY_COW_BLOCK_NUMBER_NOT_SET )
	{
		cow_file->last_data_block_number += 1;

		block_number = cow_file->last_data_block_number;

		byte_stream_copy_from_uint32_big_endian(
		 cow_allocation_table_entry_data,
		 block_number );

		write_count = libbfio_pool_write_buffer_at_offset(
		               file_io_pool,
		               file_io_pool_entry,
		               cow_allocation_table_entry_data,
		               sizeof( bfoverlay_cow_allocation_table_block_entry_t ),
		               safe_file_offset,
		               error );

		if( write_count != (ssize_t) sizeof( bfoverlay_cow_allocation_table_block_entry_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write COW level 1 allocation table entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 entry_index,
			 safe_file_offset,
			 safe_file_offset );

			return( -1 );
		}
	}
	depth = 2;

	while( blocks_per_entry > 1 )
	{
		safe_file_offset = (off64_t) block_number * cow_file->block_size;

		table_index      -= entry_index * blocks_per_entry;
		blocks_per_entry /= cow_file->allocation_table_block->number_of_entries;
		entry_index       = table_index / blocks_per_entry;

		if( libbfoverlay_cow_allocation_table_block_get_block_number_by_index(
		     cow_file->allocation_table_block,
		     file_io_pool,
		     file_io_pool_entry,
		     safe_file_offset,
		     entry_index,
		     &block_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve level %d COW allocation table entry: %d at offset %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 depth,
			 entry_index,
			 safe_file_offset,
			 safe_file_offset );

			return( -1 );
		}
		if( block_number == LIBBFOVERLAY_COW_BLOCK_NUMBER_NOT_SET )
		{
			cow_file->last_data_block_number += 1;

			block_number = cow_file->last_data_block_number;

			if( libbfoverlay_cow_allocation_table_block_set_block_number_by_index(
			     cow_file->allocation_table_block,
			     file_io_pool,
			     file_io_pool_entry,
			     safe_file_offset,
			     entry_index,
			     block_number,
			     write_header,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set level %d COW allocation table entry: %d at offset %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 depth,
				 entry_index,
				 safe_file_offset,
				 safe_file_offset );

				return( -1 );
			}
			write_header = 1;
		}
		depth++;
	}
	*file_offset = (off64_t) cow_file->last_data_block_number * cow_file->block_size;

	return( 1 );
}

/* Retrieve the data block for a specific offset
 * Returns 1 if successful, 0 if no data block exists or -1 on error
 */
int libbfoverlay_cow_file_get_block_at_offset(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t offset,
     off64_t *range_start_offset,
     off64_t *range_end_offset,
     off64_t *file_offset,
     libcerror_error_t **error )
{
	uint8_t cow_allocation_table_entry_data[ sizeof( bfoverlay_cow_allocation_table_block_entry_t ) ];

	static char *function     = "libbfoverlay_cow_file_get_block_at_offset";
	ssize_t read_count        = 0;
	off64_t safe_file_offset  = 0;
	off64_t table_index       = 0;
	uint32_t block_number     = 0;
	uint32_t blocks_per_entry = 0;
	int depth                 = 0;
	int entry_index           = 0;

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

	blocks_per_entry = cow_file->number_of_allocated_blocks / cow_file->allocation_table_block->number_of_entries;
	entry_index      = table_index / blocks_per_entry;
	safe_file_offset = cow_file->l1_allocation_table_offset + ( entry_index * sizeof( bfoverlay_cow_allocation_table_block_entry_t ) );

	read_count = libbfio_pool_read_buffer_at_offset(
	             file_io_pool,
	             file_io_pool_entry,
	             cow_allocation_table_entry_data,
	             sizeof( bfoverlay_cow_allocation_table_block_entry_t ),
	             safe_file_offset,
	             error );

	if( read_count != (ssize_t) sizeof( bfoverlay_cow_allocation_table_block_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read level 1 COW allocation table entry: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 entry_index,
		 safe_file_offset,
		 safe_file_offset );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 cow_allocation_table_entry_data,
	 block_number );

	if( ( block_number != LIBBFOVERLAY_COW_BLOCK_NUMBER_NOT_SET )
	 && ( block_number > cow_file->last_data_block_number ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid level 1 COW allocation table entry: %d block number value: %" PRIu32 " out of bounds.",
		 function,
		 entry_index,
		 block_number );

		return( -1 );
	}
	depth = 2;

	while( blocks_per_entry > 1 )
	{
		safe_file_offset = (off64_t) block_number * cow_file->block_size;

		table_index      -= entry_index * blocks_per_entry;
		blocks_per_entry /= cow_file->allocation_table_block->number_of_entries;
		entry_index       = table_index / blocks_per_entry;

		if( libbfoverlay_cow_allocation_table_block_get_block_number_by_index(
		     cow_file->allocation_table_block,
		     file_io_pool,
		     file_io_pool_entry,
		     safe_file_offset,
		     entry_index,
		     &block_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve level %d COW allocation table entry: %d at offset %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 depth,
			 entry_index,
			 safe_file_offset,
			 safe_file_offset );

			return( -1 );
		}
		if( ( block_number != LIBBFOVERLAY_COW_BLOCK_NUMBER_NOT_SET )
		 && ( block_number > cow_file->last_data_block_number ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid level %d COW allocation table entry: %d at offset %" PRIi64 " (0x%08" PRIx64 ") block number value: %" PRIu32 " out of bounds.",
			 function,
			 depth,
			 entry_index,
			 safe_file_offset,
			 safe_file_offset,
			 block_number );

			return( -1 );
		}
		depth++;
	}
	*file_offset        = (off64_t) block_number * cow_file->block_size;
	*range_start_offset = (off64_t) table_index * cow_file->block_size;
	*range_end_offset   = ( (off64_t) table_index * cow_file->block_size ) + cow_file->block_size;

	if( block_number == LIBBFOVERLAY_COW_BLOCK_NUMBER_NOT_SET )
	{
		return( 0 );
	}
	return( 1 );
}

