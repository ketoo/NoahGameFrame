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
#ifndef _TRAIT_H
#define _TRAIT_H

//pointer traits
template<typename T>
struct pointerTraits
{
	enum { isPointer = false};
	typedef T PointeeType;
};

template<typename T>
struct pointerTraits<const T*>
{
	enum { isPointer = true};
	typedef T PointeeType;
};

template<typename T>
struct pointerTraits<const T* const>
{
	enum { isPointer = true};
	typedef const T PointeeType;
};

template<typename T>
struct pointerTraits<T* const>
{
	enum { isPointer = true};
	typedef const T PointeeType;
};


template<typename T>
struct pointerTraits<T*>
{
	enum { isPointer = true};
	typedef T PointeeType;
};

//pointer traits
template<typename T>
struct refTraits
{
	enum { isRef = false};
	typedef T RefType;
};

template<typename T>
struct refTraits<const T&>
{
	enum { isRef = true};
	typedef T RefType;
};

template<typename T>
struct refTraits<T&>
{
	enum { isPointer = true};
	typedef T RefType;
};
/////

template<typename T>
struct isVoid
{
	enum {is_Void = 0};
};

template<>
struct isVoid<void>
{
	enum {is_Void = 1};
};


#endif