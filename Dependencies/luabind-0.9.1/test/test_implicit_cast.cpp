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
#include <boost/shared_ptr.hpp>

struct A : counted_type<A> 
{ virtual ~A() {} };

struct B : A, counted_type<B>  
{};


struct enum_placeholder {};
typedef enum { VAL1 = 1, VAL2 = 2 } LBENUM_t;
LBENUM_t enum_by_val(LBENUM_t e)    { return e; }
LBENUM_t enum_by_const_ref(const LBENUM_t &e)   { return e; }



struct test_implicit : counted_type<test_implicit>
{
	char const* f(A*) { return "f(A*)"; }
	char const* f(B*) { return "f(B*)"; }
};

struct char_pointer_convertable
  : counted_type<char_pointer_convertable>
{
	operator const char*() const { return "foo!"; }
};

void func(const char_pointer_convertable& f)
{
}

void not_convertable(boost::shared_ptr<A>)
{
	TEST_CHECK(false);
}

int f(int& a)
{
	return a;
}

COUNTER_GUARD(A);
COUNTER_GUARD(B);
COUNTER_GUARD(test_implicit);
COUNTER_GUARD(char_pointer_convertable);

void test_main(lua_State* L)
{
    using namespace luabind;

    typedef char const*(test_implicit::*f1)(A*);
    typedef char const*(test_implicit::*f2)(B*);

    module(L)
    [
        class_<A>("A")
            .def(constructor<>()),
    
        class_<B, A>("B")
            .def(constructor<>()),
    
        class_<test_implicit>("test")
            .def(constructor<>())
            .def("f", (f1)&test_implicit::f)
            .def("f", (f2)&test_implicit::f),

        class_<char_pointer_convertable>("char_ptr")
            .def(constructor<>()),

        class_<enum_placeholder>("LBENUM")
            .enum_("constants")
            [
                value("VAL1", VAL1),
                value("VAL2", VAL2)
            ],
        def("enum_by_val", &enum_by_val),
        def("enum_by_const_ref", &enum_by_const_ref),

        def("func", &func),
		def("no_convert", &not_convertable),
		def("f", &f)
    ];

    DOSTRING(L, "a = A()");
    DOSTRING(L, "b = B()");
    DOSTRING(L, "t = test()");

    DOSTRING(L, "assert(t:f(a) == 'f(A*)')");
    DOSTRING(L, "assert(t:f(b) == 'f(B*)')");

    DOSTRING(L, 
        "a = char_ptr()\n"
        "func(a)");

    DOSTRING(L, "assert(LBENUM.VAL1 == 1)");
    DOSTRING(L, "assert(LBENUM.VAL2 == 2)");
    DOSTRING(L, "assert(enum_by_val(LBENUM.VAL1) == LBENUM.VAL1)");
    DOSTRING(L, "assert(enum_by_val(LBENUM.VAL2) == LBENUM.VAL2)");
    DOSTRING(L, "assert(enum_by_const_ref(LBENUM.VAL1) == LBENUM.VAL1)");
    DOSTRING(L, "assert(enum_by_const_ref(LBENUM.VAL2) == LBENUM.VAL2)");

	DOSTRING_EXPECTED(L,
		"a = A()\n"
		"no_convert(a)",
		("No matching overload found, candidates:\n"
		"void no_convert(custom ["
		+ std::string(typeid(boost::shared_ptr<A>).name()) + "])").c_str());

	DOSTRING_EXPECTED(L,
		"a = nil\n"
		"f(a)",
		"No matching overload found, candidates:\n"
		"int f(int&)");
}

