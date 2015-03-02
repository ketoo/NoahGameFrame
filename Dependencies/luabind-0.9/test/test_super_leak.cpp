// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include "test.hpp"
#include <luabind/luabind.hpp>

struct Foo
{
    static std::size_t count;

    Foo()
    {
        ++count;
    }

    ~Foo()
    {
        --count;
    }
};

std::size_t Foo::count = 0;

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        class_<Foo>("Foo")
          .def(constructor<>())
    ];

    // This used to leak the last created instance in one of the
    // upvalues to the "super" function.

    disable_super_deprecation();

    DOSTRING(L,
        "class 'Bar' (Foo)\n"
        "function Bar.__init(self)\n"
        "  Foo.__init(self)\n"
        "end\n"
    );

    DOSTRING(L,
        "x = Bar()\n"
    );

    assert(Foo::count == 1);

    DOSTRING(L,
        "x = nil\n"
    );

    lua_gc(L, LUA_GCCOLLECT, 0);

    assert(Foo::count == 0);
}
