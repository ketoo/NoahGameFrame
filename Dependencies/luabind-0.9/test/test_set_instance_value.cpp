// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

void test_main(lua_State* L)
{
    DOSTRING(L,
        "class 'X'\n"
        "  function X:__init()\n"
        "    self.value = 1\n"
        "    self.second = 2\n"
        "  end\n"
        "\n"
        "x = X()\n"
        "assert(x.value == 1)\n"
        "assert(x.second == 2)\n"
        "x.value = 2\n"
        "assert(x.value == 2)\n"
        "assert(x.second == 2)\n"
        "x.second = 3\n"
        "y = X()\n"
        "assert(y.value == 1)\n"
        "assert(y.second == 2)\n"
        "assert(x.value == 2)\n"
        "assert(x.second == 3)\n"
    );
}
