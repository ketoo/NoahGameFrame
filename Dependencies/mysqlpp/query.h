/// \file query.h
/// \brief Defines a class for building and executing SQL queries.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2011 by Educational Technology Resources, Inc.  Others may
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

#if !defined(MYSQLPP_QUERY_H)
#define MYSQLPP_QUERY_H

#include "common.h"

#include "exceptions.h"
#include "noexceptions.h"
#include "qparms.h"
#include "querydef.h"
#include "result.h"
#include "row.h"
#include "sqlstream.h"
#include "stadapter.h"
#include "transaction.h"

#include <deque>
#include <iomanip>
#include <list>
#include <map>
#include <set>
#include <vector>

#ifdef HAVE_EXT_SLIST
#  include <ext/slist>
#else
#  if defined(HAVE_STD_SLIST) || defined(HAVE_GLOBAL_SLIST)
#      include <slist>
#  endif
#endif

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT Connection;
class MYSQLPP_EXPORT Transaction;
#endif

/// \brief A class for building and executing SQL queries.
///
/// One does not generally create Query objects directly. Instead, call
/// mysqlpp::Connection::query() to get one tied to that connection.
///
/// There are several ways to build and execute SQL queries with this
/// class.
///
/// The way most like other database libraries is to pass a SQL
/// statement in either the form of a C or C++ string to one of the
/// \link mysqlpp::Query::execute() exec*(), \endlink
/// \link mysqlpp::Query::store() store*(), \endlink or use() methods.
/// The query is executed immediately, and any results returned.
///
/// For more complicated queries, it's often more convenient to build up
/// the query string over several C++ statements using Query's stream
/// interface. It works like any other C++ stream (\c std::cout,
/// \c std::ostringstream, etc.) in that you can just insert things
/// into the stream, building the query up piece by piece. When the
/// query string is complete, you  call the overloaded version of
/// \link mysqlpp::Query::execute() exec*(), \endlink
/// \link mysqlpp::Query::store() store*(), \endlink or
/// \link mysqlpp::Query::use() use() \endlink takes no parameters,
/// which executes the built query and returns any results.
///
/// If you are using the library's Specialized SQL Structures feature,
/// Query has several special functions for generating common SQL
/// queries from those structures. For instance, it offers the
/// \link mysqlpp::Query::insert() insert() \endlink method, which
/// builds an INSERT query to add the contents of the SSQLS to the
/// database. As with the stream interface, these methods only build
/// the query string; call one of the parameterless methods mentioned
/// previously to actually execute the query.
///
/// Finally, you can build "template queries". This is something like
/// C's \c printf() function, in that you insert a specially-formatted
/// query string into the object which contains placeholders for data.
/// You call the parse() method to tell the Query object that the query
/// string contains placeholders. Having done that, you call one of the
/// the many
/// \link mysqlpp::Query::execute(const SQLTypeAdapter&) exec*(), \endlink
/// \link mysqlpp::Query::store(const SQLTypeAdapter&) store*(), \endlink
/// or \link mysqlpp::Query::use(const SQLTypeAdapter&) use() \endlink
/// overloads that take SQLTypeAdapter objects.  There are 25 of each by
/// default, differing only in the number of STA objects they take.
/// (See \c lib/querydef.pl if you need to change the limit, or 
/// \c examples/tquery2.cpp for a way around it that doesn't require 
/// changing the library.)  Only the version taking a single STA object
/// is documented below, as to document all of them would just be
/// repetitive.  For each Query method that takes a single STA object,
/// there's a good chance there's a set of undocumented overloads that
/// take more of them for the purpose of filling out a template query.
///
/// See the user manual for more details about these options.

class MYSQLPP_EXPORT Query :
		public std::ostream,
		public OptionalExceptions
{
public:
#if !defined(DOXYGEN_IGNORE)
	// Bring in InsertPolicy template as part of this class's interface,
	// separate only in the sense that it's a self-contained concept.
	#include "insertpolicy.h"
#endif

	/// \brief Create a new query object attached to a connection.
	///
	/// This is the constructor used by mysqlpp::Connection::query().
	///
	/// \param c connection the finished query should be sent out on
	/// \param te if true, throw exceptions on errors
	/// \param qstr an optional initial query string
	Query(Connection* c, bool te = true, const char* qstr = 0);

	/// \brief Create a new query object as a copy of another.
	///
	/// This is \b not a traditional copy ctor!  Its only purpose is to
	/// make it possible to assign the return of Connection::query()
	/// to an empty Query object.  In particular, the stream buffer and
	/// template query stuff will be empty in the copy, regardless of
	/// what values they have in the original.
	Query(const Query& q);

	/// \brief Return the number of rows affected by the last query
	ulonglong affected_rows();

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

	/// \brief Get the last error number that was set.
	///
	/// This just delegates to Connection::errnum().  Query has nothing
	/// extra to say, so use either, as makes sense in your program.
	int errnum() const;

	/// \brief Get the last error message that was set.
	///
	/// This just delegates to Connection::error().  Query has nothing
	/// extra to say, so use either, as makes sense in your program.
	const char* error() const;

	/// \brief Returns information about the most recently executed
	/// query.
	std::string info();

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
	ulonglong insert_id();

	/// \brief Assign another query's state to this object
	///
	/// The same caveats apply to this operator as apply to the copy
	/// ctor.
	Query& operator=(const Query& rhs);

	/// \brief Test whether the object has experienced an error condition
	///
	/// Allows for code constructs like this:
	///
	/// \code
	///	Query q = conn.query();
	///	.... use query object
	///	if (q) {
	///	    ... no problems in using query object
	///	}
	///	else {
	///	    ... an error has occurred
	///	}
	/// \endcode
	///
	/// This method returns false if either the Query object or its
	/// associated Connection object has seen an error condition since
	/// the last operation.
	operator void*() const;

	/// \brief Returns true if the query object is not in a bad state
	///
	/// This just returns the opposite of operator void*(), and is
	/// required only because basic_ios defines it, so we have to
	/// override it to get Query-specific behavior in code like this:
	///
	/// \code if (!query) ... \endcode
	bool operator !() const { return !operator void*(); }

	/// \brief Treat the contents of the query string as a template
	/// query.
	///
	/// This method sets up the internal structures used by all of the
	/// other members that accept template query parameters.  See the
	/// "Template Queries" chapter in the user manual for more
	/// information.
	void parse();

	/// \brief Reset the query object so that it can be reused.
	///
	/// As of v3.0, Query objects auto-reset upon query execution unless
	/// you've set it up for making template queries.  (It can't auto-reset
	/// in that situation, because it would forget the template info.)
	/// Therefore, the only time you must call this is if you have a Query
	/// object set up for making template queries, then want to build
	/// queries using one of the other methods.  (Static strings, SSQLS,
	/// or the stream interface.)
	void reset();

	/// \brief Returns true if the most recent result set was empty
	///
	/// Wraps DBDriver::result_empty()
	bool result_empty();

	/// \brief Get built query as a C++ string
	std::string str() { return str(template_defaults); }

	/// \brief Get built query as a C++ string with template query
	/// parameter substitution.
	///
	/// \param arg0 the value to substitute for the first template query
	/// parameter; because SQLTypeAdapter implicitly converts from many
	/// different data types, this method is very flexible in what it
	/// accepts as a parameter.  You shouldn't have to use the
	/// SQLTypeAdapter data type directly in your code.
	///
	/// There many more overloads of this type (25 total, by default;
	/// see \c lib/querydef.pl), each taking one more SQLTypeAdapter object
	/// than the previous one.  See the template query overview above
	/// for more about this topic.
	std::string str(const SQLTypeAdapter& arg0)
			{ return str(SQLQueryParms() << arg0); }

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param p template query parameters to use, overriding the ones
	/// this object holds, if any
	std::string str(SQLQueryParms& p);

	/// \brief Execute a built-up query
	///
	/// Same as exec(), except that it uses the query string built up
	/// within the query object already instead of accepting a query
	/// string from the caller.
	///
	/// \return true if query was executed successfully
	///
	/// \sa exec(const std::string& str), execute(), store(),
	/// storein(), and use()
	bool exec() { return exec(str(template_defaults)); }

	/// \brief Execute a query
	///
	/// Same as execute(), except that it only returns a flag indicating
	/// whether the query succeeded or not.  It is basically a thin
	/// wrapper around the C API function \c mysql_real_query().
	///
	/// \param str the query to execute
	///
	/// \return true if query was executed successfully
	///
	/// \sa execute(), store(), storein(), and use()
	bool exec(const std::string& str);

	/// \brief Execute built-up query
	///
	/// Use one of the execute() overloads if you don't expect the
	/// server to return a result set. For instance, a DELETE query.
	/// The returned SimpleResult object contains status information from
	/// the server, such as whether the query succeeded, and if so how
	/// many rows were affected.
	///
	/// This overloaded version of execute() simply executes the query
	/// that you have built up in the object in some way. (For instance,
	/// via the insert() method, or by using the object's stream
	/// interface.)
	///
	/// \return SimpleResult status information about the query
	///
	/// \sa exec(), store(), storein(), and use()
	SimpleResult execute(); 

	/// \brief Execute template query using given parameters.
	///
	/// This method should only be used by code that doesn't know,
	/// at compile time, how many parameters it will have.  This is
	/// useful within the library, and also for code that builds
	/// template queries dynamically, at run time.
	///
	/// \param p parameters to use in the template query.
	SimpleResult execute(SQLQueryParms& p);

	/// \brief Execute a query that returns no rows
	///
	/// \param str if this object is set up as a template query, this is
	/// the value to substitute for the first template query parameter;
	/// else, it is the SQL query string to execute
	///
	/// Because SQLTypeAdapter can be initialized from either a C string
	/// or a C++ string, this overload accepts query strings in either
	/// form.  Beware, SQLTypeAdapter also accepts many other data types
	/// (this is its \e raison \e d'etre), so it will let you write code
	/// that compiles but results in bogus SQL queries.
	///
	/// To support template queries, there many more overloads of this
	/// type (25 total, by default; see \c lib/querydef.pl), each taking
	/// one more SQLTypeAdapter object than the previous one.  See the
	/// template query overview above for more about this topic.
	SimpleResult execute(const SQLTypeAdapter& str);

	/// \brief Execute query in a known-length string of characters.
	/// This can include null characters.
	///
	/// Executes the query immediately, and returns the results.
	SimpleResult execute(const char* str, size_t len);

	/// \brief Execute a query that can return rows, with access to
	/// the rows in sequence
	/// 
	/// Use one of the use() overloads if memory efficiency is
	/// important.  They return an object that can walk through
	/// the result records one by one, without fetching the entire
	/// result set from the server.  This is superior to store()
	/// when there are a large number of results; store() would have to
	/// allocate a large block of memory to hold all those records,
	/// which could cause problems.
	///
	/// A potential downside of this method is that MySQL database
	/// resources are tied up until the result set is completely
	/// consumed.  Do your best to walk through the result set as
	/// expeditiously as possible.
	///
	/// The name of this method comes from the MySQL C API function
	/// that initiates the retrieval process, \c mysql_use_result().
	/// This method is implemented in terms of that function.
	///
	/// This function has the same set of overloads as execute().
	///
	/// \return UseQueryResult object that can walk through result set serially
	///
	/// \sa exec(), execute(), store() and storein()
	UseQueryResult use();

	/// \brief Execute a template query that can return rows, with
	/// access to the rows in sequence
	///
	/// This method should only be used by code that doesn't know,
	/// at compile time, how many parameters it will have.  This is
	/// useful within the library, and also for code that builds
	/// template queries dynamically, at run time.
	///
	/// \param p parameters to use in the template query.
	UseQueryResult use(SQLQueryParms& p);

	/// \brief Execute a query that can return rows, with access to
	/// the rows in sequence
	///
	/// \param str if this object is set up as a template query, this is
	/// the value to substitute for the first template query parameter;
	/// else, it is the SQL query string to execute
	///
	/// Because SQLTypeAdapter can be initialized from either a C string
	/// or a C++ string, this overload accepts query strings in either
	/// form.  Beware, SQLTypeAdapter also accepts many other data types
	/// (this is its \e raison \e d'etre), so it will let you write code
	/// that compiles but results in bogus SQL queries.
	///
	/// To support template queries, there many more overloads of this
	/// type (25 total, by default; see \c lib/querydef.pl), each taking
	/// one more SQLTypeAdapter object than the previous one.  See the
	/// template query overview above for more about this topic.
	UseQueryResult use(const SQLTypeAdapter& str);

	/// \brief Execute a query that can return rows, with access to
	/// the rows in sequence
	///
	/// This overload is for situations where you have the query in a
	/// C string and have its length already.  If you want to execute
	/// a query in a null-terminated C string or have the query string
	/// in some other form, you probably want to call
	/// use(const SQLTypeAdapter&) instead.  SQLTypeAdapter converts
	/// from plain C strings and other useful data types implicitly.
	UseQueryResult use(const char* str, size_t len);

	/// \brief Execute a query that can return a result set
	///
	/// Use one of the store() overloads to execute a query and retrieve
	/// the entire result set into memory.  This is useful if you
	/// actually need all of the records at once, but if not, consider
	/// using one of the use() methods instead, which returns the results
	/// one at a time, so they don't allocate as much memory as store().
	///
	/// You must use store(), storein() or use() for \c SELECT, \c SHOW,
	/// \c DESCRIBE and \c EXPLAIN queries.  You can use these functions
	/// with other query types, but since they don't return a result
	/// set, exec() and execute() are more efficient.
	///
	/// The name of this method comes from the MySQL C API function it
	/// is implemented in terms of, \c mysql_store_result().
	///
	/// This function has the same set of overloads as execute().
	///
	/// \return StoreQueryResult object containing entire result set
	///
	/// \sa exec(), execute(), storein(), and use()
	StoreQueryResult store();

	/// \brief Store results from a template query using given parameters.
	///
	/// This method should only be used by code that doesn't know,
	/// at compile time, how many parameters it will have.  This is
	/// useful within the library, and also for code that builds
	/// template queries dynamically, at run time.
	///
	/// \param p parameters to use in the template query.
	StoreQueryResult store(SQLQueryParms& p);

	/// \brief Execute a query that can return rows, returning all
	/// of the rows in a random-access container
	///
	/// \param str if this object is set up as a template query, this is
	/// the value to substitute for the first template query parameter;
	/// else, it is the SQL query string to execute
	///
	/// Because SQLTypeAdapter can be initialized from either a C string
	/// or a C++ string, this overload accepts query strings in either
	/// form.  Beware, SQLTypeAdapter also accepts many other data types
	/// (this is its \e raison \e d'etre), so it will let you write code
	/// that compiles but results in bogus SQL queries.
	///
	/// To support template queries, there many more overloads of this
	/// type (25 total, by default; see \c lib/querydef.pl), each taking
	/// one more SQLTypeAdapter object than the previous one.  See the
	/// template query overview above for more about this topic.
	StoreQueryResult store(const SQLTypeAdapter& str);

	/// \brief Execute a query that can return rows, returning all
	/// of the rows in a random-access container
	///
	/// This overload is for situations where you have the query in a
	/// C string and have its length already.  If you want to execute
	/// a query in a null-terminated C string or have the query string
	/// in some other form, you probably want to call
	/// store(const SQLTypeAdapter&) instead.  SQLTypeAdapter converts
	/// from plain C strings and other useful data types implicitly.
	StoreQueryResult store(const char* str, size_t len);

	/// \brief Execute a query, and call a functor for each returned row
	///
	/// This method wraps a use() query, calling the given functor for
	/// every returned row.  It is analogous to STL's for_each()
	/// algorithm, but instead of iterating over some range within a
	/// container, it iterates over a result set produced by a query.
	///
	/// \param query the query string
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <typename Function>
	Function for_each(const SQLTypeAdapter& query, Function fn)
	{	
		mysqlpp::UseQueryResult res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				fn(row);
			}
		}

		return fn;
	}

	/// \brief Execute the query, and call a functor for each returned row
	///
	/// Just like for_each(const SQLTypeAdapter&, Function), but it uses
	/// the query string held by the Query object already
	///
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <typename Function>
	Function for_each(Function fn)
	{	
		mysqlpp::UseQueryResult res = use();
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				fn(row);
			}
		}

		return fn;
	}

	/// \brief Run a functor for every row in a table
	///
	/// Just like for_each(Function), except that it builds a
	/// "select * from TABLE" query using the SQL table name from
	/// the SSQLS instance you pass.
	///
	/// \param ssqls the SSQLS instance to get a table name from
	/// \param fn the functor called for each row
	///
	/// \return a copy of the passed functor
	template <class SSQLS, typename Function>
	Function for_each(const SSQLS& ssqls, Function fn)
	{	
		std::string query("select * from `");
		query += ssqls.table();
      query += '`';
		mysqlpp::UseQueryResult res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				fn(row);
			}
		}

		return fn;
	}

	/// \brief Execute a query, conditionally storing each row in a
	/// container
	///
	/// This method wraps a use() query, calling the given functor for
	/// every returned row, and storing the results in the given
	/// sequence container if the functor returns true.
	///
	/// This is analogous to the STL copy_if() algorithm, except that
	/// the source rows come from a database query instead of another
	/// container.  (copy_if() isn't a standard STL algorithm, but only
	/// due to an oversight by the standardization committee.)  This
	/// fact may help you to remember the order of the parameters: the
	/// container is the destination, the query is the source, and the
	/// functor is the predicate; it's just like an STL algorithm.
	///
	/// \param con the destination container; needs a push_back() method
	/// \param query the query string
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <class Sequence, typename Function>
	Function store_if(Sequence& con, const SQLTypeAdapter& query, Function fn)
	{	
		mysqlpp::UseQueryResult res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				if (fn(row)) {
					con.push_back(row);
				}
			}
		}

		return fn;
	}

	/// \brief Pulls every row in a table, conditionally storing each
	/// one in a container
	///
	/// Just like store_if(Sequence&, const SQLTypeAdapter&, Function), but
	/// it uses the SSQLS instance to construct a "select * from TABLE"
	/// query, using the table name field in the SSQLS.
	///
	/// \param con the destination container; needs a push_back() method
	/// \param ssqls the SSQLS instance to get a table name from
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <class Sequence, class SSQLS, typename Function>
	Function store_if(Sequence& con, const SSQLS& ssqls, Function fn)
	{	
		std::string query("select * from `");
		query += ssqls.table();
      query += '`';
		mysqlpp::UseQueryResult res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				if (fn(row)) {
					con.push_back(row);
				}
			}
		}

		return fn;
	}

	/// \brief Execute the query, conditionally storing each row in a
	/// container
	///
	/// Just like store_if(Sequence&, const SQLTypeAdapter&, Function), but
	/// it uses the query string held by the Query object already
	///
	/// \param con the destination container; needs a push_back() method
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <class Sequence, typename Function>
	Function store_if(Sequence& con, Function fn)
	{	
		mysqlpp::UseQueryResult res = use();
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				if (fn(row)) {
					con.push_back(row);
				}
			}
		}

		return fn;
	}

	/// \brief Return next result set, when processing a multi-query
	///
	/// There are two cases where you'd use this function instead of
	/// the regular store() functions.
	/// 
	/// First, when handling the result of executing multiple queries
	/// at once.  (See <a
	/// href="http://dev.mysql.com/doc/mysql/en/c-api-multiple-queries.html">this
	/// page</a> in the MySQL documentation for details.) 
	///
	/// Second, when calling a stored procedure, MySQL can return the
	/// result as a set of results.
	///
	/// In either case, you must consume all results before making
	/// another MySQL query, even if you don't care about the remaining
	/// results or result sets.
	///
	/// As the MySQL documentation points out, you must set the
	/// MYSQL_OPTION_MULTI_STATEMENTS_ON flag on the connection in order
	/// to use this feature.  See Connection::set_option().
	///
	/// Multi-queries only exist in MySQL v4.1 and higher.  Therefore,
	/// this function just wraps store() when built against older API
	/// libraries.
	///
	/// \return StoreQueryResult object containing the next result set.
	StoreQueryResult store_next();

	/// \brief Return whether more results are waiting for a multi-query
	/// or stored procedure response.
	///
	/// If this function returns true, you must call store_next() to
	/// fetch the next result set before you can execute more queries.
	///
	/// Wraps mysql_more_results() in the MySQL C API.  That function
	/// only exists in MySQL v4.1 and higher.  Therefore, this function
	/// always returns false when built against older API libraries.
	///
	/// \return true if another result set exists
	bool more_results();

	/// \brief Execute a query, storing the result set in an STL
	/// sequence container.
	///
	/// This function works much like store() from the caller's
	/// perspective, because it returns the entire result set at once.
	/// It's actually implemented in terms of use(), however, so that
	/// memory for the result set doesn't need to be allocated twice.
	///
	/// There are many overloads for this function, pretty much the same
	/// as for execute(), except that there is a Container parameter at
	/// the front of the list.  So, you can pass a container and a query
	/// string, or a container and template query parameters.
	///
	/// \param con any STL sequence container, such as \c std::vector
	///
	/// \sa exec(), execute(), store(), and use()
	template <class Sequence>
	void storein_sequence(Sequence& con)
	{
		storein_sequence(con, str(template_defaults));
	}

	/// \brief Executes a query, storing the result rows in an STL
	/// sequence container.
	///
	/// \param con the container to store the results in
	///
	/// \param s if Query is set up as a template query, this is the value
	/// to substitute for the first template query parameter; else, the
	/// SQL query string
	///
	/// There many more overloads of this type (25 total, by default;
	/// see \c lib/querydef.pl), each taking one more SQLTypeAdapter object
	/// than the previous one.  See the template query overview above
	/// for more about this topic.
	template <class Sequence>
	void storein_sequence(Sequence& con, const SQLTypeAdapter& s)
	{
		if (UseQueryResult result = use(s)) {
			while (1) {
				MYSQL_ROW d = result.fetch_raw_row();
				if (!d) break;
				Row row(d, &result, result.fetch_lengths(),
						throw_exceptions());
				if (!row) break;
				con.push_back(typename Sequence::value_type(row));
			}
		}
		else if (!result_empty()) {
			// Underlying MySQL C API returned an empty result for this
			// query, but it also says it should have returned
			// something.  Reasons it can do that are given here:
			// http://dev.mysql.com/doc/refman/5.5/en/null-mysql-store-result.html
			// Regardless, it means the C library barfed, so we can't
			// just return an empty result set.
			copacetic_ = false;
			if (throw_exceptions()) {
				throw UseQueryError("Bogus empty result");
			}
		}
		// else, it was *supposed* to return nothing, because query was
		// an INSERT, CREATE, etc. sort.  So, leave con untouched.
	}

	/// \brief Execute template query using given parameters, storing
	/// the results in a sequence type container.
	///
	/// This method should only be used by code that doesn't know,
	/// at compile time, how many parameters it will have.  This is
	/// useful within the library, and also for code that builds
	/// template queries dynamically, at run time.
	///
	/// \param con container that will receive the results
	/// \param p parameters to use in the template query.
	template <class Seq>
	void storein_sequence(Seq& con, SQLQueryParms& p)
	{
		storein_sequence(con, str(p));
	}

	/// \brief Execute a query, storing the result set in an STL
	/// associative container.
	///
	/// The same thing as storein_sequence(), except that it's used with
	/// associative STL containers, such as \c std::set.  Other than
	/// that detail, that method's comments apply equally well to this
	/// one.
	template <class Set>
	void storein_set(Set& con)
	{
		storein_set(con, str(template_defaults));
	}

	/// \brief Executes a query, storing the result rows in an STL
	/// set-associative container.
	///
	/// \param con the container to store the results in
	///
	/// \param s if Query is set up as a template query, this is the value
	/// to substitute for the first template query parameter; else, the
	/// SQL query string
	///
	/// There many more overloads of this type (25 total, by default;
	/// see \c lib/querydef.pl), each taking one more SQLTypeAdapter object
	/// than the previous one.  See the template query overview above
	/// for more about this topic.
	template <class Set>
	void storein_set(Set& con, const SQLTypeAdapter& s)
	{
		if (UseQueryResult result = use(s)) {
			while (1) {
				MYSQL_ROW d = result.fetch_raw_row();
				if (!d) break;
				Row row(d, &result, result.fetch_lengths(),
						throw_exceptions());
				if (!row) break;
				con.insert(typename Set::value_type(row));
			}
		}
		else if (!result_empty()) {
			// Underlying MySQL C API returned an empty result for this
			// query, but it also says it should have returned
			// something.  Reasons it can do that are given here:
			// http://dev.mysql.com/doc/refman/5.5/en/null-mysql-store-result.html
			// Regardless, it means the C library barfed, so we can't
			// just return an empty result set.
			copacetic_ = false;
			if (throw_exceptions()) {
				throw UseQueryError("Bogus empty result");
			}
		}
		// else, it was *supposed* to return nothing, because query was
		// an INSERT, CREATE, etc. sort.  So, leave con untouched.
	}

	/// \brief Execute template query using given parameters, storing
	/// the results in a set type container.
	///
	/// This method should only be used by code that doesn't know,
	/// at compile time, how many parameters it will have.  This is
	/// useful within the library, and also for code that builds
	/// template queries dynamically, at run time.
	///
	/// \param con container that will receive the results
	/// \param p parameters to use in the template query.
	template <class Set>
	void storein_set(Set& con, SQLQueryParms& p)
	{
		storein_set(con, str(p));
	}

	/// \brief Execute a query, and store the entire result set
	/// in an STL container.
	///
	/// This is a set of specialized template functions that call either
	/// storein_sequence() or storein_set(), depending on the type of
	/// container you pass it. It understands \c std::vector, \c deque,
	/// \c list, \c slist (a common C++ library extension), \c set,
	/// and \c multiset.
	///
	/// Like the functions it wraps, this is actually an overloaded set
	/// of functions. See the other functions' documentation for details.
	///
	/// Use this function if you think you might someday switch your
	/// program from using a set-associative container to a sequence
	/// container for storing result sets, or vice versa.
	///
	/// See exec(), execute(), store(), and use() for alternative
	/// query execution mechanisms.
	template <class Container>
	void storein(Container& con)
	{
		storein(con, str(template_defaults));
	}

	/// \brief Store template query results into a container
	///
	/// This method is not intended to be used directly.  It is part
	/// of the call chain in processing calls to one of the many
	/// storein() overloads that take a container and one or more
	/// SQLTypeAdapter parameters.
	template <class T>
	void storein(T& con, SQLQueryParms& p)
	{
		storein(con, str(p));
	}

	/// \brief Specialization of storein_sequence() for \c std::vector
	template <class T>
	void storein(std::vector<T>& con, const SQLTypeAdapter& s)
	{
		storein_sequence(con, s);
	}

	/// \brief Specialization of storein_sequence() for \c std::deque
	template <class T>
	void storein(std::deque<T>& con, const SQLTypeAdapter& s)
	{
		storein_sequence(con, s);
	}

	/// \brief Specialization of storein_sequence() for \c std::list
	template <class T>
	void storein(std::list<T>& con, const SQLTypeAdapter& s)
	{
		storein_sequence(con, s);
	}

#if defined(HAVE_EXT_SLIST)
	/// \brief Specialization of storein_sequence() for g++ STL
	/// extension \c slist
	template <class T>
	void storein(__gnu_cxx::slist<T>& con, const SQLTypeAdapter& s)
	{
		storein_sequence(con, s);
	}
#elif defined(HAVE_GLOBAL_SLIST)
	/// \brief Specialization of storein_sequence() for STL
	/// extension \c slist
	///
	/// This is primarily for older versions of g++, which put \c slist
	/// in the global namespace.  This is a common language extension,
	/// so this may also work for other compilers.
	template <class T>
	void storein(slist<T>& con, const SQLTypeAdapter& s)
	{
		storein_sequence(con, s);
	}
#elif defined(HAVE_STD_SLIST)
	/// \brief Specialization of storein_sequence() for STL
	/// extension \c slist
	///
	/// This is for those benighted compilers that include an \c slist
	/// implementation, but erroneously put it in the \c std namespace!
	template <class T>
	void storein(std::slist<T>& con, const SQLTypeAdapter& s)
	{
		storein_sequence(con, s);
	}
#endif

	/// \brief Specialization of storein_set() for \c std::set
	template <class T>
	void storein(std::set<T>& con, const SQLTypeAdapter& s)
	{
		storein_set(con, s);
	}

	/// \brief Specialization of storein_set() for \c std::multiset
	template <class T>
	void storein(std::multiset<T>& con, const SQLTypeAdapter& s)
	{
		storein_set(con, s);
	}

	/// \brief Replace an existing row's data with new data.
	///
	/// This function builds an UPDATE SQL query using the new row data
	/// for the SET clause, and the old row data for the WHERE clause.
	/// One uses it with MySQL++'s Specialized SQL Structures mechanism.
	///
	/// \param o old row
	/// \param n new row
	///
	/// \sa insert(), replace()
	template <class T>
	Query& update(const T& o, const T& n)
	{
		reset();

		// Cast required for VC++ 2003 due to error in overloaded operator
		// lookup logic.  For an explanation of the problem, see:
		// http://groups-beta.google.com/group/microsoft.public.vc.stl/browse_thread/thread/9a68d84644e64f15
		MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
				"UPDATE `" << o.table() << "` SET " << n.equal_list() <<
				" WHERE " << o.equal_list(" AND ", sql_use_compare);
		return *this;
	}

	/// \brief Insert a new row.
	///
	/// This function builds an INSERT SQL query.  One uses it with
	/// MySQL++'s Specialized SQL Structures mechanism.
	///
	/// \param v new row
	///
	/// \sa replace(), update()
	template <class T>
	Query& insert(const T& v)
	{
		reset();

		MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
				"INSERT INTO `" << v.table() << "` (" <<
				v.field_list() << ") VALUES (" <<
				v.value_list() << ')';
		return *this;
	}

	/// \brief Insert multiple new rows.
	///
	/// Builds an INSERT SQL query using items from a range within an
	/// STL container.  Insert the entire contents of the container by
	/// using the begin() and end() iterators of the container as
	/// parameters to this function.
	///
	/// \param first iterator pointing to first element in range to
	///    insert
	/// \param last iterator pointing to one past the last element to
	///    insert
	///
	/// \sa insertfrom(), replace(), update()
	template <class Iter>
	Query& insert(Iter first, Iter last)
	{
		reset();

		if (first != last) {
			// Build SQL for first item in the container.  It's special
			// because we need the table name and field list.
			MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
					"INSERT INTO `" << first->table() << "` (" <<
					first->field_list() << ") VALUES (" <<
					first->value_list() << ')';

			// Now insert any remaining container elements.  Be careful
			// hacking on the iterator use here: we want it to work
			// with containers providing only a forward iterator.
			Iter it = first;
			while (++it != last) {
				MYSQLPP_QUERY_THISPTR << ",(" << it->value_list() << ')';
			}
		}

		return *this;
	}

	/// \brief Insert multiple new rows using an insert policy to
	/// control how the INSERT statements are created using
	/// items from an STL container.
	///
	/// \param first iterator pointing to first element in range to
	///    insert
	/// \param last iterator pointing to one past the last element to
	///    insert
	/// \param policy insert policy object, see insertpolicy.h for
	/// details
	///
	/// \sa insert()
	template <class Iter, class InsertPolicy>
	Query& insertfrom(Iter first, Iter last, InsertPolicy& policy)
	{
		bool success = true;
		bool empty = true;

		reset();

		if (first == last) {
			return *this;   // empty set!
		}

		typename InsertPolicy::access_controller ac(*conn_);
		
		for (Iter it = first; it != last; ++it) {
			if (policy.can_add(int(tellp()), *it)) {
				if (empty) {
					MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
						"INSERT INTO `" << it->table() << "` (" <<
						it->field_list() << ") VALUES (";
				} 
				else {
					MYSQLPP_QUERY_THISPTR << ",(";
				}

				MYSQLPP_QUERY_THISPTR << it->value_list() << ')';

				empty = false;
			} 
			else {
				// Execute what we've built up already, if there is anything
				if (!empty) {
					if (!exec()) {
						success = false;
						break;
					}

					empty = true;
				}

				// If we _still_ can't add, the policy is too strict
				if (policy.can_add(int(tellp()), *it)) {
					MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
						"INSERT INTO `" << it->table() << "` (" <<
						it->field_list() << ") VALUES (" <<
						it->value_list() << ')';

					empty = false;
				} 
				else {
					// At this point all we can do is give up
					if (throw_exceptions()) {
						throw BadInsertPolicy("Insert policy is too strict");
					}

					success = false;
					break;
				}
			}
		}

		// We might need to execute the last query here.
		if (success && !empty && !exec()) {
			success = false;
		}

		if (success) {
			ac.commit();
		} 
		else {
			ac.rollback();
		}

		return *this;
	}

	/// \brief Replace multiple new rows using an insert policy to
	/// control how the REPLACE statements are created using
	/// items from an STL container.
	///
	/// \param first iterator pointing to first element in range to
	///    replace
	/// \param last iterator pointing to one past the last element to
	///    replace
	/// \param policy insert policy object, see insertpolicy.h for
	/// details
	///
	/// \sa insert()
	template <class Iter, class InsertPolicy>
	Query& replacefrom(Iter first, Iter last, InsertPolicy& policy)
	{
		bool success = true;
		bool empty = true;

		reset();

		if (first == last) {
			return *this;   // empty set!
		}

		typename InsertPolicy::access_controller ac(*conn_);

		for (Iter it = first; it != last; ++it) {
			if (policy.can_add(int(tellp()), *it)) {
				if (empty) {
					MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
						"REPLACE INTO `" << it->table() << "` (" <<
						it->field_list() << ") VALUES (";
				}
				else {
					MYSQLPP_QUERY_THISPTR << ",(";
				}

				MYSQLPP_QUERY_THISPTR << it->value_list() << ')';

				empty = false;
			}
			else {
				// Execute what we've built up already, if there is anything
				if (!empty) {
					if (!exec()) {
						success = false;
						break;
					}

					empty = true;
				}

				// If we _still_ can't add, the policy is too strict
				if (policy.can_add(int(tellp()), *it)) {
					MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
						"REPLACE INTO `" << it->table() << "` (" <<
						it->field_list() << ") VALUES (" <<
						it->value_list() << ')';

					empty = false;
				}
				else {
					// At this point all we can do is give up
					if (throw_exceptions()) {
						throw BadInsertPolicy("Insert policy is too strict");
					}

					success = false;
					break;
				}
			}
		}

		// We might need to execute the last query here.
		if (success && !empty && !exec()) {
			success = false;
		}

		if (success) {
			ac.commit();
		}
		else {
			ac.rollback();
		}

		return *this;
	}

	/// \brief Insert new row unless there is an existing row that
	/// matches on a unique index, in which case we replace it.
	///
	/// This function builds a REPLACE SQL query.  One uses it with
	/// MySQL++'s Specialized SQL Structures mechanism.
	///
	/// \param v new row
	///
	/// \sa insert(), update()
	template <class T>
	Query& replace(const T& v)
	{
		reset();

		MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
				"REPLACE INTO `" << v.table() << "` (" <<
				v.field_list() << ") VALUES (" << v.value_list() << ')';
		return *this;
	}

	/// \brief Insert multiple new rows, or replace existing ones if
	/// there are existing rows that match on key fields.
	///
	/// Builds a REPLACE SQL query using items from a range within an
	/// STL container.  Insert the entire contents of the container by
	/// using the begin() and end() iterators of the container as
	/// parameters to this function.
	///
	/// \param first iterator pointing to first element in range to
	///    insert/replace
	/// \param last iterator pointing to one past the last element to
	///    insert/replace
	///
	/// \sa insertfrom(), replace(), update()
	template <class Iter>
	Query& replace(Iter first, Iter last)
	{
		reset();
		if (first != last) {
			// Build SQL for first item in the container.  It's special
			// because we need the table name and field list.
			MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
					"REPLACE INTO " << first->table() << " (" <<
					first->field_list() << ") VALUES (" <<
					first->value_list() << ')';

			// Now insert any remaining container elements.  Be careful
			// hacking on the iterator use here: we want it to work
			// with containers providing only a forward iterator.
			Iter it = first;
			while (++it != last) {
				MYSQLPP_QUERY_THISPTR << ",(" << it->value_list() << ')';
			}
		}

		return *this;
	}	

#if !defined(DOXYGEN_IGNORE)
	// Declare the remaining overloads.  These are hidden down here partly
	// to keep the above code clear, but also so that we may hide them
	// from Doxygen, which gets confused by macro instantiations that look
	// like method declarations.
	mysql_query_define0(std::string, str)
	mysql_query_define0(SimpleResult, execute)
	mysql_query_define0(StoreQueryResult, store)
	mysql_query_define0(UseQueryResult, use)
	mysql_query_define1(storein_sequence)
	mysql_query_define1(storein_set)
	mysql_query_define1(storein)
#endif // !defined(DOXYGEN_IGNORE)

	/// \brief The default template parameters
	///
	/// Used for filling in parameterized queries.
	SQLQueryParms template_defaults;

private:
	friend class SQLQueryParms;

	/// \brief Connection to send queries through
	Connection* conn_;

	/// \brief If true, last query succeeded
	bool copacetic_;

	/// \brief List of template query parameters
	std::vector<SQLParseElement> parse_elems_;

	/// \brief Maps template parameter position values to the
	/// corresponding parameter name.
	std::vector<std::string> parsed_names_;

	/// \brief Maps template parameter names to their position value.
	std::map<std::string, short int> parsed_nums_;

	/// \brief String buffer for storing assembled query
	std::stringbuf sbuffer_;

	/// \brief Process a parameterized query list.
	void proc(SQLQueryParms& p);

	SQLTypeAdapter* pprepare(char option, SQLTypeAdapter& S, bool replace = true);
};


/// \brief Insert raw query string into the given stream.
///
/// This is just syntactic sugar for Query::str(void)
inline std::ostream& operator <<(std::ostream& os, Query& q)
{
	return os << q.str();
}


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_QUERY_H)

