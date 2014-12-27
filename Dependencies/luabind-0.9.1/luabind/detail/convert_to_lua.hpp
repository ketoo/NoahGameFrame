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


#ifndef LUABIND_CONVERT_TO_LUA_HPP_INCLUDED
#define LUABIND_CONVERT_TO_LUA_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <boost/ref.hpp>

#include <boost/mpl/apply_wrap.hpp>

namespace luabind { namespace detail
{
	template<bool IsReferenceWrapper = false>
	struct unwrap_ref
	{
		template<class T>
		static const T& get(const T& r) { return r; }

		template<class T>
		struct apply
		{
			typedef T type;
		};
	};

	template<>
	struct unwrap_ref<true>
	{
		template<class T>
		static T& get(const boost::reference_wrapper<T>& r) { return r.get(); }

		template<class T>
		struct apply
		{
			typedef typename T::type& type;
		};
	};

	namespace mpl = boost::mpl;
	
	template<class T>
	void convert_to_lua(lua_State* L, const T& v)
	{
		typedef typename mpl::apply_wrap1<
			unwrap_ref<boost::is_reference_wrapper<T>::value>
		  , T
		>::type value_type;

		typename mpl::apply_wrap2<default_policy,value_type,cpp_to_lua>::type converter;

		converter.apply(L, unwrap_ref<boost::is_reference_wrapper<T>::value>::get(v));
	}

	template<int Index, class T, class Policies>
	void convert_to_lua_p(lua_State* L, const T& v, const Policies&)
	{
		typedef typename mpl::apply_wrap1<
			unwrap_ref<boost::is_reference_wrapper<T>::value>
		  , T
		>::type value_type;

		typedef typename find_conversion_policy<Index, Policies>::type converter_policy;
		typename mpl::apply_wrap2<converter_policy,value_type,cpp_to_lua>::type converter;

		converter.apply(L, unwrap_ref<boost::is_reference_wrapper<T>::value>::get(v));
	}
}}

#endif

