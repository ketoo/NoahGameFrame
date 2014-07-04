// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

void raw_function(lua_State* L)
{}

void raw_function2(int, lua_State* L, int)
{}

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        def("raw_function", &raw_function),
        def("raw_function2", &raw_function2)
    ];

    DOSTRING(L,
        "raw_function()\n"
        "raw_function2(1,2)\n"
    );
}
