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

#ifndef LUABIND_VALUE_WRAPPER_050419_HPP
#define LUABIND_VALUE_WRAPPER_050419_HPP

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/msvc_eti_base.hpp>

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
# define LUABIND_USE_VALUE_WRAPPER_TAG 
#else
#endif

#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
# include <boost/mpl/identity.hpp>
# include <boost/mpl/eval_if.hpp>
# include <boost/mpl/has_xxx.hpp>
# include <boost/mpl/not.hpp>
# include <boost/mpl/and.hpp>
# include <boost/mpl/or.hpp>
# include <boost/type_traits/is_reference.hpp>
# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/is_array.hpp>
#endif

namespace luabind {

//
// Concept ``ValueWrapper``
//

#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
template<class T>
struct value_wrapper_traits;
    
namespace detail 
{ 

  BOOST_MPL_HAS_XXX_TRAIT_DEF(value_wrapper_tag);

  struct unspecialized_value_wrapper_traits
  {
      typedef boost::mpl::false_ is_specialized;
  };

  template<class T>
  struct value_wrapper_traits_aux
  {
      typedef value_wrapper_traits<typename T::value_wrapper_tag> type;
  };

} // namespace detail
#endif

template<class T>
struct value_wrapper_traits
#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
  : boost::mpl::eval_if<
        boost::mpl::and_<
            boost::mpl::not_<
                boost::mpl::or_<
                    boost::is_reference<T>
                  , boost::is_pointer<T>
                  , boost::is_array<T>
                >
            >
          , detail::has_value_wrapper_tag<T>
        >
      , detail::value_wrapper_traits_aux<T>
      , boost::mpl::identity<detail::unspecialized_value_wrapper_traits>
    >::type
{};
#else
{
    typedef boost::mpl::false_ is_specialized;
};
#endif

template<class T>
struct is_value_wrapper
  : boost::mpl::aux::msvc_eti_base<
        typename value_wrapper_traits<T>::is_specialized
    >::type
{};

} // namespace luabind

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

# include <boost/type_traits/remove_const.hpp>
# include <boost/type_traits/remove_reference.hpp>

namespace luabind {

template<class T>
struct is_value_wrapper_arg
  : is_value_wrapper<
      typename boost::remove_const<
          typename boost::remove_reference<T>::type
      >::type
    >
{};

} // namespace luabind

#else

# include <luabind/detail/yes_no.hpp>
# include <boost/type_traits/add_reference.hpp>

namespace luabind {

namespace detail
{
  template<class T>
  typename is_value_wrapper<T>::type is_value_wrapper_arg_check(T const*);
  
  yes_t to_yesno(boost::mpl::true_);
  no_t to_yesno(boost::mpl::false_);

  template<class T>
  struct is_value_wrapper_arg_aux
  {
      static typename boost::add_reference<T>::type x;

      BOOST_STATIC_CONSTANT(bool, value = 
          sizeof(to_yesno(is_value_wrapper_arg_check(&x)))
            == sizeof(yes_t)
      );

      typedef boost::mpl::bool_<value> type;
  };

} // namespace detail

template<class T>
struct is_value_wrapper_arg
  : detail::is_value_wrapper_arg_aux<T>::type
{
};

} // namespace luabind

#endif

#endif // LUABIND_VALUE_WRAPPER_050419_HPP

