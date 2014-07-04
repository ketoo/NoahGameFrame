// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#include "test.hpp"
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

struct base : counted_type<base>
{
    int f()
    {
        return 5;
    }
};

COUNTER_GUARD(base);

int f(int x)
{
    return x + 1;
}

int f(int x, int y)
{
    return x + y;
}

base* create_base()
{
    return new base();
}

void test_value_converter(const std::string str)
{
    TEST_CHECK(str == "converted string");
}

void test_pointer_converter(const char* const str)
{
    TEST_CHECK(std::strcmp(str, "converted string") == 0);
}

struct copy_me
{
};

void take_by_value(copy_me m)
{
}

int function_should_never_be_called(lua_State* L)
{
    lua_pushnumber(L, 10);
    return 1;
}

void test_main(lua_State* L)
{
    using namespace luabind;

    lua_pushstring(L, "f");
    lua_pushcclosure(L, &function_should_never_be_called, 0);
    lua_settable(L, LUA_GLOBALSINDEX);

    DOSTRING(L, "assert(f() == 10)");

    module(L)
    [
        class_<copy_me>("copy_me")
            .def(constructor<>()),
    
        class_<base>("base")
            .def("f", &base::f),


        def("by_value", &take_by_value),

        def("f", (int(*)(int)) &f),
        def("f", (int(*)(int, int)) &f),
        def("create", &create_base, adopt(return_value))
//        def("set_functor", &set_functor)
            
#if !(BOOST_MSVC < 1300)
        ,
        def("test_value_converter", &test_value_converter),
        def("test_pointer_converter", &test_pointer_converter)
#endif
            
    ];

    DOSTRING(L,
        "e = create()\n"
        "assert(e:f() == 5)");

    DOSTRING(L, "assert(f(7) == 8)");

    DOSTRING(L, "assert(f(3, 9) == 12)");

//    DOSTRING(L, "set_functor(function(x) return x * 10 end)");

//    TEST_CHECK(functor_test(20) == 200);

//    DOSTRING(L, "set_functor(nil)");

    DOSTRING(L, "function lua_create() return create() end");
    base* ptr = call_function<base*>(L, "lua_create") [ adopt(result) ];
    delete ptr;

#if !(BOOST_MSVC < 1300)
    DOSTRING(L, "test_value_converter('converted string')");
    DOSTRING(L, "test_pointer_converter('converted string')");
#endif

    DOSTRING_EXPECTED(L, "f('incorrect', 'parameters')",
        "No matching overload found, candidates:\n"
        "int f(int,int)\n"
        "int f(int)");


    DOSTRING(L, "function failing_fun() error('expected error message') end");
    try
    {
        call_function<void>(L, "failing_fun");
        TEST_ERROR("function didn't fail when it was expected to");
    }
    catch(luabind::error const& e)
    {
        if (std::string("[string \"function failing_fun() error('expected "
            "erro...\"]:1: expected error message") != lua_tostring(L, -1))
        {
            TEST_ERROR("function failed with unexpected error message");
        }

        lua_pop(L, 1);
    }

}

