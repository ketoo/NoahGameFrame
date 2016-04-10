/*	
    Copyright (C) <2012>  <huangweilook@21cn.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/	
#ifndef _UTILITY
#define _UTILITY
#include "TypeList.h"
#include "Trait.h"

template<typename T>
struct GetReplaceType
{
	typedef T type;
	//typedef typename refTraits<T>::RefType type;
};

template<>
struct GetReplaceType<const char *>
{
	typedef std::string type;
};

template<>
struct GetReplaceType<char *>
{
	typedef std::string type;
};

template<typename T>
struct GetRawType
{
	typedef T type;
};

template<typename T>
struct GetRawType<T&>
{
	typedef T &type;
};

template<>
struct GetRawType<std::string>
{
	typedef const char * type;
};

template<typename T>
inline typename GetRawType<T>::type GetRawValue(T &in)
{
	return in;
}

template<>
inline GetRawType<std::string>::type GetRawValue(std::string &in)
{
	return in.c_str();
}

#endif
