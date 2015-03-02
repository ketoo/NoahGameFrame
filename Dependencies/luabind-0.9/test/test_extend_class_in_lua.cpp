// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

struct CppClass
{
    int f(int x)
    {
        return x;
    }
};

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        class_<CppClass>("CppClass")
          .def(constructor<>())
          .def("f", &CppClass::f)
    ];

    DOSTRING(L,
        "function CppClass:f_in_lua(x)\n"
        "    return self:f(x) * 2\n"
        "end\n"
    );

    DOSTRING(L,
        "x = CppClass()\n"
        "assert(x:f(1) == 1)\n"
        "assert(x:f_in_lua(1) == 2)\n"
    );
}
