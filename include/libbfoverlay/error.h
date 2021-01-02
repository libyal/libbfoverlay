/*
 * The error code definitions for libbfoverlay
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

#if !defined( _LIBBFOVERLAY_ERROR_H )
#define _LIBBFOVERLAY_ERROR_H

#include <libbfoverlay/types.h>

/* External error type definition hides internal structure
 */
typedef intptr_t libbfoverlay_error_t;

/* The error domains
 */
enum LIBBFOVERLAY_ERROR_DOMAINS
{
	LIBBFOVERLAY_ERROR_DOMAIN_ARGUMENTS			= (int) 'a',
	LIBBFOVERLAY_ERROR_DOMAIN_CONVERSION			= (int) 'c',
	LIBBFOVERLAY_ERROR_DOMAIN_COMPRESSION			= (int) 'C',
	LIBBFOVERLAY_ERROR_DOMAIN_IO				= (int) 'I',
	LIBBFOVERLAY_ERROR_DOMAIN_INPUT				= (int) 'i',
	LIBBFOVERLAY_ERROR_DOMAIN_MEMORY			= (int) 'm',
	LIBBFOVERLAY_ERROR_DOMAIN_OUTPUT			= (int) 'o',
	LIBBFOVERLAY_ERROR_DOMAIN_RUNTIME			= (int) 'r'
};

/* The argument error codes
 * to signify errors regarding arguments passed to a function
 */
enum LIBBFOVERLAY_ARGUMENT_ERROR
{
	LIBBFOVERLAY_ARGUMENT_ERROR_GENERIC			= 0,

	/* The argument contains an invalid value
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_INVALID_VALUE		= 1,

	/* The argument contains a value less than zero
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO	= 2,

	/* The argument contains a value zero or less
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS		= 3,

	/* The argument contains a value that exceeds the maximum
	 * for the specific type
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM	= 4,

	/* The argument contains a value that is too small
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_VALUE_TOO_SMALL		= 5,

	/* The argument contains a value that is too large
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_VALUE_TOO_LARGE		= 6,

	/* The argument contains a value that is out of bounds
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS		= 7,

	/* The argument contains a value that is not supported
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_UNSUPPORTED_VALUE		= 8,

	/* The argument contains a value that conficts with another argument
	 */
	LIBBFOVERLAY_ARGUMENT_ERROR_CONFLICTING_VALUE		= 9
};

/* The conversion error codes
 * to signify errors regarding conversions
 */
enum LIBBFOVERLAY_CONVERSION_ERROR
{
	LIBBFOVERLAY_CONVERSION_ERROR_GENERIC			= 0,

	/* The conversion failed on the input
	 */
	LIBBFOVERLAY_CONVERSION_ERROR_INPUT_FAILED		= 1,

	/* The conversion failed on the output
	 */
	LIBBFOVERLAY_CONVERSION_ERROR_OUTPUT_FAILED		= 2
};

/* The compression error codes
 * to signify errors regarding compression
 */
enum LIBBFOVERLAY_COMPRESSION_ERROR
{
	LIBBFOVERLAY_COMPRESSION_ERROR_GENERIC			= 0,

	/* The compression failed
	 */
	LIBBFOVERLAY_COMPRESSION_ERROR_COMPRESS_FAILED		= 1,

	/* The decompression failed
	 */
	LIBBFOVERLAY_COMPRESSION_ERROR_DECOMPRESS_FAILED	= 2
};

/* The input/output error codes
 * to signify errors regarding input/output
 */
enum LIBBFOVERLAY_IO_ERROR
{
	LIBBFOVERLAY_IO_ERROR_GENERIC				= 0,

	/* The open failed
	 */
	LIBBFOVERLAY_IO_ERROR_OPEN_FAILED			= 1,

	/* The close failed
	 */
	LIBBFOVERLAY_IO_ERROR_CLOSE_FAILED			= 2,

	/* The seek failed
	 */
	LIBBFOVERLAY_IO_ERROR_SEEK_FAILED			= 3,

	/* The read failed
	 */
	LIBBFOVERLAY_IO_ERROR_READ_FAILED			= 4,

	/* The write failed
	 */
	LIBBFOVERLAY_IO_ERROR_WRITE_FAILED			= 5,

	/* Access denied
	 */
	LIBBFOVERLAY_IO_ERROR_ACCESS_DENIED			= 6,

	/* The resource is invalid i.e. a missing file
	 */
	LIBBFOVERLAY_IO_ERROR_INVALID_RESOURCE			= 7,

	/* The ioctl failed
	 */
	LIBBFOVERLAY_IO_ERROR_IOCTL_FAILED			= 8,

	/* The unlink failed
	 */
	LIBBFOVERLAY_IO_ERROR_UNLINK_FAILED			= 9
};

/* The input error codes
 * to signify errors regarding handing input data
 */
enum LIBBFOVERLAY_INPUT_ERROR
{
	LIBBFOVERLAY_INPUT_ERROR_GENERIC			= 0,

	/* The input contains invalid data
	 */
	LIBBFOVERLAY_INPUT_ERROR_INVALID_DATA			= 1,

	/* The input contains an unsupported signature
	 */
	LIBBFOVERLAY_INPUT_ERROR_SIGNATURE_MISMATCH		= 2,

	/* A checksum in the input did not match
	 */
	LIBBFOVERLAY_INPUT_ERROR_CHECKSUM_MISMATCH		= 3,

	/* A value in the input did not match a previously
	 * read value or calculated value
	 */
	LIBBFOVERLAY_INPUT_ERROR_VALUE_MISMATCH			= 4
};

/* The memory error codes
 * to signify errors regarding memory
 */
enum LIBBFOVERLAY_MEMORY_ERROR
{
	LIBBFOVERLAY_MEMORY_ERROR_GENERIC			= 0,

	/* There is insufficient memory available
	 */
	LIBBFOVERLAY_MEMORY_ERROR_INSUFFICIENT			= 1,

	/* The memory failed to be copied
	 */
	LIBBFOVERLAY_MEMORY_ERROR_COPY_FAILED			= 2,

	/* The memory failed to be set
	 */
	LIBBFOVERLAY_MEMORY_ERROR_SET_FAILED			= 3
};

/* The output error codes
 */
enum LIBBFOVERLAY_OUTPUT_ERROR
{
	LIBBFOVERLAY_OUTPUT_ERROR_GENERIC			= 0,

	/* There is insuficient space to write the output
	 */
	LIBBFOVERLAY_OUTPUT_ERROR_INSUFFICIENT_SPACE		= 1
};

/* The runtime error codes
 * to signify errors regarding runtime processing
 */
enum LIBBFOVERLAY_RUNTIME_ERROR
{
	LIBBFOVERLAY_RUNTIME_ERROR_GENERIC			= 0,

	/* The value is missing
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_VALUE_MISSING		= 1,

	/* The value was already set
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_VALUE_ALREADY_SET		= 2,

	/* The creation and/or initialization of an internal structure failed
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_INITIALIZE_FAILED		= 3,

	/* The resize of an internal structure failed
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_RESIZE_FAILED		= 4,

	/* The free and/or finalization of an internal structure failed
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_FINALIZE_FAILED		= 5,

	/* The value could not be determined
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_GET_FAILED			= 6,

	/* The value could not be set
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_SET_FAILED			= 7,

	/* The value could not be appended/prepended
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_APPEND_FAILED		= 8,

	/* The value could not be copied
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_COPY_FAILED			= 9,

	/* The value could not be removed
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_REMOVE_FAILED		= 10,

	/* The value could not be printed
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_PRINT_FAILED			= 11,

	/* The value was out of bounds
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS		= 12,

	/* The value exceeds the maximum for its specific type
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM	= 13,

	/* The value is unsupported
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_UNSUPPORTED_VALUE		= 14,

	/* An abort was requested
	 */
	LIBBFOVERLAY_RUNTIME_ERROR_ABORT_REQUESTED		= 15
};

#endif /* !defined( _LIBBFOVERLAY_ERROR_H ) */

