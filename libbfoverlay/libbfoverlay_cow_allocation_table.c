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
	( *cow_allocation_table )->number_of_blocks = number_of_blocks;

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

/* Reads the COW allocation table data
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_read_data(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_cow_allocation_table_read_data";

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
	if( ( data_size == 0 )
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
		 "%s: COW allocation table data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
/* TODO implement */

	return( 1 );
}

/* Reads the COW allocation table
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_read_file_io_handle(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t *cow_allocation_table_data    = NULL;
	static char *function                 = "libbfoverlay_cow_allocation_table_read_file_io_handle";
	size_t cow_allocation_table_data_size = 0; 
	ssize_t read_count                    = 0;

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
	if( ( cow_allocation_table->number_of_blocks == 0 )
	 || ( cow_allocation_table->number_of_blocks > ( (uint64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / 8 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid COW allocation table - number of blocks value exceed maximum allocation size.",
		 function );

		goto on_error;
	}
	cow_allocation_table_data_size = (size_t) cow_allocation_table->number_of_blocks * 8;

	cow_allocation_table_data = (uint8_t *) memory_allocate(
	                                         sizeof( uint8_t ) * cow_allocation_table_data_size );

	if( cow_allocation_table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create COW allocation table data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     cow_allocation_table_data,
	     0,
	     cow_allocation_table_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW allocation table data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading COW allocation table at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              cow_allocation_table_data,
	              cow_allocation_table_data_size,
	              0,
	              error );

	if( read_count != (ssize_t) cow_allocation_table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read COW allocation table data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( libbfoverlay_cow_allocation_table_read_data(
	     cow_allocation_table,
	     cow_allocation_table_data,
	     cow_allocation_table_data_size,
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
	memory_free(
	 cow_allocation_table_data );

	return( 1 );

on_error:
	if( cow_allocation_table_data != NULL )
	{
		memory_free(
		 cow_allocation_table_data );
	}
	return( -1 );
}

/* Writes the COW allocation table
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_cow_allocation_table_write_file_io_handle(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t *cow_allocation_table_data    = NULL;
	static char *function                 = "libbfoverlay_cow_allocation_table_write_file_io_handle";
	size_t cow_allocation_table_data_size = 0; 
	ssize_t write_count                   = 0;

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
	if( ( cow_allocation_table->number_of_blocks == 0 )
	 || ( cow_allocation_table->number_of_blocks > ( (uint64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / 8 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid COW allocation table - number of blocks value exceed maximum allocation size.",
		 function );

		goto on_error;
	}
	cow_allocation_table_data_size = (size_t) cow_allocation_table->number_of_blocks * 8;

	cow_allocation_table_data = (uint8_t *) memory_allocate(
	                                         sizeof( uint8_t ) * cow_allocation_table_data_size );

	if( cow_allocation_table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create COW allocation table data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     cow_allocation_table_data,
	     0,
	     cow_allocation_table_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear COW allocation table data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: writing COW allocation table at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	write_count = libbfio_handle_write_buffer_at_offset(
	              file_io_handle,
	              cow_allocation_table_data,
	              cow_allocation_table_data_size,
	              0,
	              error );

	if( write_count != (ssize_t) cow_allocation_table_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write COW allocation table data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	memory_free(
	 cow_allocation_table_data );

	return( 1 );

on_error:
	if( cow_allocation_table_data != NULL )
	{
		memory_free(
		 cow_allocation_table_data );
	}
	return( -1 );
}
