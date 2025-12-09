/*
 * Handle functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libbfoverlay_definitions.h"
#include "libbfoverlay_descriptor_file.h"
#include "libbfoverlay_handle.h"
#include "libbfoverlay_layer.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcdata.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libcfile.h"
#include "libbfoverlay_libcnotify.h"
#include "libbfoverlay_libcthreads.h"
#include "libbfoverlay_libuna.h"
#include "libbfoverlay_range.h"
#include "libbfoverlay_types.h"

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_initialize(
     libbfoverlay_handle_t **handle,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	internal_handle = memory_allocate_structure(
	                   libbfoverlay_internal_handle_t );

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_handle,
	     0,
	     sizeof( libbfoverlay_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear handle.",
		 function );

		memory_free(
		 internal_handle );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_handle->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_handle->cow_file_io_pool_entry         = -1;
	internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

	*handle = (libbfoverlay_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		memory_free(
		 internal_handle );
	}
	return( -1 );
}

/* Frees a handle
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_free(
     libbfoverlay_handle_t **handle,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_free";
	int result                                      = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libbfoverlay_internal_handle_t *) *handle;

		if( ( internal_handle->file_io_handle != NULL )
		 || ( internal_handle->data_file_io_pool != NULL ) )
		{
			if( libbfoverlay_handle_close(
			     *handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		*handle = NULL;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
		if( libcthreads_read_write_lock_free(
		     &( internal_handle->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Signals a handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_signal_abort(
     libbfoverlay_handle_t *handle,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_signal_abort";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	internal_handle->abort = 1;

	return( 1 );
}

/* Opens a handle
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_open(
     libbfoverlay_handle_t *handle,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                = NULL;
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_open";
	size_t string_length                            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBFOVERLAY_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBFOVERLAY_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	string_length = narrow_string_length(
	                 filename );

	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     string_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libbfoverlay_handle_open_file_io_handle(
	     handle,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle: %s.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->file_io_handle_created_in_library = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a handle
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_open_wide(
     libbfoverlay_handle_t *handle,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                = NULL;
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_open_wide";
	size_t string_length                            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBFOVERLAY_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBFOVERLAY_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	string_length = wide_string_length(
	                 filename );

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     string_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libbfoverlay_handle_open_file_io_handle(
	     handle,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle: %ls.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->file_io_handle_created_in_library = 1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a handle using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_open_file_io_handle(
     libbfoverlay_handle_t *handle,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_open_file_io_handle";
	uint8_t file_io_handle_opened_in_library        = 0;
	int bfio_access_flags                           = 0;
	int file_io_handle_is_open                      = 0;
	int result                                      = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBFOVERLAY_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBFOVERLAY_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBBFOVERLAY_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to determine if file IO handle is open.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libbfoverlay_internal_handle_open_read(
	     internal_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file IO handle.",
		 function );

		result = -1;
	}
	else
	{
		internal_handle->access_flags                     = access_flags;
		internal_handle->file_io_handle                   = file_io_handle;
		internal_handle->file_io_handle_opened_in_library = file_io_handle_opened_in_library;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Opens the data files
 * This function is not multi-thread safe acquire write lock before call
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_internal_handle_open_data_files(
     libbfoverlay_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	libbfio_pool_t *file_io_pool = NULL;
	libbfoverlay_layer_t *layer  = NULL;
	static char *function        = "libbfoverlay_internal_handle_open_data_files";
	size64_t file_size           = 0;
	int bfio_access_flags        = 0;
	int layer_index              = 0;
	int number_of_layers         = 0;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t *wide_file_path      = NULL;
	size_t wide_file_path_size   = 0;
#endif

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->cow_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - COW file already exists.",
		 function );

		return( -1 );
	}
	if( internal_handle->cow_block_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - COW block data already exists.",
		 function );

		return( -1 );
	}
	if( libbfoverlay_descriptor_get_number_of_layers(
	     internal_handle->descriptor_file,
	     &number_of_layers,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of layers.",
		 function );

		goto on_error;
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     number_of_layers,
	     internal_handle->maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO pool.",
		 function );

		goto on_error;
	}
	for( layer_index = 0;
	     layer_index < number_of_layers;
	     layer_index++ )
	{
		if( libbfoverlay_descriptor_get_layer_by_index(
		     internal_handle->descriptor_file,
		     layer_index,
		     &layer,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve layer: %d.",
			 function,
			 layer_index );

			goto on_error;
		}
		if( layer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing layer: %d.",
			 function,
			 layer_index );

			goto on_error;
		}
		if( ( layer->data_file_path != NULL )
		 && ( layer->data_file_path_size > 0 ) )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#if SIZEOF_WCHAR_T == 4
			result = libuna_utf32_string_size_from_utf8(
			          (libuna_utf8_character_t *) layer->data_file_path,
			          layer->data_file_path_size,
			          &wide_file_path_size,
			          error );
#elif SIZEOF_WCHAR_T == 2
			result = libuna_utf16_string_size_from_utf8(
			          (libuna_utf8_character_t *) layer->data_file_path,
			          layer->data_file_path_size,
			          &wide_file_path_size,
			          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to determine wide data file path size.",
				 function );

				goto on_error;
			}
			if( ( wide_file_path_size == 0 )
			 || ( wide_file_path_size > ( LIBBFOVERLAY_MAXIMUM_PATH_SIZE * sizeof( wchar_t ) ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid wide data file path size value out of bounds.",
				 function );

				goto on_error;
			}
			wide_file_path = wide_string_allocate(
			                  wide_file_path_size );

			if( wide_file_path == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create wide data file path.",
				 function );

				goto on_error;
			}
#if SIZEOF_WCHAR_T == 4
			result = libuna_utf32_string_copy_from_utf8(
			          wide_file_path,
			          wide_file_path_size,
			          (libuna_utf8_character_t *) layer->data_file_path,
			          layer->data_file_path_size,
			          error );
#elif SIZEOF_WCHAR_T == 2
			result = libuna_utf16_string_copy_from_utf8(
			          wide_file_path,
			          wide_file_path_size,
			          (libuna_utf8_character_t *) layer->data_file_path,
			          layer->data_file_path_size,
			          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to determine wide data file path size.",
				 function );

				goto on_error;
			}
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

			bfio_access_flags = LIBBFIO_OPEN_READ;

			if( ( layer_index == ( number_of_layers - 1 ) )
			 && ( layer->use_cow != 0 ) )
			{
				if( ( internal_handle->access_flags & LIBBFOVERLAY_ACCESS_FLAG_WRITE ) == 0 )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libcfile_file_exists_wide(
					          wide_file_path,
					          error );
#else
					result = libcfile_file_exists(
					          (char *) layer->data_file_path,
					          error );
#endif
					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to determine if COW layer: %d data file exists.",
						 function,
						 layer_index );

						goto on_error;
					}
					else if( result == 0 )
					{
						continue;
					}
				}
				bfio_access_flags |= LIBBFIO_OPEN_WRITE;

				internal_handle->cow_file_io_pool_entry = number_of_layers - 1;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libbfio_file_pool_open_wide(
			     file_io_pool,
			     layer_index,
			     wide_file_path,
			     bfio_access_flags,
			     error ) != 1 )
#else
			if( libbfio_file_pool_open(
			     file_io_pool,
			     layer_index,
			     (char *) layer->data_file_path,
			     bfio_access_flags,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open layer: %d data file.",
				 function,
				 layer_index );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			memory_free(
			 wide_file_path );

			wide_file_path = NULL;
#endif
			if( ( layer_index == ( number_of_layers - 1 ) )
			 && ( layer->use_cow != 0 ) )
			{
				continue;
			}
			if( libbfio_pool_get_size(
			     file_io_pool,
			     layer_index,
			     &file_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve layer: %d data file size.",
				 function,
				 layer_index );

				goto on_error;
			}
			if( file_size == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d data file size is 0.",
				 function,
				 layer_index );

				goto on_error;
			}
			if( file_size > (size64_t) INT64_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid layer: %d data file size value exceeds maximum.",
				 function,
				 layer_index );

				goto on_error;
			}
			if( layer->size == -1 )
			{
				layer->size = file_size;
			}
			/* A negative file offset indicates an offset relative from the end of the data file
			 */
			if( layer->file_offset < 0 )
			{
				if( layer->file_offset <= ( -1 * (off64_t) file_size ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d file offset value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
				if( ( layer->size > (int64_t) file_size )
				 || ( layer->file_offset < ( -1 * (off64_t) ( file_size - layer->size ) ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d size value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
			}
			else
			{
				if( layer->file_offset >= (off64_t) file_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d file offset value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
				if( ( layer->size > (int64_t) file_size )
				 || ( layer->file_offset > (off64_t) ( file_size - layer->size ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d size value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
			}
		}
		if( layer_index == 0 )
		{
			internal_handle->size = layer->size;
		}
		else
		{
			/* The logical offset of successive layers must be in bounds of the base layer
			 * a negative offset indicates an offset relative from the end of the base layer
			 */
			if( layer->offset < 0 )
			{
				if( layer->offset <= ( -1 * (off64_t) internal_handle->size ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d offset value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
				layer->offset = internal_handle->size + layer->offset;
			}
			else
			{
				if( layer->offset >= (off64_t) internal_handle->size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d offset value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
			}
			if( ( (size64_t) layer->size > internal_handle->size )
			 || ( layer->offset > (off64_t) ( internal_handle->size - layer->size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d size value out of bounds.",
				 function,
				 layer_index );

				goto on_error;
			}
		}
	}
	if( internal_handle->cow_file_io_pool_entry != -1 )
	{
		if( libbfoverlay_cow_file_initialize(
		     &( internal_handle->cow_file ),
		     internal_handle->size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create COW file.",
			 function );

			goto on_error;
		}
		if( libbfoverlay_cow_file_open(
		     internal_handle->cow_file,
		     file_io_pool,
		     internal_handle->cow_file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open COW file.",
			 function );

			goto on_error;
		}
		if( ( internal_handle->cow_file->block_size == 0 )
		 || ( internal_handle->cow_file->block_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid COW file - block size exceed maximum allocation size.",
			 function );

			goto on_error;
		}
		internal_handle->cow_block_data = (uint8_t *) memory_allocate(
		                                               internal_handle->cow_file->block_size );

		if( internal_handle->cow_block_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create COW block data.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     internal_handle->cow_block_data,
		     0,
		     internal_handle->cow_file->block_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear COW block data.",
			 function );

			goto on_error;
		}
		internal_handle->size = internal_handle->cow_file->data_size;
	}
	if( libbfoverlay_internal_handle_open_determine_ranges(
	     internal_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ranges.",
		 function );

		goto on_error;
	}
	internal_handle->data_file_io_pool                    = file_io_pool;
	internal_handle->data_file_io_pool_created_in_library = 1;

	return( 1 );

on_error:
	if( internal_handle->cow_block_data != NULL )
	{
		memory_free(
		 internal_handle->cow_block_data );

		internal_handle->cow_block_data = NULL;
	}
	if( internal_handle->cow_file != NULL )
	{
		libbfoverlay_cow_file_free(
		 &( internal_handle->cow_file ),
		 NULL );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( wide_file_path != NULL )
	{
		memory_free(
		 wide_file_path );
	}
#endif
	if( file_io_pool != NULL )
	{
		libbfio_pool_close_all(
		 file_io_pool,
		 NULL );
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	return( -1 );
}

/* Opens the data files
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_open_data_files(
     libbfoverlay_handle_t *handle,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_open_data_files";
	int result                                      = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing descriptor file.",
		 function );

		return( -1 );
	}
	if( internal_handle->data_file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - data file IO pool already exists.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libbfoverlay_internal_handle_open_data_files(
	     internal_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open data files.",
		 function );

		result = -1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Opens the data files using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_open_data_files_file_io_pool(
     libbfoverlay_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_open_data_files_file_io_pool";
	int result                                      = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing descriptor file.",
		 function );

		return( -1 );
	}
	if( internal_handle->data_file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - data file IO pool already exists.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif

/* TODO iterate layers for data files */
/* TODO check if file_offset - size range falls within data file */
	result = -1;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Closes the handle
 * Returns 0 if successful or -1 on error
 */
int libbfoverlay_handle_close(
     libbfoverlay_handle_t *handle,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_close";
	int result                                      = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_handle->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_handle->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_handle->file_io_handle_opened_in_library = 0;
	}
	if( internal_handle->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_handle->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_handle->file_io_handle_created_in_library = 0;
	}
	internal_handle->file_io_handle = NULL;

	if( internal_handle->data_file_io_pool_created_in_library != 0 )
	{
		if( libbfio_pool_close_all(
		     internal_handle->data_file_io_pool,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close all files in data file IO pool.",
			 function );

			result = -1;
		}
		if( libbfio_pool_free(
		     &( internal_handle->data_file_io_pool ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data file IO pool.",
			 function );

			result = -1;
		}
		internal_handle->data_file_io_pool_created_in_library = 0;
	}
	internal_handle->data_file_io_pool = NULL;
	internal_handle->current_offset    = 0;

	if( internal_handle->descriptor_file != NULL )
	{
		if( libbfoverlay_descriptor_file_free(
		     &( internal_handle->descriptor_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free descriptor file.",
			 function );

			result = -1;
		}
	}
	if( internal_handle->cow_file != NULL )
	{
		if( libbfoverlay_cow_file_free(
		     &( internal_handle->cow_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free COW file.",
			 function );

			result = -1;
		}
		internal_handle->cow_file_io_pool_entry = -1;
	}
	if( internal_handle->cow_block_data != NULL )
	{
		memory_free(
		 internal_handle->cow_block_data );

		internal_handle->cow_block_data = NULL;
	}
	if( internal_handle->ranges_array != 0 )
	{
		if( libcdata_array_free(
		     &( internal_handle->ranges_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libbfoverlay_range_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free ranges array.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Opens a handle for reading
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_internal_handle_open_read(
     libbfoverlay_internal_handle_t *internal_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libbfoverlay_layer_t *base_layer = NULL;
	static char *function            = "libbfoverlay_internal_handle_open_read";

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libbfoverlay_descriptor_file_initialize(
	     &( internal_handle->descriptor_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptor file.",
		 function );

		goto on_error;
	}
	if( libbfoverlay_descriptor_file_read_file_io_handle(
	     internal_handle->descriptor_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor file.",
		 function );

		goto on_error;
	}
	if( libbfoverlay_descriptor_get_layer_by_index(
	     internal_handle->descriptor_file,
	     0,
	     &base_layer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve base layer.",
		 function );

		goto on_error;
	}
	if( base_layer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing base layer.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_handle->descriptor_file != NULL )
	{
		libbfoverlay_descriptor_file_free(
		 &( internal_handle->descriptor_file ),
		 NULL );
	}
	return( -1 );
}

/* Determine the ranges for reading
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_internal_handle_open_determine_ranges(
     libbfoverlay_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	libbfoverlay_layer_t *layer      = NULL;
	libbfoverlay_range_t *range      = NULL;
	libbfoverlay_range_t *safe_range = NULL;
	static char *function            = "libbfoverlay_internal_handle_open_determine_ranges";
	off64_t current_data_file_offset = 0;
	off64_t current_layer_offset     = 0;
	int64_t range_size               = 0;
	int64_t remaining_layer_size     = 0;
	uint32_t range_flags             = 0;
	int entry_index                  = 0;
	int layer_index                  = 0;
	int number_of_layers             = 0;
	int number_of_ranges             = 0;
	int range_index                  = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->ranges_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - ranges array value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_handle->ranges_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create ranges array.",
		 function );

		goto on_error;
	}
	if( libbfoverlay_descriptor_get_number_of_layers(
	     internal_handle->descriptor_file,
	     &number_of_layers,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of layers.",
		 function );

		goto on_error;
	}
	for( layer_index = number_of_layers - 1;
	     layer_index >= 0;
	     layer_index-- )
	{
		if( libbfoverlay_descriptor_get_layer_by_index(
		     internal_handle->descriptor_file,
		     layer_index,
		     &layer,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve layer: %d.",
			 function,
			 layer_index );

			goto on_error;
		}
		if( layer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing layer: %d.",
			 function,
			 layer_index );

			goto on_error;
		}
		if( layer->use_cow != 0 )
		{
			continue;
		}
		current_layer_offset     = layer->offset;
		remaining_layer_size     = layer->size;
		current_data_file_offset = layer->file_offset;

		range_flags = 0;

		if( layer->data_file_path == NULL )
		{
			range_flags |= LIBBFOVERLAY_RANGE_FLAG_IS_SPARSE;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: layer: %d with offset: %" PRIi64 " (0x%08" PRIx64 ") and size: %" PRIi64 ".\n",
			 function,
			 layer_index,
			 current_layer_offset,
			 current_layer_offset,
			 remaining_layer_size );
		}
#endif
		for( range_index = 0;
		     range_index < number_of_ranges;
		     range_index++ )
		{
			if( libcdata_array_get_entry_by_index(
			     internal_handle->ranges_array,
			     range_index,
			     (intptr_t **) &range,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve range: %d.",
				 function,
				 range_index );

				goto on_error;
			}
			if( range == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing range: %d.",
				 function,
				 range_index );

				goto on_error;
			}
			if( current_layer_offset > range->end_offset )
			{
				continue;
			}
			if( current_layer_offset < range->start_offset )
			{
				/* Insert a new range before the current range
				 */
				range_size = remaining_layer_size;

				if( range_size > ( range->start_offset - current_layer_offset ) )
				{
					range_size = (int64_t) ( range->start_offset - current_layer_offset );
				}
				if( libbfoverlay_range_initialize(
				     &safe_range,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create range.",
					 function );

					goto on_error;
				}
				safe_range->start_offset     = current_layer_offset;
				safe_range->end_offset       = current_layer_offset + range_size;
				safe_range->size             = range_size;
				safe_range->flags            = range_flags;
				safe_range->data_file_index  = layer_index;
				safe_range->data_file_offset = current_data_file_offset;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: inserting range for layer: %d with offset: %" PRIi64 " (0x%08" PRIx64 ") and size: %" PRIi64 ".\n",
					 function,
					 layer_index,
					 current_layer_offset,
					 current_layer_offset,
					 range_size );
				}
#endif
				if( libcdata_array_insert_entry(
				     internal_handle->ranges_array,
				     &entry_index,
				     (intptr_t *) safe_range,
				     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libbfoverlay_range_compare,
				     LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to insert range into array.",
					 function );

					goto on_error;
				}
				safe_range = NULL;

				range_index++;
				number_of_ranges++;

				current_layer_offset     += range_size;
				remaining_layer_size     -= range_size;
				current_data_file_offset += range_size;
			}
			if( ( current_layer_offset >= range->start_offset )
			 && ( current_layer_offset < range->end_offset ) )
			{
				current_layer_offset      = range->end_offset;
				remaining_layer_size     -= range->size;
				current_data_file_offset += range->size;
			}
			if( remaining_layer_size <= 0 )
			{
				break;
			}
		}
		if( remaining_layer_size > 0 )
		{
			if( libbfoverlay_range_initialize(
			     &safe_range,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create range.",
				 function );

				goto on_error;
			}
			safe_range->start_offset     = current_layer_offset;
			safe_range->end_offset       = current_layer_offset + remaining_layer_size;
			safe_range->size             = remaining_layer_size;
			safe_range->flags            = range_flags;
			safe_range->data_file_index  = layer_index;
			safe_range->data_file_offset = current_data_file_offset;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: inserting range for layer: %d with offset: %" PRIi64 " (0x%08" PRIx64 ") and size: %" PRIi64 ".\n",
				 function,
				 layer_index,
				 current_layer_offset,
				 current_layer_offset,
				 remaining_layer_size );
			}
#endif
			if( libcdata_array_insert_entry(
			     internal_handle->ranges_array,
			     &entry_index,
			     (intptr_t *) safe_range,
			     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libbfoverlay_range_compare,
			     LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert range into array.",
				 function );

				goto on_error;
			}
			safe_range = NULL;

			number_of_ranges++;
		}
	}
	return( 1 );

on_error:
	if( safe_range != NULL )
	{
		libbfoverlay_range_free(
		 &safe_range,
		 NULL );
	}
	return( -1 );
}

/* Retrieve the range for a specific offset
 * Returns 1 if successful, 0 if no range exists or -1 on error
 */
int libbfoverlay_internal_handle_get_range_at_offset(
     libbfoverlay_internal_handle_t *internal_handle,
     off64_t offset,
     int *range_index,
     libbfoverlay_range_t **range,
     libcerror_error_t **error )
{
	libbfoverlay_range_t *safe_range = NULL;
	static char *function            = "libbfoverlay_internal_handle_get_range_at_offset";
	int number_of_ranges             = 0;
	int safe_range_index             = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( range_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range index.",
		 function );

		return( -1 );
	}
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
/* TODO preserve current range and index */
	if( libcdata_array_get_number_of_entries(
	     internal_handle->ranges_array,
	     &number_of_ranges,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of ranges.",
		 function );

		return( -1 );
	}
	for( safe_range_index = 0;
	     safe_range_index < number_of_ranges;
	     safe_range_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_handle->ranges_array,
		     safe_range_index,
		     (intptr_t **) &safe_range,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve range: %d.",
			 function,
			 safe_range_index );

			return( -1 );
		}
		if( range == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range: %d.",
			 function,
			 safe_range_index );

			return( -1 );
		}
		if( ( offset >= safe_range->start_offset )
		 && ( offset < safe_range->end_offset ) )
		{
			*range_index = safe_range_index;
			*range       = safe_range;

			return( 1 );
		}
	}
	return( 0 );
}

/* Reads data from the current offset into a buffer
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes read or -1 on error
 */
ssize_t libbfoverlay_internal_handle_read_buffer(
         libbfoverlay_internal_handle_t *internal_handle,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libbfoverlay_range_t *range    = NULL;
	static char *function          = "libbfoverlay_internal_handle_read_buffer";
	size_t buffer_offset           = 0;
	size_t cow_block_offset        = 0;
	size_t read_size               = 0;
	ssize_t read_count             = 0;
	off64_t cow_block_end_offset   = 0;
	off64_t cow_block_start_offset = 0;
	off64_t file_offset            = 0;
	int range_index                = 0;
	int result                     = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_handle->current_offset >= internal_handle->size )
	{
		return( 0 );
	}
	if( (size64_t) buffer_size > ( internal_handle->size - internal_handle->current_offset ) )
	{
		buffer_size = (size_t) ( internal_handle->size - internal_handle->current_offset );
	}
	while( buffer_offset < buffer_size )
	{
		read_size = buffer_size - buffer_offset;

		if( internal_handle->cow_file == NULL )
		{
			result = 0;
		}
		else
		{
			result = libbfoverlay_cow_file_get_block_at_offset(
			          internal_handle->cow_file,
			          internal_handle->data_file_io_pool,
			          internal_handle->cow_file_io_pool_entry,
			          internal_handle->current_offset,
			          &cow_block_start_offset,
			          &cow_block_end_offset,
			          &file_offset,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve COW block at offset %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 internal_handle->current_offset,
				 internal_handle->current_offset );

				return( -1 );
			}
		}
		if( result != 0 )
		{
			read_count = libbfio_pool_read_buffer_at_offset(
			              internal_handle->data_file_io_pool,
			              internal_handle->cow_file_io_pool_entry,
			              internal_handle->cow_block_data,
			              internal_handle->cow_file->block_size,
			              file_offset,
			              error );

			if( read_count != (ssize_t) internal_handle->cow_file->block_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read COW block of size: %" PRIzd " from COW file at offset %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 internal_handle->cow_file->block_size,
				 cow_block_offset,
				 cow_block_offset );

				return( -1 );
			}
			cow_block_offset = (size_t) ( internal_handle->current_offset - cow_block_start_offset );

			if( (int64_t) read_size > ( cow_block_end_offset - internal_handle->current_offset ) )
			{
				read_size = (size_t) ( cow_block_end_offset - internal_handle->current_offset );
			}
			if( memory_copy(
			     &( buffer[ buffer_offset ] ),
			     &( ( internal_handle->cow_block_data )[ cow_block_offset ] ),
			     read_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy buffer from COW block data.",
				 function );

				return( -1 );
			}
			read_count = (ssize_t) read_size;
		}
		else
		{
			if( range == NULL )
			{
				if( libbfoverlay_internal_handle_get_range_at_offset(
			             internal_handle,
			             internal_handle->current_offset,
			             &range_index,
			             &range,
			             error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve range at offset %" PRIi64 " (0x%08" PRIx64 ").",
					 function,
					 internal_handle->current_offset,
					 internal_handle->current_offset );

					return( -1 );
				}
			}
			else
			{
/* TODO preserve current range and index ? */
				range_index++;

				if( libcdata_array_get_entry_by_index(
				     internal_handle->ranges_array,
				     range_index,
				     (intptr_t **) &range,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve range: %d.",
					 function,
					 range_index );

					return( -1 );
				}
			}
			if( range == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing range: %d.",
				 function,
				 range_index );

				return( -1 );
			}
			if( (int64_t) read_size > ( range->end_offset - internal_handle->current_offset ) )
			{
				read_size = (size_t) ( range->end_offset - internal_handle->current_offset );
			}
			if( ( range->flags & LIBBFOVERLAY_RANGE_FLAG_IS_SPARSE ) != 0 )
			{
				if( memory_set(
				     &( buffer[ buffer_offset ] ),
				     0,
				     read_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to clear buffer.",
					 function );

					return( -1 );
				}
			}
			else
			{
				file_offset = range->data_file_offset + ( internal_handle->current_offset - range->start_offset );

				read_count = libbfio_pool_read_buffer_at_offset(
				              internal_handle->data_file_io_pool,
				              range->data_file_index,
				              &( buffer[ buffer_offset ] ),
				              read_size,
				              file_offset,
				              error );

				if( read_count != (ssize_t) read_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read data of size: %" PRIzd " from layer: %d at offset %" PRIi64 " (0x%08" PRIx64 ").",
					 function,
					 read_size,
					 range->data_file_index,
					 file_offset,
					 file_offset );

					return( -1 );
				}
			}
		}
		buffer_offset += read_size;

		internal_handle->current_offset += read_size;

		if( buffer_offset >= buffer_size )
		{
			break;
		}
	}
	return( (ssize_t) buffer_offset );
}

/* Reads data from the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libbfoverlay_handle_read_buffer(
         libbfoverlay_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_read_buffer";
	ssize_t read_count                              = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libbfoverlay_internal_handle_read_buffer(
		      internal_handle,
		      buffer,
		      buffer_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libbfoverlay_handle_read_buffer_at_offset(
         libbfoverlay_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_read_buffer_at_offset";
	ssize_t read_count                              = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libbfoverlay_internal_handle_seek_offset(
	     internal_handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		read_count = -1;
	}
	else
	{
		read_count = libbfoverlay_internal_handle_read_buffer(
			      internal_handle,
			      buffer,
			      buffer_size,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Writes data from buffer at the current offset
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes written or -1 on error
 */
ssize_t libbfoverlay_internal_handle_write_buffer(
         libbfoverlay_internal_handle_t *internal_handle,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	static char *function          = "libbfoverlay_internal_handle_write_buffer";
	size_t buffer_offset           = 0;
	size_t cow_block_offset        = 0;
	size_t write_size              = 0;
	ssize_t read_count             = 0;
	ssize_t write_count            = 0;
	off64_t cow_block_end_offset   = 0;
	off64_t cow_block_start_offset = 0;
	off64_t file_offset            = 0;
	off64_t safe_current_offset    = 0;
	int result                     = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	while( buffer_offset < buffer_size )
	{
		result = libbfoverlay_cow_file_get_block_at_offset(
		          internal_handle->cow_file,
		          internal_handle->data_file_io_pool,
		          internal_handle->cow_file_io_pool_entry,
		          internal_handle->current_offset,
		          &cow_block_start_offset,
		          &cow_block_end_offset,
		          &file_offset,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve COW block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 internal_handle->current_offset,
			 internal_handle->current_offset );

			return( -1 );
		}
		safe_current_offset = internal_handle->current_offset;

		if( libbfoverlay_internal_handle_seek_offset(
		     internal_handle,
		     cow_block_start_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek COW block offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 cow_block_start_offset,
			 cow_block_start_offset );

			return( -1 );
		}
		read_count = libbfoverlay_internal_handle_read_buffer(
			      internal_handle,
		              internal_handle->cow_block_data,
		              internal_handle->cow_file->block_size,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read COW block data.",
			 function );

			return( -1 );
		}
		if( read_count < (ssize_t) internal_handle->cow_file->block_size )
		{
			if( memory_set(
			     &( ( internal_handle->cow_block_data )[ read_count ] ),
			     0,
			     internal_handle->cow_file->block_size - read_count ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear read COW block data.",
				 function );

				return( -1 );
			}
		}
		internal_handle->current_offset = safe_current_offset;

		cow_block_offset = (size_t) ( internal_handle->current_offset - cow_block_start_offset );

		write_size = buffer_size - buffer_offset;

		if( (int64_t) write_size > ( cow_block_end_offset - internal_handle->current_offset ) )
		{
			write_size = (size_t) ( cow_block_end_offset - internal_handle->current_offset );
		}
		if( memory_copy(
		     &( ( internal_handle->cow_block_data )[ cow_block_offset ] ),
		     &( buffer[ buffer_offset ] ),
		     write_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy buffer to COW block data.",
			 function );

			return( -1 );
		}
		if( result == 0 )
		{
			if( libbfoverlay_cow_file_allocate_block_for_offset(
			     internal_handle->cow_file,
			     internal_handle->data_file_io_pool,
			     internal_handle->cow_file_io_pool_entry,
			     internal_handle->current_offset,
			     &file_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to allocate block in COW file for offset %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 internal_handle->current_offset,
				 internal_handle->current_offset );

				return( -1 );
			}
		}
		write_count = libbfio_pool_write_buffer_at_offset(
		               internal_handle->data_file_io_pool,
		               internal_handle->cow_file_io_pool_entry,
		               internal_handle->cow_block_data,
		               internal_handle->cow_file->block_size,
		               file_offset,
		               error );

		if( write_count != (ssize_t) internal_handle->cow_file->block_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to write COW block of size: %" PRIzd " to COW file at offset %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 internal_handle->cow_file->block_size,
			 cow_block_offset,
			 cow_block_offset );

			return( -1 );
		}
		buffer_offset                   += write_size;
		internal_handle->current_offset += write_size;
	}
	if( (size64_t) internal_handle->current_offset > internal_handle->size )
	{
		if( libbfoverlay_cow_file_set_data_size(
		     internal_handle->cow_file,
		     internal_handle->data_file_io_pool,
		     internal_handle->cow_file_io_pool_entry,
		     (size64_t) internal_handle->current_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data size in COW file.",
			 function );

			return( -1 );
		}
		internal_handle->size = (size64_t) internal_handle->current_offset;
	}
	return( (ssize_t) buffer_offset );
}

/* Writes data from buffer at the current offset
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t libbfoverlay_handle_write_buffer(
         libbfoverlay_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_write_buffer";
	ssize_t write_count                             = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing COW file.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	write_count = libbfoverlay_internal_handle_write_buffer(
	               internal_handle,
	               buffer,
	               buffer_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write buffer.",
		 function );

		write_count = -1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( write_count );
}

/* Writes data at a specific offset
 * Returns the number of bytes written or -1 on error
 */
ssize_t libbfoverlay_handle_write_buffer_at_offset(
         libbfoverlay_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_write_buffer_at_offset";
	ssize_t write_count                             = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing COW file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libbfoverlay_internal_handle_seek_offset(
	     internal_handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		write_count = -1;
	}
	else
	{
		write_count = libbfoverlay_internal_handle_write_buffer(
			       internal_handle,
			       buffer,
			       buffer_size,
			       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to write buffer.",
			 function );

			write_count = -1;
		}
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( write_count );
}

/* Seeks a certain offset of the data
 * This function is not multi-thread safe acquire write lock before call
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libbfoverlay_internal_handle_seek_offset(
         libbfoverlay_internal_handle_t *internal_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libbfoverlay_internal_handle_seek_offset";

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{
		offset += internal_handle->current_offset;
	}
	else if( whence == SEEK_END )
	{
		offset += (off64_t) internal_handle->size;
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_handle->current_offset = offset;

	return( offset );
}

/* Seeks a certain offset
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libbfoverlay_handle_seek_offset(
         libbfoverlay_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_seek_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libbfoverlay_internal_handle_seek_offset(
	          internal_handle,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		offset = -1;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Resizes the data of the handle
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_resize(
     libbfoverlay_handle_t *handle,
     size64_t size,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_resize";
	int result                                      = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->cow_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing COW file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libbfoverlay_cow_file_set_data_size(
	     internal_handle->cow_file,
	     internal_handle->data_file_io_pool,
	     internal_handle->cow_file_io_pool_entry,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data size in COW file.",
		 function );

		result = -1;
	}
	else
	{
		internal_handle->size = size;
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the current offset
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_get_offset(
     libbfoverlay_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_get_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*offset = internal_handle->current_offset;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Returns the size of the data of the handle
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_handle_get_size(
     libbfoverlay_handle_t *handle,
     size64_t *size,
     libcerror_error_t **error )
{
	libbfoverlay_internal_handle_t *internal_handle = NULL;
	static char *function                           = "libbfoverlay_handle_get_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libbfoverlay_internal_handle_t *) handle;

	if( internal_handle->data_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing data file IO pool.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size",
		 function );

		return( -1 );
	}
#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*size = internal_handle->size;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_handle->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

