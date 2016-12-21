/// \file tcp_connection.h
/// \brief Declares the TCPConnection class.

/***********************************************************************
 Copyright (c) 2007-2008 by Educational Technology Resources, Inc.
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

#if !defined(MYSQLPP_TCP_CONNECTION_H)
#define MYSQLPP_TCP_CONNECTION_H

#include "connection.h"

namespace mysqlpp {

/// \brief Specialization of \c Connection for TCP/IP
///
/// This class just simplifies the connection creation interface of
/// \c Connection.  It does not add new functionality.

class MYSQLPP_EXPORT TCPConnection : public Connection
{
public:
	/// \brief Create object without connecting it to the MySQL server.
	TCPConnection() :
	Connection()
	{
	}

	/// \brief Create object and connect to database server over TCP/IP
	/// in one step.
	///
	/// \param addr TCP/IP address of server, in either dotted quad form
	///     or as a host or domain name; may be followed by a colon and
	///     a port number or service name to override default port
	/// \param db name of database to use
	/// \param user user name to log in under, or 0 to use the user
	///		name the program is running under
	/// \param password password to use when logging in
	///
	/// \b BEWARE: These parameters are not in the same order as those
	/// in the corresponding constructor for Connection.  This is a
	/// feature, not a bug. :)
	TCPConnection(const char* addr, const char* db = 0, const char* user = 0,
			const char* password = 0) :
	Connection()
	{
		connect(addr, db, user, password);
	}

	/// \brief Establish a new connection using the same parameters as
	/// an existing connection.
	///
	/// \param other pre-existing connection to clone
	TCPConnection(const TCPConnection& other) :
	Connection(other)
	{
	}

	/// \brief Destroy object
	~TCPConnection() { }

	/// \brief Connect to database after object is created.
	///
	/// It's better to use the connect-on-create constructor if you can.
	/// See its documentation for the meaning of these parameters.
	///
	/// If you call this method on an object that is already connected
	/// to a database server, the previous connection is dropped and a
	/// new connection is established.
	bool connect(const char* addr = 0, const char* db = 0,
			const char* user = 0, const char* password = 0);

	/// \brief Break the given TCP/IP address up into a separate address
	/// and port form
	///
	/// Does some sanity checking on the address.  Only intended to
	/// try and prevent library misuse, not ensure that the address can
	/// actually be used to contact a server.
	///
	/// It understands the following forms:
	///
	///	- 1.2.3.4
	///
	/// - a.b.com:89
	///
	/// - d.e.fr:mysvcname
	///
	/// It also understands IPv6 addresses, but to avoid confusion
	/// between the colons they use and the colon separating the address
	/// part from the service/port part, they must be in RFC 2732 form.
	/// Example: \c [2010:836B:4179::836B:4179]:1234
	///
	/// \param addr the address and optional port/service combo to check
	/// on input, and the verified address on successful return
	/// \param port the port number (resolved from the service name if
	/// necessary) on successful return
	/// \param error on false return, reason for failure is placed here
	///
	/// \return false if address fails to pass sanity checks
	static bool parse_address(std::string& addr, unsigned int& port,
			std::string& error);

private:
	/// \brief Provide uncallable versions of the parent class ctors we
	/// don't want to provide so we don't get warnings about hidden
	/// overloads with some compilers
	TCPConnection(bool) { }
	TCPConnection(const char*, const char*, const char*, const char*,
			unsigned int) { }

	/// \brief Explicitly override parent class version so we don't get
	/// complaints about hidden overloads with some compilers
	bool connect(const char*, const char*, const char*, const char*,
			unsigned int) { return false; }
};


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_TCP_CONNECTION_H)

