/// \file field.h
/// \brief Declares the Field and Fields classes.

/***********************************************************************
 Copyright (c) 2007 by Educational Technology Resources, Inc.
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

#if !defined(MYSQLPP_FIELD_H)
#define MYSQLPP_FIELD_H

#include "common.h"
#include "type_info.h"

#include <vector>

namespace mysqlpp {

/// \brief Class to hold information about a SQL field
///
/// This is a cut-down version of MYSQL_FIELD, using MySQL++ and generic
/// C++ types instead of the C types it uses, and hiding all fields
/// behind accessors.  It leaves out data members we have decided aren't
/// very useful.  Given a good argument, we're willing to mirror more of
/// the fields; we just don't want to mirror the underlying structure
/// slavishly for no benefit.

class Field
{
public:
	/// \brief Create empty object
	Field() :
	length_(0),
	max_length_(0),
	flags_(0)
	{
	}

	/// \brief Create object from C API field structure
	Field(const MYSQL_FIELD* pf) :
	name_(pf->name),
	table_(pf->table),
#if MYSQL_VERSION_ID > 40000	// only in 4.0 +
	db_(pf->db),
#endif
	type_(pf->type, (pf->flags & UNSIGNED_FLAG) != 0,
			(pf->flags & NOT_NULL_FLAG) == 0),
	length_(pf->length),
	max_length_(pf->max_length),
	flags_(pf->flags)
	{
	}

	/// \brief Create object as a copy of another Field
	Field(const Field& other) :
	name_(other.name_),
	table_(other.table_),
	db_(other.db_),
	type_(other.type_),
	length_(other.length_),
	max_length_(other.max_length_),
	flags_(other.flags_)
	{
	}

	/// \brief Returns true if field auto-increments
	bool auto_increment() const { return flags_ & AUTO_INCREMENT_FLAG; }

	/// \brief Returns true if field is of some binary type
	bool binary_type() const { return flags_ & BINARY_FLAG; }

	/// \brief Returns true if field is of some BLOB type
	bool blob_type() const { return flags_ & BLOB_FLAG; }

	/// \brief Return the name of the database the field comes from
	const char* db() const { return db_.c_str(); }

	/// \brief Returns true if field is of an enumerated value type
	bool enumeration() const { return flags_ & ENUM_FLAG; }

	/// \brief Return the creation size of the field
	///
	/// This is the number of bytes the field can hold, not how much is
	/// actually stored in the field on any particular row.
	size_t length() const { return length_; }

	/// \brief Return the maximum number of bytes stored in this field
	/// in any of the rows in the result set we were created from.
	size_t max_length() const { return max_length_; }

	/// \brief Returns true if field is part of a key
	bool multiple_key() const { return flags_ & MULTIPLE_KEY_FLAG; }

	/// \brief Return the field's name
	const char* name() const { return name_.c_str(); }

#if defined(NO_DEFAULT_VALUE_FLAG)
	/// \brief Returns true if field has no default value
	bool no_default() const { return flags_ & NO_DEFAULT_VALUE_FLAG; }
#endif

	/// \brief Returns true if field is part of a primary key
	bool primary_key() const { return flags_ & PRI_KEY_FLAG; }

	/// \brief Returns true if field is of some 'set' type
	bool set_type() const { return flags_ & SET_FLAG; }

	/// \brief Return the name of the table the field comes from
	const char* table() const { return table_.c_str(); }

	/// \brief Returns true if field's type is timestamp
	bool timestamp() const { return flags_ & TIMESTAMP_FLAG; }

	/// \brief Return information about the field's type
	const mysql_type_info& type() const { return type_; }

	/// \brief Returns true if field is part of a unique key
	bool unique_key() const { return flags_ & UNIQUE_KEY_FLAG; }

	/// \brief Returns true if field has the zerofill attribute
	bool zerofill() const { return flags_ & ZEROFILL_FLAG; }

private:
	std::string name_;		///< the field's name
	std::string table_;		///< name of table field comes from
	std::string db_;		///< name of database field comes from
	mysql_type_info type_;	///< info about the field's type
	size_t length_;			///< creation size of column
	size_t max_length_;		///< size of largest item in column in result set
	unsigned int flags_;	///< DB engine-specific set of bit flags
};


/// \brief The list-of-Fields type
typedef std::vector<Field> Fields;

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_FIELD_H)
