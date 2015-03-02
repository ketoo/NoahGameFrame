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

#if !BOOST_PP_IS_ITERATING

# include <luabind/detail/signature_match.hpp>

#ifndef LUABIND_CALC_ARITY_HPP_INCLUDED
#define LUABIND_CALC_ARITY_HPP_INCLUDED

#define LUABIND_FIND_CONV(z,n,text) typedef typename find_conversion_policy<n + 1, Policies>::type p##n;
#define LUABIND_CALC_ARITY(z,n,text) + BOOST_PP_CAT(p,n)::has_arg

namespace luabind { namespace detail
{
	template<int N> struct calc_arity;

	#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, <luabind/detail/calc_arity.hpp>, 1))
	#include BOOST_PP_ITERATE()
}}

#undef LUABIND_CALC_ARITY
#undef LUABIND_FIND_CONV


#endif // LUABIND_CALC_ARITY_HPP_INCLUDED

#else // BOOST_PP_ITERATE

	template<>
	struct calc_arity<BOOST_PP_ITERATION()>
	{
		template<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A), class Policies>
		static int apply(constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)>, Policies*)
		{
			BOOST_PP_REPEAT(BOOST_PP_ITERATION(), LUABIND_FIND_CONV, _)
			return 0 BOOST_PP_REPEAT(BOOST_PP_ITERATION(), LUABIND_CALC_ARITY, _);
		}
	};

#endif

