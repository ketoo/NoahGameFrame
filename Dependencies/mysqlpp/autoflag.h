/// \file autoflag.h
/// \brief Defines a template for setting a flag within a given variable
/// scope, and resetting it when exiting that scope.

/***********************************************************************
 Copyright (c) 2007 by Educational Technology Resources, Inc.
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

#if !defined(MYSQLPP_AUTOFLAG_H)
#define MYSQLPP_AUTOFLAG_H

/// \brief A template for setting a flag on a variable as long as the
/// object that set it is in scope.  Flag resets when object goes
/// out of scope.  Works on anything that looks like bool.

template <class T = bool>
class AutoFlag
{
public:
	/// \brief Constructor: sets ref to true.
	AutoFlag(T& ref) :
	referent_(ref)
	{
		referent_ = true;
	}

	/// \brief Destructor: sets referent passed to ctor to false.
	~AutoFlag()
	{
		referent_ = false;
	}

private:
	T& referent_;
};

#endif // !defined(MYSQLPP_AUTOFLAG_H)

