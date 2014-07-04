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

#ifndef IS_INDIRECT_CONST_040211_HPP
#define IS_INDIRECT_CONST_040211_HPP

#include <luabind/detail/yes_no.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/bool.hpp>

namespace luabind {

    namespace detail {

    template<class T>
    typename boost::is_const<T>::type
    is_indirect_const_check(T(*)(), int);

    template<class T>
    typename boost::is_const<T>::type 
    is_indirect_const_check(T*(*)(), long);

    template<class T>
    typename boost::is_const<T>::type 
    is_indirect_const_check(T&(*)(), long);
    
    yes_t to_yes_no(boost::mpl::true_);
    no_t to_yes_no(boost::mpl::false_);

    } // namespace detail

    // returns true for:
    //    T = U*  is_const<U>
    //    T = U&  is_const<U>
    //    T = U   is_const<U>
    template<class T>
    struct is_indirect_const
    {
        BOOST_STATIC_CONSTANT(int, value = (
            sizeof(
                detail::to_yes_no(
                    detail::is_indirect_const_check((T(*)())0, 0L)
            ))
         == sizeof(detail::yes_t)
        ));
    };

} // namespace luabind

#endif // IS_INDIRECT_CONST_040211_HPP

