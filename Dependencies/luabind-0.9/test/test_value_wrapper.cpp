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

#include <luabind/value_wrapper.hpp>
#include <luabind/object.hpp>
#include <boost/mpl/assert.hpp>

struct X_tag;

struct X
{
    typedef X_tag value_wrapper_tag;
};

namespace luabind
{
#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
  template<>
  struct value_wrapper_traits<X_tag>
  {
      typedef boost::mpl::true_ is_specialized;
  };
#else
  // used on compilers supporting partial template specialization
  template<>
  struct value_wrapper_traits<X>
  {
      typedef boost::mpl::true_ is_specialized;
  };
#endif

} // namespace luabind

BOOST_MPL_ASSERT(( luabind::is_value_wrapper<X> ));
BOOST_MPL_ASSERT_NOT(( luabind::is_value_wrapper<X&> ));
BOOST_MPL_ASSERT_NOT(( luabind::is_value_wrapper<X const&> ));

BOOST_MPL_ASSERT(( luabind::is_value_wrapper_arg<X> ));
BOOST_MPL_ASSERT(( luabind::is_value_wrapper_arg<X const> ));
BOOST_MPL_ASSERT(( luabind::is_value_wrapper_arg<X&> ));
BOOST_MPL_ASSERT(( luabind::is_value_wrapper_arg<X const&> ));
BOOST_MPL_ASSERT_NOT(( luabind::is_value_wrapper_arg<int> ));
BOOST_MPL_ASSERT_NOT(( luabind::is_value_wrapper_arg<int[4]> ));

BOOST_MPL_ASSERT(( luabind::is_value_wrapper_arg<X const&> ));
BOOST_MPL_ASSERT(( luabind::is_value_wrapper_arg<luabind::object&> ));
BOOST_MPL_ASSERT(( luabind::is_value_wrapper_arg<luabind::object const&> ));

int main()
{
}

