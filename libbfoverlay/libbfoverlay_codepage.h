/*
 * Codepage functions
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

#if !defined( _LIBBFOVERLAY_INTERNAL_CODEPAGE_H )
#define _LIBBFOVERLAY_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBBFOVERLAY for local use of libbfoverlay
 * The definitions in <libbfoverlay/codepage.h> are copied here
 * for local use of libbfoverlay
 */
#if !defined( HAVE_LOCAL_LIBBFOVERLAY )

#include <libbfoverlay/codepage.h>

#else

/* The codepage definitions
 */
enum LIBBFOVERLAY_CODEPAGES
{
	LIBBFOVERLAY_CODEPAGE_ASCII		= 20127,

	LIBBFOVERLAY_CODEPAGE_ISO_8859_1	= 28591,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_2	= 28592,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_3	= 28593,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_4	= 28594,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_5	= 28595,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_6	= 28596,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_7	= 28597,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_8	= 28598,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_9	= 28599,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_10	= 28600,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_11	= 28601,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_13	= 28603,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_14	= 28604,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_15	= 28605,
	LIBBFOVERLAY_CODEPAGE_ISO_8859_16	= 28606,

	LIBBFOVERLAY_CODEPAGE_KOI8_R		= 20866,
	LIBBFOVERLAY_CODEPAGE_KOI8_U		= 21866,

	LIBBFOVERLAY_CODEPAGE_WINDOWS_874	= 874,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_932	= 932,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_936	= 936,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_949	= 949,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_950	= 950,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1250	= 1250,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1251	= 1251,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1252	= 1252,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1253	= 1253,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1254	= 1254,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1255	= 1255,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1256	= 1256,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1257	= 1257,
	LIBBFOVERLAY_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBBFOVERLAY ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFOVERLAY_INTERNAL_CODEPAGE_H ) */

