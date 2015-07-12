/// \file field_types.h
/// \brief Declares a class to hold a list of SQL field type info.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2007 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

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

#ifndef MYSQLPP_FIELD_TYPES_H
#define MYSQLPP_FIELD_TYPES_H

#include "type_info.h"

#include <vector>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT ResultBase;
#endif

/// \brief A vector of SQL field types.
class FieldTypes : public std::vector<mysql_type_info>
{
public:
	/// \brief Default constructor
	FieldTypes() { }
	
	/// \brief Create list of field types from a result set
	FieldTypes(const ResultBase* res)
	{
		init(res);
	}

	/// \brief Create fixed-size list of uninitialized field types
	FieldTypes(int i) :
	std::vector<mysql_type_info>(i)
	{
	}

	/// \brief Initialize field list based on a result set
	FieldTypes& operator =(const ResultBase* res)
	{
		init(res);
		return *this;
	}

	/// \brief Insert a given number of uninitialized field type
	/// objects at the beginning of the list
	///
	/// \param i number of field type objects to insert
	FieldTypes& operator =(int i)
	{
		insert(begin(), i, mysql_type_info());
		return *this;
	}

private:
	void init(const ResultBase* res);
};

} // end namespace mysqlpp

#endif
