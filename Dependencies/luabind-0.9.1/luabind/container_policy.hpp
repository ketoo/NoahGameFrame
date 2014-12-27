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


#ifndef LUABIND_CONTAINER_POLICY_HPP_INCLUDED
#define LUABIND_CONTAINER_POLICY_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <boost/mpl/apply_wrap.hpp>

namespace luabind { namespace detail {

	namespace mpl = boost::mpl;

	template<class Policies>
	struct container_converter_lua_to_cpp
	{
        int const consumed_args(...)
        {
            return 1;
        }

        template<class T>
		T apply(lua_State* L, by_const_reference<T>, int index)
		{
			typedef typename T::value_type value_type;

			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,value_type,lua_to_cpp>::type converter;

			T container;

			lua_pushnil(L);
			while (lua_next(L, index))
			{
				container.push_back(converter.apply(L, LUABIND_DECORATE_TYPE(value_type), -1));
				lua_pop(L, 1); // pop value
			}

			return container;
		}

		template<class T>
		T apply(lua_State* L, by_value<T>, int index)
		{
			return apply(L, by_const_reference<T>(), index);
		}

		template<class T>
		static int match(lua_State* L, by_const_reference<T>, int index)
		{
			if (lua_istable(L, index)) return 0; else return -1;
		}

		template<class T>
		void converter_postcall(lua_State*, T, int) {}
	};

	template<class Policies>
	struct container_converter_cpp_to_lua
	{
		template<class T>
		void apply(lua_State* L, const T& container)
		{
			typedef typename T::value_type value_type;

			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,value_type,lua_to_cpp>::type converter;

			lua_newtable(L);

			int index = 1;

			for (typename T::const_iterator i = container.begin(); i != container.end(); ++i)
			{
				converter.apply(L, *i);
				lua_rawseti(L, -2, index);
				++index;
			}
		}
	};

	template<int N, class Policies>
//	struct container_policy : converter_policy_tag
	struct container_policy : conversion_policy<N>
	{
//		BOOST_STATIC_CONSTANT(int, index = N);

		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}

		struct only_accepts_nonconst_pointers {};

		template<class T, class Direction>
		struct apply
		{
			typedef typename boost::mpl::if_<boost::is_same<lua_to_cpp, Direction>
				, container_converter_lua_to_cpp<Policies>
				, container_converter_cpp_to_lua<Policies>
			>::type type;
		};
	};

}}

namespace luabind
{
	template<int N>
	detail::policy_cons<detail::container_policy<N, detail::null_type>, detail::null_type> 
	container(LUABIND_PLACEHOLDER_ARG(N)) 
	{ 
		return detail::policy_cons<detail::container_policy<N, detail::null_type>, detail::null_type>(); 
	}

	template<int N, class Policies>
	detail::policy_cons<detail::container_policy<N, Policies>, detail::null_type> 
	container(LUABIND_PLACEHOLDER_ARG(N), const Policies&) 
	{ 
		return detail::policy_cons<detail::container_policy<N, Policies>, detail::null_type>(); 
	}
}

#endif // LUABIND_CONTAINER_POLICY_HPP_INCLUDED
