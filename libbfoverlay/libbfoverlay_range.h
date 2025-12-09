/*
 * The range functions
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

#if !defined( _LIBBFOVERLAY_RANGE_H )
#define _LIBBFOVERLAY_RANGE_H

#include <common.h>
#include <types.h>

#include "libbfoverlay_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfoverlay_range libbfoverlay_range_t;

struct libbfoverlay_range
{
	/* The start offset
	 */
	off64_t start_offset;

	/* The end offset
	 */
	off64_t end_offset;

	/* The size
	 */
	size64_t size;

	/* The flags
	 */
	uint32_t flags;

	/* The data file index
	 */
	int data_file_index;

	/* The data file offset
	 */
	off64_t data_file_offset;
};

int libbfoverlay_range_initialize(
     libbfoverlay_range_t **range,
     libcerror_error_t **error );

int libbfoverlay_range_free(
     libbfoverlay_range_t **range,
     libcerror_error_t **error );

int libbfoverlay_range_set(
     libbfoverlay_range_t *range,
     int data_file_index,
     off64_t offset,
     size64_t size,
     uint32_t flags,
     libcerror_error_t **error );

int libbfoverlay_range_compare(
     libbfoverlay_range_t *first_range,
     libbfoverlay_range_t *second_range,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_RANGE_H ) */

