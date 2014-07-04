// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

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

#include <luabind/out_value_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include <luabind/copy_policy.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/discard_result_policy.hpp>
#include <luabind/dependency_policy.hpp>
#include <luabind/luabind.hpp>

struct test_copy {};


struct secret_type {};

secret_type sec_;


struct policies_test_class
{
	policies_test_class(const char* name): name_(name)
	{ ++count; }
	policies_test_class() { ++count; }
	~policies_test_class()
	{ --count; }

	std::string name_;

	policies_test_class* make(const char* name) const
	{
		return new policies_test_class(name);
	}

	void f(policies_test_class* p)
	{
		delete p;
	}
	const policies_test_class* internal_ref() { return this; }
	policies_test_class* self_ref()
	{ return this; }

	static int count;

	//	private:
	policies_test_class(policies_test_class const& c): name_(c.name_)
	{ ++count; }

	void member_out_val(int a, int* v) { *v = a * 2; }
	secret_type* member_secret() { return &sec_; }
};

int policies_test_class::count = 0;

policies_test_class global;

void out_val(float* f) { *f = 3.f; }
policies_test_class* copy_val() { return &global; }
policies_test_class const* copy_val_const() { return &global; }

secret_type* secret() { return &sec_; }

void aux_test();

struct test_t {
	test_t *make(int) { return new test_t(); }
	void take(test_t*) {}
};

struct MI2;

struct MI1
{
	void add(MI2 *) {}
};

struct MI2 : public MI1
{
	virtual ~MI2()
	{}
};

struct MI2W : public MI2, public luabind::wrap_base {};

void test_main(lua_State* L)
{
	using namespace luabind;

	module(L)
	[
		class_<test_t>("test_t")
		.def("make", &test_t::make, adopt(return_value))
		.def("take", &test_t::take, adopt(_2))
	];
	
	module(L)
	[
		class_<policies_test_class>("test")
			.def(constructor<>())
			.def("member_out_val", &policies_test_class::member_out_val, pure_out_value(_3))
			.def("member_secret", &policies_test_class::member_secret, discard_result)
			.def("f", &policies_test_class::f, adopt(_2))
			.def("make", &policies_test_class::make, adopt(return_value))
			.def("internal_ref", &policies_test_class::internal_ref, dependency(result, _1))
			.def("self_ref", &policies_test_class::self_ref, return_reference_to(_1)),

		def("out_val", &out_val, pure_out_value(_1)),
		def("copy_val", &copy_val, copy(result)),
		def("copy_val_const", &copy_val_const, copy(result)),
		def("secret", &secret, discard_result),

		class_<MI1>("mi1")
			.def(constructor<>())
			.def("add",&MI1::add,adopt(_2)),

		class_<MI2,MI2W,MI1>("mi2")
			.def(constructor<>())
	];

	// test copy
	DOSTRING(L,	"a = secret()\n");

	TEST_CHECK(policies_test_class::count == 1);

	DOSTRING(L, "a = copy_val()\n");
	TEST_CHECK(policies_test_class::count == 2);

	DOSTRING(L, "b = copy_val_const()\n");
	TEST_CHECK(policies_test_class::count == 3);

	DOSTRING(L,
		"a = nil\n"
		"b = nil\n"
		"collectgarbage()\n");

	// only the global variable left here
	TEST_CHECK(policies_test_class::count == 1);

	// out_value
	DOSTRING(L,
		"a = out_val()\n"
		"assert(a == 3)");

	// return_reference_to
	DOSTRING(L,
		"a = test()\n"
		"b = a:self_ref()\n"
		"a = nil\n"
		"collectgarbage()");

	// a is kept alive as long as b is alive
	TEST_CHECK(policies_test_class::count == 2);

	DOSTRING(L,
		"b = nil\n"
		"collectgarbage()");

	TEST_CHECK(policies_test_class::count == 1);

	DOSTRING(L, "a = test()");

	TEST_CHECK(policies_test_class::count == 2);

	DOSTRING(L,
		"b = a:internal_ref()\n"
		"a = nil\n"
		"collectgarbage()");

	// a is kept alive as long as b is alive
	TEST_CHECK(policies_test_class::count == 2);

	// two gc-cycles because dependency-table won't be collected in the
	// same cycle as the object_rep
	DOSTRING(L,
		"b = nil\n"
		"collectgarbage()\n"
		"collectgarbage()");

	TEST_CHECK(policies_test_class::count == 1);

	// adopt
	DOSTRING(L, "a = test()");

	TEST_CHECK(policies_test_class::count == 2);

	DOSTRING(L, "b = a:make('tjosan')");
	DOSTRING(L, "assert(a:member_out_val(3) == 6)");
	DOSTRING(L, "a:member_secret()");

	// make instantiated a new policies_test_class
	TEST_CHECK(policies_test_class::count == 3);

	DOSTRING(L, "a:f(b)\n");

	// b was adopted by c++ and deleted the object
	TEST_CHECK(policies_test_class::count == 2);

	DOSTRING(L, "a = nil\n"
		"collectgarbage()");

	TEST_CHECK(policies_test_class::count == 1);

	// adopt with wrappers
	DOSTRING(L, "mi1():add(mi2())");
}

