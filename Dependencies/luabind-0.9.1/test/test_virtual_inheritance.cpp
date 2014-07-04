// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

// Test the following hierarchy:
//
//         X
//        / \.
//       Y   Z
//        \ /
//         U

struct X
{
    X(int x)
      : value(x)
    {}

    virtual ~X()
    {}

    int f() const
    {
        return value;
    }

    int value;
};

struct Y : virtual X
{
    Y(int value)
      : X(value)
    {}

    int g() const
    {
        return 2;
    }

    int dummy;
};

struct Z : virtual X
{
    Z(int value)
      : X(value)
    {}

    int h() const
    {
        return 3;
    }

    int dummy;
};

struct U : Y, Z
{
    U(int value)
      : X(value)
      , Y(value)
      , Z(value)
    {}

    int dummy;
};

X* upcast(U* p)
{
    return p;
}

// This hiearchy tries to prove that conversion and caching works for all sub
// object pointers:
//
//
//      Base  Base
//       |     |
//      Left  Right
//        \   /
//         \ /
//       Derived
//

struct Base
{
    Base(int value)
      : value(value)
    {}

    virtual ~Base()
    {}

    int value;
};

struct Left : Base
{
    Left()
      : Base(1)
    {}

    int left() const
    {
        return value;
    }

    int dummy;
};

struct Right : Base
{
    Right()
      : Base(2)
    {}

    int right() const
    {
        return value;
    }

    int dummy;
};

struct Derived : Left, Right
{
    int f() const
    {
        return 3;
    }

    int dummy;
};

Base* left(Left* p)
{
    return p;
}

Base* right(Right* p)
{
    return p;
}

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        class_<X>("X")
            .def("f", &X::f),
        class_<Y, X>("Y")
            .def("g", &Y::g),
        class_<Z, X>("Z")
            .def("h", &Z::h),
        class_<U, bases<Y, Z> >("U")
            .def(constructor<int>()),
        def("upcast", &upcast)
    ];

    // Do everything twice to verify that caching works.

    DOSTRING(L,
        "function assert2(x)\n"
        "    assert(x)\n"
        "    assert(x)\n"
        "end\n"
    );

    DOSTRING(L,
        "x = U(1)\n"
        "assert2(x:f() == 1)\n"
        "assert2(x:g() == 2)\n"
        "assert2(x:h() == 3)\n"
    );

    DOSTRING(L,
        "y = upcast(x)\n"
        "assert2(y:f() == 1)\n"
        "assert2(y:g() == 2)\n"
        "assert2(y:h() == 3)\n"
    );

    module(L) [
        class_<Base>("Base"),
        class_<Left, Base>("Left")
            .def("left", &Left::left),
        class_<Right, Base>("Right")
            .def("right", &Right::right),
        class_<Derived, bases<Left, Right> >("Derived")
            .def(constructor<>())
            .def("f", &Derived::f),
        def("left", &left),
        def("right", &right)
    ];

    DOSTRING(L,
        "x = Derived()\n"
        "assert2(x:left() == 1)\n"
        "assert2(x:right() == 2)\n"
        "assert2(x:f() == 3)\n"
    );

    DOSTRING(L,
        "y = left(x)\n"
        "assert2(y:left() == 1)\n"
        "assert2(y:right() == 2)\n"
        "assert2(y:f() == 3)\n"
    );

    DOSTRING(L,
        "y = right(x)\n"
        "assert2(y:left() == 1)\n"
        "assert2(y:right() == 2)\n"
        "assert2(y:f() == 3)\n"
    );
}
