// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>
#include <luabind/tag_function.hpp>
#include <boost/bind.hpp>

int f(int x, int y)
{
    return x + y;
}

struct X
{
    int f(int x, int y)
    {
        return x + y;
    }
};

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        def("f", tag_function<int(int)>(boost::bind(&f, 5, _1))),

        class_<X>("X")
            .def(constructor<>())
            .def("f", tag_function<int(X&, int)>(boost::bind(&X::f, _1, 10, _2)))
    ];

    DOSTRING(L,
        "assert(f(0) == 5)\n"
        "assert(f(1) == 6)\n"
        "assert(f(5) == 10)\n"
    );

    DOSTRING(L,
        "x = X()\n"
        "assert(x:f(0) == 10)\n"
        "assert(x:f(1) == 11)\n"
        "assert(x:f(5) == 15)\n"
    );
}
