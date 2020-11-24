/*
 * Support functions
 *
 * Copyright (C) 2020, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libbfoverlay_definitions.h"
#include "libbfoverlay_libcerror.h"
#include "libbfoverlay_libclocale.h"
#include "libbfoverlay_support.h"

#if !defined( HAVE_LOCAL_LIBBFOVERLAY )

/* Returns the library version
 */
const char *libbfoverlay_get_version(
             void )
{
	return( (const char *) LIBBFOVERLAY_VERSION_STRING );
}

/* Returns the access flags for reading
 */
int libbfoverlay_get_access_flags_read(
     void )
{
	return( (int) LIBBFOVERLAY_ACCESS_FLAG_READ );
}

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_get_codepage(
     int *codepage,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_get_codepage";

	if( libclocale_codepage_get(
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libbfoverlay_set_codepage(
     int codepage,
     libcerror_error_t **error )
{
	static char *function = "libbfoverlay_set_codepage";

	if( libclocale_codepage_set(
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBBFOVERLAY ) */

