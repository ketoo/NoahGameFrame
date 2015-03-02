// Copyright (c) 2005 Daniel Wallin, Arvid Norberg

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
#include <luabind/wrapper_base.hpp>
#include <luabind/adopt_policy.hpp>
#include <boost/shared_ptr.hpp>

namespace luabind {

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
    template<class T>
    T* get_pointer(boost::shared_ptr<T> const& p) { return p.get(); }
#endif

}

using namespace luabind;

struct A : counted_type<A>
{
    virtual ~A() {}

    virtual std::string f()
    { return "A:f()"; }

    virtual std::string g() const
    { return "A:g()"; }
};

struct A_wrap : A, wrap_base
{
    std::string f()
    { 
        return call_member<std::string>(this, "f"); 
    }

    static std::string default_f(A* p)
    { return p->A::f(); }

    std::string g() const
    {
        return call_member<std::string>(this, "g");
    }

    static std::string default_g(A const* p)
    { return p->A::g(); }
};

struct B : A
{
    virtual std::string f()
    { return "B:f()"; }
};

struct B_wrap : B, wrap_base
{
    virtual std::string f()
    { return call_member<std::string>(this, "f"); }

    static std::string default_f(B* p)
    { return p->B::f(); }

    virtual std::string g() const
    { return call_member<std::string>(this, "g"); }

    static std::string default_g(B const* p)
    { return p->B::g(); }
};


struct base : counted_type<base>
{
    virtual ~base() {}

    virtual std::string f()
    {
        return "base:f()";
    }

    virtual std::string g() const { return ""; }
};

base* filter(base* p) { return p; }

struct base_wrap : base, wrap_base
{
    virtual std::string f()
    {
		return call_member<std::string>(this, "f");
    }

    static std::string default_f(base* p)
    {
        return p->base::f();
    }

	virtual std::string g() const
	{
		return call_member<std::string>(this, "g");
	}
};

struct T_ // vc6.5, don't name your types T!
{
	int f(int) { return 1; }
};

struct U : T_
{
	int g() { return 3; }
	int f(int, int) { return 2; }
};

void test_main(lua_State* L)
{
	module(L)
	[
		class_<A, A_wrap, boost::shared_ptr<A> >("A")
			.def(constructor<>())
			.def("f", &A::f, &A_wrap::default_f)
			.def("g", &A::g, &A_wrap::default_g),

		class_<B, A, B_wrap, boost::shared_ptr<A> >("B")
			.def(constructor<>())
			.def("f", &B::f, &B_wrap::default_f)
			.def("g", &B::g, &B_wrap::default_g),

        def("filter", &filter),

        class_<base, base_wrap>("base")
			.def(constructor<>())
			.def("f", &base::f, &base_wrap::default_f)
			.def("g", &base::g),

		class_<T_>("T")
			.def("f", &T_::f),

		class_<U, T_>("U")
			.def(constructor<>())
			.def("f", &T_::f)
			.def("f", &U::f)
			.def("g", &U::g)
	];
                              
	DOSTRING(L, 
		"u = U()\n"
		"assert(u:f(0) == 1)\n"
		"assert(u:f(0,0) == 2)\n"
		"assert(u:g() == 3)\n");

	DOSTRING(L,
		"u = U()\n"
		"assert(u:f(0) == 1)\n"
		"assert(u:f(0,0) == 2)\n"
		"assert(u:g() == 3)\n");

	DOSTRING(L,
		"function base:fun()\n"
		"  return 4\n"
		"end\n"
		"ba = base()\n"
		"assert(ba:fun() == 4)");

    DOSTRING(L, 
        "class 'derived' (base)\n"
        "  function derived:__init() base.__init(self) end\n"
        "  function derived:f()\n"
        "    return 'derived:f() : ' .. base.f(self)\n"
        "  end\n"

		"class 'empty_derived' (base)\n"
		"  function empty_derived:__init() base.__init(self) end\n"

		"class 'C' (B)\n"
        "  function C:__init() B.__init(self) end\n"
		"  function C:f() return 'C:f()' end\n"

        "function make_derived()\n"
        "  return derived()\n"
        "end\n"
		
        "function make_empty_derived()\n"
        "  return empty_derived()\n"
        "end\n"
		
		"function adopt_ptr(x)\n"
		"  a = x\n"
		"end\n");
		
	DOSTRING(L,
		"function gen_error()\n"
		"  assert(0 == 1)\n"
		"end\n");

	DOSTRING(L,
		"a = A()\n"
		"b = B()\n"
		"c = C()\n"

		"assert(c:f() == 'C:f()')\n"
		"assert(b:f() == 'B:f()')\n"
		"assert(a:f() == 'A:f()')\n"
		"assert(b:g() == 'A:g()')\n"
		"assert(c:g() == 'A:g()')\n"

		"assert(C.f(c) == 'C:f()')\n"
		"assert(B.f(c) == 'B:f()')\n"
		"assert(A.f(c) == 'A:f()')\n"
		"assert(A.g(c) == 'A:g()')\n");

#ifndef LUABIND_NO_EXCEPTONS
	{
		LUABIND_CHECK_STACK(L);

		try { call_function<int>(L, "gen_error"); }
		catch (luabind::error&)
		{
            bool result(
                lua_tostring(L, -1) == std::string("[string \"function "
                    "gen_error()...\"]:2: assertion failed!"));
			TEST_CHECK(result);
			lua_pop(L, 1);
		}
	}

	{
		A a;

		DOSTRING(L, "function test_ref(x) end");
		call_function<void>(L, "test_ref", boost::ref(a));
	}

	{
		LUABIND_CHECK_STACK(L);

		try { call_function<void>(L, "gen_error"); }
		catch (luabind::error&)
		{
            bool result(
                lua_tostring(L, -1) == std::string("[string \"function "
                    "gen_error()...\"]:2: assertion failed!"));
			TEST_CHECK(result);
			lua_pop(L, 1);
		}
	}

	{
		LUABIND_CHECK_STACK(L);

		try { call_function<void>(L, "gen_error") [ adopt(result) ]; }
		catch (luabind::error&)
		{
            bool result(
                lua_tostring(L, -1) == std::string("[string \"function "
                    "gen_error()...\"]:2: assertion failed!"));
			TEST_CHECK(result);
			lua_pop(L, 1);
		}
	}

#endif
	
	base* ptr;
	{
		LUABIND_CHECK_STACK(L);

		TEST_NOTHROW(
			object a = globals(L)["ba"];
			TEST_CHECK(call_member<int>(a, "fun") == 4);
		);
	}

	{
		LUABIND_CHECK_STACK(L);

		object make_derived = globals(L)["make_derived"];
		TEST_NOTHROW(
			call_function<void>(make_derived)
			);
	}

	std::auto_ptr<base> own_ptr;
	{
		LUABIND_CHECK_STACK(L);

		TEST_NOTHROW(
		    own_ptr = std::auto_ptr<base>(
                call_function<base*>(L, "make_derived") [ adopt(result) ])
			);
	}

	// make sure the derived lua-part is still referenced by
	// the adopted c++ pointer
	DOSTRING(L,
		"collectgarbage()\n"
		"collectgarbage()\n"
		"collectgarbage()\n"
		"collectgarbage()\n"
		"collectgarbage()\n");

    TEST_NOTHROW(
        TEST_CHECK(own_ptr->f() == "derived:f() : base:f()")
    );
	own_ptr = std::auto_ptr<base>();

	// test virtual functions that are not overridden by lua
    TEST_NOTHROW(
        own_ptr = std::auto_ptr<base>(
            call_function<base*>(L, "make_empty_derived") [ adopt(result) ])
        );
    TEST_NOTHROW(
        TEST_CHECK(own_ptr->f() == "base:f()")
	);
    TEST_NOTHROW(
        call_function<void>(L, "adopt_ptr", own_ptr.get()) [ adopt(_1) ]
    );
	own_ptr.release();

	// test virtual functions that are overridden by lua
    TEST_NOTHROW(
        ptr = call_function<base*>(L, "derived")
    );

    TEST_NOTHROW(
        TEST_CHECK(ptr->f() == "derived:f() : base:f()")
    );

	// test virtual function dispatch from within lua
	DOSTRING(L,
		"a = derived()\n"
        "b = filter(a)\n"
        "assert(b:f() == 'derived:f() : base:f()')\n");

	// test back references
	DOSTRING(L,
		"a = derived()\n"
		"assert(a == filter(a))\n");
}

