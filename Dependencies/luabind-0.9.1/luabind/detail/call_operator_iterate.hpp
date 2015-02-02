// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

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

#define N BOOST_PP_ITERATION()

#define LUABIND_UNWRAP_PARAMETER(z, n, _) \
    typename detail::unwrap_parameter_type<T, BOOST_PP_CAT(A, n)>::type \
        BOOST_PP_CAT(_, n)

template<class Self BOOST_PP_ENUM_TRAILING_PARAMS(N, class A)>
struct BOOST_PP_CAT(call_operator, N) 
    : detail::operator_<
          BOOST_PP_CAT(call_operator, N)<
              Self BOOST_PP_ENUM_TRAILING_PARAMS(N, A)
          >
      >
{
    BOOST_PP_CAT(call_operator, N)(int) {}

    template<class T, class Policies>
    struct apply
    {
        static void execute(
            lua_State* L
          , typename detail::unwrap_parameter_type<T, Self>::type self
            BOOST_PP_ENUM_TRAILING(N, LUABIND_UNWRAP_PARAMETER, _)
        )
        {
            using namespace detail;
            operator_result(
                L
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
              , self(BOOST_PP_ENUM_PARAMS(N, _))
#else
              , (self(BOOST_PP_ENUM_PARAMS(N, _)), detail::operator_void_return())
#endif
              , (Policies*)0
            );
        } 
    };

    static char const* name() { return "__call"; }
};

#undef LUABIND_UNWRAP_PARAMETER
#undef N

