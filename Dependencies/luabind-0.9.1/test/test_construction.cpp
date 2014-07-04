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

struct A : counted_type<A>
{
	int test;
	A(int a) { test = a; }
	A(const A&) { test = 1; }
	A() { test = 2; }
	~A() {}
};

void f(A*, const A&) {}

struct B: A, counted_type<B>
{
	int test2;
	B(int a):A(a) { test2 = a; }
	B() { test2 = 3; }
	~B() {}
};

struct C : counted_type<C> {};

struct base1 : counted_type<base1>
{
	virtual void doSomething() = 0;
	virtual ~base1() {}
};

struct deriv_1 : base1, counted_type<deriv_1>
{
	void doSomething() {}
};

struct deriv_2 : deriv_1, counted_type<deriv_2>
{
	void doMore() {}
};

COUNTER_GUARD(A);
COUNTER_GUARD(B);
COUNTER_GUARD(C);
COUNTER_GUARD(base1);
COUNTER_GUARD(deriv_1);
COUNTER_GUARD(deriv_2);

void test_main(lua_State* L)
{
	using namespace luabind;

	module(L)
	[
		class_<A>("A")
			.def("f", &f)
			.def_readonly("test", &A::test)
			.def(constructor<int>())
			.def(constructor<const A&>())
			.def(constructor<>()),

		class_<B, A>("B")
			.def(constructor<int>())
			.def(constructor<>())
			.def_readonly("test2", &B::test2),

		class_<C>("C")

	];

	DOSTRING_EXPECTED(L, "a = C()", "attempt to call a nil value");

	DOSTRING(L,
		"a = A(4)\n"
		"assert(a.test == 4)");

	DOSTRING(L,
		"a2 = A(a)\n"
		"assert(a2.test == 1)");

	DOSTRING(L,
		"b = B(6)\n"
		"assert(b.test2 == 6)\n");
	DOSTRING(L, "assert(b.test == 6)");

	DOSTRING(L,
		"b2 = B()\n"
		"assert(b2.test2 == 3)\n");
	DOSTRING(L, "assert(b2.test == 2)");
}

