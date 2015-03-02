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

#ifndef LUABIND_WRAPPER_BASE_HPP_INCLUDED
#define LUABIND_WRAPPER_BASE_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/weak_ref.hpp>
#include <luabind/detail/ref.hpp>
#include <luabind/detail/call_member.hpp>

#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#include <stdexcept>

namespace luabind
{
	namespace detail
	{
		struct wrap_access;

		// implements the selection between dynamic dispatch
		// or default implementation calls from within a virtual
		// function wrapper. The input is the self reference on
		// the top of the stack. Output is the function to call
		// on the top of the stack (the input self reference will
		// be popped)
		LUABIND_API void do_call_member_selection(lua_State* L, char const* name);
	}

	struct wrapped_self_t: weak_ref
	{
		detail::lua_reference m_strong_ref;
	};

	struct wrap_base
	{
		friend struct detail::wrap_access;
		wrap_base() {}

    #define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, <luabind/wrapper_base.hpp>, 1))
	#include BOOST_PP_ITERATE()

	private:
		wrapped_self_t m_self;
	};

#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, <luabind/wrapper_base.hpp>, 2))
#include BOOST_PP_ITERATE()

	namespace detail
	{
		struct wrap_access
		{
			static wrapped_self_t const& ref(wrap_base const& b)
			{
				return b.m_self;
			}

			static wrapped_self_t& ref(wrap_base& b)
			{
				return b.m_self;
			}
		};
	}
}

#endif // LUABIND_WRAPPER_BASE_HPP_INCLUDED

#elif BOOST_PP_ITERATION_FLAGS() == 1

#define LUABIND_TUPLE_PARAMS(z, n, data) const A##n *
#define LUABIND_OPERATOR_PARAMS(z, n, data) const A##n & a##n

	template<class R BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
		typename boost::mpl::if_<boost::is_void<R>
				, luabind::detail::proxy_member_void_caller<boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> >
				, luabind::detail::proxy_member_caller<R, boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> > >::type
				call(char const* name BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_OPERATOR_PARAMS, _), detail::type_<R>* = 0) const
		{
			typedef boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> tuple_t;
	#if BOOST_PP_ITERATION() == 0
			tuple_t args;
	#else
			tuple_t args(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), &a));
	#endif

			typedef typename boost::mpl::if_<boost::is_void<R>
				, luabind::detail::proxy_member_void_caller<boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> >
				, luabind::detail::proxy_member_caller<R, boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> > >::type proxy_type;

			// this will be cleaned up by the proxy object
			// once the call has been made

			// TODO: what happens if this virtual function is
			// dispatched from a lua thread where the state
			// pointer is different?

			// get the function
			lua_State* L = m_self.state();
			m_self.get(L);
			assert(!lua_isnil(L, -1));
			detail::do_call_member_selection(L, name);

			if (lua_isnil(L, -1))
			{
				lua_pop(L, 1);
				throw std::runtime_error("Attempt to call nonexistent function");
			}

			// push the self reference as the first parameter
			m_self.get(L);

			// now the function and self objects
			// are on the stack. These will both
			// be popped by pcall
			return proxy_type(L, args);
		}

#undef LUABIND_CALL_MEMBER_NAME
#undef LUABIND_OPERATOR_PARAMS
#undef LUABIND_TUPLE_PARAMS

#else // free call_member forwardarding functions

#define N BOOST_PP_ITERATION()

#define LUABIND_TUPLE_PARAMS(z, n, data) const A##n *
#define LUABIND_OPERATOR_PARAMS(z, n, data) const A##n & a##n

    template<
        class R 
        BOOST_PP_ENUM_TRAILING_PARAMS(N, class A)
    >
    typename boost::mpl::if_<
        boost::is_void<R>
      , detail::proxy_member_void_caller<
            boost::tuples::tuple<
                BOOST_PP_ENUM(N, LUABIND_TUPLE_PARAMS, _)
            >
        >
      , detail::proxy_member_caller<
            R
          , boost::tuples::tuple<
                BOOST_PP_ENUM(N, LUABIND_TUPLE_PARAMS, _)
            >
        >
    >::type
    call_member(
        wrap_base const* self
      , char const* fn
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, A, &a)
      , detail::type_<R>* = 0
    )
    {
        return self->call(
            fn
            BOOST_PP_ENUM_TRAILING_PARAMS(N, a)
         , (detail::type_<R>*)0
        );
    }

#undef LUABIND_OPERATOR_PARAMS
#undef LUABIND_TUPLE_PARAMS

#undef N

#endif
