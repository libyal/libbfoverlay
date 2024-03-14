/*
 * Descriptor file functions
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

#if !defined( _LIBBFOVERLAY_DESCRIPTOR_FILE_H )
#define _LIBBFOVERLAY_DESCRIPTOR_FILE_H

#include <common.h>
#include <types.h>

#include "libbfoverlay_layer.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcdata.h"
#include "libbfoverlay_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfoverlay_descriptor_file libbfoverlay_descriptor_file_t;

struct libbfoverlay_descriptor_file
{
	/* The layers array
	 */
	libcdata_array_t *layers_array;
};

int libbfoverlay_descriptor_file_initialize(
     libbfoverlay_descriptor_file_t **descriptor_file,
     libcerror_error_t **error );

int libbfoverlay_descriptor_file_free(
     libbfoverlay_descriptor_file_t **descriptor_file,
     libcerror_error_t **error );

int libbfoverlay_descriptor_file_read_data(
     libbfoverlay_descriptor_file_t *descriptor_file,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbfoverlay_descriptor_file_read_file_io_handle(
     libbfoverlay_descriptor_file_t *descriptor_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libbfoverlay_descriptor_get_number_of_layers(
     libbfoverlay_descriptor_file_t *descriptor_file,
     int *number_of_layers,
     libcerror_error_t **error );

int libbfoverlay_descriptor_get_layer_by_index(
     libbfoverlay_descriptor_file_t *descriptor_file,
     int layer_index,
     libbfoverlay_layer_t **layer,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_DESCRIPTOR_FILE_H ) */

