/// \file sqlstream.h
/// \brief Defines a class for building quoted and escaped SQL text.

/***********************************************************************
 Copyright (c) 2008 by AboveNet, Inc.  Others may also hold copyrights
 on code in this file.  See the CREDITS file in the top directory of
 the distribution for details.

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

#if !defined(MYSQLPP_SQLSTREAM_H)
#define MYSQLPP_SQLSTREAM_H

#include "common.h"

#include <sstream>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT Connection;
#endif

/// \brief A class for building SQL-formatted strings.
///
/// See the user manual for more details about these options.

class MYSQLPP_EXPORT SQLStream :
public std::ostringstream
{
public:
	/// \brief Create a new stream object attached to a connection.
	///
	/// \param c connection used for escaping text
	/// \param pstr an optional initial string
	SQLStream(Connection* c, const char* pstr = 0);

	/// \brief Create a new stream object as a copy of another.
	///
	/// This is a traditional copy ctor.
	SQLStream(const SQLStream& s);

	/// \brief Return a SQL-escaped version of a character buffer
	///
	/// \param ps pointer to C++ string to hold escaped version; if
	/// original is 0, also holds the original data to be escaped
	/// \param original if given, pointer to the character buffer to
	/// escape instead of contents of *ps
	/// \param length if both this and original are given, number of
	/// characters to escape instead of ps->length()
	///
	/// \retval number of characters placed in *ps
	///
	/// \see comments for escape_string(char*, const char*, size_t)
	/// and DBDriver::escape_string(std::string*, const char *, size_t)
	/// for further details.
	size_t escape_string(std::string* ps, const char* original = 0,
			size_t length = 0) const;

	/// \brief Return a SQL-escaped version of the given character
	/// buffer
	///
	/// \param escaped character buffer to hold escaped version; must
	/// point to at least (length * 2 + 1) bytes
	/// \param original pointer to the character buffer to escape
	/// \param length number of characters to escape
	///
	/// \retval number of characters placed in escaped
	///
	/// DBDriver provides two versions of this method and 
	/// Query::escape_string() calls the appropriate one based on whether
	/// or not a database connection is available.  If the connection
	/// is available, it can call the DBDriver::escape_string() method.
	/// If there is no database connection available (normally only in
	/// testing), then DBDriver provides a static version of the function 
	/// that doesn't use a database connection.
	///
	/// \see comments for DBDriver::escape_string(char*, const char*, size_t),
	/// DBDriver::escape_string_no_conn(char*, const char*, size_t)
	/// for further details.
	size_t escape_string(char* escaped, const char* original,
			size_t length) const;

	/// \brief Assigns contents of another SQLStream to this one
	SQLStream& operator=(const SQLStream& rhs);

	/// \brief Connection to send queries through
	Connection* conn_;
};


/// \brief Insert raw string into the given stream.
///
/// This is just syntactic sugar for SQLStream::str(void)
inline std::ostream& operator <<(std::ostream& os, SQLStream& s)
{
	return os << s.str();
}


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_SQLSTREAM_H)

