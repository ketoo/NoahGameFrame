// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>

struct X
{
private:
    ~X() {}
};

int ptr_count = 0;

template <class T>
struct ptr
{
    ptr()
      : p(0)
    {
        ptr_count++;
    }

    ptr(T* p)
      : p(p)
    {
        ptr_count++;
    }

    ptr(ptr const& other)
      : p(other.p)
    {
        ptr_count++;
    }

    template <class U>
    ptr(ptr<U> const& other)
      : p(other.p)
    {
        ptr_count++;
    }

    ~ptr()
    {
        ptr_count--;
    }

    T* p;
};

template <class T>
T* get_pointer(ptr<T> const& x)
{
    return x.p;
}

template <class T>
ptr<T const>* get_const_holder(ptr<T>*)
{
    return 0;
}

void f1(X const&)
{}

void f2(X&)
{}

void f3(X const*)
{}

void f4(X*)
{}

void g1(ptr<X> p)
{
    TEST_CHECK(ptr_count == (p.p ? 2 : 3));
}

void g2(ptr<X> const& p)
{
    TEST_CHECK(ptr_count == (p.p ? 1 : 2));
}

void g3(ptr<X>*)
{
    TEST_CHECK(ptr_count == 1);
}

void g4(ptr<X> const*)
{
    TEST_CHECK(ptr_count == 1);
}

ptr<X> get()
{
    return ptr<X>(new X);
}

void test_main(lua_State* L)
{
    using namespace luabind;

    module(L) [
        class_<X, ptr<X> >("X"),

        def("get", &get),

        def("f1", &f1),
        def("f2", &f2),
        def("f3", &f3),
        def("f4", &f4),

        def("g1", &g1),
        def("g2", &g2),
        def("g3", &g3),
        def("g4", &g4)
    ];

    DOSTRING(L, "x = get()\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "f1(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "f2(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "f3(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "f4(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "g1(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "g2(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "g3(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L, "g4(x)\n");
    TEST_CHECK(ptr_count == 1);

    DOSTRING(L,
        "x = nil\n"
    );

    lua_gc(L, LUA_GCCOLLECT, 0);
    TEST_CHECK(ptr_count == 0);
}

