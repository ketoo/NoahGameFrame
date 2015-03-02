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


#ifndef LUABIND_DISCARD_RESULT_POLICY_HPP_INCLUDED
#define LUABIND_DISCARD_RESULT_POLICY_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>

namespace luabind { namespace detail 
{
	struct discard_converter
	{
		template<class T>
		void apply(lua_State*, T) {}
	};

	struct discard_result_policy : conversion_policy<0>
	{
		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}

		struct can_only_convert_from_cpp_to_lua {};

		template<class T, class Direction>
		struct apply
		{
			typedef typename boost::mpl::if_<boost::is_same<Direction, cpp_to_lua>
				, discard_converter
				, can_only_convert_from_cpp_to_lua
			>::type type;
		};
	};

}}

namespace luabind
{
  detail::policy_cons<
      detail::discard_result_policy, detail::null_type> const discard_result = {};

  namespace detail
  {
    inline void ignore_unused_discard_result()
    {
        (void)discard_result;
    }
  }
}

#endif // LUABIND_DISCARD_RESULT_POLICY_HPP_INCLUDED

