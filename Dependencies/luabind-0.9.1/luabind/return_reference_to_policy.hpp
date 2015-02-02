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

#ifndef LUABIND_RETURN_REFERENCE_TO_POLICY_HPP_INCLUDED
#define LUABIND_RETURN_REFERENCE_TO_POLICY_HPP_INCLUDED

namespace luabind { namespace detail
{
	template<class T>
	struct return_reference_to_converter;

	template<>
	struct return_reference_to_converter<cpp_to_lua>
	{
		template<class T>
		void apply(lua_State* L, const T&)
		{
			lua_pushnil(L);
		}
	};

	template<int N>
	struct return_reference_to_policy : conversion_policy<0>
	{
		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State* L, const index_map& indices) 
		{
			int result_index = indices[0];
			int ref_to_index = indices[N];

			lua_pushvalue(L, ref_to_index);
			lua_replace(L, result_index);
		}

		template<class T, class Direction>
		struct apply
		{
			typedef return_reference_to_converter<Direction> type;
		};
	};
}}

namespace luabind
{
	template<int N>
	detail::policy_cons<detail::return_reference_to_policy<N>, detail::null_type> 
	return_reference_to(LUABIND_PLACEHOLDER_ARG(N)) 
	{ 
		return detail::policy_cons<detail::return_reference_to_policy<N>, detail::null_type>(); 
	}
}

#endif // LUABIND_RETURN_REFERENCE_TO_POLICY_HPP_INCLUDED

