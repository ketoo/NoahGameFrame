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

#include <luabind/detail/has_get_pointer.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/get_pointer.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace lb = luabind::detail;

namespace test
{

  struct X
  {
  };

  struct Y
  {
  };

  Y* get_pointer(Y const&);
  
  struct Z : boost::enable_shared_from_this<Z> {};
  
} // namespace test

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace luabind
{
  using test::get_pointer;
  using boost::get_pointer;
} // namespace luabind
#endif

BOOST_MPL_ASSERT(( lb::has_get_pointer<boost::shared_ptr<int> > ));
BOOST_MPL_ASSERT(( lb::has_get_pointer<test::Y> ));
BOOST_MPL_ASSERT(( lb::has_get_pointer<char*> ));
BOOST_MPL_ASSERT_NOT(( lb::has_get_pointer<int> ));
BOOST_MPL_ASSERT_NOT(( lb::has_get_pointer<test::X> ));
BOOST_MPL_ASSERT(( lb::has_get_pointer<test::Z*> ));

