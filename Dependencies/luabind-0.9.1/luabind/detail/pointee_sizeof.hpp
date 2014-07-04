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

#ifndef POINTEE_SIZEOF_040211_HPP
#define POINTEE_SIZEOF_040211_HPP

#include <boost/mpl/int.hpp>

namespace luabind {

    namespace detail {

    template<class T> T& deref_type(T(*)(), int);
    template<class T> T& deref_type(T*(*)(), long);

    } // namespace detail

    // returns the indirect sizeof U, as in
    //    sizeof(T*) = sizeof(T)
    //    sizeof(T&) = sizeof(T)
    //    sizeof(T)  = sizeof(T)
    template<class T>
    struct pointee_sizeof
    {
        BOOST_STATIC_CONSTANT(int, value = (
            sizeof(detail::deref_type((T(*)())0), 0L)
        ));

        typedef boost::mpl::int_<value> type;
    };

} // namespace luabind

#endif // POINTEE_SIZEOF_040211_HPP

