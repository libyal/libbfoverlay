/*
 * Layer functions
 *
 * Copyright (C) 2020-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBFOVERLAY_LAYER_H )
#define _LIBBFOVERLAY_LAYER_H

#include <common.h>
#include <types.h>

#include "libbfoverlay_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfoverlay_layer libbfoverlay_layer_t;

struct libbfoverlay_layer
{
	/* Offset
	 */
	off64_t offset;

	/* Size
	 */
	int64_t size;

	/* Data file path
	 */
	uint8_t *data_file_path;

	/* Data file path size
	 */
	size_t data_file_path_size;

	/* Data file offset
	 */
	off64_t file_offset;

	/* Use copy-on-write (COW)
	 */
	int8_t use_cow;
};

int libbfoverlay_layer_initialize(
     libbfoverlay_layer_t **layer,
     libcerror_error_t **error );

int libbfoverlay_layer_free(
     libbfoverlay_layer_t **layer,
     libcerror_error_t **error );

int libbfoverlay_layer_set_data_file_path(
     libbfoverlay_layer_t *layer,
     const uint8_t *path,
     size_t path_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_LAYER_H ) */

