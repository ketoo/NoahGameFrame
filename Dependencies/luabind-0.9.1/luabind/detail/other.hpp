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


#ifndef LUABIND_OTHER_HPP_INCLUDED
#define LUABIND_OTHER_HPP_INCLUDED

// header derived from source code found in Boost.Python

// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <luabind/config.hpp>
#include <boost/config.hpp>

namespace luabind
{
	template<class T>
	struct other
	{
		typedef T type;
	};
}

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace luabind { namespace detail
{
	template<typename T>
	class unwrap_other
	{
	public:
			typedef T type;
	};

	template<typename T>
	class unwrap_other<other<T> >
	{
	public:
			typedef T type;
	};
}} // namespace luabind::detail

# else // no partial specialization

#include <boost/type.hpp>

namespace luabind { namespace detail
{
	typedef char (&yes_other_t)[1];
	typedef char (&no_other_t)[2];

	no_other_t is_other_test(...);

	template<typename T>
	yes_other_t is_other_test(type_< other<T> >);

	template<bool wrapped>
	struct other_unwrapper
	{
			template <class T>
			struct apply
			{
					typedef T type;
			};
	};

	template<>
	struct other_unwrapper<true>
	{
			template <class T>
			struct apply
			{
					typedef typename T::type type;
			};
	};

	template<typename T>
	class is_other
	{
	 public:
			BOOST_STATIC_CONSTANT(
					bool, value = (
							sizeof(detail::is_other_test(type_<T>()))
							== sizeof(detail::yes_other_t)));
	};

	template <typename T>
	class unwrap_other
			: public detail::other_unwrapper<
			is_other<T>::value
	>::template apply<T>
	{};

}} // namespace luabind::detail
#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // LUABIND_OTHER_HPP_INCLUDED
