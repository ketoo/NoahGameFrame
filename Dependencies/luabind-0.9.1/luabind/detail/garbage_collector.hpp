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


#ifndef LUABIND_GARBAGE_COLLECTOR_HPP_INCLUDED
#define LUABIND_GARBAGE_COLLECTOR_HPP_INCLUDED

#include <luabind/config.hpp>

namespace luabind { namespace detail
{
	// function that is used as __gc metafunction on several objects
	template<class T>
	inline int garbage_collector(lua_State* L)
	{
		T* obj = static_cast<T*>(lua_touserdata(L, -1));
		obj->~T();
		return 0;
	}

	template<class T>
	struct garbage_collector_s
	{
		static int apply(lua_State* L)
		{
			T* obj = static_cast<T*>(lua_touserdata(L, -1));
			obj->~T();
			return 0;
		}
	};

}}

#endif // LUABIND_GARBAGE_COLLECTOR_HPP_INCLUDED
