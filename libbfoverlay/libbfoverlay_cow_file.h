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

#if !defined( _LIBBFOVERLAY_COW_FILE_H )
#define _LIBBFOVERLAY_COW_FILE_H

#include <common.h>
#include <types.h>

#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfoverlay_cow_file libbfoverlay_cow_file_t;

struct libbfoverlay_cow_file
{
	/* Data size
	 */
	size64_t data_size;
};

int libbfoverlay_cow_file_initialize(
     libbfoverlay_cow_file_t **cow_file,
     size64_t data_size,
     libcerror_error_t **error );

int libbfoverlay_cow_file_free(
     libbfoverlay_cow_file_t **cow_file,
     libcerror_error_t **error );

int libbfoverlay_cow_file_open_file_io_handle(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libbfoverlay_cow_file_close(
     libbfoverlay_cow_file_t *cow_file,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_COW_FILE_H ) */

