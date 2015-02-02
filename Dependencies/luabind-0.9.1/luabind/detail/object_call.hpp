// Copyright (c) 2005 Daniel Wallin and Arvid Norberg

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
# error Do not include object_call.hpp directly!
#endif

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#define N BOOST_PP_ITERATION()

template<BOOST_PP_ENUM_PARAMS(N, class A)>
call_proxy<
    Derived
  , boost::tuples::tuple<
        BOOST_PP_ENUM_BINARY_PARAMS(N, A, const* BOOST_PP_INTERCEPT)
    >
> operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a))
{
    typedef boost::tuples::tuple<
        BOOST_PP_ENUM_BINARY_PARAMS(N, A, const* BOOST_PP_INTERCEPT)
    > arguments;

    return call_proxy<Derived, arguments>(
        derived()
      , arguments(BOOST_PP_ENUM_PARAMS(N, &a))
    );
}

#undef N

