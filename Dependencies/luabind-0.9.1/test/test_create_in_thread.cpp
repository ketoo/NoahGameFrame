// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

int count = 0;

struct X
{
    X()
    {
        ++count;
    }

    ~X()
    {
        --count;
    }
};

struct Y
{
    virtual ~Y()
    {}
};

struct Y_wrap : Y, luabind::wrap_base
{};

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        class_<X>("X")
            .def(constructor<>()),

        class_<Y, Y_wrap>("Y")
            .def(constructor<>())
    ];

    DOSTRING(L,
        "class 'Y_lua' (Y)\n"
        "  function Y_lua.__init(self)\n"
        "      Y.__init(self)\n"
        "  end\n"
    );

    for (int i = 0; i < 100; ++i)
    {
        lua_State* thread = lua_newthread(L);
        int ref = luaL_ref(L, LUA_REGISTRYINDEX);

        DOSTRING(thread,
            "local x = X()\n"
        );

        DOSTRING(thread,
            "local y = Y_lua()\n"
        );

        luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }
}
