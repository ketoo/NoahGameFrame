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

#ifndef LUABIND_DEBUG_HPP_INCLUDED
#define LUABIND_DEBUG_HPP_INCLUDED

#ifndef NDEBUG

#include <luabind/lua_include.hpp>
#include <cassert>

namespace luabind { namespace detail
{
	struct stack_checker_type
	{
		stack_checker_type(lua_State* L)
			: m_L(L)
			, m_stack(lua_gettop(m_L))
		{}

		~stack_checker_type()
		{
			assert(m_stack == lua_gettop(m_L));
		}

		lua_State* m_L;
		int m_stack;
	};

}}
#define LUABIND_CHECK_STACK(L) luabind::detail::stack_checker_type stack_checker_object(L)
#else
#define LUABIND_CHECK_STACK(L) do {} while (0)
#endif

#endif // LUABIND_DEBUG_HPP_INCLUDED
