// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This test expands on test_attributes.cpp, testing the new property
// implementation features.

#include "test.hpp"
#include <luabind/luabind.hpp>

struct Base
{};

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        class_<Base>("Base")
            .def(constructor<>())
    ];

    DOSTRING(L,
        "class 'Readonly' (Base)\n"
        "  function Readonly:__init(x)\n"
        "    Base.__init(self)\n"
        "    self._x = x\n"
        "  end\n"

        "  function Readonly:getX()\n"
        "    return self._x\n"
        "  end\n"

        "  Readonly.x = property(Readonly.getX)\n"
    );

    DOSTRING(L,
        "class 'Readwrite' (Readonly)\n"
        "  function Readwrite:__init(x)\n"
        "    Readonly.__init(self, x)\n"
        "  end\n"

        "  function Readwrite:setX(x)\n"
        "    self._x = x\n"
        "  end\n"

        "  Readwrite.x = property(Readonly.getX, Readwrite.setX)\n"
    );

    DOSTRING(L,
        "r = Readonly(1)\n"
        "assert(r.x == 1)\n"
    );

    DOSTRING_EXPECTED(L,
        "r = Readonly(1)\n"
        "r.x = 2\n"
      , "property 'x' is read only"
    );

    DOSTRING(L,
        "r = Readwrite(2)\n"
        "assert(r.x == 2)\n"
        "r.x = 3\n"
        "assert(r.x == 3)\n"
        "assert(r._x == 3)\n"
    );

    DOSTRING(L,
        "r = Readonly(1)\n"
        "r.y = 2\n"
        "assert(r.y == 2)\n"
    );
}
