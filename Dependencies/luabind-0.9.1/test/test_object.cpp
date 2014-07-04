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
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/detail/debug.hpp>
#include <luabind/error.hpp>
#include <luabind/operator.hpp>

#include <boost/lexical_cast.hpp>

#include <utility>

using namespace luabind;

int test_object_param(const object& table)
{
	LUABIND_CHECK_STACK(table.interpreter());

	object current_object;
	current_object = table;
	lua_State* L = table.interpreter();

	if (type(table) == LUA_TTABLE)
	{
		int sum1 = 0;
		for (iterator i(table), e; i != e; ++i)
		{
			assert(type(*i) == LUA_TNUMBER);
			sum1 += object_cast<int>(*i);
		}

		int sum2 = 0;
		for (raw_iterator i(table), e; i != e; ++i)
		{
			assert(type(*i) == LUA_TNUMBER);
			sum2 += object_cast<int>(*i);
		}

		// test iteration of empty table
		object empty_table = newtable(L);
		for (iterator i(empty_table), e; i != e; ++i)
		{}
		
		table["sum1"] = sum1;
		table["sum2"] = sum2;
		table["blurp"] = 5;
		table["sum3"] = table["sum1"];
		return 0;
	}
	else
	{
		if (type(table) != LUA_TNIL)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
}

int test_fun()
{
	return 42;
}

struct test_param : counted_type<test_param>
{
	luabind::object obj;
	luabind::object obj2;

	bool operator==(test_param const& rhs) const
	{ return obj == rhs.obj && obj2 == rhs.obj2; }
};

COUNTER_GUARD(test_param);

int test_match(const luabind::object& o)
{
	return 0;
}

int test_match(int i)
{
	return 1;
}

void test_match_object(
	luabind::object p1
  , luabind::object p2
  , luabind::object p3)
{
	p1["ret"] = 1;
	p2["ret"] = 2;
	p3["ret"] = 3;
}

void test_call(lua_State* L)
{
    DOSTRING(L,
        "function sum(...)\n"
        "    local result = 0\n"
        "    for _,x in pairs({...}) do\n"
        "        result = result + x\n"
        "    end\n"
        "    return result\n"
        "end\n"
    );

    object sum = globals(L)["sum"];

    TEST_CHECK(object_cast<int>(sum()) == 0);
    TEST_CHECK(object_cast<int>(sum(1)) == 1);
    TEST_CHECK(object_cast<int>(sum(1,2)) == 3);
    TEST_CHECK(object_cast<int>(sum(1,2,3)) == 6);
    TEST_CHECK(object_cast<int>(sum(1,2,3,4)) == 10);
    TEST_CHECK(object_cast<int>(sum(1,2,3,4,5)) == 15);
}

void test_metatable(lua_State* L)
{
	object metatable = newtable(L);
	object G = globals(L);

	DOSTRING(L,
		"function index_function(table, key)\n"
		"  return key*2\n"
		"end\n"

		"index_table = {}"
	);

	metatable["__index"] = G["index_function"];
	setmetatable(G["index_table"], metatable);

	DOSTRING(L,
		"assert(index_table[0] == 0)\n"
		"assert(index_table[1] == 2)\n"
		"assert(index_table[2] == 4)\n"
	);

	DOSTRING(L,
		"setmetatable(index_table, { ['luabind.metatable'] = 1 })"
	);

	assert(getmetatable(G["index_table"])["luabind.metatable"] == 1);
	assert(type(getmetatable(G["index_table"])["nonexistent"]) == LUA_TNIL);
}

int with_upvalues(lua_State *)
{
	return 0;
}

void test_upvalues(lua_State* L)
{
	lua_pushnumber(L, 3);
	lua_pushnumber(L, 4);
	lua_pushcclosure(L, &with_upvalues, 2);

	object f(from_stack(L, -1));
	lua_pop(L, 1);

	assert(getupvalue(f, 1) == 3);
	assert(getupvalue(f, 2) == 4);

	setupvalue(f, 1, object(L, 4));
	assert(getupvalue(f, 1) == 4);
	assert(getupvalue(f, 2) == 4);

	setupvalue(f, 2, object(L, 5));
	assert(getupvalue(f, 1) == 4);
	assert(getupvalue(f, 2) == 5);
}

void test_explicit_conversions(lua_State* L)
{
	lua_pushcclosure(L, &with_upvalues, 0);
	object f(from_stack(L, -1));
	lua_pop(L, 1);
	assert(tocfunction(f) == &with_upvalues);

	int* p = static_cast<int*>(lua_newuserdata(L, sizeof(int)));
	*p = 1234;
	object x(from_stack(L, -1));
	lua_pop(L, 1);
	assert(*touserdata<int>(x) == 1234);
}

int with_argument(argument const& arg)
{
	return object_cast<int>(arg) * 2;
}

int with_table_argument(argument const& arg, argument const& key)
{
	return object_cast<int>(arg[key]);
}

void test_argument(lua_State* L)
{
    module(L) [
        def("with_argument", &with_argument),
        def("with_table_argument", &with_table_argument)
    ];

    DOSTRING(L,
        "assert(with_argument(1) == 2)\n"
        "assert(with_argument(2) == 4)\n"
    );

    DOSTRING(L,
        "x = { foo = 1, bar = 2 }\n"

        "assert(with_table_argument(x, 'foo') == 1)\n"
        "assert(with_table_argument(x, 'bar') == 2)\n"
    );
}

void test_bool_convertible(lua_State* L)
{
    DOSTRING(L,
        "x1 = nil\n"
        "x2 = false\n"
        "x3 = 0\n"
        "x4 = 1\n"
    );

    DOSTRING(L,
        "assert(not x1)\n"
        "assert(not x2)\n"
        "assert(x3)\n"
        "assert(x4)\n"
    );

    object G = globals(L);

    object x1 = G["x1"];
    object x2 = G["x2"];
    object x3 = G["x3"];
    object x4 = G["x4"];

    assert(!x1);
    assert(!x2);
    assert(x3);
    assert(x4);

    assert(!G["x1"]);
    assert(!G["x2"]);
    assert(G["x3"]);
    assert(G["x4"]);
}

void test_main(lua_State* L)
{
	using namespace luabind;

	module(L)
	[
		def("test_object_param", &test_object_param),
		def("test_fun", &test_fun),
		def("test_match", (int(*)(const luabind::object&))&test_match),
		def("test_match", (int(*)(int))&test_match),
		def("test_match_object", &test_match_object),
	
		class_<test_param>("test_param")
			.def(constructor<>())
			.def_readwrite("obj", &test_param::obj)
			.def_readonly("obj2", &test_param::obj2)
			.def(const_self == const_self)
	];

	object uninitialized;
	TEST_CHECK(!uninitialized);
	TEST_CHECK(!uninitialized.is_valid());

	test_param temp_object;
	globals(L)["temp"] = temp_object;
	TEST_CHECK(object_cast<test_param>(globals(L)["temp"]) == temp_object);
	globals(L)["temp"] = &temp_object;
	TEST_CHECK(object_cast<test_param const*>(globals(L)["temp"]) == &temp_object);
	TEST_CHECK(globals(L)["temp"] == temp_object);

	// test the registry
	object reg = registry(L);
	reg["__a"] = "foobar";
	TEST_CHECK(object_cast<std::string>(registry(L)["__a"]) == "foobar");
	
	DOSTRING(L,
		"t = 2\n"
		"assert(test_object_param(t) == 1)");

	DOSTRING(L, "assert(test_object_param(nil) == 2)");
	DOSTRING(L, "t = { ['oh'] = 4, 3, 5, 7, 13 }");
	DOSTRING(L, "assert(test_object_param(t) == 0)");
	DOSTRING(L, "assert(t.sum1 == 4 + 3 + 5 + 7 + 13)");
	DOSTRING(L, "assert(t.sum2 == 4 + 3 + 5 + 7 + 13)");
	DOSTRING(L, "assert(t.blurp == 5)");

	object g = globals(L);
	object ret = g["test_fun"]();
	TEST_CHECK(object_cast<int>(ret) == 42);

	DOSTRING(L, "function test_param_policies(x, y) end");
	object test_param_policies = g["test_param_policies"];
	int a = type(test_param_policies);
	TEST_CHECK(a == LUA_TFUNCTION);

	luabind::object obj;
	obj = luabind::object();

	// call the function and tell lua to adopt the pointer passed as first argument
	test_param_policies(5, new test_param())[adopt(_2)];

	DOSTRING(L, "assert(test_match(7) == 1)");
	DOSTRING(L, "assert(test_match('oo') == 0)");

	DOSTRING(L,
		"t = test_param()\n"
		"t.obj = 'foo'\n"
		"assert(t.obj == 'foo')\n"
		"assert(t.obj2 == nil)");

	DOSTRING(L,
		"function test_object_policies(a) glob = a\n"
		"return 6\n"
		"end");
	object test_object_policies = g["test_object_policies"];
	object ret_val = test_object_policies("teststring")[detail::null_type()];
	TEST_CHECK(object_cast<int>(ret_val) == 6);
	TEST_CHECK(ret_val == 6);
	TEST_CHECK(6 == ret_val);
	g["temp_val"] = 6;
	TEST_CHECK(ret_val == g["temp_val"]);
	object temp_val = g["temp_val"];
	TEST_CHECK(ret_val == temp_val);

	g["temp"] = "test string";
	TEST_CHECK(boost::lexical_cast<std::string>(g["temp"]) == "test string");
	g["temp"] = 6;
	TEST_CHECK(boost::lexical_cast<std::string>(g["temp"]) == "6");

	TEST_CHECK(object_cast<std::string>(g["glob"]) == "teststring");
	TEST_CHECK(object_cast<std::string>(gettable(g, "glob")) == "teststring");
	TEST_CHECK(object_cast<std::string>(rawget(g, "glob")) == "teststring");

	object t = newtable(L);
	TEST_CHECK(iterator(t) == iterator());
	TEST_CHECK(raw_iterator(t) == raw_iterator());

	t["foo"] = "bar";

	TEST_CHECK(object_cast<std::string>(t["foo"]) == "bar");
	TEST_CHECK(object_cast<std::string>(*iterator(t)) == "bar");
	TEST_CHECK(object_cast<std::string>(*raw_iterator(t)) == "bar");

	t["foo"] = nil; // luabind::nil_type

	TEST_CHECK(iterator(t) == iterator());
	TEST_CHECK(raw_iterator(t) == raw_iterator());

	t["foo"] = "bar";
	iterator it1(t);
	*it1 = nil;

	TEST_CHECK(iterator(t) == iterator());
	TEST_CHECK(raw_iterator(t) == raw_iterator());

	t["foo"] = "bar";
	raw_iterator it2(t);
	*it2 = nil;

	TEST_CHECK(iterator(t) == iterator());
	TEST_CHECK(raw_iterator(t) == raw_iterator());

	DOSTRING(L,
		"p1 = {}\n"
		"p2 = {}\n"
		"p3 = {}\n"
		"test_match_object(p1, p2, p3)\n"
		"assert(p1.ret == 1)\n"
		"assert(p2.ret == 2)\n"
		"assert(p3.ret == 3)\n");


#ifndef LUABIND_NO_EXCEPTIONS

	try
	{
		object not_initialized;
		int i = object_cast<int>(not_initialized);
		(void)i;
		TEST_ERROR("invalid cast succeeded");
	}
	catch(luabind::cast_failed&) {}

#endif

    object not_initialized;
    TEST_CHECK(!object_cast_nothrow<int>(not_initialized));
	 TEST_CHECK(!not_initialized.is_valid());
	 TEST_CHECK(!not_initialized);

    DOSTRING(L, "t = { {1}, {2}, {3}, {4} }");

    int inner_sum = 0;

    for (iterator i(globals(L)["t"]), e; i != e; ++i)
    {
        inner_sum += object_cast<int>((*i)[1]);
    }

    TEST_CHECK(inner_sum == 1 + 2 + 3 + 4);

    DOSTRING(L, "t = { {1, 2}, {3, 4}, {5, 6}, {7, 8} }");

    inner_sum = 0;

    for (iterator i(globals(L)["t"]), e; i != e; ++i)
    {
        for (iterator j(*i), e; j != e; ++j)
        {
            inner_sum += object_cast<int>(*j);
        }
    }

    TEST_CHECK(inner_sum == 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8);
    TEST_CHECK(object_cast<int>(globals(L)["t"][2][2]) == 4);

    test_call(L);
    test_metatable(L);
    test_upvalues(L);
    test_explicit_conversions(L);
    test_argument(L);
    test_bool_convertible(L);
}

