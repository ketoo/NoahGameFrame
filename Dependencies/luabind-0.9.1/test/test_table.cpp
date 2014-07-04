// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

void f(luabind::table<> const& x)
{
    TEST_CHECK(luabind::type(x) == LUA_TTABLE);
}

void g(luabind::table<luabind::argument> const& x)
{
    TEST_CHECK(luabind::type(x) == LUA_TTABLE);
}

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        def("f", &f),
        def("g", &g)
    ];

    DOSTRING(L,
        "f {1,2,3}\n"
        "g {1,2,3}\n"
    );

    DOSTRING_EXPECTED(L,
        "f(1)\n",
        "No matching overload found, candidates:\n"
        "void f(table const&)"
    );

    DOSTRING_EXPECTED(L,
        "g(1)\n",
        "No matching overload found, candidates:\n"
        "void g(table const&)"
    );
}
