/*
 * Descriptor file functions
 *
 * Copyright (C) 2020-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libbfoverlay_definitions.h"
#include "libbfoverlay_descriptor_file.h"
#include "libbfoverlay_layer.h"
#include "libbfoverlay_libbfio.h"
#include "libbfoverlay_libcdata.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libfvalue.h"

/* Creates an descriptor file
 * Make sure the value descriptor_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_descriptor_file_initialize(
     libbfoverlay_descriptor_file_t **descriptor_file,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_descriptor_file_initialize";

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( *descriptor_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid descriptor file value already set.",
		 function );

		return( -1 );
	}
	*descriptor_file = memory_allocate_structure(
	                    libbfoverlay_descriptor_file_t );

	if( *descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create descriptor file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *descriptor_file,
	     0,
	     sizeof( libbfoverlay_descriptor_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear descriptor file.",
		 function );

		memory_free(
		 *descriptor_file );

		*descriptor_file = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *descriptor_file )->layers_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *descriptor_file != NULL )
	{
		memory_free(
		 *descriptor_file );

		*descriptor_file = NULL;
	}
	return( -1 );
}

/* Frees an descriptor file
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_descriptor_file_free(
     libbfoverlay_descriptor_file_t **descriptor_file,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_descriptor_file_free";
	int result            = 1;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( *descriptor_file != NULL )
	{
		if( libcdata_array_free(
		     &( ( *descriptor_file )->layers_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libbfoverlay_layer_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free layers array.",
			 function );

			result = -1;
		}
		memory_free(
		 *descriptor_file );

		*descriptor_file = NULL;
	}
	return( result );
}

/* Reads the descriptor file
 * Returns the 1 if succesful or -1 on error
 */
int libbfoverlay_descriptor_file_read_data(
     libbfoverlay_descriptor_file_t *descriptor_file,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libbfoverlay_layer_t *layer           = NULL;
	libfvalue_split_utf8_string_t *lines  = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libbfoverlay_descriptor_file_read";
	size_t line_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int64_t base_layer_size               = -1;
	int cow_layer_index                   = -1;
	int entry_index                       = 0;
	int layer_index                       = 0;
	int line_index                        = 0;
	int number_of_lines                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 36 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_compare(
	     data,
	     "# basic file overlay descriptor file",
	     36 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported basic file overlay descriptor file signature.",
		 function );

		goto on_error;
	}
	if( libfvalue_utf8_string_split(
	     data,
	     data_size,
	     (uint8_t) '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split data into lines.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     lines,
	     &number_of_lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of lines",
		 function );

		goto on_error;
	}
	/* Supporting 25 layers should be sufficient for the immediate needs
	 */
	if( ( number_of_lines < 2 )
	 || ( number_of_lines > 26 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of lines: %d.",
		 function,
		 number_of_lines );

		goto on_error;
	}
	for( line_index = 1;
	     line_index < number_of_lines;
	     line_index++ )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     line_index,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: %d.",
			 function,
			 line_index );

			goto on_error;
		}
		if( ( line_string == NULL )
		 || ( line_string_size == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: %d.",
			 function,
			 line_index );

			goto on_error;
		}
		/* Note on Ubuntu 22.04 platforms a trailing empty line is added
		 */
		if( ( line_string_size == 1 )
		 && ( number_of_lines > 2 )
		 && ( line_index == ( number_of_lines - 1 ) ) )
		{
			break;
		}
		if( ( line_string_size < 7 )
		 || ( memory_compare(
		       line_string,
		       "layer: ",
		       7 ) != 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: missing layer definition on line: %d.",
			 function,
			 line_index );

			goto on_error;
		}
		if( libfvalue_utf8_string_split(
		     &( line_string[ 7 ] ),
		     line_string_size - 7,
		     (uint8_t) ' ',
		     &values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split line: %d into values.",
			 function,
			 line_index );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_number_of_segments(
		     values,
		     &number_of_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of values",
			 function );

			goto on_error;
		}
		if( ( number_of_values < 2 )
		 || ( number_of_values > 4 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported number of values: %d for line: %d.",
			 function,
			 number_of_values,
			 line_index );

			goto on_error;
		}
		if( libbfoverlay_layer_initialize(
		     &layer,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create layer.",
			 function );

			goto on_error;
		}
		for( value_index = 0;
		     value_index < number_of_values;
		     value_index++ )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     values,
			     value_index,
			     &value_string,
			     &value_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d for line: %d.",
				 function,
				 value_index,
				 line_index );

				goto on_error;
			}
			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing value string: %d for line: %d.",
				 function,
				 value_index,
				 line_index );

				goto on_error;
			}
			if( ( value_string_size > 5 )
			 && ( memory_compare(
			       value_string,
			       "size=",
			       5 ) == 0 ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     &( value_string[ 5 ] ),
				     value_string_size - 5,
				     (uint64_t *) &value_64bit,
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set layer: %d size.",
					 function,
					 layer_index );

					goto on_error;
				}
				if( value_64bit > (size64_t) INT64_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid layer: %d size value exceeds maximum.",
					 function,
					 layer_index );

					goto on_error;
				}
				layer->size = (int64_t) value_64bit;
			}
			else if( ( value_string_size > 7 )
			      && ( memory_compare(
			            value_string,
			            "file=\"",
			            6 ) == 0 )
			      && ( value_string[ value_string_size - 2 ] == '"' ) )
			{
				if( libbfoverlay_layer_set_data_file_path(
				     layer,
				     &( value_string[ 6 ] ),
				     value_string_size - 7,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set layer: %d data file path.",
					 function,
					 layer_index );

					goto on_error;
				}
			}
			else if( ( value_string_size > 7 )
			      && ( memory_compare(
			            value_string,
			            "offset=",
			            7 ) == 0 ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     &( value_string[ 7 ] ),
				     value_string_size - 7,
				     (uint64_t *) &( layer->offset ),
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set layer: %d offset.",
					 function,
					 layer_index );

					goto on_error;
				}
			}
			else if( ( value_string_size > 13 )
			      && ( memory_compare(
			            value_string,
			            "copy-on-write",
			            13 ) == 0 ) )
			{
				layer->use_cow = 1;
			}
			else if( ( value_string_size > 12 )
			      && ( memory_compare(
			            value_string,
			            "file_offset=",
			            12 ) == 0 ) )
			{
				if( libfvalue_utf8_string_copy_to_integer(
				     &( value_string[ 12 ] ),
				     value_string_size - 12,
				     (uint64_t *) &( layer->file_offset ),
				     64,
				     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to set layer: %d file offset.",
					 function,
					 layer_index );

					goto on_error;
				}
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value in layer: %d.",
				 function,
				 layer_index );

				goto on_error;
			}
		}
		if( libfvalue_split_utf8_string_free(
		     &values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split values.",
			 function );

			goto on_error;
		}
		if( layer->use_cow != 0 )
		{
			if( cow_layer_index != -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d only the last layer can use COW.",
				 function,
				 cow_layer_index );

				goto on_error;
			}
			if( layer->size != -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d size specified in combination COW.",
				 function,
				 layer_index );

				goto on_error;
			}
			if( layer->file_offset != -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d file offset specified in combination COW.",
				 function,
				 layer_index );

				goto on_error;
			}
			cow_layer_index = layer_index;
		}
		else if( layer->data_file_path == NULL )
		{
			if( layer->size == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d size value out of bounds - size of layer without a data file must be 0 or greater.",
				 function,
				 layer_index );

				goto on_error;
			}
			if( layer->file_offset >= 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d file offset specified without data file.",
				 function,
				 layer_index );

				goto on_error;
			}
		}
		else if( layer->file_offset == -1 )
		{
			layer->file_offset = 0;
		}
		if( layer_index == 0 )
		{
			/* The logical offset of the base layer must be 0
			 */
			if( layer->offset != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d offset value out of bounds - (logical) offset of base layer must be 0.",
				 function,
				 layer_index );

				goto on_error;
			}
			if( ( layer->data_file_path == NULL )
			 || ( layer->size != -1 ) )
			{
				base_layer_size = layer->size;
			}
		}
		else if( base_layer_size >= 0 )
		{
			/* The logical offset of successive layers must be in bounds of the base layer
			 * a negative offset indicates an offset relative from the end of the base layer
			 */
			if( layer->offset < 0 )
			{
				if( layer->offset <= ( -1 * (off64_t) base_layer_size ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d offset value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
				layer->offset = base_layer_size + layer->offset;
			}
			else
			{
				if( layer->offset >= (off64_t) base_layer_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid layer: %d offset value out of bounds.",
					 function,
					 layer_index );

					goto on_error;
				}
			}
			if( ( layer->size > base_layer_size )
			 || ( layer->offset > (off64_t) ( base_layer_size - layer->size ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid layer: %d size value out of bounds.",
				 function,
				 layer_index );

				goto on_error;
			}
		}
		if( libcdata_array_append_entry(
		     descriptor_file->layers_array,
		     &entry_index,
		     (intptr_t *) layer,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append layer: %d to array.",
			 function,
			 layer_index );

			goto on_error;
		}
		layer = NULL;

		layer_index++;
	}
	if( ( cow_layer_index != -1 )
	 && ( cow_layer_index < ( layer_index - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid layer: %d only the last layer can use COW.",
		 function,
		 cow_layer_index );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_free(
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split lines.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( layer != NULL )
	{
		libbfoverlay_layer_free(
		 &layer,
		 NULL );
	}
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	if( lines != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &lines,
		 NULL );
	}
	libcdata_array_empty(
	 descriptor_file->layers_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libbfoverlay_layer_free,
	 NULL );

	return( -1 );
}

/* Reads the descriptor file
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_descriptor_file_read_file_io_handle(
     libbfoverlay_descriptor_file_t *descriptor_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	uint8_t *file_data    = NULL;
	static char *function = "libbfoverlay_descriptor_file_read_file_io_handle";
	size64_t file_size    = 0;
	ssize_t read_count    = 0;

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_size(
	     file_io_handle,
	     &file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		goto on_error;
	}
	if( ( file_size == 0 )
	 || ( file_size > LIBBFOVERLAY_MAXIMUM_DESCRIPTOR_FILE_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file size value out of bounds.",
		 function );

		goto on_error;
	}
	file_data = (uint8_t *) memory_allocate(
	                         sizeof( uint8_t ) * (size_t) file_size );

	if( file_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              file_data,
	              (size_t) file_size,
	              0,
	              error );

	if( read_count != (ssize_t) file_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor file data at offset: 0 (0x00000000).",
		 function );

		goto on_error;
	}
	if( libbfoverlay_descriptor_file_read_data(
	     descriptor_file,
	     file_data,
	     (size_t) file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor file.",
		 function );

		goto on_error;
	}
	memory_free(
	 file_data );

	return( 1 );

on_error:
	if( file_data != NULL )
	{
		memory_free(
		 file_data );
	}
	libcdata_array_empty(
	 descriptor_file->layers_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libbfoverlay_layer_free,
	 NULL );

	return( -1 );
}

/* Retrieves the number of layers
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_descriptor_get_number_of_layers(
     libbfoverlay_descriptor_file_t *descriptor_file,
     int *number_of_layers,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_descriptor_get_number_of_layers";

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     descriptor_file->layers_array,
	     number_of_layers,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific layer
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_descriptor_get_layer_by_index(
     libbfoverlay_descriptor_file_t *descriptor_file,
     int layer_index,
     libbfoverlay_layer_t **layer,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_descriptor_get_layer_by_index";

	if( descriptor_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor file.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     descriptor_file->layers_array,
	     layer_index,
	     (intptr_t **) layer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve layer: %d.",
		 function,
		 layer_index );

		return( -1 );
	}
	return( 1 );
}

