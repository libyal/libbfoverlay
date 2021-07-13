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

#if !defined( _LIBBFOVERLAY_COW_ALLOCATION_TABLE_H )
#define _LIBBFOVERLAY_COW_ALLOCATION_TABLE_H

#include <common.h>
#include <types.h>

#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfoverlay_cow_allocation_table libbfoverlay_cow_allocation_table_t;

struct libbfoverlay_cow_allocation_table
{
	/* Number of blocks
	 */
	uint64_t number_of_blocks;

	/* Block table
	 */
	uint64_t *block_table;
};

int libbfoverlay_cow_allocation_table_initialize(
     libbfoverlay_cow_allocation_table_t **cow_allocation_table,
     uint64_t number_of_blocks,
     libcerror_error_t **error );

int libbfoverlay_cow_allocation_table_free(
     libbfoverlay_cow_allocation_table_t **cow_allocation_table,
     libcerror_error_t **error );

int libbfoverlay_cow_allocation_table_read_data(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbfoverlay_cow_allocation_table_read_file_io_pool(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error );

int libbfoverlay_cow_allocation_table_get_block_number_by_index(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     int table_index,
     uint64_t *block_number,
     libcerror_error_t **error );

int libbfoverlay_cow_allocation_table_set_block_number_by_index(
     libbfoverlay_cow_allocation_table_t *cow_allocation_table,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     int table_index,
     uint64_t block_number,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_COW_ALLOCATION_TABLE_H ) */

