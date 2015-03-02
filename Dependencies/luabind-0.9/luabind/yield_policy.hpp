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


#ifndef LUABIND_YIELD_POLICY_HPP_INCLUDED
#define LUABIND_YIELD_POLICY_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>

namespace luabind { namespace detail 
{
	struct yield_policy
	{
		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}
	};

	template<class T>
	struct has_yield
	{
		BOOST_STATIC_CONSTANT(bool,
			value = (boost::is_same<yield_policy, typename T::head>::value ||
					  has_yield<typename T::tail>::value));
	};

	template<>
	struct has_yield<null_type>
	{
		BOOST_STATIC_CONSTANT(bool, value = false);
	};
}}

namespace luabind
{
  detail::policy_cons<detail::yield_policy, detail::null_type> const yield = {};

  namespace detail
  {
    inline void ignore_unused_yield()
    {
        (void)yield;
    }
  }
}

#endif // LUABIND_YIELD_POLICY_HPP_INCLUDED

