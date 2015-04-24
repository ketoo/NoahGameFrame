/// \file tiny_int.h
/// \brief Declares class for holding a SQL TINYINT

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

#if !defined(MYSQLPP_TINY_INT_H)
#define MYSQLPP_TINY_INT_H

#include "common.h"

#include <ostream>

namespace mysqlpp {

/// \brief Class for holding an SQL \c TINYINT value
///
/// This is required because the closest C++ type, \c char, doesn't
/// have all the right semantics.  For one, inserting a \c char into a
/// stream won't give you a number.  For another, if you don't specify
/// signedness explicitly, C++ doesn't give a default, so it's signed
/// on some platforms, unsigned on others.
///
/// The template parameter is intended to allow instantiating it as
/// tiny_int<unsigned char> to hold \c TINYINT \c UNSIGNED values.
/// There's nothing stopping you from using any other integer type if
/// you want to be perverse, but please don't do that.
///
/// Several of the functions below accept an \c int argument, but
/// internally we store the data as a \c char by default. Beware of
/// integer overflows!

template <typename VT = signed char>
class tiny_int
{
public:
	//// Type aliases
	typedef tiny_int<VT> this_type;	///< alias for this object's type
	typedef VT value_type;			///< alias for type of internal value

	/// \brief Default constructor
	///
	/// Value is uninitialized
	tiny_int() { }
	
	/// \brief Create object from any integral type that can be
	/// converted to a \c short \c int.
	tiny_int(value_type v) :
	value_(value_type(v))
	{
	}
	
	/// \brief Return truthiness of value
	operator bool() const
	{
		return value_;
	}

	/// \brief Return value as an \c int.
	operator int() const
	{
		return static_cast<int>(value_);
	}

	/// \brief Return raw data value with no size change
	operator value_type() const
	{
		return value_;
	}

	/// \brief Assign a new value to the object.
	this_type& operator =(int v)
	{
		value_ = static_cast<value_type>(v);
		return *this;
	}

	/// \brief Add another value to this object
	this_type& operator +=(int v)
	{
		value_ += static_cast<value_type>(v);
		return *this;
	}

	/// \brief Subtract another value to this object
	this_type& operator -=(int v)
	{
		value_ -= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Multiply this value by another object
	this_type& operator *=(int v)
	{
		value_ *= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Divide this value by another object
	this_type& operator /=(int v)
	{
		value_ /= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Divide this value by another object and store the
	/// remainder
	this_type& operator %=(int v)
	{
		value_ %= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Bitwise AND this value by another value
	this_type& operator &=(int v)
	{
		value_ &= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Bitwise OR this value by another value
	this_type& operator |=(int v)
	{
		value_ |= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Bitwise XOR this value by another value
	this_type& operator ^=(int v)
	{
		value_ ^= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Shift this value left by \c v positions
	this_type& operator <<=(int v)
	{
		value_ <<= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Shift this value right by \c v positions
	this_type& operator >>=(int v)
	{
		value_ >>= static_cast<value_type>(v);
		return *this;
	}

	/// \brief Add one to this value and return that value
	this_type& operator ++()
	{
		++value_;
		return *this;
	}

	/// \brief Subtract one from this value and return that value
	this_type& operator --()
	{
		--value_;
		return *this;
	}

	/// \brief Add one to this value and return the previous value
	this_type operator ++(int)
	{
		this_type tmp = value_;
		++value_;
		return tmp;
	}

	/// \brief Subtract one from this value and return the previous
	/// value
	this_type operator --(int)
	{
		this_type tmp = value_;
		--value_;
		return tmp;
	}

	/// \brief Return this value minus \c i
	this_type operator -(const this_type& i) const
	{
		return value_ - i.value_;
	}
	
	/// \brief Return this value plus \c i
	this_type operator +(const this_type& i) const
	{
		return value_ + i.value_;
	}
	
	/// \brief Return this value multiplied by \c i
	this_type operator *(const this_type& i) const
	{
		return value_ * i.value_;
	}
	
	/// \brief Return this value divided by \c i
	this_type operator /(const this_type& i) const
	{
		return value_ / i.value_;
	}
	
	/// \brief Return the modulus of this value divided by \c i
	this_type operator %(const this_type& i) const
	{
		return value_ % i.value_;
	}
	
	/// \brief Return this value bitwise OR'd by \c i
	this_type operator |(const this_type& i) const
	{
		return value_ | i.value_;
	}
	
	/// \brief Return this value bitwise AND'd by \c i
	this_type operator &(const this_type& i) const
	{
		return value_ & i.value_;
	}
	
	/// \brief Return this value bitwise XOR'd by \c i
	this_type operator ^(const this_type& i) const
	{
		return value_ ^ i.value_;
	}
	
	/// \brief Return this value bitwise shifted left by \c i
	this_type operator <<(const this_type& i) const
	{
		return value_ << i.value_;
	}
	
	/// \brief Return this value bitwise shifted right by \c i
	this_type operator >>(const this_type& i) const
	{
		return value_ >> i.value_;
	}

	/// \brief Check for equality
	bool operator ==(const this_type& i) const
	{
		return value_ == i.value_;
	}

	/// \brief Check for inequality
	bool operator !=(const this_type& i) const
	{
		return value_ != i.value_;
	}

	/// \brief Check that this object is less than another
	bool operator <(const this_type& i) const
	{
		return value_ < i.value_;
	}

	/// \brief Check that this object is greater than another
	bool operator >(const this_type& i) const
	{
		return value_ > i.value_;
	}

	/// \brief Check this object is less than or equal to another
	bool operator <=(const this_type& i) const
	{
		return value_ <= i.value_;
	}

	/// \brief Check this object is greater than or equal to another
	bool operator >=(const this_type& i) const
	{
		return value_ >= i.value_;
	}

private:
	value_type value_;
};

/// \brief Insert a \c tiny_int into a C++ stream
template <typename VT>
std::ostream& operator <<(std::ostream& os, tiny_int<VT> i)
{
	os << static_cast<int>(i);
	return os;
}

} // end namespace mysqlpp

#endif
