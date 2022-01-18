/*
 * Handle functions
 *
 * Copyright (C) 2020-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBFOVERLAY_HANDLE_H )
#define _LIBBFOVERLAY_HANDLE_H

#include <common.h>
#include <types.h>

#include "libbfoverlay_cow_file.h"
#include "libbfoverlay_descriptor_file.h"
#include "libbfoverlay_extern.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcdata.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libcthreads.h"
#include "libbfoverlay_range.h"
#include "libbfoverlay_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfoverlay_internal_handle libbfoverlay_internal_handle_t;

struct libbfoverlay_internal_handle
{
	/* The descriptor file
	 */
	libbfoverlay_descriptor_file_t *descriptor_file;

	/* The copy-on-write (COW) file
	 */
	libbfoverlay_cow_file_t *cow_file;

	/* The copy-on-write (COW) file IO pool entry
	 */
	int cow_file_io_pool_entry;

	/* The copy-on-write (COW) block data
	 */
	uint8_t *cow_block_data;

	/* The ranges array
	 */
	libcdata_array_t *ranges_array;

	/* The current offset
	 */
	off64_t current_offset;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The data file IO pool
	 */
	libbfio_pool_t *data_file_io_pool;

	/* Value to indicate if the file IO pool was created inside the library
	 */
	uint8_t data_file_io_pool_created_in_library;

	/* The maximum number of open handles in the file IO pool
	 */
	int maximum_number_of_open_handles;

	/* The size
	 */
	size64_t size;

	/* The access flags
	 */
	int access_flags;

	/* Value to indicate if abort was signalled
	 */
	int abort;

#if defined( HAVE_MULTI_THREAD_SUPPORT ) && !defined( HAVE_LOCAL_LIBBFOVERLAY )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_initialize(
     libbfoverlay_handle_t **handle,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_free(
     libbfoverlay_handle_t **handle,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_signal_abort(
     libbfoverlay_handle_t *handle,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_open(
     libbfoverlay_handle_t *handle,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_open_wide(
     libbfoverlay_handle_t *handle,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_open_file_io_handle(
     libbfoverlay_handle_t *handle,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

int libbfoverlay_internal_handle_open_data_files(
     libbfoverlay_internal_handle_t *internal_handle,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_open_data_files(
     libbfoverlay_handle_t *handle,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_open_data_files_file_io_pool(
     libbfoverlay_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_close(
     libbfoverlay_handle_t *handle,
     libcerror_error_t **error );

int libbfoverlay_internal_handle_open_read(
     libbfoverlay_internal_handle_t *internal_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

ssize_t libbfoverlay_internal_handle_read_buffer(
         libbfoverlay_internal_handle_t *internal_handle,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

int libbfoverlay_internal_handle_open_determine_ranges(
     libbfoverlay_internal_handle_t *internal_handle,
     libcerror_error_t **error );

int libbfoverlay_internal_handle_get_range_at_offset(
     libbfoverlay_internal_handle_t *internal_handle,
     off64_t offset,
     int *range_index,
     libbfoverlay_range_t **range,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
ssize_t libbfoverlay_handle_read_buffer(
         libbfoverlay_handle_t *handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
ssize_t libbfoverlay_handle_read_buffer_at_offset(
         libbfoverlay_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

ssize_t libbfoverlay_internal_handle_write_buffer(
         libbfoverlay_internal_handle_t *internal_handle,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
ssize_t libbfoverlay_handle_write_buffer(
         libbfoverlay_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
ssize_t libbfoverlay_handle_write_buffer_at_offset(
         libbfoverlay_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

off64_t libbfoverlay_internal_handle_seek_offset(
         libbfoverlay_internal_handle_t *internal_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
off64_t libbfoverlay_handle_seek_offset(
         libbfoverlay_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_resize(
     libbfoverlay_handle_t *handle,
     size64_t size,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_get_offset(
     libbfoverlay_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error );

LIBBFOVERLAY_EXTERN \
int libbfoverlay_handle_get_size(
     libbfoverlay_handle_t *handle,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_HANDLE_H ) */

