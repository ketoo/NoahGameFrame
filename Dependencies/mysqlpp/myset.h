/// \file myset.h
/// \brief Declares templates for generating custom containers used
/// elsewhere in the library.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
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

#ifndef MYSQLPP_MYSET_H
#define MYSQLPP_MYSET_H

#include "common.h"

#include "mystring.h"
#include "stream2string.h"

#include <iostream>
#include <set>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

template <class T, class key_type = typename T::key_type>
class MYSQLPP_EXPORT SetInsert
{
public:
	SetInsert(T* o) : object_(o) { }
	void operator ()(const key_type& data) { object_->insert(data); }

private:
	T* object_;
};

template <class T>
inline SetInsert< std::set<T> > set_insert(std::set<T>* o)
{
	return SetInsert< std::set<T> >(o);
}

template <class Insert>
void set2container(const char* str, Insert insert);

#endif // !defined(DOXYGEN_IGNORE)


/// \brief A special std::set derivative for holding MySQL data sets.

template <class Container = std::set<std::string> >
class MYSQLPP_EXPORT Set : public Container
{
public:
	/// \brief Default constructor
	Set() {};

	/// \brief Create object from a comma-separated list of values
	Set(const char* str)
	{
		set2container(str, set_insert(this));
	}
	
	/// \brief Create object from a comma-separated list of values
	Set(const std::string& str)
	{
		set2container(str.c_str(), set_insert(this));
	}
	
	/// \brief Create object from a comma-separated list of values
	Set(const String& str)
	{
		set2container(str.c_str(), set_insert(this));
	}

	/// \brief Convert this set's data to a string containing
	/// comma-separated items.
	operator std::string() const { return stream2string(*this); }

	/// \brief Return our value in std::string form
	std::string str() const { return *this; }
};


/// \brief Inserts a Set object into a C++ stream
template <class Container>
inline std::ostream& operator <<(std::ostream& s,
		const Set<Container>& d)
{
	typename Container::const_iterator i = d.begin();
	typename Container::const_iterator e = d.end();

	if (i != e) {
		while (true) {
			s << *i;
			if (++i == e) {
				break;
			}
			s << ",";
		}
	}
	
	return s;
}


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

template <class Insert>
void set2container(const char* str, Insert insert)
{
	std::string temp;

	// Break str up using comma separators
	while (str && *str) {
		if (*str == ',') {
			insert(temp);
			temp.clear();

			// Handle comma at end of string case
			if (*++str) {
				++str;
			}
		}
		else {
			temp += *str++;
		}
	}

	// Save final element of set, if any
	if (temp.size()) {
		insert(temp);
	}
}

#endif // !defined(DOXYGEN_IGNORE)


} // end namespace mysqlpp

#endif
