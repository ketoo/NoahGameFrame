/// \file field_names.h
/// \brief Declares a class to hold a list of field names.

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

#ifndef MYSQLPP_FIELD_NAMES_H
#define MYSQLPP_FIELD_NAMES_H

#include <string>
#include <vector>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT ResultBase;
#endif

/// \brief Holds a list of SQL field names
class FieldNames : public std::vector<std::string>
{
public:
	/// \brief Default constructor
	FieldNames() { }

	/// \brief Copy constructor
	FieldNames(const FieldNames& other) :
	std::vector<std::string>()
	{
		assign(other.begin(), other.end());
	}
	
	/// \brief Create field name list from a result set
	FieldNames(const ResultBase* res) :
	std::vector<std::string>()
	{
		init(res);
	}

	/// \brief Create empty field name list, reserving space for
	/// a fixed number of field names.
	FieldNames(int i) :
	std::vector<std::string>(i)
	{
	}

	/// \brief Initializes the field list from a result set
	FieldNames& operator =(const ResultBase* res)
	{
		init(res);
		return *this;
	}

	/// \brief Insert \c i empty field names at beginning of list
	FieldNames& operator =(int i)
	{
		insert(begin(), i, "");
		return *this;
	}

	/// \brief Get the name of a field given its index.
	std::string& operator [](int i)
	{
		return at(i);
	}

	/// \brief Get the name of a field given its index, in const
	/// context.
	const std::string& operator [](int i) const
	{
		return at(i);
	}

	/// \brief Get the name of a field given its index.
	std::string& operator [](size_type i)
	{
		return at(i);
	}

	/// \brief Get the name of a field given its index, in const
	/// context.
	const std::string& operator [](size_type i) const
	{
		return at(i);
	}

	/// \brief Get the index number of a field given its name
	unsigned int operator [](const std::string& s) const;

private:
	void init(const ResultBase* res);
};

} // end namespace mysqlpp

#endif
