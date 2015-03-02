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

#ifndef LUABIND_SIGNATURE_MATCH_HPP_INCLUDED
#define LUABIND_SIGNATURE_MATCH_HPP_INCLUDED

#include <luabind/config.hpp>

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/remove.hpp>

namespace luabind
{

	namespace adl
	{
		class argument;
	}

	template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(LUABIND_MAX_ARITY, class A, detail::null_type)>
	struct constructor
	{
		typedef BOOST_PP_CAT(
			boost::mpl::vector, BOOST_PP_INC(BOOST_PP_INC(LUABIND_MAX_ARITY)))<
				void, argument const&, BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)
		> signature0;

		typedef typename boost::mpl::remove<
			signature0, detail::null_type>::type signature;
	};

}

#endif // LUABIND_SIGNATURE_MATCH_HPP_INCLUDED

