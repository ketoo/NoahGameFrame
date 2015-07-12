/// \file sql_types.h
/// \brief Declares the closest C++ equivalent of each MySQL column type
///
/// The typedefs defined here are only for the "non-NULL" variants.
/// To get nullable versions, wrap the appropriate type in the
/// \c Null<T> template.  See null.h for more information.

/***********************************************************************
 Copyright (c) 2006-2009 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS.txt file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#if !defined(MYSQLPP_SQL_TYPES_H_MAIN)
#define MYSQLPP_SQL_TYPES_H_MAIN

#include "common.h"
#include "tiny_int.h"

#include <string>

#if !defined(MYSQLPP_NO_STDINT_H)
#	include <stdint.h>
#endif

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Suppress refman documentation for these typedefs, as they're
// system-dependent.

// Define C++ integer types that are most nearly equivalent to those
// used by the MySQL server.
#if defined(MYSQLPP_NO_STDINT_H)
	// Boo, we're going to have to wing it.
	typedef tiny_int<signed char>	sql_tinyint;
	typedef tiny_int<unsigned char>	sql_tinyint_unsigned;
	typedef signed short			sql_smallint;
	typedef unsigned short			sql_smallint_unsigned;
	typedef signed int				sql_int;
	typedef unsigned int			sql_int_unsigned;
	typedef signed int				sql_mediumint;
	typedef unsigned int			sql_mediumint_unsigned;
	typedef longlong				sql_bigint;
	typedef ulonglong				sql_bigint_unsigned;
#else
	// Assume a system where C99 is supported in C++ in advance of
	// actual standardization, so we can do this portably.
	typedef tiny_int<int8_t>		sql_tinyint;
	typedef tiny_int<uint8_t>		sql_tinyint_unsigned;
	typedef int16_t					sql_smallint;
	typedef uint16_t				sql_smallint_unsigned;
	typedef int32_t					sql_int;
	typedef uint32_t				sql_int_unsigned;
	typedef int32_t					sql_mediumint;
	typedef uint32_t				sql_mediumint_unsigned;
	typedef int64_t					sql_bigint;
	typedef uint64_t				sql_bigint_unsigned;
#endif

// Now define typedef equivalencies for the other standard MySQL
// data types.  There aren't serious portability issues here.
typedef float					sql_float;
typedef double					sql_double;
typedef double					sql_decimal;
typedef std::string				sql_enum;
typedef std::string				sql_char;
typedef std::string				sql_varchar;
typedef std::string				sql_tinytext;
typedef std::string				sql_text;
typedef std::string				sql_mediumtext;
typedef std::string				sql_longtext;

// Aliases to match the rules MySQL uses in translating data types
// from other database servers into its own type system.  From:
// http://dev.mysql.com/doc/refman/5.0/en/other-vendor-data-types.html
typedef sql_tinyint				sql_bool;
typedef sql_tinyint				sql_boolean;
typedef sql_varchar				sql_character_varying;
typedef sql_decimal				sql_fixed;
typedef sql_float				sql_float4;
typedef sql_double				sql_float8;
typedef sql_tinyint				sql_int1;
typedef sql_smallint			sql_int2;
typedef sql_mediumint			sql_int3;
typedef sql_int					sql_int4;
typedef sql_bigint				sql_int8;
typedef sql_mediumtext			sql_long_varchar;
typedef sql_mediumtext			sql_long;
typedef sql_mediumint			sql_middleint;
typedef sql_decimal				sql_numeric;
#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_SQL_TYPES_H_MAIN)


// The following sections are treated separately to avoid making the
// #include tree too dense: if mystring.h (for example) is not yet
// #included, no sense pulling it in to define all the typedefs based
// on String.  The separate #include guards for each section allow
// this file to be #included as many times as necessary to build up the
// full typedef set.  This trickery is necessary because sql_types.h
// is needed in a few places within MySQL++, but we can't (and don't)
// depend on having the full set of typedefs.  mysql++.h #includes this
// at a late stage, ensuring that end-user code does see the full set.
#if defined(MYSQLPP_MYSTRING_H) && !defined(MYSQLPP_SQL_TYPES_H_MYSTRING) && !defined(DOXYGEN_IGNORE)
#	define MYSQLPP_SQL_TYPES_H_MYSTRING
	namespace mysqlpp {
		typedef String			sql_blob;
		typedef String			sql_tinyblob;
		typedef String			sql_mediumblob;
		typedef String			sql_longblob;
		typedef sql_mediumblob	sql_long_varbinary;
	} // end namespace mysqlpp
#endif


#if defined(MYSQLPP_DATETIME_H) && !defined(MYSQLPP_SQL_TYPES_H_DATETIME) && !defined(DOXYGEN_IGNORE)
#	define MYSQLPP_SQL_TYPES_H_DATETIME
	namespace mysqlpp {
		typedef Date			sql_date;
		typedef Time			sql_time;
		typedef DateTime		sql_timestamp;
		typedef DateTime		sql_datetime;
	} // end namespace mysqlpp
#endif


#if defined(MYSQLPP_MYSET_H) && !defined(MYSQLPP_SQL_TYPES_H_SET) && !defined(DOXYGEN_IGNORE)
#	define MYSQLPP_SQL_TYPES_H_SET
	namespace mysqlpp {
		typedef Set<>				sql_set;
	} // end namespace mysqlpp
#endif

#if defined(MYSQLPP_NULL_H) && !defined(MYSQLPP_SQL_TYPES_H_NULL) && !defined(DOXYGEN_IGNORE)
#	define MYSQLPP_SQL_TYPES_H_NULL
	// We have null.h, so define nullable versions of all the above
	namespace mysqlpp {
		typedef Null<sql_bigint> sql_bigint_null;
		typedef Null<sql_bigint_unsigned> sql_bigint_unsigned_null;
		typedef Null<sql_bool> sql_bool_null;
		typedef Null<sql_boolean> sql_boolean_null;
		typedef Null<sql_char> sql_char_null;
		typedef Null<sql_character_varying> sql_character_varying_null;
		typedef Null<sql_decimal> sql_decimal_null;
		typedef Null<sql_double> sql_double_null;
		typedef Null<sql_enum> sql_enum_null;
		typedef Null<sql_fixed> sql_fixed_null;
		typedef Null<sql_float> sql_float_null;
		typedef Null<sql_float4> sql_float4_null;
		typedef Null<sql_float8> sql_float8_null;
		typedef Null<sql_int> sql_int_null;
		typedef Null<sql_int1> sql_int1_null;
		typedef Null<sql_int2> sql_int2_null;
		typedef Null<sql_int3> sql_int3_null;
		typedef Null<sql_int4> sql_int4_null;
		typedef Null<sql_int8> sql_int8_null;
		typedef Null<sql_int_unsigned> sql_int_unsigned_null;
		typedef Null<sql_long> sql_long_null;
		typedef Null<sql_longtext> sql_longtext_null;
		typedef Null<sql_long_varchar> sql_long_varchar_null;
		typedef Null<sql_mediumint> sql_mediumint_null;
		typedef Null<sql_mediumint_unsigned> sql_mediumint_unsigned_null;
		typedef Null<sql_mediumtext> sql_mediumtext_null;
		typedef Null<sql_middleint> sql_middleint_null;
		typedef Null<sql_numeric> sql_numeric_null;
		typedef Null<sql_smallint> sql_smallint_null;
		typedef Null<sql_smallint_unsigned> sql_smallint_unsigned_null;
		typedef Null<sql_text> sql_text_null;
		typedef Null<sql_tinyint> sql_tinyint_null;
		typedef Null<sql_tinyint_unsigned> sql_tinyint_unsigned_null;
		typedef Null<sql_tinytext> sql_tinytext_null;
		typedef Null<sql_varchar> sql_varchar_null;

		// Also do nullable versions of optional sql_* types, where possible
#		if defined(MYSQLPP_SQL_TYPES_H_MYSTRING)
			typedef Null<sql_blob> sql_blob_null;
			typedef Null<sql_longblob> sql_longblob_null;
			typedef Null<sql_mediumblob> sql_mediumblob_null;
			typedef Null<sql_tinyblob> sql_tinyblob_null;
			typedef Null<sql_long_varbinary> sql_long_varbinary_null;
#		endif
#		if defined(MYSQLPP_SQL_TYPES_H_DATETIME)
			typedef Null<sql_date> sql_date_null;
			typedef Null<sql_datetime> sql_datetime_null;
			typedef Null<sql_time> sql_time_null;
			typedef Null<sql_timestamp> sql_timestamp_null;
#		endif
#		if defined(MYSQLPP_SQL_TYPES_H_SET)
			typedef Null<sql_set> sql_set_null;
#		endif
	} // end namespace mysqlpp
#endif
