/// \file mystring.h
/// \brief Declares String class, MySQL++'s generic std::string-like
/// class, used for holding data received from the database server.

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

#if !defined(MYSQLPP_MYSTRING_H)
#define MYSQLPP_MYSTRING_H

#include "common.h"

#include "datetime.h"
#include "exceptions.h"
#include "null.h"
#include "sql_buffer.h"

#include <string>
#include <sstream>
#include <limits>

#include <stdlib.h>
#include <string.h>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

namespace detail
{
	template<typename T, bool is_signed = std::numeric_limits<T>::is_signed>
	struct conv_promotion;

	template<>
	struct conv_promotion<float>
	{
		typedef double type;
	};

	template<>
	struct conv_promotion<double>
	{
		typedef double type;
	};

#	if !defined(NO_LONG_LONGS)
	template<>
	struct conv_promotion<unsigned long long>
	{
		typedef unsigned long long type;
	};

	template<>
	struct conv_promotion<long long>
	{
		typedef long long type;
	};
#	endif

	// preserve existing behaviour, char converted as signed long
	template<>
	struct conv_promotion<char>
	{
		typedef long type;
	};

	// all other types use signed/unsigned long

	template<typename T>
	struct conv_promotion<T, true>
	{
		typedef long type;
	};

	template<typename T>
	struct conv_promotion<T, false>
	{
		typedef unsigned long type;
	};
} // namespace detail

class MYSQLPP_EXPORT SQLTypeAdapter;
#endif // !defined(DOXYGEN_IGNORE)

/// \brief A std::string work-alike that can convert itself from SQL
/// text data formats to C++ data types.
///
/// This class is an intermediate form for a SQL field, normally
/// converted to a more useful native C++ type, not used directly.
/// The only exception is in dealing with BLOB data, which stays in
/// String form for efficiency and to avoid corrupting the data with
/// facile conversions.  Even then, it's best to use it through the 
/// typedef aliases like sql_blob in sql_types.h, in case we later
/// change this underlying representation.
///
/// String's implicit conversion operators let you can use these
/// objects naturally:
///
/// \code String("12.86") + 2.0 \endcode
///
/// That will give you 14.86 (approximately) as you expect, but be
/// careful not to get tripped up by C++'s type conversion rules.  If
/// you had said this instead:
/// 
/// \code String("12.86") + 2 \endcode
/// 
/// the result would be 14 because 2 is an integer, and C++'s type
/// conversion rules put the String object in an integer context.
///
/// You can disable the operator overloads that allow these things by
/// defining MYSQLPP_NO_BINARY_OPERS.
///
/// This class also has some basic information about the type of data
/// stored in it, to allow it to do the conversions more intelligently
/// than a trivial implementation would allow.

class MYSQLPP_EXPORT String
{
public:
	/// \brief Type of the data stored in this object, when it is not
	/// equal to SQL null.
	typedef const char value_type;

	/// \brief Type of "size" integers
	typedef size_t size_type;

	/// \brief Type of iterators
	typedef const char* const_iterator;

	/// \brief Same as const_iterator because the data cannot be
	/// changed.
	typedef const_iterator iterator;

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	typedef int difference_type;
	typedef const char* const_pointer;
	typedef const_pointer pointer;
#endif // !defined(DOXYGEN_IGNORE)

	/// \brief Default constructor
	///
	/// An object constructed this way is essentially useless, but
	/// sometimes you just need to construct a default object.
	String() :
	buffer_()
	{
	}

	/// \brief Copy ctor
	///
	/// \param other the other String object
	///
	/// This ctor only copies the pointer to the other String's data
	/// buffer and increments its reference counter.  If you need a
	/// deep copy, use one of the ctors that takes a string.
	String(const String& other) :
	buffer_(other.buffer_)
	{
	}

	/// \brief Full constructor.
	///
	/// \param str the string this object represents, or 0 for SQL null
	/// \param len the length of the string; embedded nulls are legal
	/// \param type MySQL type information for data within str
	/// \param is_null string represents a SQL null, not literal data
	///
	/// The resulting object will contain a copy of the string buffer.
	/// The buffer will actually be 1 byte longer than the value given
	/// for \c len, to hold a null terminator for safety.  We do this
	/// because this ctor may be used for things other than
	/// null-terminated C strings.  (e.g. BLOB data)
	explicit String(const char* str, size_type len,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false) :
	buffer_(new SQLBuffer(str, len, type, is_null))
	{
	}

	/// \brief C++ string version of full ctor
	///
	/// \param str the string this object represents, or 0 for SQL null
	/// \param type MySQL type information for data within str
	/// \param is_null string represents a SQL null, not literal data
	///
	/// The resulting object will contain a copy of the string buffer.
	explicit String(const std::string& str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false) :
	buffer_(new SQLBuffer(str.data(), static_cast<size_type>(str.length()),
			type, is_null))
	{
	}

	/// \brief Null-terminated C string version of full ctor
	///
	/// \param str the string this object represents, or 0 for SQL null
	/// \param type MySQL type information for data within str
	/// \param is_null string represents a SQL null, not literal data
	///
	/// The resulting object will contain a copy of the string buffer.
	explicit String(const char* str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false) :
	buffer_(new SQLBuffer(str, static_cast<size_type>(strlen(str)),
			type, is_null))
	{
	}

	/// \brief Destroy string
	~String() { }

	/// \brief Assign raw data to this object
	///
	/// This parallels the ctor with the same parameters, for when you
	/// must do a 2-step create, or when you want to reassign the data 
	/// without creating a String temporary to get around the fact
	/// that operator=() can only take one parameter.
	void assign(const char* str, size_type len,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false)
	{
		buffer_ = new SQLBuffer(str, len, type, is_null);
	}

	/// \brief Assign a C++ string to this object
	///
	/// This parallels the ctor with the same parameters, for when you
	/// must do a 2-step create, or when you want to reassign the data 
	/// without creating a String temporary to get around the fact
	/// that operator=() can only take one parameter.
	void assign(const std::string& str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false)
	{
		buffer_ = new SQLBuffer(str.data(),
				static_cast<size_type>(str.length()), type, is_null);
	}

	/// \brief Assign a C string to this object
	///
	/// This parallels the ctor with the same parameters, for when you
	/// must do a 2-step create, or when you want to reassign the data 
	/// without creating a String temporary to get around the fact
	/// that operator=() can only take one parameter.
	void assign(const char* str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false)
	{
		buffer_ = new SQLBuffer(str, static_cast<size_type>(strlen(str)),
				type, is_null);
	}

	/// \brief Return a character within the string.
	///
	/// \throw mysqlpp::BadIndex if the row is not initialized or there
	/// are less than \c i fields in the row.
	char at(size_type pos) const;

	/// \brief Return iterator pointing to the first character of
	/// the string
	const_iterator begin() const { return data(); }

	/// \brief Return a const pointer to the string data.
	const char* c_str() const { return data(); }
	
#if defined(MYSQLPP_PLATFORM_VISUAL_CPP)
// Squish VC++ warning about "possible loss of data" for these conversions
#   pragma warning(disable: 4244)
#endif

	/// \brief Template for converting the column data to most any
	/// numeric data type.
	template <class Type>
	Type conv(Type) const
	{
		// Conversions are done using one of double/long/ulong/llong/ullong
		// so we call a helper function to do the work using that type.
		// This reduces the amount of template code instantiated.
		typedef typename detail::conv_promotion<Type>::type conv_type;
		return do_conv<conv_type>(typeid(Type).name());
	}

#if defined(MYSQLPP_PLATFORM_VISUAL_CPP)
#   pragma warning(default: 4244)
#endif

	/// \brief Overload of conv() for types wrapped with Null<>
	///
	/// If the String object was initialized with some string we
	/// recognize as a SQL null, we just return a copy of the global
	/// 'null' object converted to the requested type.  Otherwise, we
	/// return the String's value wrapped in the Null<> template.
	template <class T, class B>
	Null<T, B> conv(Null<T, B>) const
	{
		if (is_null()) {
			return Null<T, B>(null);
		}
		else {
			return Null<T, B>(conv(T()));
		}
	}

	/// \brief Lexically compare this string to another.
	///
	/// \param other string to compare against this one
	///
	/// \see compare(size_type, size_type, const char*)
	int compare(const String& other) const;

	/// \brief Lexically compare this string to another.
	///
	/// \param other string to compare against this one
	///
	/// \see compare(size_type, size_type, const char*)
	int compare(const std::string& other) const;

	/// \brief Lexically compare this string to another.
	///
	/// \param pos position within this string to begin comparison
	/// \param num maximum number of characters within this string to
	/// use in comparison
	/// \param other string to compare against this one
	///
	/// \see compare(size_type, size_type, const char*)
	int compare(size_type pos, size_type num, std::string& other) const;

	/// \brief Lexically compare this string to another.
	///
	/// \param other string to compare against this one
	///
	/// \see compare(size_type, size_type, const char*)
	int compare(const char* other) const;

	/// \brief Lexically compare this string to another.
	///
	/// \param pos position within this string to begin comparison
	/// \param num maximum number of characters within this string to
	/// use in comparison
	/// \param other string to compare against this one
	///
	/// \retval < 0 if this string is lexically "less than" other
	/// \retval 0 if this string is equal to other
	/// \retval > 0 if this string is lexically "greater than" other
	int compare(size_type pos, size_type num, const char* other) const;

	/// \brief Raw access to the underlying buffer, with no C string
	/// interpretation.
	const char* data() const;

	/// \brief Returns true if size() == 0
	bool empty() const { return size() == 0; }
	
	/// \brief Return iterator pointing to one past the last character
	/// of the string.
	const_iterator end() const;

	/// \brief Returns true if data of this type should be escaped, false
	/// otherwise.
	bool escape_q() const;

	/// \brief Returns true if this object is a SQL null.
	bool is_null() const;

	/// \brief Set a flag indicating that this object is a SQL null.
	void it_is_null();

	/// \brief Return number of bytes in the string
	///
	/// Note that this doesn't count the number of \b characters in the
	/// string.  If your database is configured to use an 8-bit character
	/// set, this is a distinction without a difference.  But, if you're
	/// using UTF-8 in the database, you will need to "widen" the UTF-8
	/// data to use a fixed-size character set like UCS-2 and count the
	/// characters that way.  You might use std::wstring, for example.
	size_type length() const;
	
	/// \brief Return the maximum number of characters in the string.
	///
	/// Because this is a \c const string, this is just an alias for
	/// size(); its size is always equal to the amount of data currently
	/// stored.
	size_type max_size() const { return size(); }

	/// \brief Returns true if data of this type should be quoted, false
	/// otherwise.
	bool quote_q() const;

	/// \brief Return number of bytes in string
	///
	/// See commentary for length() about the difference between bytes
	/// and characters.
	size_type size() const { return length(); }
	
	/// \brief Returns a copy of our internal string without leading
	/// blanks.
	void strip_leading_blanks(std::string& s) const
	{
		const char* pc = data();
		if (pc) {
			size_type n = length();
			while (n && (*pc == ' ')) {
				++pc;
				--n;
			}

			s.assign(pc, n);
		}
		else {
			s.clear();
		}
	}

	/// \brief Copies this object's data into a C++ string.
	///
	/// If you know the data doesn't contain null characters (i.e. it's
	/// a typical string, not BLOB data), it's more efficient to just
	/// assign this object to anything taking \c const \c char*.  (Or
	/// equivalently, call the \c data() method.)  This copies a pointer
	/// to a buffer instead of copying the buffer's contents.
	void to_string(std::string& s) const;

	/// \brief Get this object's current MySQL type.
	mysql_type_info type() const
	{
		return buffer_ ? buffer_->type() : mysql_type_info::string_type;
	}

	/// \brief Assignment operator, from C++ string
	String& operator =(const std::string& rhs)
	{
		buffer_ = new SQLBuffer(rhs.data(),
				static_cast<size_type>(rhs.length()),
				mysql_type_info::string_type, false);

		return *this;
	}

	/// \brief Assignment operator, from C string
	///
	/// This creates a copy of the entire string, not just a copy of
	/// the pointer.
	String& operator =(const char* str)
	{
		buffer_ = new SQLBuffer(str,
				static_cast<size_type>(strlen(str)),
				mysql_type_info::string_type, false);

		return *this;
	}

	/// \brief Assignment operator, from other String
	///
	/// This only copies the pointer to the other String's data
	/// buffer and increments its reference counter.  If you need a
	/// deep copy, assign a string to this object instead.
	String& operator =(const String& other)
	{
		buffer_ = other.buffer_;

		return *this;
	}

	/// \brief Equality comparison operator
	///
	/// For comparing this object to any of the data types we have a
	/// compare() overload for.
	template <typename T>
	bool operator ==(const T& rhs) const
	{
		return compare(rhs) == 0;
	}

	/// \brief Equality comparison operator
	///
	/// For checking object against MySQL++'s global \c null constant
	bool operator ==(const mysqlpp::null_type&) const
	{
		return is_null();
	}

	/// \brief Inequality comparison operator
	///
	/// For comparing this object to any of the data types we have a
	/// compare() overload for.
	template <typename T>
	bool operator !=(const T& rhs) const
	{
		return compare(rhs) != 0;
	}

	/// \brief Inequality comparison operator
	///
	/// For checking object against MySQL++'s global \c null constant
	bool operator !=(const mysqlpp::null_type&) const
	{
		return !is_null();
	}

	/// \brief Return a character within the string.
	///
	/// This function is just syntactic sugar, wrapping the at() method.
	///
	/// \throw mysqlpp::BadIndex if the string is not initialized or there
	/// are less than \c i fields in the string.
	char operator [](size_type pos) const { return at(pos); }

	/// \brief Returns a const char pointer to the object's raw data
	operator const char*() const { return data(); }
	
	/// \brief Converts this object's string data to a signed char
	operator signed char() const
			{ return conv(static_cast<signed char>(0)); }
	
	/// \brief Converts this object's string data to an unsigned char
	operator unsigned char() const
			{ return conv(static_cast<unsigned char>(0)); }
	
	/// \brief Converts this object's string data to an int
	operator int() const
			{ return conv(static_cast<int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned int
	operator unsigned int() const
			{ return conv(static_cast<unsigned int>(0)); }
	
	/// \brief Converts this object's string data to a short int
	operator short int() const
			{ return conv(static_cast<short int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned short
	/// int
	operator unsigned short int() const
			{ return conv(static_cast<unsigned short int>(0)); }
	
	/// \brief Converts this object's string data to a long int
	operator long int() const
			{ return conv(static_cast<long int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned long
	/// int
	operator unsigned long int() const
			{ return conv(static_cast<unsigned long int>(0)); }
	
#if !defined(NO_LONG_LONGS)
	/// \brief Converts this object's string data to the platform-
	/// specific 'longlong' type, usually a 64-bit integer.
	operator longlong() const
			{ return conv(static_cast<longlong>(0)); }
	
	/// \brief Converts this object's string data to the platform-
	/// specific 'ulonglong' type, usually a 64-bit unsigned integer.
	operator ulonglong() const
			{ return conv(static_cast<ulonglong>(0)); }
#endif
	
	/// \brief Converts this object's string data to a float
	operator float() const
			{ return conv(static_cast<float>(0)); }
	
	/// \brief Converts this object's string data to a double
	operator double() const
			{ return conv(static_cast<double>(0)); }
	
	/// \brief Converts this object's string data to a bool
	operator bool() const { return buffer_ ? atoi(c_str()) : false; }

	/// \brief Converts this object's string data to a mysqlpp::Date
	operator Date() const { return buffer_ ? Date(*this) : Date(); }

	/// \brief Converts this object's string data to a mysqlpp::DateTime
	operator DateTime() const
			{ return buffer_ ? DateTime(*this) : DateTime(); }

	/// \brief Converts this object's string data to a mysqlpp::Time
	operator Time() const { return buffer_ ? Time(*this) : Time(); }

	/// \brief Converts the String to a nullable data type
	///
	/// This is just an implicit version of conv(Null<T, B>)
	template <class T, class B>
	operator Null<T, B>() const { return conv(Null<T, B>()); }

private:
	/// \brief Do the actual numeric conversion via @p Type.
	template <class Type>
	Type do_conv(const char* type_name) const
	{
		if (buffer_) {
			std::stringstream buf;
			buf.write(data(), static_cast<std::streamsize>(length()));
			buf.imbue(std::locale::classic()); // "C" locale
			Type num = Type();
			
			if (buf >> num) {
				char c;
				if (!(buf >> c)) {
					// Nothing left in buffer, so conversion complete,
					// and thus successful.
					return num;
				}

				if (c == '.' &&
						(typeid(Type) != typeid(float)) &&
						(typeid(Type) != typeid(double))) {
					// Conversion stopped on a decimal point -- locale
					// doesn't matter to MySQL -- so only way to succeed
					// is if it's an integer and everything following
					// the decimal is inconsequential.
					c = '0';	// handles '.' at end of string
					while (buf >> c && c == '0') /* spin */ ;
					if (buf.eof() && c == '0') {
						return num;  // only zeros after decimal point
					}
				}
			}
			else if (buf.eof()) {
				return num;  // nothing to convert, return default value
			}

			throw BadConversion(type_name, data(), 0, length());
		}
		else {
			return 0;
		}
	}

	RefCountedBuffer buffer_;	///< reference-counted data buffer

	friend class SQLTypeAdapter;
};

MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& o,
		const String& in);


#if !defined(MYSQLPP_NO_BINARY_OPERS) && !defined(DOXYGEN_IGNORE)
// Ignore this section is MYSQLPP_NO_BINARY_OPERS is defined, or if this
// section is being parsed by Doxygen.  In the latter case, it's ignored
// because Doxygen doesn't understand it correctly, and we can't be
// bothered to explain it to Doxygen.

#define oprsw(opr, other, conv) \
	inline other operator opr (String x, other y) \
			{ return static_cast<conv>(x) opr y; } \
	inline other operator opr (other x, String y) \
			{ return x opr static_cast<conv>(y); }

#define operator_binary(other, conv) \
	oprsw(+, other, conv) \
	oprsw(-, other, conv) \
	oprsw(*, other, conv) \
	oprsw(/, other, conv)

#define operator_binary_int(other, conv) \
	operator_binary(other, conv) \
	oprsw(%, other, conv) \
	oprsw(&, other, conv) \
	oprsw(^, other, conv) \
	oprsw(|, other, conv) \
	oprsw(<<, other, conv) \
	oprsw(>>, other, conv)

// Squish more complaints about possible loss of data
#if defined(MYSQLPP_PLATFORM_VISUAL_CPP)
#	pragma warning(disable: 4244)
#endif

operator_binary(float, double)
operator_binary(double, double)

operator_binary_int(char, long int)
operator_binary_int(int, long int)
operator_binary_int(short int, long int)
operator_binary_int(long int, long int)

operator_binary_int(unsigned char, unsigned long int)
operator_binary_int(unsigned int, unsigned long int)
operator_binary_int(unsigned short int, unsigned long int)
operator_binary_int(unsigned long int, unsigned long int)

#if defined(MYSQLPP_PLATFORM_VISUAL_CPP)
#	pragma warning(default: 4244)
#endif

#if !defined(NO_LONG_LONGS)
operator_binary_int(longlong, longlong)
operator_binary_int(ulonglong, ulonglong)
#endif // !defined(NO_LONG_LONGS)
#endif // !defined(MYSQLPP_NO_BINARY_OPERS) && !defined(DOXYGEN_IGNORE)


#if !defined(DOXYGEN_IGNORE)
// Doxygen isn't smart enough to recognize these template
// specializations.  Maybe it's the MYSQLPP_EXPORT tags?

/// \brief Specialization of String::conv<Type>() for bool
///
/// We can either do it this way, or define "\c strtob()" (string to
/// bool, like \c strtol(), \c strtod()...) so we can use
/// internal_string_to_number_proxy.
template <> MYSQLPP_EXPORT bool String::conv(bool) const;

/// \brief Specialization of String::conv<Type>() for String
///
/// Yes, I hear you crying, "WTF!?  Why does String need to be able to
/// convert itself to String?"  SSQLSes with BLOB columns, that's why.
///
/// SSQLSes populate their data members from the raw field data by
/// calling row["fieldname"].conv().  The raw field data is stored in a
/// String, and the MySQL++ native BLOB type is String.  Since we're
/// dealing with generated code, we need this specialization which hand-
/// written code wouldn't need.  Prove the truth of this to yourself by
/// removing this and counting how many pieces examples/cgi_jpeg.cpp
/// breaks into.
template <> MYSQLPP_EXPORT String String::conv(String) const;

/// \brief Specialization of String::conv<Type>() for C++ strings
template <> MYSQLPP_EXPORT std::string String::conv(std::string) const;

/// \brief Specialization of String::conv<Type>() for mysqlpp::Date
///
/// This is necessary because as of MySQL++ v3, Date no longer has an
/// implicit conversion ctor from String, and SSQLS uses conv() instead
/// of the C++ type conversion system anyway.
template <> MYSQLPP_EXPORT Date String::conv(Date) const;

/// \brief Specialization of String::conv<Type>() for mysqlpp::DateTime
///
/// This is necessary because as of MySQL++ v3, DateTime no longer has
/// an implicit conversion ctor from String, and SSQLS uses conv()
/// instead of the C++ type conversion system anyway.
template <> MYSQLPP_EXPORT DateTime String::conv(DateTime) const;

/// \brief Specialization of String::conv<Type>() for mysqlpp::Time
///
/// This is necessary because as of MySQL++ v3, Time no longer has an
/// implicit conversion ctor from String, and SSQLS uses conv() instead
/// of the C++ type conversion system anyway.
template <> MYSQLPP_EXPORT Time String::conv(Time) const;

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_MYSTRING_H)
