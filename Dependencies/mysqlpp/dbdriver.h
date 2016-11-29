/// \file dbdriver.h
/// \brief Declares the DBDriver class.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2009 by Educational Technology Resources, Inc.  Others may
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

#if !defined(MYSQLPP_DBDRIVER_H)
#define MYSQLPP_DBDRIVER_H

#include "common.h"

#include "options.h"

#include <typeinfo>

#include <limits.h>

namespace mysqlpp {

/// \brief Provides a thin abstraction layer over the underlying database 
/// client library.
///
/// This class does as little as possible to adapt between its public
/// interface and the interface required by the underlying C API.  That
/// is, in fact, its only mission.  The high-level interfaces indended
/// for use by MySQL++ users are in Connection, Query, Result, and
/// ResUse, all of which delegate the actual database communication to
/// an object of this type, created by Connection.  If you really need
/// access to the low-level database driver, get it via
/// Connection::driver(); don't create DBDriver objects directly.
///
/// Currently this is a concrete class for wrapping the MySQL C API.
/// In the future, it may be turned into an abstract base class, with
/// subclasses for different database server types.

class MYSQLPP_EXPORT DBDriver
{
public:
	/// \brief Result code returned by next_result()
	enum nr_code {
		nr_more_results,	///< success, with more results to come
		nr_last_result,		///< success, last result received
		nr_error,			///< problem retrieving next result
		nr_not_supported	///< this C API doesn't support "next result"
	};

	/// \brief Create object
	DBDriver();

	/// \brief Duplicate an existing driver
	///
	/// \param other existing DBDriver object
	///
	/// This establishes a new database server connection with the same
	/// parameters as the other driver's.
	DBDriver(const DBDriver& other);

	/// \brief Destroy object
	virtual ~DBDriver();

	/// \brief Return the number of rows affected by the last query
	///
	/// Wraps \c mysql_affected_rows() in the MySQL C API.
	ulonglong affected_rows()
	{
		error_message_.clear();
		return mysql_affected_rows(&mysql_);
	}

	/// \brief Get database client library version
	///
	/// Wraps \c mysql_get_client_info() in the MySQL C API.
	std::string client_version() const
	{
		error_message_.clear();
		return mysql_get_client_info();
	}

	/// \brief Establish a new connection using the same parameters as
	/// an existing connection.
	///
	/// \param mysql existing MySQL C API connection object
	bool connect(const MYSQL& mysql);

	/// \brief Connect to database server
	///
	/// If you call this method on an object that is already connected
	/// to a database server, the previous connection is dropped and a
	/// new connection is established.
	virtual bool connect(const char* host, const char* socket_name,
			unsigned int port, const char* db, const char* user,
			const char* password);

	/// \brief Return true if we have an active connection to the
	/// database server.
	///
	/// This does not actually check whether the connection is viable,
	/// it just indicates whether there was previously a successful
	/// connect() call and no disconnect().  Call ping() to actually
	/// test the connection's viability.
	bool connected() const { return is_connected_; }

	/// \brief Establish a new connection as a copy of an existing one
	///
	/// \param other the connection to copy
	void copy(const DBDriver& other);

	/// \brief Ask the database server to create a database
	///
	/// \param db name of database to create
	///
	/// \return true if database was created successfully
	bool create_db(const char* db) const;

	/// \brief Seeks to a particualr row within the result set
	///
	/// Wraps mysql_data_seek() in MySQL C API.
	void data_seek(MYSQL_RES* res, ulonglong offset) const
	{
		error_message_.clear();
		mysql_data_seek(res, offset);
	}

	/// \brief Drop the connection to the database server
	///
	/// This method should only be used by MySQL++ library internals.
	/// Unless you use the default constructor, this object should
	/// always be connected.
	void disconnect();

	/// \brief Drop a database
	///
	/// \param db name of database to destroy
	///
	/// \return true if database was created successfully
	bool drop_db(const std::string& db) const;

	/// \brief Enable SSL-encrypted connection.
	///
	/// \param key the pathname to the key file
	/// \param cert the pathname to the certificate file
	/// \param ca the pathname to the certificate authority file
	/// \param capath directory that contains trusted SSL CA
	///        certificates in pem format.
	/// \param cipher list of allowable ciphers to use
	///
	/// \return False if call fails or the C API library wasn't compiled
	/// with SSL support enabled.
	///
	/// Must be called before connection is established.
	///
	/// Wraps \c mysql_ssl_set() in MySQL C API.
	bool enable_ssl(const char* key = 0, const char* cert = 0,
			const char* ca = 0, const char* capath = 0,
			const char* cipher = 0);

	/// \brief Return error message for last MySQL error associated with
	/// this connection.
	///
	/// Can return a MySQL++ DBDriver-specific error message if there
	/// is one.  If not, it simply wraps \c mysql_error() in the MySQL C API.
	const char* error()
	{
		return error_message_.length() ? error_message_.c_str() : mysql_error(&mysql_);
	}

	/// \brief Return last MySQL error number associated with this
	/// connection
	///
	/// Wraps \c mysql_errno() in the MySQL C API.
	int errnum() { return mysql_errno(&mysql_); }

	/// \brief Return a SQL-escaped version of the given character
	/// buffer
	///
	/// \param to character buffer to hold escaped version; must
	/// point to at least (length * 2 + 1) bytes
	/// \param from pointer to the character buffer to escape
	/// \param length number of characters to escape
	///
	/// \retval number of characters placed in escaped
	///
	/// Wraps \c mysql_real_escape_string() in the MySQL C API.
	///
	/// Proper SQL escaping takes the database's current character set 
	/// into account, however if a database connection isn't available
	/// DBDriver also provides a static version of this same method.
	///
	/// \sa escape_string_no_conn(char*, const char*, size_t)
	size_t escape_string(char* to, const char* from, size_t length)
	{
		error_message_.clear();
		return mysql_real_escape_string(&mysql_, to, from, 
				static_cast<unsigned long>(length));
	}

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
	/// This method has three basic operation modes:
	///
	/// - Pass just a pointer to a C++ string containing the original
	///   data to escape, plus act as receptacle for escaped version
	/// - Pass a pointer to a C++ string to receive escaped string plus
	///   a pointer to a C string to be escaped
	/// - Pass nonzero for all parameters, taking original to be a
	///   pointer to an array of char with given length; does not treat
	///   null characters as special
	///
	/// There's a degenerate fourth mode, where ps is zero: simply
	/// returns 0, because there is nowhere to store the result.
	///
	/// Note that if original is 0, we always ignore the length
	/// parameter even if it is nonzero.  Length always comes from
	/// ps->length() in this case.
	///
	/// ps is a pointer because if it were a reference, the other
	/// overload would be impossible to call: the compiler would
	/// complain that the two overloads are ambiguous because
	/// std::string has a char* conversion ctor. A nice bonus is that
	/// pointer syntax makes it clearer that the first parameter is an
	/// "out" parameter.
	///
	/// \see comments for escape_string(char*, const char*, size_t)
	/// for further details.
	/// 
	/// \sa escape_string_no_conn(std::string*, const char*, size_t)
	size_t escape_string(std::string* ps, const char* original,
			size_t length);

	/// \brief SQL-escapes the given string without reference to the 
	/// character set of a database server.
	///
	/// Wraps \c mysql_escape_string() in the MySQL C API.
	///
	/// \sa escape_string(char*, const char*, size_t)
	static size_t escape_string_no_conn(char* to, const char* from,
			size_t length)
	{
		return mysql_escape_string(to, from,
				static_cast<unsigned long>(length));
	}

	/// \brief SQL-escapes the given string without reference to the 
	/// character set of a database server.
	///
	/// \sa escape_string(std::string*, const char*, size_t),
	/// escape_string_no_conn(char*, const char*, size_t)
	static size_t escape_string_no_conn(std::string* ps, 
			const char* original = 0, size_t length = 0);

	/// \brief Executes the given query string
	///
	/// Wraps \c mysql_real_query() in the MySQL C API.
	bool execute(const char* qstr, size_t length)
	{
		error_message_.clear();
		return !mysql_real_query(&mysql_, qstr,
				static_cast<unsigned long>(length));
	}

	/// \brief Returns the next raw C API row structure from the given
	/// result set.
	///
	/// This is for "use" query result sets only.  "store" queries have
	/// all the rows already.
	///
	/// Wraps \c mysql_fetch_row() in MySQL C API.
	MYSQL_ROW fetch_row(MYSQL_RES* res) const
	{
		error_message_.clear();
		return mysql_fetch_row(res);
	}

	/// \brief Returns the lengths of the fields in the current row
	/// from a "use" query.
	///
	/// Wraps \c mysql_fetch_lengths() in MySQL C API.
	const unsigned long* fetch_lengths(MYSQL_RES* res) const
	{
		error_message_.clear();
		return mysql_fetch_lengths(res);
	}

	/// \brief Returns information about a particular field in a result
	/// set
	///
	/// \param res result set to fetch field information for
	/// \param i field number to fetch information for, if given
	///
	/// If i parameter is given, this call is like a combination of
	/// field_seek() followed by fetch_field() without the i parameter,
	/// which otherwise just iterates through the set of fields in the
	/// given result set.
	///
	/// Wraps \c mysql_fetch_field() and mysql_fetch_field_direct() in
	/// MySQL C API.  (Which one it uses depends on i parameter.)
	MYSQL_FIELD* fetch_field(MYSQL_RES* res, size_t i = UINT_MAX) const
	{
		error_message_.clear();
		return i == UINT_MAX ? mysql_fetch_field(res) :
				mysql_fetch_field_direct(res,
				static_cast<unsigned int>(i));
	}

	/// \brief Jumps to the given field within the result set
	///
	/// Wraps \c mysql_field_seek() in MySQL C API.
	void field_seek(MYSQL_RES* res, size_t field) const
	{
		error_message_.clear();
		mysql_field_seek(res, MYSQL_FIELD_OFFSET(field));
	}

	/// \brief Releases memory used by a result set
	///
	/// Wraps \c mysql_free_result() in MySQL C API.
	void free_result(MYSQL_RES* res) const
	{
		error_message_.clear();
		mysql_free_result(res);
	}

	/// \brief Return the connection options object
	st_mysql_options get_options() const { return mysql_.options; }

	/// \brief Get information about the IPC connection to the
	/// database server
	///
	/// String contains info about type of connection (e.g. TCP/IP,
	/// named pipe, Unix socket...) and the server hostname.
	///
	/// Wraps \c mysql_get_host_info() in the MySQL C API.
	std::string ipc_info()
	{
		error_message_.clear();
		return mysql_get_host_info(&mysql_);
	}

	/// \brief Get ID generated for an AUTO_INCREMENT column in the
	/// previous INSERT query.
	///
	/// \retval 0 if the previous query did not generate an ID.  Use
	/// the SQL function LAST_INSERT_ID() if you need the last ID
	/// generated by any query, not just the previous one.  This
	/// applies to stored procedure calls because this function returns
	/// the ID generated by the last query, which was a CALL statement,
	/// and CALL doesn't generate IDs.  You need to use LAST_INSERT_ID()
	/// to get the ID in this case.
	ulonglong insert_id()
	{
		error_message_.clear();
		return mysql_insert_id(&mysql_);
	}

	/// \brief Kill a MySQL server thread
	///
	/// \param tid ID of thread to kill
	///
	/// Wraps \c mysql_kill() in the MySQL C API.
	///
	/// \see thread_id()
	bool kill(unsigned long tid)
	{
		error_message_.clear();
		return !mysql_kill(&mysql_, tid);
	}

	/// \brief Returns true if there are unconsumed results from the
	/// most recent query.
	///
	/// Wraps \c mysql_more_results() in the MySQL C API.
	bool more_results()
	{
		error_message_.clear();
		#if MYSQL_VERSION_ID > 41000		// only in MySQL v4.1 +
			return mysql_more_results(&mysql_);
		#else
			return false;
		#endif
	}

	/// \brief Moves to the next result set from a multi-query
	///
	/// \return A code indicating whether we successfully found another
	/// result, there were no more results (but still success) or
	/// encountered an error trying to find the next result set.
	///
	/// Wraps \c mysql_next_result() in the MySQL C API, with
	/// translation of its return value from magic integers to nr_code
	/// enum values.
	nr_code next_result()
	{
		error_message_.clear();
		#if MYSQL_VERSION_ID > 41000		// only in MySQL v4.1 +
			switch (mysql_next_result(&mysql_)) {
				case 0:  return nr_more_results;
				case -1: return nr_last_result;
				default: return nr_error;
			}
		#else
			return nr_not_supported;
		#endif
	}

	/// \brief Returns the number of fields in the given result set
	///
	/// Wraps \c mysql_num_fields() in MySQL C API.
	int num_fields(MYSQL_RES* res) const
	{
		error_message_.clear();
		return mysql_num_fields(res);
	}

	/// \brief Returns the number of rows in the given result set
	///
	/// Wraps \c mysql_num_rows() in MySQL C API.
	ulonglong num_rows(MYSQL_RES* res) const
	{
		error_message_.clear();
		return mysql_num_rows(res);
	}

	/// \brief "Pings" the MySQL database
	///
	/// This function will try to reconnect to the server if the 
	/// connection has been dropped.  Wraps \c mysql_ping() in the MySQL C API.
	/// 
	/// \retval true if server is responding, regardless of whether we had
	/// to reconnect or not
	/// \retval false if either we already know the connection is down
	/// and cannot re-establish it, or if the server did not respond to
	/// the ping and we could not re-establish the connection.
	bool ping()
	{
		error_message_.clear();
		return !mysql_ping(&mysql_);
	}

	/// \brief Returns version number of MySQL protocol this connection
	/// is using
	///
	/// Wraps \c mysql_get_proto_info() in the MySQL C API.
	int protocol_version()
	{
		error_message_.clear();
		return mysql_get_proto_info(&mysql_);
	}

	/// \brief Returns information about the last executed query
	///
	/// Wraps \c mysql_info() in the MySQL C API
	std::string query_info();

	/// \brief Asks the database server to refresh certain internal data
	/// structures.
	///
	/// Wraps \c mysql_refresh() in the MySQL C API.  There is no
	/// corresponding interface for this in higher level MySQL++ classes
	/// because it was undocumented until recently, and it's a pretty
	/// low-level thing.  It's designed for things like MySQL
	/// Administrator.
	bool refresh(unsigned options)
	{
		error_message_.clear();
		return !mysql_refresh(&mysql_, options);
	}

	/// \brief Returns true if the most recent result set was empty
	///
	/// Wraps \c mysql_field_count() in the MySQL C API, returning true
	/// if it returns 0.
	bool result_empty()
	{
		error_message_.clear();
		return mysql_field_count(&mysql_) == 0;
	}

	/// \brief Asks the database server to switch to a different database
	bool select_db(const char* db)
	{
		error_message_.clear();
		return !mysql_select_db(&mysql_, db);
	}

	/// \brief Get the database server's version number
	///
	/// Wraps \c mysql_get_server_info() in the MySQL C API.
	std::string server_version()
	{
		error_message_.clear();
		return mysql_get_server_info(&mysql_);
	}

	/// \brief Sets a connection option
	///
	/// This is the database-independent high-level option setting
	/// interface that Connection::set_option() calls.  There are
	/// several private overloads that actually implement the option
	/// setting.
	///
	/// \see Connection::set_option(Option*) for commentary
	bool set_option(Option* o);

	/// \brief Set MySQL C API connection option
	///
	/// \internal Wraps \c mysql_options() in C API.
	bool set_option(mysql_option moption, const void* arg = 0)
	{
		error_message_.clear();
		return !mysql_options(&mysql_, moption,
				static_cast<const char*>(arg));
	}

	#if MYSQL_VERSION_ID >= 40101
	/// \brief Set MySQL C API connection option
	///
	/// \internal Wraps \c mysql_set_server_option() in C API.
	bool set_option(enum_mysql_set_option msoption)
	{
		error_message_.clear();
		return !mysql_set_server_option(&mysql_, msoption);
	}
	#endif

	/// \brief Set MySQL C API connection option
	///
	/// Manipulates the MYSQL.client_flag bit mask.  This allows these
	/// flags to be treated the same way as any other connection option,
	/// even though the C API handles them differently.
	bool set_option(unsigned int option, bool arg);

	/// \brief Same as set_option(), except that it won't override
	/// a previously-set option.
	bool set_option_default(Option* o)
	{
		const std::type_info& ti = typeid(o);
		for (OptionList::const_iterator it = applied_options_.begin();
				it != applied_options_.end();
				++it) {
			if (typeid(*it) == ti) {
				delete o;
				return "";		// option of this type already set
			}
		}

		return set_option(o);
	}

	/// \brief Ask database server to shut down.
	///
	/// User must have the "shutdown" privilege.
	///
	/// Wraps \c mysql_shutdown() in the MySQL C API.
	bool shutdown();

	/// \brief Returns the database server's status
	///
	/// String is similar to that returned by the \c mysqladmin
	/// \c status command.  Among other things, it contains uptime 
	/// in seconds, and the number of running threads, questions
	/// and open tables.
	///
	/// Wraps \c mysql_stat() in the MySQL C API.
	std::string server_status()
	{
		error_message_.clear();
		return mysql_stat(&mysql_);
	}

	/// \brief Saves the results of the query just execute()d in memory
	/// and returns a pointer to the MySQL C API data structure the
	/// results are stored in.
	///
	/// \sa use_result()
	///
	/// Wraps \c mysql_store_result() in the MySQL C API.
	MYSQL_RES* store_result()
	{
		error_message_.clear();
		return mysql_store_result(&mysql_);
	}

	/// \brief Returns true if MySQL++ and the underlying MySQL C API
	/// library were both compiled with thread awareness.
	///
	/// This is based in part on a MySQL C API function
	/// mysql_thread_safe().  We deliberately don't call this wrapper
	/// thread_safe() because it's a misleading name: linking to
	/// thread-aware versions of the MySQL++ and C API libraries doesn't
	/// automatically make your program "thread-safe".  See the
	/// <a href="../userman/threads.html">chapter on threads</a> in the
	/// user manual for more information and guidance.
	static bool thread_aware();

	/// \brief Tells the underlying MySQL C API library that this thread
	/// is done using the library.
	///
	/// This exists because the MySQL C API library allocates some per-thread
	/// memory which it doesn't release until you call this.
	static void thread_end()
	{
		#if MYSQL_VERSION_ID > 40000		// only in MySQL v4.0 +
			mysql_thread_end();
		#endif
	}

	/// \brief Returns the MySQL server thread ID for this connection
	///
	/// This has nothing to do with threading on the client side. It's
	/// a server-side thread ID, to be used with kill().
	unsigned long thread_id()
	{
		error_message_.clear();
		return mysql_thread_id(&mysql_);
	}

	/// \brief Tells the underlying C API library that the current
	/// thread will be using the library's services.
	///
	/// \retval True if there was no problem
	///
	/// The MySQL++ user manual's <a href="../userman/threads.html">chapter
	/// on threads</a> details two major strategies for dealing with
	/// connections in the face of threads.  If you take the simpler
	/// path, creating one DBDriver object per thread, it is never
	/// necessary to call this function; the underlying C API will call it
	/// for you when you establish the first database server connection
	/// from that thread.  If you use a more complex connection
	/// management strategy where it's possible for one thread to
	/// establish a connection that another thread uses, you must call
	/// this from each thread that can use the database before it creates
	/// any MySQL++ objects.  If you use a DBDriverPool object, this
	/// applies; DBDriverPool isn't smart enough to call this for you,
	/// and the MySQL C API won't do it, either.
	static bool thread_start()
	{
		#if MYSQL_VERSION_ID > 40000		// only in MySQL v4.0 +
			return !mysql_thread_init();
		#else
			return false;
		#endif
	}

	/// \brief Returns a result set from the last-executed query which
	/// we can walk through in linear fashion, which doesn't store all
	/// result sets in memory.
	///
	/// \sa store_result
	///
	/// Wraps \c mysql_use_result() in the MySQL C API.
	MYSQL_RES* use_result()
	{
		error_message_.clear();
		return mysql_use_result(&mysql_);
	}

protected:
	/// \brief Does things common to both connect() overloads, before
	/// each go and establish the connection in their different ways.
	bool connect_prepare();

	/// \brief Common implementation of set_option(Option*) and the
	/// delayed option setting code in connect_prepare()
	bool set_option_impl(Option* o);

private:
	/// \brief Data type of the list of applied connection options
	typedef std::deque<Option*> OptionList;

	/// \brief Iterator into an OptionList
	typedef OptionList::iterator OptionListIt;

	/// \brief Hidden assignment operator; we don't want to be copied
	/// that way.  What would it mean?
	DBDriver& operator=(const DBDriver&);

	MYSQL mysql_;
	bool is_connected_;
	OptionList applied_options_;
	OptionList pending_options_;
	mutable std::string error_message_;
};


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_DBDRIVER_H)

