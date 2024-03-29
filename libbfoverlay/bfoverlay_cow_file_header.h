/*
 * File header of a basic file overlay copy-on-write (COW) file
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

#if !defined( _BFOVERLAY_COW_FILE_HEADER_H )
#define _BFOVERLAY_COW_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct bfoverlay_cow_file_header bfoverlay_cow_file_header_t;

struct bfoverlay_cow_file_header
{
	/* Signature
	 * Consists of 12 bytes
	 */
	uint8_t signature[ 12 ];

	/* Format version
	 * Consists of 4 bytes
	 */
	uint8_t format_version[ 4 ];

	/* Data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* Block size
	 * Consists of 4 bytes
	 */
	uint8_t block_size[ 4 ];

	/* Number of allocated blocks
	 * Consists of 4 bytes
	 */
	uint8_t number_of_allocated_blocks[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _BFOVERLAY_COW_FILE_HEADER_H ) */

