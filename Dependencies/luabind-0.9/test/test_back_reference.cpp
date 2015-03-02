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

#include "test.hpp"
#include <luabind/luabind.hpp>
#include <boost/shared_ptr.hpp>

using namespace luabind;

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace luabind
{
  using boost::get_pointer;
}
#endif

struct base0
{
    virtual ~base0() {}
};

struct base_wrap0 : base0, wrap_base
{};

struct base1
{
    virtual ~base1() {}
};

struct base_wrap1 : base1, wrap_base
{};

base0* filter0(base0* p)
{
    return p;
}

boost::shared_ptr<base1> filter1(boost::shared_ptr<base1> const& p)
{
    return p;
}

void test_main(lua_State* L)
{
    module(L)
    [
        class_<base0, base_wrap0>("base0")
          .def(constructor<>()),
        def("filter0", &filter0),

        class_<base1, base_wrap1, boost::shared_ptr<base1> >("base1")
          .def(constructor<>()),
        def("filter1", &filter1)
    ];

    DOSTRING(L,
        "class 'derived0' (base0)\n"
        "  function derived0:__init()\n"
        "    base0.__init(self)\n"
        "  end\n"

        "class 'derived1' (base1)\n"
        "  function derived1:__init()\n"
        "    base1.__init(self)\n"
        "  end\n"
    );

    DOSTRING(L,
        "x = derived0()\n"
        "y = filter0(x)\n"
        "assert(x == y)\n"
    );

    DOSTRING(L,
        "x = derived1()\n"
        "y = filter1(x)\n"
        "assert(x == y)\n"
    );
}

