/// \file stream2string.h
/// \brief Declares an adapter that converts something that can be
/// inserted into a C++ stream into a std::string type.

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

#if !defined(MYSQLPP_STREAM2STRING_H)
#define MYSQLPP_STREAM2STRING_H

#include <sstream>
#include <string>

namespace mysqlpp {

/// \brief Converts anything you can insert into a C++ stream to a
/// \c std::string via \c std::ostringstream.

template <class T>
std::string stream2string(const T& object)
{
	std::ostringstream str;
	str << object;
	return str.str();
}

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_STREAM2STRING_H)

