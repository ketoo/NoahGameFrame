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


#ifndef LUABIND_RAW_POLICY_HPP_INCLUDED
#define LUABIND_RAW_POLICY_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>

namespace luabind { namespace detail  {

	struct raw_converter
	{
        int const consumed_args(...)
        {
            return 0;
        }

        lua_State* apply(lua_State* L, by_pointer<lua_State>, int)
		{
			return L;
		}

		static int match(...)
		{
			return 0;
		}

		void converter_postcall(lua_State*, by_pointer<lua_State>, int) {}
	};

	template<int N>
	struct raw_policy : conversion_policy<N, false>
	{
		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}

		template<class T, class Direction>
		struct apply
		{
			typedef raw_converter type;
		};
	};

}} // namespace luabind::detail

namespace luabind {

	template<int N>
	detail::policy_cons<
		detail::raw_policy<N>
	  , detail::null_type
	>
	inline raw(LUABIND_PLACEHOLDER_ARG(N))
	{ 
		return detail::policy_cons<
			detail::raw_policy<N>
		  , detail::null_type
		>(); 
	}

} // namespace luabind

#endif // LUABIND_RAW_POLICY_HPP_INCLUDED

