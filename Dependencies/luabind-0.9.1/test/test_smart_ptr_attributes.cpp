// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luabind/luabind.hpp>
#include <boost/shared_ptr.hpp>
#include <luabind/shared_ptr_converter.hpp>
#include <luabind/class_info.hpp>

struct Foo
{
    Foo() : m_baz(0) {}
    int m_baz;
};

struct Bar
{
    boost::shared_ptr<Foo> getFoo() const { return m_foo; }
    void setFoo( boost::shared_ptr<Foo> foo ) {  m_foo = foo; }

    boost::shared_ptr<Foo> m_foo;
};

void test_main(lua_State* L)
{
    using namespace luabind;

    bind_class_info(L);

    module( L )
    [
        class_<Foo, boost::shared_ptr<Foo> >( "Foo" )
            .def( constructor<>() )
            .def_readwrite("baz", &Foo::m_baz),
        class_<Bar, boost::shared_ptr<Bar> >( "Bar" )
            .def( constructor<>() )
            .property("fooz", &Bar::getFoo, &Bar::setFoo)
            .def_readwrite("foo", &Bar::m_foo)
    ];
    dostring( L, "foo = Foo();");
    dostring( L,    "foo.baz = 42;");
    dostring( L,    "x = Bar();");
    dostring( L,    "x.fooz = foo;");
    dostring( L,    "print(type(x), class_info(x).name);");
    dostring( L,    "print(type(x.fooz), class_info(x.fooz).name);");
    dostring( L,    "print(type(x.foo), class_info(x.foo).name);"); // crashes here);
}
