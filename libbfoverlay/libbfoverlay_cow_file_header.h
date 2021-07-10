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

#if !defined( _LIBBFOVERLAY_COW_FILE_HEADER_H )
#define _LIBBFOVERLAY_COW_FILE_HEADER_H

#include <common.h>
#include <types.h>

#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfoverlay_cow_file_header libbfoverlay_cow_file_header_t;

struct libbfoverlay_cow_file_header
{
	/* The format version
	 */
	uint32_t format_version;

	/* The block size
	 */
	uint32_t block_size;

	/* The data size
	 */
	uint64_t data_size;
};

int libbfoverlay_cow_file_header_initialize(
     libbfoverlay_cow_file_header_t **cow_file_header,
     libcerror_error_t **error );

int libbfoverlay_cow_file_header_free(
     libbfoverlay_cow_file_header_t **cow_file_header,
     libcerror_error_t **error );

int libbfoverlay_cow_file_header_read_data(
     libbfoverlay_cow_file_header_t *cow_file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbfoverlay_cow_file_header_read_file_io_handle(
     libbfoverlay_cow_file_header_t *cow_file_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libbfoverlay_cow_file_header_write_file_io_handle(
     libbfoverlay_cow_file_header_t *cow_file_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_COW_FILE_HEADER_H ) */

