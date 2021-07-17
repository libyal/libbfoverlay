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

#include "libbfoverlay_cow_allocation_table_block.h"
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

	/* Block size
	 */
	size32_t block_size;

	/* Number of allocated blocks
	 */
	uint32_t number_of_allocated_blocks;

	/* Last block number used to store data or metadata
	 */
	uint32_t last_data_block_number;

	/* Offset of the level 1 allocation table
	 */
	off64_t l1_allocation_table_offset;

	/* Allocation table block
	 */
	libbfoverlay_cow_allocation_table_block_t *allocation_table_block;
};

int libbfoverlay_cow_file_initialize(
     libbfoverlay_cow_file_t **cow_file,
     size64_t data_size,
     libcerror_error_t **error );

int libbfoverlay_cow_file_free(
     libbfoverlay_cow_file_t **cow_file,
     libcerror_error_t **error );

int libbfoverlay_cow_file_open(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error );

int libbfoverlay_cow_file_close(
     libbfoverlay_cow_file_t *cow_file,
     libcerror_error_t **error );

int libbfoverlay_cow_file_enlarge_allocation_table(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libcerror_error_t **error );

int libbfoverlay_cow_file_allocate_block_for_offset(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t offset,
     off64_t *file_offset,
     libcerror_error_t **error );

int libbfoverlay_cow_file_get_block_at_offset(
     libbfoverlay_cow_file_t *cow_file,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t offset,
     off64_t *range_start_offset,
     off64_t *range_end_offset,
     off64_t *file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_COW_FILE_H ) */

