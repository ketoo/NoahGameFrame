// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef LUABIND_PRIMITIVES_HPP_INCLUDED
#define LUABIND_PRIMITIVES_HPP_INCLUDED

#include <algorithm>
#include <cstring>

#include <luabind/config.hpp>
#include <luabind/detail/yes_no.hpp>

namespace luabind { namespace detail
{
	template<class T>
	struct identity
	{
		typedef T type;
	};

	template<class T>
    struct type_ {};

	struct null_type {};

/*	typedef char yes_t;
	typedef double no_t;*/

	struct lua_to_cpp {};
	struct cpp_to_lua {};

	template<class T> struct by_value {};
	template<class T> struct by_reference {};
	template<class T> struct by_const_reference {};
	template<class T> struct by_pointer {};
	template<class T> struct by_const_pointer {};

	struct converter_policy_tag {};

	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const { return std::strcmp(s1, s2) < 0; }
	};

	template<int N>
	struct aligned 
	{
		char storage[N];
	};

	// returns the offset added to a Derived* when cast to a Base*
	// TODO: return ptrdiff
	template<class Derived, class Base>
	int ptr_offset(type_<Derived>, type_<Base>)
	{
		aligned<sizeof(Derived)> obj;
		Derived* ptr = reinterpret_cast<Derived*>(&obj);

		return int(static_cast<char*>(static_cast<void*>(static_cast<Base*>(ptr)))
		- static_cast<char*>(static_cast<void*>(ptr)));
	}

}}

#endif // LUABIND_PRIMITIVES_HPP_INCLUDED
