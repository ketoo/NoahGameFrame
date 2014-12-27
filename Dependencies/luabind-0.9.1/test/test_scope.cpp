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

int f() { return 1; }
int f_(int a) { return 2; }
int f__(int a) { return 3; }
int g() { return 4; }
int g_(int)	{ return 5; }
int h() { return 6; }

struct test_class : counted_type<test_class>
{
	test_class()
	  : test(1)
	{}
	int test;
};

struct test_class2 : counted_type<test_class2>
{
	test_class2() {}
	int string_string(std::string const& s1, std::string const& s2)
	{ return 1; }
};

COUNTER_GUARD(test_class);
COUNTER_GUARD(test_class2);

void test_main(lua_State* L)
{
	using namespace luabind;

	module(L)
	[
		class_<test_class2>("test_class2")
				.def(constructor<>())
				.def("string_string", &test_class2::string_string)
	];

	module(L, "test")
	[
		class_<test_class>("test_class")
			.def(constructor<>())
			.def_readonly("test", &test_class::test)
			.scope
			[
				def("inner_fun", &f)
			]
            .def("inner_fun2", &f) // this should become static
			.enum_("vals")
			[
				value("val1", 1),
				value("val2", 2)
			],

		def("f", &f),
		def("f", &f_),

		namespace_("inner")
		[
			def("g", &g),
			def("f", &f__)
		],

		namespace_("inner")
		[
			def("g", &g_)
		]

	];

	module(L, "test")
	[
		namespace_("inner")
		[
			def("h", &h)
		]
	];


	DOSTRING(L, "assert(test.f() == 1)");
	DOSTRING(L, "assert(test.f(3) == 2)");
	DOSTRING(L, "assert(test.test_class.inner_fun() == 1)");
    DOSTRING(L,
		"a = test.test_class()\n"
		"assert(a.test == 1)");
	DOSTRING(L, "assert(a.inner_fun2() == 1)"); // free function
    DOSTRING(L,
		"b = test.test_class.val2\n"
		"assert(b == 2)");
	DOSTRING(L, "assert(test.inner.g() == 4)");
	DOSTRING(L, "assert(test.inner.g(7) == 5)");
	DOSTRING(L, "assert(test.inner.f(4) == 3)");
	DOSTRING(L, "assert(test.inner.h() == 6)");
}

