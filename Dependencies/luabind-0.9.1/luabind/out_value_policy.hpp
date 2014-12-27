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


#ifndef LUABIND_OUT_VALUE_POLICY_HPP_INCLUDED
#define LUABIND_OUT_VALUE_POLICY_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <boost/mpl/apply_wrap.hpp>

namespace luabind { namespace detail
{
	template<int N>
	struct char_array
	{
		char storage[N];
	};

#if defined(__GNUC__) && ( __GNUC__ == 3 && __GNUC_MINOR__ == 1 )

	template<class U>
	char_array<sizeof(U)> indirect_sizeof_test(by_reference<U>);

	template<class U>
	char_array<sizeof(U)> indirect_sizeof_test(by_const_reference<U>);

	template<class U>
	char_array<sizeof(U)> indirect_sizeof_test(by_pointer<U>);

	template<class U>
	char_array<sizeof(U)> indirect_sizeof_test(by_const_pointer<U>);

	template<class U>
	char_array<sizeof(U)> indirect_sizeof_test(by_value<U>);

#else

	template<class U>
	char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_reference<U>);

	template<class U>
	char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_const_reference<U>);

	template<class U>
	char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_pointer<U>);

	template<class U>
	char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_const_pointer<U>);

	template<class U>
	char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_value<U>);

#endif

	template<class T>
	struct indirect_sizeof
	{
		BOOST_STATIC_CONSTANT(int, value = sizeof(indirect_sizeof_test(LUABIND_DECORATE_TYPE(T))));
	};

	namespace mpl = boost::mpl;
	
	template<int Size, class Policies = detail::null_type>
	struct out_value_converter
	{
        int const consumed_args(...)
        {
            return 1;
        }

        template<class T>
		T& apply(lua_State* L, by_reference<T>, int index)
		{
			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
			new (m_storage) T(converter.apply(L, LUABIND_DECORATE_TYPE(T), index));
			return *reinterpret_cast<T*>(m_storage);
		}

		template<class T>
		static int match(lua_State* L, by_reference<T>, int index)
		{
			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typedef typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
			return converter::match(L, LUABIND_DECORATE_TYPE(T), index);
		}

		template<class T>
		void converter_postcall(lua_State* L, by_reference<T>, int) 
		{
			typedef typename find_conversion_policy<2, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
			converter.apply(L, *reinterpret_cast<T*>(m_storage));
			reinterpret_cast<T*>(m_storage)->~T();
		}

		template<class T>
		T* apply(lua_State* L, by_pointer<T>, int index)
		{
			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
			new (m_storage) T(converter.apply(L, LUABIND_DECORATE_TYPE(T), index));
			return reinterpret_cast<T*>(m_storage);
		}

		template<class T>
		static int match(lua_State* L, by_pointer<T>, int index)
		{
			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typedef typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
			return converter::match(L, LUABIND_DECORATE_TYPE(T), index);
		}

		template<class T>
		void converter_postcall(lua_State* L, by_pointer<T>, int)
		{
			typedef typename find_conversion_policy<2, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
			converter.apply(L, *reinterpret_cast<T*>(m_storage));
			reinterpret_cast<T*>(m_storage)->~T();
		}

		char m_storage[Size];
	};

	template<int N, class Policies = detail::null_type>
	struct out_value_policy : conversion_policy<N>
	{
		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}

		struct only_accepts_nonconst_references_or_pointers {};
		struct can_only_convert_from_lua_to_cpp {};

		template<class T, class Direction>
		struct apply
		{
			typedef typename boost::mpl::if_<boost::is_same<lua_to_cpp, Direction>
				, typename boost::mpl::if_<boost::mpl::or_<is_nonconst_reference<T>, is_nonconst_pointer<T> >
					, out_value_converter<indirect_sizeof<T>::value, Policies>
					, only_accepts_nonconst_references_or_pointers
					>::type
				, can_only_convert_from_lua_to_cpp
			>::type type;
		};
	};

	template<int Size, class Policies = detail::null_type>
	struct pure_out_value_converter
	{
        int const consumed_args(...)
        {
            return 0;
        }

        template<class T>
		T& apply(lua_State*, by_reference<T>, int)
		{
			new (m_storage) T();
			return *reinterpret_cast<T*>(m_storage);
		}

		template<class T>
		static int match(lua_State*, by_reference<T>, int)
		{
			return 0;
		}

		template<class T>
		void converter_postcall(lua_State* L, by_reference<T>, int) 
		{
			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
			converter.apply(L, *reinterpret_cast<T*>(m_storage));
			reinterpret_cast<T*>(m_storage)->~T();
		}

		template<class T>
		T* apply(lua_State*, by_pointer<T>, int)
		{
			new (m_storage) T();
			return reinterpret_cast<T*>(m_storage);
		}

		template<class T>
		static int match(lua_State*, by_pointer<T>, int)
		{
			return 0;
		}

		template<class T>
		void converter_postcall(lua_State* L, by_pointer<T>, int) 
		{
			typedef typename find_conversion_policy<1, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
			converter.apply(L, *reinterpret_cast<T*>(m_storage));
			reinterpret_cast<T*>(m_storage)->~T();
		}


		char m_storage[Size];
	};

	template<int N, class Policies = detail::null_type>
	struct pure_out_value_policy : conversion_policy<N, false>
	{
		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}

		struct only_accepts_nonconst_references_or_pointers {};
		struct can_only_convert_from_lua_to_cpp {};

		template<class T, class Direction>
		struct apply
		{
			typedef typename boost::mpl::if_<boost::is_same<lua_to_cpp, Direction>
				, typename boost::mpl::if_<boost::mpl::or_<is_nonconst_reference<T>, is_nonconst_pointer<T> >
					, pure_out_value_converter<indirect_sizeof<T>::value, Policies>
					, only_accepts_nonconst_references_or_pointers
					>::type
				, can_only_convert_from_lua_to_cpp
			>::type type;
		};
	};
	
}}

namespace luabind
{
	template<int N>
	detail::policy_cons<detail::out_value_policy<N>, detail::null_type> 
	out_value(LUABIND_PLACEHOLDER_ARG(N)) 
	{ 
		return detail::policy_cons<detail::out_value_policy<N>, detail::null_type>(); 
	}

	template<int N, class Policies>
	detail::policy_cons<detail::out_value_policy<N, Policies>, detail::null_type> 
	out_value(LUABIND_PLACEHOLDER_ARG(N), const Policies&) 
	{ 
		return detail::policy_cons<detail::out_value_policy<N, Policies>, detail::null_type>(); 
	}

	template<int N>
	detail::policy_cons<detail::pure_out_value_policy<N>, detail::null_type> 
	pure_out_value(LUABIND_PLACEHOLDER_ARG(N)) 
	{ 
		return detail::policy_cons<detail::pure_out_value_policy<N>, detail::null_type>(); 
	}

	template<int N, class Policies>
	detail::policy_cons<detail::pure_out_value_policy<N, Policies>, detail::null_type> 
	pure_out_value(LUABIND_PLACEHOLDER_ARG(N), const Policies&) 
	{ 
		return detail::policy_cons<detail::pure_out_value_policy<N, Policies>, detail::null_type>(); 
	}
}

#endif // LUABIND_OUT_VALUE_POLICY_HPP_INCLUDED

