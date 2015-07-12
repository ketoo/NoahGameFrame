/// \file type_info.h
/// \brief Declares classes that provide an interface between the SQL
/// and C++ type systems.
///
/// These classes are mostly used internal to the library.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2008 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS.txt file
 in the top directory of the distribution for details.

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

#if !defined(MYSQLPP_TYPE_INFO_H)
#define MYSQLPP_TYPE_INFO_H

#include "common.h"

#include "exceptions.h"

#include <map>
#include <sstream>
#include <typeinfo>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

class MYSQLPP_EXPORT mysql_type_info;
class MYSQLPP_EXPORT mysql_ti_sql_type_info_lookup;

class MYSQLPP_EXPORT mysql_ti_sql_type_info
{
private:
	// For use with flags_ bitset
	enum {
		tf_default = 1,
		tf_null = 2,
		tf_unsigned = 4
	};

	friend class mysql_type_info;
	friend class mysql_ti_sql_type_info_lookup;

	mysql_ti_sql_type_info& operator=(
			const mysql_ti_sql_type_info& b);
	
	// Not initting _base_type and _default because only mysql_type_info
	// can create them.  There *must* be only one copy of each.
	mysql_ti_sql_type_info() :
	sql_name_(0),
	c_type_(0),
	base_type_(
#if MYSQL_VERSION_ID > 40000
		MYSQL_TYPE_NULL
#else
		FIELD_TYPE_NULL
#endif
	),
	flags_(0)
	{
	}
	
	mysql_ti_sql_type_info(const char* s,
			const std::type_info& t, const enum_field_types bt,
			const unsigned int flags = 0) :
	sql_name_(s),
	c_type_(&t),
	base_type_(bt),
	flags_(flags)
	{
	}

	bool is_default() const { return flags_ & tf_default; }
	bool is_null() const { return flags_ & tf_null; }
	bool is_unsigned() const { return flags_ & tf_unsigned; }

	const char* sql_name_;
	const std::type_info* c_type_;
	const enum_field_types base_type_;
	const unsigned int flags_;
};


struct type_info_cmp
{
	bool operator() (const std::type_info* lhs,
			const std::type_info* rhs) const
	{
		return lhs->before(*rhs) != 0;
	}
};

class MYSQLPP_EXPORT mysql_ti_sql_type_info_lookup
{
private:
	friend class mysql_type_info;

	typedef mysql_ti_sql_type_info sql_type_info;
	typedef std::map<const std::type_info*, unsigned char, type_info_cmp>
			map_type;

	mysql_ti_sql_type_info_lookup(const sql_type_info types[],
			const int size);

	const unsigned char& operator [](
			const std::type_info& ti) const
	{
		map_type::const_iterator it = map_.find(&ti);
		if (it != map_.end()) {
			return it->second;
		}
		else {
			std::ostringstream outs;
			outs << "Failed to find MySQL C API type ID for " << ti.name();
			throw TypeLookupFailed(outs.str());
		}
	}

	map_type map_;
};

#endif // !defined(DOXYGEN_IGNORE)


/// \brief SQL field type information
///
/// \internal Used within MySQL++ for mapping SQL types to C++ types
/// and vice versa.
class MYSQLPP_EXPORT mysql_type_info
{
public:
	/// \brief Default constructor
	///
	/// This only exists because FieldTypes keeps a vector of these
	/// objects.  You are expected to copy real values into it before
	/// using it via the copy ctor or one of the assignment operators.
	/// If you don't, we have arranged a pretty spectacular crash for
	/// your program.  So there.
	mysql_type_info() :
	num_(static_cast<unsigned char>(-1))
	{
	}

	/// \brief Create object from MySQL C API type info
	///
	/// \param t the underlying C API type ID for this type
	/// \param _unsigned if true, this is the unsigned version of the type
	/// \param _null if true, this type can hold a SQL null
	mysql_type_info(enum_field_types t, bool _unsigned = false,
			bool _null = false) :
	num_(type(t, _unsigned, _null))
	{
	}

	/// \brief Create object as a copy of another
	mysql_type_info(const mysql_type_info& t) :
	num_(t.num_)
	{
	}

	/// \brief Create object from a C++ type_info object
	///
	/// This tries to map a C++ type to the closest MySQL data type.
	/// It is necessarily somewhat approximate.
	mysql_type_info(const std::type_info& t) :
	num_(lookups[t])
	{
	}

	/// \brief Assign another mysql_type_info object to this object
	mysql_type_info& operator =(const mysql_type_info& t)
	{
		num_ = t.num_;
		return *this;
	}

	/// \brief Assign a C++ type_info object to this object
	///
	/// This tries to map a C++ type to the closest MySQL data type.
	/// It is necessarily somewhat approximate.
	mysql_type_info& operator =(const std::type_info& t)
	{
		num_ = lookups[t];
		return *this;
	}

	/// \brief Returns an implementation-defined name of the C++ type.
	///
	/// Returns the name that would be returned by typeid().name() for
	/// the C++ type associated with the SQL type.
	const char* name() const { return deref().c_type_->name(); }

	/// \brief Returns the name of the SQL type.
	///
	/// Returns the SQL name for the type.
	const char* sql_name() const { return deref().sql_name_; }

	/// \brief Returns the type_info for the C++ type associated with
	/// the SQL type.
	///
	/// Returns the C++ type_info record corresponding to the SQL type.
	const std::type_info& c_type() const { return *deref().c_type_; }

	/// \brief Returns the type_info for the C++ type inside of the
	/// mysqlpp::Null type.
	///
	/// Returns the type_info for the C++ type inside the mysqlpp::Null
	/// type.  If the type is not Null then this is the same as c_type().
	const mysql_type_info base_type() const
	{
		return mysql_type_info(deref().base_type_);
	}

	/// \brief Returns the ID of the SQL type.
	///
	/// Returns the ID number MySQL uses for this type.  Note: Do not
	/// depend on the value of this ID as it may change between MySQL
	/// versions.
	int id() const
	{
		return num_;
	}
	
	/// \brief Returns true if the SQL type is of a type that needs to
	/// be quoted.
	///
	/// \return true if the type needs to be quoted for syntactically
	/// correct SQL.
	bool quote_q() const;

	/// \brief Returns true if the SQL type is of a type that needs to
	/// be escaped.
	///
	/// \return true if the type needs to be escaped for syntactically
	/// correct SQL.
	bool escape_q() const;

	/// \brief Provides a way to compare two types for sorting.
	///
	/// Returns true if the SQL ID of this type is lower than that of
	/// another.  Used by mysqlpp::type_info_cmp when comparing types.
	bool before(mysql_type_info& b)
	{
		return num_ < b.num_;
	}

	/// \brief The internal constant we use for our string type.
	///
	/// We expose this because other parts of MySQL++ need to know
	/// what the string constant is at the moment.
	static const enum_field_types string_type =
#if MYSQL_VERSION_ID > 40000
		MYSQL_TYPE_STRING;
#else
		FIELD_TYPE_STRING;
#endif

private:
	typedef mysql_ti_sql_type_info sql_type_info;
	typedef mysql_ti_sql_type_info_lookup sql_type_info_lookup;

	static const sql_type_info types[];
	static const int num_types;

	static const sql_type_info_lookup lookups;

	/// \brief Return an index into mysql_type_info::types array given
	/// MySQL type information.
	///
	/// This function is used in mapping from MySQL type information
	/// (a type enum, and flags indicating whether it is unsigned and
	/// whether it can be 'null') to the closest C++ types available
	/// within MySQL++.  Notice that nulls have to be handled specially:
	/// the SQL null concept doesn't map directly onto the C++ type
	/// system.  See null.h for details.
	///
	/// \param t Underlying C API type constant
	/// \param _unsigned if true, indicates the unsigned variant of a
	/// MySQL type
	/// \param _null if true, indicates the variant of the MySQL type
	/// that can also hold an SQL 'null' instead of regular data.
	///
	/// While MySQL++ is tied to MySQL, \c t is just an abstraction
	/// of enum_field_types from mysql_com.h.
	static unsigned char type(enum_field_types t,
			bool _unsigned, bool _null = false);

	const sql_type_info& deref() const
	{
		return types[num_];
	}

	unsigned char num_;
};

/// \brief Returns true if two mysql_type_info objects are equal.
inline bool operator ==(const mysql_type_info& a, const mysql_type_info& b)
{
	return a.id() == b.id();
}

/// \brief Returns true if two mysql_type_info objects are not equal.
inline bool operator !=(const mysql_type_info& a, const mysql_type_info& b)
{
	return a.id() != b.id();
}

/// \brief Returns true if a given mysql_type_info object is equal
/// to a given C++ type_info object.
inline bool operator ==(const std::type_info& a, const mysql_type_info& b)
{
	return a == b.c_type();
}

/// \brief Returns true if a given mysql_type_info object is not equal
/// to a given C++ type_info object.
inline bool operator !=(const std::type_info& a, const mysql_type_info& b)
{
	return a != b.c_type();
}

/// \brief Returns true if a given mysql_type_info object is equal
/// to a given C++ type_info object.
inline bool operator ==(const mysql_type_info& a, const std::type_info& b)
{
	return a.c_type() == b;
}

/// \brief Returns true if a given mysql_type_info object is not equal
/// to a given C++ type_info object.
inline bool operator !=(const mysql_type_info& a, const std::type_info& b)
{
	return a.c_type() != b;
}

}								// end namespace mysqlpp

#endif // !defined(MYSQLPP_TYPE_INFO_H)

