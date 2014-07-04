// Copyright (c) 2005 Daniel Wallin

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

#ifndef LUABIND_HAS_GET_POINTER_051022_HPP
# define LUABIND_HAS_GET_POINTER_051022_HPP

# include <boost/type_traits/add_reference.hpp>

# ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#  include <memory>
# endif

namespace luabind { namespace detail { 

namespace has_get_pointer_
{

  struct any 
  { 
      template<class T> any(T const&);
  };

  struct no_overload_tag 
  {};

  typedef char (&yes)[1];
  typedef char (&no)[2];

  no_overload_tag operator,(no_overload_tag, int);

//
// On compilers with ADL, we need these generic overloads in this
// namespace as well as in luabind::. Otherwise get_pointer(any)
// will be found before them.
//
# ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

  template<class T>
  T* get_pointer(T const volatile*);

  template<class T>
  T* get_pointer(std::auto_ptr<T> const&);

# endif

//
// On compilers that doesn't support ADL, the overload below has to
// live in luabind::.
//
# ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
}} // namespace detail::has_get_pointer_
# endif

detail::has_get_pointer_::no_overload_tag 
  get_pointer(detail::has_get_pointer_::any);

# ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace detail { namespace has_get_pointer_ 
{
# endif

  template<class T>
  yes check(T const&);
  no check(no_overload_tag);

  template<class T>
  struct impl
  {
      static typename boost::add_reference<T>::type x;

      BOOST_STATIC_CONSTANT(bool,
          value = sizeof(has_get_pointer_::check( (get_pointer(x),0) )) == 1
      );

      typedef boost::mpl::bool_<value> type;
  };

} // namespace has_get_pointer_

template<class T>
struct has_get_pointer
  : has_get_pointer_::impl<T>::type
{};

}} // namespace luabind::detail

#endif // LUABIND_HAS_GET_POINTER_051022_HPP

