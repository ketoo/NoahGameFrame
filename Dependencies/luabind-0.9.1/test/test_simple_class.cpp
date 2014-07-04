// Copyright (c) 2004 Daniel Wallin

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

struct simple_class : counted_type<simple_class>
{
	static int feedback;

	void f()
	{
		feedback = 1;
	}

	void f(int, int) {}
	void f(std::string a)
	{
		const char str[] = "foo\0bar";
		if (a == std::string(str, sizeof(str)-1))
			feedback = 2;
	}

	std::string g()
	{
		const char str[] = "foo\0bar";
		return std::string(str, sizeof(str)-1);
	}

};

int simple_class::feedback = 0;
	
COUNTER_GUARD(simple_class);

void test_main(lua_State* L)
{
	using namespace luabind;

	typedef void(simple_class::*f_overload1)();
	typedef void(simple_class::*f_overload2)(int, int);
	typedef void(simple_class::*f_overload3)(std::string);

    module(L)
    [
        class_<simple_class>("simple")
            .def(constructor<>())
			.def("f", (f_overload1)&simple_class::f)
			.def("f", (f_overload2)&simple_class::f)
			.def("f", (f_overload3)&simple_class::f)
			.def("g", &simple_class::g)
    ];

    DOSTRING(L,
        "class 'simple_derived' (simple)\n"
        "  function simple_derived:__init() simple.__init(self) end\n"
        "a = simple_derived()\n"
        "a:f()\n");
    TEST_CHECK(simple_class::feedback == 1);

    DOSTRING(L, "a:f('foo\\0bar')");
    TEST_CHECK(simple_class::feedback == 2);

	DOSTRING(L,
		"b = simple_derived()\n"
		"a.foo = 'yo'\n"
		"assert(b.foo == nil)");

	DOSTRING(L,
		"simple_derived.foobar = 'yi'\n"
		"assert(b.foobar == 'yi')\n"
		"assert(a.foobar == 'yi')\n");

    simple_class::feedback = 0;

    DOSTRING_EXPECTED(L, "a:f('incorrect', 'parameters')",
        "No matching overload found, candidates:\n"
		"void f(simple&,std::string)\n"
		"void f(simple&,int,int)\n"
		"void f(simple&)");

    DOSTRING(L, "if a:g() == \"foo\\0bar\" then a:f() end");
    TEST_CHECK(simple_class::feedback == 1);
}

