// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

void test_main(lua_State* L)
{
    DOSTRING(L, "x = 4294967295");

    unsigned int x = luabind::object_cast<unsigned int>(
        luabind::globals(L)["x"]);

    unsigned int y = 4294967295UL;

    TEST_CHECK(x == y);
}
