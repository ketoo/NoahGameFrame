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

using namespace luabind;

struct abstract
{
    virtual ~abstract() {}
    virtual std::string hello() = 0;
}; 

COUNTER_GUARD(abstract);

struct concrete : abstract
{
    std::string hello()
    {
        return "test string";
    }
};

struct abstract_wrap : abstract, wrap_base
{
    std::string hello()
    {
        return call_member<std::string>(this, "hello");
    }

    static void default_hello(abstract const&)
    {
        throw std::runtime_error("abstract function");
    }
};

std::string call_hello(abstract& a)
{
    return a.hello();
}

abstract& return_abstract_ref()
{
	static concrete c;
	return c;
}

abstract const& return_const_abstract_ref()
{
	static concrete c;
	return c;
}


void test_main(lua_State* L)
{
    module(L)
    [
        class_<abstract, abstract_wrap>("abstract")
            .def(constructor<>())
            .def("hello", &abstract::hello),

        def("call_hello", &call_hello),
		  def("return_abstract_ref", &return_abstract_ref),
		  def("return_const_abstract_ref", &return_const_abstract_ref)
    ];
    
    DOSTRING_EXPECTED(L,
        "x = abstract()\n"
        "x:hello()\n"
      , "std::runtime_error: 'Attempt to call nonexistent function'");

    DOSTRING_EXPECTED(L, 
        "call_hello(x)\n"
      , "std::runtime_error: 'Attempt to call nonexistent function'");
    
    DOSTRING(L,
        "class 'concrete' (abstract)\n"
        "  function concrete:__init()\n"
        "      abstract.__init(self)\n"
        "  end\n"

        "  function concrete:hello()\n"
        "      return 'hello from lua'\n"
        "  end\n");

    DOSTRING(L,
        "y = concrete()\n"
        "y:hello()\n");

    DOSTRING(L, "call_hello(y)\n");
}

