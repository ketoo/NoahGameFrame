// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

using namespace luabind;

struct X
{
    void f()
    {}
};

struct Y : X
{
    void f()
    {}
};

void test_main(lua_State* L)
{
    module(L) [
        class_<X>("X")
          .def(constructor<>())
          .def("f", &X::f),

        class_<Y, X>("Y")
          .def(constructor<>())
          .def("f", &Y::f)
    ];

    DOSTRING(L,
        "x = X()\n"
        "x:f()\n"
    );

    DOSTRING(L,
        "x = Y()\n"
        "x:f()\n"
    );
}
