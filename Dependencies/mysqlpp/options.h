/// \file options.h
/// \brief Declares the Option class hierarchy, used to implement
/// connection options in Connection and DBDriver classes.
///
/// This is tied closely enough to DBDriver that there's a pure-OO
/// argument that it should be declared as protected or private members
/// within DBDriver.  We do it outside DBDriver because there's so much
/// of it.  It'd overwhelm everything else that's going on in that class
/// totally out of proprortion to the importance of options.

/***********************************************************************
 Copyright (c) 2007-2009 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS file in the top directory of the distribution for details.

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

#if !defined(MYSQLPP_OPTIONS_H)
#define MYSQLPP_OPTIONS_H

#include "common.h"

#include <deque>
#include <string>


namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
class DBDriver;
#endif


////////////////////////////////////////////////////////////////////////
// Classes

/// \brief Define abstract interface for all *Option subclasses.
///
/// This is the base class for the mid-level interface classes that take
/// arguments, plus the direct base for options that take no arguments.
class MYSQLPP_EXPORT Option
{
public:
	/// \brief Types of option setting errors we can diagnose
	enum Error {
		err_NONE,		///< option was set successfully
		err_api_limit,	///< option not supported by underlying C API
		err_api_reject,	///< underlying C API returned error when setting option
		err_connected,	///< can't set the given option while connected
		err_disconnected///< can only set the given option while connected
	};
	
	virtual ~Option() { }					///< Destroy object
	virtual Error set(DBDriver* dbd) = 0;	///< Apply option
};


/// \brief Define abstract interface for all *Options that take a
/// lone scalar as an argument.
template <typename T>
class MYSQLPP_EXPORT DataOption : public Option
{
public:
	typedef T ArgType;						///< Alias for template param

protected:
	DataOption(const T& arg) : arg_(arg) { }///< Construct object
	T arg_;									///< The argument value
};

typedef DataOption<unsigned> IntegerOption;		///< Option w/ int argument
typedef DataOption<bool> BooleanOption;			///< Option w/ bool argument
typedef DataOption<std::string> StringOption;	///< Option w/ string argument


/// \brief Enable data compression on the connection
class MYSQLPP_EXPORT CompressOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	CompressOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Change Connection::connect() default timeout
class MYSQLPP_EXPORT ConnectTimeoutOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ConnectTimeoutOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Make Query::affected_rows() return number of matched rows
///
/// Default is to return number of \b changed rows.
class MYSQLPP_EXPORT FoundRowsOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	FoundRowsOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Allow C API to guess what kind of connection to use
///
/// This is the default.  The option exists to override
/// UseEmbeddedConnectionOption and UseEmbeddedConnectionOption.
class MYSQLPP_EXPORT GuessConnectionOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	GuessConnectionOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Allow spaces after function names in queries
class MYSQLPP_EXPORT IgnoreSpaceOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	IgnoreSpaceOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Give SQL executed on connect
class MYSQLPP_EXPORT InitCommandOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	InitCommandOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Assert that this is an interactive program
///
/// Affects connection timeouts.
class MYSQLPP_EXPORT InteractiveOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	InteractiveOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable LOAD DATA LOCAL statement
class MYSQLPP_EXPORT LocalFilesOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	LocalFilesOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable LOAD LOCAL INFILE statement
class MYSQLPP_EXPORT LocalInfileOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	LocalInfileOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable multiple result sets in a reply
class MYSQLPP_EXPORT MultiResultsOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	MultiResultsOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable multiple queries in a request to the server
class MYSQLPP_EXPORT MultiStatementsOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	MultiStatementsOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Suggest use of named pipes
class MYSQLPP_EXPORT NamedPipeOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	NamedPipeOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Disable db.tbl.col syntax in queries
class MYSQLPP_EXPORT NoSchemaOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	NoSchemaOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


#if MYSQL_VERSION_ID > 40000		// only in 4.0 +
/// \brief Set type of protocol to use
class MYSQLPP_EXPORT ProtocolOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ProtocolOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};
#endif


/// \brief Override use of my.cnf
class MYSQLPP_EXPORT ReadDefaultFileOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReadDefaultFileOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Override use of my.cnf
class MYSQLPP_EXPORT ReadDefaultGroupOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReadDefaultGroupOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set timeout for IPC data reads
class MYSQLPP_EXPORT ReadTimeoutOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReadTimeoutOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable automatic reconnection to server
class MYSQLPP_EXPORT ReconnectOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReconnectOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set reporting of data truncation errors
class MYSQLPP_EXPORT ReportDataTruncationOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReportDataTruncationOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enforce use of secure authentication, refusing connection if
/// not available
class MYSQLPP_EXPORT SecureAuthOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SecureAuthOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Give path to charset definition files
class MYSQLPP_EXPORT SetCharsetDirOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SetCharsetDirOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Give name of default charset
class MYSQLPP_EXPORT SetCharsetNameOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SetCharsetNameOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Fake client IP address when connecting to embedded server
class MYSQLPP_EXPORT SetClientIpOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SetClientIpOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set name of shmem segment for IPC
class MYSQLPP_EXPORT SharedMemoryBaseNameOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SharedMemoryBaseNameOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Specialized option for handling SSL parameters.
class MYSQLPP_EXPORT SslOption : public Option
{
public:
	/// \brief Create a set of SSL connection option parameters
	///
	/// \param key the pathname to the key file
	/// \param cert the pathname to the certificate file
	/// \param ca the pathname to the certificate authority file
	/// \param capath directory that contains trusted SSL CA
	///        certificates in pem format.
	/// \param cipher list of allowable ciphers to use
	///
	/// This option replaces \c Connection::enable_ssl() from MySQL++
	/// version 2.  Now you can set this connection option just like any
	/// other.
	SslOption(const char* key = 0, const char* cert = 0,
			const char* ca = 0, const char* capath = 0,
			const char* cipher = 0)
	{
		if (key)	key_.assign(key);
		if (cert)	cert_.assign(cert);
		if (ca)		ca_.assign(ca);
		if (capath)	capath_.assign(capath);
		if (cipher)	cipher_.assign(cipher);
	}

private:
	std::string key_, cert_, ca_, capath_, cipher_;
	Error set(DBDriver* dbd);
};


/// \brief Connect to embedded  server in preference to remote server
class MYSQLPP_EXPORT UseEmbeddedConnectionOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	UseEmbeddedConnectionOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Connect to remote server in preference to embedded server
class MYSQLPP_EXPORT UseRemoteConnectionOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	UseRemoteConnectionOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set timeout for IPC data reads
class MYSQLPP_EXPORT WriteTimeoutOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	WriteTimeoutOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


////////////////////////////////////////////////////////////////////////
// Typedefs

/// \brief The data type of the list of connection options
typedef std::deque<Option*> OptionList;

/// \brief Primary iterator type into List
typedef OptionList::const_iterator OptionListIt;

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_OPTIONS_H)
