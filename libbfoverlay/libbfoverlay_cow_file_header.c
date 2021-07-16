/*
 * Copy-on-write (COW) file header functions
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

#include "libbfoverlay_cow_file_header.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libcnotify.h"

#include "bfoverlay_cow_file_header.h"

/* Creates COW file header
 * Make sure the value cow_file_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_header_initialize(
     libbfoverlay_cow_file_header_t **cow_file_header,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_file_header_initialize";

	if( cow_file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file header.",
		 function );

		return( -1 );
	}
	if( *cow_file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid COW file header value already set.",
		 function );

		return( -1 );
	}
	*cow_file_header = memory_allocate_structure(
	                    libbfoverlay_cow_file_header_t );

	if( *cow_file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create COW file header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *cow_file_header,
	     0,
	     sizeof( libbfoverlay_cow_file_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW file header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *cow_file_header != NULL )
	{
		memory_free(
		 *cow_file_header );

		*cow_file_header = NULL;
	}
	return( -1 );
}

/* Frees COW file header
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_header_free(
     libbfoverlay_cow_file_header_t **cow_file_header,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_file_header_free";

	if( cow_file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file header.",
		 function );

		return( -1 );
	}
	if( *cow_file_header != NULL )
	{
		memory_free(
		 *cow_file_header );

		*cow_file_header = NULL;
	}
	return( 1 );
}

/* Reads the COW file header data
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_header_read_data(
     libbfoverlay_cow_file_header_t *cow_file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libbfoverlay_cow_file_header_read_data";
	uint32_t format_version = 0;

	if( cow_file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file header.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( bfoverlay_cow_file_header_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: COW file header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( bfoverlay_cow_file_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (bfoverlay_cow_file_header_t *) data )->signature,
	     "# BFO-COW-FH",
	     12 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (bfoverlay_cow_file_header_t *) data )->format_version,
	 format_version );

	byte_stream_copy_to_uint32_big_endian(
	 ( (bfoverlay_cow_file_header_t *) data )->block_size,
	 cow_file_header->block_size );

	byte_stream_copy_to_uint64_big_endian(
	 ( (bfoverlay_cow_file_header_t *) data )->data_size,
	 cow_file_header->data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t: %s\n",
		 function,
		 ( (bfoverlay_cow_file_header_t *) data )->signature );

		libcnotify_printf(
		 "%s: format version\t\t\t: %" PRIu32 "\n",
		 function,
		 format_version );

		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu64 "\n",
		 function,
		 cow_file_header->data_size );

		libcnotify_printf(
		 "%s: block size\t\t\t: %" PRIu32 "\n",
		 function,
		 cow_file_header->block_size );

		libcnotify_printf(
		 "%s: padding:\n",
		 function );
		libcnotify_print_data(
		 ( (bfoverlay_cow_file_header_t *) data )->padding,
		 4,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version != 20210716 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported format version: %" PRIu32 ".",
		 function,
		 format_version );

		return( -1 );
	}
	return( 1 );
}

/* Reads the COW file header
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_header_read_file_io_pool(
     libbfoverlay_cow_file_header_t *cow_file_header,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t cow_file_header_data[ sizeof( bfoverlay_cow_file_header_t ) ];

	static char *function = "libbfoverlay_cow_file_header_read_file_io_pool";
	ssize_t read_count    = 0;

	if( cow_file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading COW file header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_pool_read_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              cow_file_header_data,
	              sizeof( bfoverlay_cow_file_header_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( bfoverlay_cow_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COW file header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libbfoverlay_cow_file_header_read_data(
	     cow_file_header,
	     cow_file_header_data,
	     sizeof( bfoverlay_cow_file_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COW file header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Writes the COW file header
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_file_header_write_file_io_pool(
     libbfoverlay_cow_file_header_t *cow_file_header,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t cow_allocation_table_entry_data[ 8 ];
	uint8_t cow_file_header_data[ sizeof( bfoverlay_cow_file_header_t ) ];

	static char *function = "libbfoverlay_cow_file_header_write_file_io_pool";
	ssize_t write_count   = 0;

	if( cow_file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid COW file header.",
		 function );

		return( -1 );
	}
	cow_file_header->block_size = 4096;

	if( memory_copy(
	     cow_file_header_data,
	     "# BFO-COW-FH",
	     12 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy COW file signature.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &( cow_file_header_data[ 12 ] ),
	     0,
	     sizeof( bfoverlay_cow_file_header_t ) - 12 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW file header data.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint32_big_endian(
	 ( (bfoverlay_cow_file_header_t *) cow_file_header_data )->format_version,
	 20210716 );

	byte_stream_copy_from_uint64_big_endian(
	 ( (bfoverlay_cow_file_header_t *) cow_file_header_data )->data_size,
	 cow_file_header->data_size );

	byte_stream_copy_from_uint32_big_endian(
	 ( (bfoverlay_cow_file_header_t *) cow_file_header_data )->block_size,
	 cow_file_header->block_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: writing COW file header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	write_count = libbfio_pool_write_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              cow_file_header_data,
	              sizeof( bfoverlay_cow_file_header_t ),
	              file_offset,
	              error );

	if( write_count != (ssize_t) sizeof( bfoverlay_cow_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write COW file header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( memory_set(
	     cow_allocation_table_entry_data,
	     0,
	     8 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW allocation table entry data.",
		 function );

		return( -1 );
	}
	write_count = libbfio_pool_write_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              cow_allocation_table_entry_data,
	              8,
	              cow_file_header->block_size - 8,
	              error );

	if( write_count != (ssize_t) 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write COW allocation table data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 (off64_t) ( file_offset + sizeof( bfoverlay_cow_file_header_t ) ),
		 (off64_t) ( file_offset + sizeof( bfoverlay_cow_file_header_t ) ) );

		return( -1 );
	}
	return( 1 );
}

