/// \file comparable.h
/// \brief Declares the Comparable<T> mixin

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

#if !defined(MYSQLPP_COMPARABLE_H)
#define MYSQLPP_COMPARABLE_H

namespace mysqlpp {

/// \brief Mix-in that gives its subclass a full set of comparison
/// operators.
///
/// Simply by inheriting publically from this and implementing
/// compare(), the subclass gains a full set of comparison operators,
/// because all of the operators are implemented in terms of compare().
template <class T>
class Comparable
{
public:
	/// \brief Returns true if "other" is equal to this object
	bool operator ==(const T& other) const
	{
		return !compare(other);
	}

	/// \brief Returns true if "other" is not equal to this object
	bool operator !=(const T& other) const
	{
		return compare(other);
	}

	/// \brief Returns true if "other" is less than this object
	bool operator <(const T& other) const
	{
		return compare(other) < 0;
	}

	/// \brief Returns true if "other" is less than or equal to this object
	bool operator <=(const T& other) const
	{
		return compare(other) <= 0;
	}

	/// \brief Returns true if "other" is greater than this object
	bool operator >(const T& other) const
	{
		return compare(other) > 0;
	}

	/// \brief Returns true if "other" is greater than or equal to this object
	bool operator >=(const T& other) const
	{
		return compare(other) >= 0;
	}

protected:
	/// \brief Destroy object
	///
	/// This class has nothing to destroy, but declaring the dtor
	/// virtual placates some compilers set to high warning levels.
	/// Protecting it ensures you can't delete subclasses through base
	/// class pointers, which makes no sense because this class isn't
	/// made for polymorphism.  It's just a mixin.
	virtual ~Comparable() { }

	/// \brief Compare this object to another of the same type
	///
	/// Returns < 0 if this object is "before" the other, 0 of they are
	/// equal, and > 0 if this object is "after" the other.
	virtual int compare(const T& other) const = 0;
};

}

#endif // !defined(MYSQLPP_COMPARABLE_H)
