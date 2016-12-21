/// \file null.h
/// \brief Declares classes that implement SQL "null" semantics within
/// C++'s type system.
///
/// This is required because C++'s own NULL type is not semantically
/// the same as SQL nulls.

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

#ifndef MYSQLPP_NULL_H
#define MYSQLPP_NULL_H

#include "exceptions.h"

#include <iostream>
#include <string>

namespace mysqlpp {

extern const std::string null_str;	///< "NULL" string constant


/// \brief The type of the global mysqlpp::null object.
///
/// User code shouldn't declare variables of this type.  Use the
/// Null template instead.
class MYSQLPP_EXPORT null_type
{
protected:
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	template <typename CannotConvertNullToAnyOtherDataType>
	operator CannotConvertNullToAnyOtherDataType() const
	{
		return CannotConvertNullToAnyOtherDataType();
	}
#endif // !defined(DOXYGEN_IGNORE)
};

/// \brief Global 'null' instance.  Use wherever you need a SQL null.
///
/// SQL null is equal to nothing else.  It is not the same as C++'s
/// NULL value, it is not a Boolean false....it is unique.  As such, if
/// you use this in some other type context, you will get a compiler
/// error saying something about \c CannotConvertNullToAnyOtherDataType.
/// The only thing you can assign this object instance to is a variable
/// of type Null<T>, and then only directly.  Code like this does not
/// work:
///
/// \code
/// int foo = return_some_value_for_foo();
/// mysqlpp::Null<int> bar = foo ? foo : mysqlpp::null;
/// \endcode
/// 
/// The compiler will try to convert mysqlpp::null to \c int to make
/// all values in the conditional operation consistent, but this is
/// not legal.  Anyway, it's questionable code because it means you're
/// using SQL null to mean the same thing as zero here.  If zero is a
/// special value, there's no reason to use SQL null.  SQL null exists
/// when every value for a particular column is legal and you need
/// something that means "no legal value".
const null_type null = null_type();


/// \brief Class for objects that define SQL null in terms of
/// MySQL++'s null_type.
///
/// Returns a null_type instance when you ask what null is, and is
/// "(NULL)" when you insert it into a C++ stream.
///
/// Used for the behavior parameter for template Null
struct NullIsNull
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	static null_type null_is() { return null; }

	static std::ostream& null_ostr(std::ostream& o)
	{
		o << "(NULL)";
		return o;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Class for objects that define SQL null as 0.
///
/// Returns 0 when you ask what null is, and is zero when you insert it
/// into a C++ stream.
///
/// Used for the behavior parameter for template Null
struct NullIsZero
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	static int null_is() { return 0; }
	
	static std::ostream& null_ostr(std::ostream& o)
	{
		o << 0;
		return o;
	}
#endif // !defined(DOXYGEN_IGNORE)
};

/// \brief Class for objects that define SQL null as a blank C string.
///
/// Returns "" when you ask what null is, and is empty when you insert
/// it into a C++ stream.
///
/// Used for the behavior parameter for template Null
struct NullIsBlank
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	static const char *null_is() { return ""; }
	
	static std::ostream& null_ostr(std::ostream& o)
	{
		o << "";
		return o;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Class for holding data from a SQL column with the NULL
/// attribute.
///
/// This template is necessary because there is nothing in the C++ type
/// system with the same semantics as SQL's null.  In SQL, a column can
/// have the optional 'NULL' attribute, so there is a difference in 
/// type between, say an \c int column that can be null and one that
/// cannot be.  C++'s NULL constant does not have these features.
///
/// It's important to realize that this class doesn't hold nulls,
/// it holds data that \e can \e be null.  It can hold a non-null
/// value, you can then assign null to it (using MySQL++'s global
/// \c null object), and then assign a regular value to it again; the
/// object will behave as you expect throughout this process.
///
/// Because one of the template parameters is a C++ type, the typeid()
/// for a null \c int is different than for a null \c string, to pick
/// two random examples.  See type_info.cpp for the table SQL types that
/// can be null.
template <class Type, class Behavior = NullIsNull>
class Null
{
public:
	/// \brief The object's value, when it is not SQL null
	Type data;
	
	/// \brief If set, this object is considered equal to SQL null.
	///
	/// This flag affects how the Type() and << operators work.
	bool is_null;

	/// \brief Type of the data stored in this object, when it is not
	/// equal to SQL null.
	typedef Type value_type;

	/// \brief Default constructor
	///
	/// "data" member is left uninitialized by this ctor, because we
	/// don't know what to initialize it to.
	Null() :
	data(),
	is_null(false)
	{
	}

	/// \brief Initialize the object with a particular value.
	///
	/// The object is marked as "not null" if you use this ctor.  This
	/// behavior exists because the class doesn't encode nulls, but
	/// rather data which \e can \e be null.  The distinction is
	/// necessary because 'NULL' is an optional attribute of SQL
	/// columns.
	Null(const Type& x) :
	data(x),
	is_null(false)
	{
	}

	/// \brief Construct a Null equal to SQL null
	///
	/// This is typically used with the global \c null object. (Not to
	/// be confused with C's NULL type.)  You can say something like...
	/// \code
	/// Null<int> foo = null;
	/// \endcode
	/// ...to get a null \c int.
	Null(const null_type&) :
	data(),
	is_null(true)
	{
	}

	/// \brief Converts this object to Type
	///
	/// If is_null is set, returns whatever we consider that null "is",
	/// according to the Behavior parameter you used when instantiating
	/// this template.  See NullIsNull, NullIsZero and NullIsBlank.
	///
	/// Otherwise, just returns the 'data' member.
	operator Type() const
	{
		if (is_null) {
			return Behavior::null_is();
		}
		else {
			return data;
		}
	}

	/// \brief Assign a value to the object.
	///
	/// This marks the object as "not null" as a side effect.
	Null& operator =(const Type& x)
	{
		data = x;
		is_null = false;
		return *this;
	}

	/// \brief Assign SQL null to this object.
	///
	/// This just sets the is_null flag; the data member is not
	/// affected until you call the Type() operator on it.
	Null& operator =(const null_type&)
	{
		is_null = true;
		return *this;
	}

	/// \brief Do equality comparison of two nullable values
	///
	/// Two null objects are equal, and null is not equal to not-null.
	/// If neither is null, we delegate to operator == for the base
	/// data type.
	bool operator ==(const Null<Type>& rhs) const
	{
		if (is_null && rhs.is_null) {
			return true;
		}
		else if (is_null != rhs.is_null) {
			return false;	// one null, the other not
		}
		else {
			return data == rhs.data;
		}
	}

	/// \brief Do equality comparison against hard-coded SQL null
	///
	/// This tells you the same thing as testing is_null member.
	bool operator ==(const null_type&) const { return is_null; }

	/// \brief Do inequality comparison of two nullable values
	bool operator !=(const Null<Type>& rhs) const
			{ return !(*this == rhs); }

	/// \brief Do inequality comparison against hard-coded SQL null
	bool operator !=(const null_type& rhs) const
			{ return !(*this == rhs); }

	/// \brief Do less-than comparison of two nullable values
	///
	/// Two null objects are equal to each other, and null is less
	/// than not-null.  If neither is null, we delegate to operator <
	/// for the base data type.
	bool operator <(const Null<Type>& rhs) const
	{
		if (is_null) {
			return !rhs.is_null;	// less than only if RHS not null
		}
		else if (rhs.is_null) {
			return false;			// non-null always greater than null
		}
		else {
			return data < rhs.data;	// neither is null, so compare data
		}
	}

	/// \brief Do less-than comparison against hard-coded SQL null
	///
	/// Always returns false because we can only be greater than or
	/// equal to a SQL null.
	bool operator <(const null_type&) const { return false; }
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

// Specialization the Null template for \c void
template <> class Null<void>
{
public:
	bool is_null;
	typedef void value_type;

	Null() :
	is_null(false)
	{
	}
	
	Null(const null_type&) :
	is_null(true)
	{
	}

	Null& operator =(const null_type&)
	{
		is_null = true;
		return *this;
	}
};

#endif // !defined(DOXYGEN_IGNORE)


/// \brief Inserts null-able data into a C++ stream if it is not
/// actually null.  Otherwise, insert something appropriate for null
/// data.
template <class Type, class Behavior>
inline std::ostream& operator <<(std::ostream& o,
		const Null<Type, Behavior>& n)
{
	if (n.is_null)
		return Behavior::null_ostr(o);
	else
		return o << n.data;
}

} // end namespace mysqlpp

#endif
