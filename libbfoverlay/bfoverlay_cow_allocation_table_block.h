/*
 * Allocation table block of a basic file overlay copy-on-write (COW) file
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

#if !defined( _BFOVERLAY_COW_ALLOCATION_TABLE_BLOCK_H )
#define _BFOVERLAY_COW_ALLOCATION_TABLE_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct bfoverlay_cow_allocation_table_block_header bfoverlay_cow_allocation_table_block_header_t;

struct bfoverlay_cow_allocation_table_block_header
{
	/* Signature
	 * Consists of 12 bytes
	 */
	uint8_t signature[ 12 ];

	/* Format version
	 * Consists of 4 bytes
	 */
	uint8_t format_version[ 4 ];

	/* Padding (unused)
	 * Consists of 16 bytes
	 */
	uint8_t padding[ 16 ];
};

typedef struct bfoverlay_cow_allocation_table_block_entry bfoverlay_cow_allocation_table_block_entry_t;

struct bfoverlay_cow_allocation_table_block_entry
{
	/* Block number
	 * Consists of 4 bytes
	 */
	uint8_t block_number[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _BFOVERLAY_COW_ALLOCATION_TABLE_BLOCK_H ) */

