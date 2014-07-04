.. include:: version.rst

+++++++++++++++++++
 luabind |version|
+++++++++++++++++++

:Author: Daniel Wallin, Arvid Norberg
:Copyright: Copyright Daniel Wallin, Arvid Norberg 2003.
:License: Permission is hereby granted, free of charge, to any person obtaining a
          copy of this software and associated documentation files (the "Software"),
          to deal in the Software without restriction, including without limitation
          the rights to use, copy, modify, merge, publish, distribute, sublicense,
          and/or sell copies of the Software, and to permit persons to whom the
          Software is furnished to do so, subject to the following conditions:

          The above copyright notice and this permission notice shall be included
          in all copies or substantial portions of the Software.

          THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
          ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
          TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
          PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
          SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
          ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
          ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
          OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
          OR OTHER DEALINGS IN THE SOFTWARE.


.. _MIT license: http://www.opensource.org/licenses/mit-license.php
.. _Boost: http://www.boost.org

.. contents::
    :depth: 2
    :backlinks: none
.. section-numbering::

.. |...| unicode:: U+02026

Introduction
============

Luabind is a library that helps you create bindings between C++ and Lua. It has
the ability to expose functions and classes, written in C++, to Lua. It will
also supply the functionality to define classes in Lua and let them derive from
other Lua classes or C++ classes. Lua classes can override virtual functions
from their C++ base classes. It is written towards Lua 5.0, and does not work
with Lua 4.

It is implemented utilizing template meta programming. That means that you
don't need an extra preprocess pass to compile your project (it is done by the
compiler). It also means you don't (usually) have to know the exact signature 
of each function you register, since the library will generate code depending 
on the compile-time type of the function (which includes the signature). The 
main drawback of this approach is that the compilation time will increase for 
the file that does the registration, it is therefore recommended that you 
register everything in the same cpp-file.

Luabind is released under the terms of the `MIT license`_.

We are very interested in hearing about projects that use luabind, please let
us know about your project.

The main channel for help and feedback is the `luabind mailing list`_.
There's also an IRC channel ``#luabind`` on irc.freenode.net.

.. _`luabind mailing list`: https://lists.sourceforge.net/lists/listinfo/luabind-user


Features
========

Luabind supports:

 - Overloaded free functions 
 - C++ classes in Lua 
 - Overloaded member functions 
 - Operators 
 - Properties 
 - Enums 
 - Lua functions in C++ 
 - Lua classes in C++ 
 - Lua classes (single inheritance) 
 - Derives from Lua or C++ classes 
 - Override virtual functions from C++ classes 
 - Implicit casts between registered types 
 - Best match signature matching 
 - Return value policies and parameter policies 


Portability
===========

Luabind has been tested to work on the following compilers:

 - Visual Studio 7.1 
 - Intel C++ 6.0 (Windows) 
 - GCC 2.95.3 (cygwin) 
 - GCC 3.0.4 (Debian/Linux) 
 - GCC 3.1 (SunOS 5.8) 
 - GCC 3.2 (cygwin) 
 - GCC 3.3.1 (cygwin)
 - GCC 3.3 (Apple, MacOS X)
 - GCC 4.0 (Apple, MacOS X)

It has been confirmed not to work with:

 - GCC 2.95.2 (SunOS 5.8) 

Metrowerks 8.3 (Windows) compiles but fails the const-test. This 
means that const member functions are treated as non-const member 
functions.

If you have tried luabind with a compiler not listed here, let us know 
your result with it.

.. include:: building.rst

Basic usage
===========

To use luabind, you must include ``lua.h`` and luabind's main header file::

    extern "C"
    {
        #include "lua.h"
    }

    #include <luabind/luabind.hpp>

This includes support for both registering classes and functions. If you just
want to have support for functions or classes you can include
``luabind/function.hpp`` and ``luabind/class.hpp`` separately::

    #include <luabind/function.hpp>
    #include <luabind/class.hpp>

The first thing you need to do is to call ``luabind::open(lua_State*)`` which
will register the functions to create classes from Lua, and initialize some
state-global structures used by luabind. If you don't call this function you
will hit asserts later in the library. There is no corresponding close function
because once a class has been registered in Lua, there really isn't any good
way to remove it. Partly because any remaining instances of that class relies
on the class being there. Everything will be cleaned up when the state is
closed though.

.. Isn't this wrong? Don't we include lua.h using lua_include.hpp ?

Luabind's headers will never include ``lua.h`` directly, but through
``<luabind/lua_include.hpp>``. If you for some reason need to include another
Lua header, you can modify this file.


Hello world
-----------

::

    #include <iostream>
    #include <luabind/luabind.hpp>

    void greet()
    {
        std::cout << "hello world!\n";
    }

    extern "C" int init(lua_State* L)
    {
        using namespace luabind;

        open(L);

        module(L)
        [
            def("greet", &greet)
        ];

        return 0;
    }

::

    Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
    > loadlib('hello_world.dll', 'init')()
    > greet()
    Hello world!
    >

Scopes
======

Everything that gets registered in Lua is registered in a namespace (Lua
tables) or in the global scope (called module). All registrations must be
surrounded by its scope. To define a module, the ``luabind::module`` class is
used. It is used like this::

    module(L)
    [
        // declarations
    ];

This will register all declared functions or classes in the global namespace in
Lua. If you want to have a namespace for your module (like the standard
libraries) you can give a name to the constructor, like this::

    module(L, "my_library")
    [
        // declarations
    ];

Here all declarations will be put in the my_library table.

If you want nested namespace's you can use the ``luabind::namespace_`` class. It
works exactly as ``luabind::module`` except that it doesn't take a lua_State*
in it's constructor. An example of its usage could look like this::

    module(L, "my_library")
    [
        // declarations

        namespace_("detail")
        [
            // library-private declarations
        ]
    ];

As you might have figured out, the following declarations are equivalent::

    module(L)
    [
        namespace_("my_library")
        [
            // declarations
        ]

    ];

::
    
    module(L, "my_library")
    [
        // declarations
    ];

Each declaration must be separated by a comma, like this::

    module(L)
    [
        def("f", &f),
        def("g", &g),
        class_<A>("A")
            .def(constructor<int, int>),
        def("h", &h)
    ];


More about the actual declarations in the `Binding functions to Lua`_ and
`Binding classes to Lua`_ sections.

A word of caution, if you are in really bad need for performance, putting your
functions in tables will increase the lookup time.


Binding functions to Lua
========================

To bind functions to Lua you use the function ``luabind::def()``. It has the
following synopsis::

    template<class F, class policies>
    void def(const char* name, F f, const Policies&);

- name is the name the function will have within Lua. 
- F is the function pointer you want to register. 
- The Policies parameter is used to describe how parameters and return values 
  are treated by the function, this is an optional parameter. More on this in 
  the `policies`_ section.

An example usage could be if you want to register the function ``float
std::sin(float)``::

    module(L)
    [
        def("sin", &std::sin)
    ];

Overloaded functions
--------------------

If you have more than one function with the same name, and want to register
them in Lua, you have to explicitly give the signature. This is to let C++ know
which function you refer to. For example, if you have two functions, ``int
f(const char*)`` and ``void f(int)``. ::

    module(L)
    [
        def("f", (int(*)(const char*)) &f),
        def("f", (void(*)(int)) &f)
    ];

Signature matching
------------------

luabind will generate code that checks the Lua stack to see if the values there
can match your functions' signatures. It will handle implicit typecasts between
derived classes, and it will prefer matches with the least number of implicit
casts. In a function call, if the function is overloaded and there's no
overload that match the parameters better than the other, you have an
ambiguity. This will spawn a run-time error, stating that the function call is
ambiguous. A simple example of this is to register one function that takes an
int and one that takes a float. Since Lua doesn't distinguish between floats and
integers, both will always match.

Since all overloads are tested, it will always find the best match (not the
first match). This also means that it can handle situations where the only
difference in the signature is that one member function is const and the other
isn't. 

.. sidebar:: Ownership transfer

   To correctly handle ownership transfer, create_a() would need an adopt
   return value policy. More on this in the `Policies`_ section.

For example, if the following function and class is registered:

::
   
    struct A
    {
        void f();
        void f() const;
    };

    const A* create_a();

    struct B: A {};
    struct C: B {};

    void g(A*);
    void g(B*);

And the following Lua code is executed::

    a1 = create_a()
    a1:f() -- the const version is called

    a2 = A()
    a2:f() -- the non-const version is called

    a = A()
    b = B()
    c = C()

    g(a) -- calls g(A*)
    g(b) -- calls g(B*)
    g(c) -- calls g(B*)


Calling Lua functions
---------------------

To call a Lua function, you can either use ``call_function()`` or
an ``object``.

::

    template<class Ret>
    Ret call_function(lua_State* L, const char* name, ...)
    template<class Ret>
    Ret call_function(object const& obj, ...)

There are two overloads of the ``call_function`` function, one that calls
a function given its name, and one that takes an object that should be a Lua
value that can be called as a function.

The overload that takes a name can only call global Lua functions. The ...
represents a variable number of parameters that are sent to the Lua
function. This function call may throw ``luabind::error`` if the function
call fails.

The return value isn't actually Ret (the template parameter), but a proxy
object that will do the function call. This enables you to give policies to the
call. You do this with the operator[]. You give the policies within the
brackets, like this::

    int ret = call_function<int>(
        L 
      , "a_lua_function"
      , new complex_class()
    )[ adopt(_1) ];

If you want to pass a parameter as a reference, you have to wrap it with the
`Boost.Ref`__.

__ http://www.boost.org/doc/html/ref.html

Like this::

	int ret = call_function(L, "fun", boost::ref(val));


If you want to use a custom error handler for the function call, see
``set_pcall_callback`` under `pcall errorfunc`_.

Using Lua threads
-----------------

To start a Lua thread, you have to call ``lua_resume()``, this means that you
cannot use the previous function ``call_function()`` to start a thread. You have
to use

::

    template<class Ret>
    Ret resume_function(lua_State* L, const char* name, ...)
    template<class Ret>
    Ret resume_function(object const& obj, ...)

and

::

    template<class Ret>
    Ret resume(lua_State* L, ...)

The first time you start the thread, you have to give it a function to execute. i.e. you
have to use ``resume_function``, when the Lua function yields, it will return the first
value passed in to ``lua_yield()``. When you want to continue the execution, you just call
``resume()`` on your ``lua_State``, since it's already executing a function, you don't pass
it one. The parameters to ``resume()`` will be returned by ``yield()`` on the Lua side.

For yielding C++-functions (without the support of passing data back and forth between the
Lua side and the c++ side), you can use the yield_ policy.

With the overload of ``resume_function`` that takes an object_, it is important that the
object was constructed with the thread as its ``lua_State*``. Like this:

.. parsed-literal::

	lua_State* thread = lua_newthread(L);
	object fun = get_global(**thread**)["my_thread_fun"];
	resume_function(fun);


Binding classes to Lua
======================

To register classes you use a class called ``class_``. Its name is supposed to
resemble the C++ keyword, to make it look more intuitive. It has an overloaded
member function ``def()`` that is used to register member functions, operators,
constructors, enums and properties on the class. It will return its
this-pointer, to let you register more members directly.

Let's start with a simple example. Consider the following C++ class::

    class testclass
    {
    public:
        testclass(const std::string& s): m_string(s) {}
        void print_string() { std::cout << m_string << "\n"; }

    private:
        std::string m_string;
    };

To register it with a Lua environment, write as follows (assuming you are using
namespace luabind)::

    module(L)
    [
        class_<testclass>("testclass")
            .def(constructor<const std::string&>())
            .def("print_string", &testclass::print_string)
    ];

This will register the class with the name testclass and constructor that takes
a string as argument and one member function with the name ``print_string``.

::

    Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
    > a = testclass('a string')
    > a:print_string()
    a string

It is also possible to register free functions as member functions. The
requirement on the function is that it takes a pointer, const pointer,
reference or const reference to the class type as the first parameter. The rest
of the parameters are the ones that are visible in Lua, while the object
pointer is given as the first parameter. If we have the following C++ code::

    struct A
    {
        int a;
    };

    int plus(A* o, int v) { return o->a + v; }

You can register ``plus()`` as if it was a member function of A like this::

    class_<A>("A")
        .def("plus", &plus)

``plus()`` can now be called as a member function on A with one parameter, int.
If the object pointer parameter is const, the function will act as if it was a
const member function (it can be called on const objects).


Overloaded member functions
---------------------------

When binding more than one overloads of a member function, or just binding
one overload of an overloaded member function, you have to disambiguate
the member function pointer you pass to ``def``. To do this, you can use an
ordinary C-style cast, to cast it to the right overload. To do this, you have
to know how to express member function types in C++, here's a short tutorial
(for more info, refer to your favorite book on C++).

The syntax for member function pointer follows:

.. parsed-literal::

    *return-value* (*class-name*::\*)(*arg1-type*, *arg2-type*, *...*)

Here's an example illlustrating this::

    struct A
    {
        void f(int);
        void f(int, int);
    };

::

    class_<A>()
        .def("f", (void(A::*)(int))&A::f)

This selects the first overload of the function ``f`` to bind. The second
overload is not bound.


Properties
----------

To register a global data member with a class is easily done. Consider the
following class::

    struct A
    {
        int a;
    };

This class is registered like this::

    module(L)
    [
        class_<A>("A")
            .def_readwrite("a", &A::a)
    ];

This gives read and write access to the member variable ``A::a``. It is also
possible to register attributes with read-only access::

    module(L)
    [
        class_<A>("A")
            .def_readonly("a", &A::a)
    ];

When binding members that are a non-primitive type, the auto generated getter
function will return a reference to it. This is to allow chained .-operators.
For example, when having a struct containing another struct. Like this::

    struct A { int m; };
    struct B { A a; };

When binding ``B`` to lua, the following expression code should work::

    b = B()
    b.a.m = 1
    assert(b.a.m == 1)

This requires the first lookup (on ``a``) to return a reference to ``A``, and
not a copy. In that case, luabind will automatically use the dependency policy
to make the return value dependent on the object in which it is stored. So, if
the returned reference lives longer than all references to the object (b in
this case) it will keep the object alive, to avoid being a dangling pointer.

You can also register getter and setter functions and make them look as if they
were a public data member. Consider the following class::

    class A
    {
    public:
        void set_a(int x) { a = x; }
        int get_a() const { return a; }

    private:
        int a;
    };

It can be registered as if it had a public data member a like this::

    class_<A>("A")
        .property("a", &A::get_a, &A::set_a)

This way the ``get_a()`` and ``set_a()`` functions will be called instead of
just writing  to the data member. If you want to make it read only you can just
omit the last parameter. Please note that the get function **has to be
const**, otherwise it won't compile. This seems to be a common source of errors.


Enums
-----

If your class contains enumerated constants (enums), you can register them as
well to make them available in Lua. Note that they will not be type safe, all
enums are integers in Lua, and all functions that takes an enum, will accept
any integer. You register them like this::

    module(L)
    [
        class_<A>("A")
            .enum_("constants")
            [
                value("my_enum", 4),
                value("my_2nd_enum", 7),
                value("another_enum", 6)
            ]
    ];

In Lua they are accessed like any data member, except that they are read-only
and reached on the class itself rather than on an instance of the class.

::

    Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
    > print(A.my_enum)
    4
    > print(A.another_enum)
    6


Operators
---------

To bind operators you have to include ``<luabind/operator.hpp>``.

The mechanism for registering operators on your class is pretty simple. You use
a global name ``luabind::self`` to refer to the class itself and then you just
write the operator expression inside the ``def()`` call. This class::

    struct vec
    {
        vec operator+(int s);
    };

Is registered like this:

.. parsed-literal::

    module(L)
    [
        class_<vec>("vec")
            .def(**self + int()**)
    ];

This will work regardless if your plus operator is defined inside your class or
as a free function.

If your operator is const (or, when defined as a free function, takes a const
reference to the class itself) you have to use ``const_self`` instead of
``self``. Like this:

.. parsed-literal::

    module(L)
    [
        class_<vec>("vec")
            .def(**const_self** + int())
    ];

The operators supported are those available in Lua:

.. parsed-literal::

    +    -    \*    /    ==    <    <=

This means, no in-place operators. The equality operator (``==``) has a little
hitch; it will not be called if the references are equal. This means that the
``==`` operator has to do pretty much what's it's expected to do.

Lua does not support operators such as ``!=``, ``>`` or ``>=``. That's why you
can only register the operators listed above. When you invoke one of the
mentioned operators, lua will define it in terms of one of the available
operators.

In the above example the other operand type is instantiated by writing
``int()``. If the operand type is a complex type that cannot easily be
instantiated you can wrap the type in a class called ``other<>``. For example:

To register this class, we don't want to instantiate a string just to register
the operator.

::

    struct vec
    {
        vec operator+(std::string);
    };

Instead we use the ``other<>`` wrapper like this:

.. parsed-literal::

    module(L)
    [
        class_<vec>("vec")
            .def(self + **other<std::string>()**)
    ];

To register an application (function call-) operator:

.. parsed-literal::

    module(L)
    [
        class_<vec>("vec")
            .def( **self(int())** )
    ];

There's one special operator. In Lua it's called ``__tostring``, it's not
really an operator. It is used for converting objects to strings in a standard
way in Lua. If you register this functionality, you will be able to use the lua
standard function ``tostring()`` for converting your object to a string.

To implement this operator in C++ you should supply an ``operator<<`` for
std::ostream. Like this example:

.. parsed-literal::

    class number {};
    std::ostream& operator<<(std::ostream&, number&);

    ...
    
    module(L)
    [
        class_<number>("number")
            .def(**tostring(self)**)
    ];


Nested scopes and static functions
----------------------------------

It is possible to add nested scopes to a class. This is useful when you need 
to wrap a nested class, or a static function.

.. parsed-literal::

    class_<foo>("foo")
        .def(constructor<>())
        **.scope
        [
            class_<inner>("nested"),
            def("f", &f)
        ]**;

In this example, ``f`` will behave like a static member function of the class
``foo``, and the class ``nested`` will behave like a nested class of ``foo``.

It's also possible to add namespaces to classes using the same syntax.


Derived classes
---------------
  
If you want to register classes that derives from other classes, you can
specify a template parameter ``bases<>`` to the ``class_`` instantiation. The
following hierarchy::
   
    struct A {};
    struct B : A {};

Would be registered like this::

    module(L)
    [
        class_<A>("A"),
        class_<B, A>("B")
    ];

If you have multiple inheritance you can specify more than one base. If B would
also derive from a class C, it would be registered like this::

    module(L)
    [
        class_<B, bases<A, C> >("B")
    ];

Note that you can omit ``bases<>`` when using single inheritance.

.. note::
   If you don't specify that classes derive from each other, luabind will not
   be able to implicitly cast pointers between the types.


Smart pointers
--------------

When registering a class you can tell luabind to hold all instances
explicitly created in Lua in a specific smart pointer type, rather than
the default raw pointer. This is done by passing an additional template
parameter to ``class_``:

.. parsed-literal::

    class_<X, **P**>(|...|)

Where the requirements of ``P`` are:

======================== =======================================
Expression               Returns
======================== =======================================
``P(raw)``
``get_pointer(p)``       Convertible to ``X*``
======================== =======================================

where:

* ``raw`` is of type ``X*``
* ``p`` is an instance of ``P``

``get_pointer()`` overloads are provided for the smart pointers in
Boost, and ``std::auto_ptr<>``. Should you need to provide your own
overload, note that it is called unqualified and is expected to be found
by *argument dependent lookup*. Thus it should be defined in the same
namespace as the pointer type it operates on.

For example:

.. parsed-literal::

    class_<X, **boost::scoped_ptr<X>** >("X")
      .def(constructor<>())

Will cause luabind to hold any instance created on the Lua side in a
``boost::scoped_ptr<X>``. Note that this doesn't mean **all** instances
will be held by a ``boost::scoped_ptr<X>``. If, for example, you
register a function::

    std::auto_ptr<X> make_X();

the instance returned by that will be held in ``std::auto_ptr<X>``. This
is handled automatically for all smart pointers that implement a
``get_pointer()`` overload.

.. important::

    ``get_const_holder()`` has been removed. Automatic conversions
    between ``smart_ptr<X>`` and ``smart_ptr<X const>`` no longer work.

.. important::

    ``__ok``  has been removed. Similar functionality can be implemented
    for specific pointer types by doing something along the lines of:

    .. parsed-literal::

      bool is_non_null(std::auto_ptr<X> const& p)
      {
          return p.get();
      }

      |...|

      def("is_non_null", &is_non_null)

When registering a hierarchy of classes, where all instances are to be held
by a smart pointer, all the classes should have the baseclass' holder type.
Like this:

.. parsed-literal::

        module(L)
        [
            class_<base, boost::shared_ptr<base> >("base")
                .def(constructor<>()),
            class_<derived, base, **boost::shared_ptr<base>** >("base")
                .def(constructor<>())
        ];

Internally, luabind will do the necessary conversions on the raw pointers, which
are first extracted from the holder type.


Splitting class registrations
-----------------------------

In some situations it may be desirable to split a registration of a class
across different compilation units. Partly to save rebuild time when changing
in one part of the binding, and in some cases compiler limits may force you
to split it. To do this is very simple. Consider the following sample code::

    void register_part1(class_<X>& x)
    {
        x.def(/*...*/);
    }

    void register_part2(class_<X>& x)
    {
        x.def(/*...*/);
    }

    void register_(lua_State* L)
    {
        class_<X> x("x");

        register_part1(x);
        register_part2(x);

        module(L) [ x ];
    }

Here, the class ``X`` is registered in two steps. The two functions
``register_part1`` and ``register_part2`` may be put in separate compilation
units.

To separate the module registration and the classes to be registered, see
`Splitting up the registration`_.


Adding converters for user defined types
========================================

It is possible to get luabind to handle user defined types like it does
the built in types by specializing ``luabind::default_converter<>``:

::

  struct int_wrapper
  {
      int_wrapper(int value)
        : value(value)
      {}

      int value;
  };

  namespace luabind
  {
      template <>
      struct default_converter<X>
        : native_converter_base<X>
      {
          static int compute_score(lua_State* L, int index)
          {
              return lua_type(L, index) == LUA_TNUMBER ? 0 : -1;
          }

          X from(lua_State* L, int index)
          {
              return X(lua_tonumber(L, index));
          }

          void to(lua_State* L, X const& x)
          {
              lua_pushnumber(L, x.value);
          }
      };

      template <>
      struct default_converter<X const&>
        : default_converter<X>
      {};
  }

Note that ``default_converter<>`` is instantiated for the actual argument and
return types of the bound functions. In the above example, we add a
specialization for ``X const&`` that simply forwards to the ``X`` converter.
This lets us export functions which accept ``X`` by const reference.

``native_converter_base<>`` should be used as the base class for the
specialized converters. It simplifies the converter interface, and
provides a mean for backward compatibility since the underlying
interface is in flux.


Binding function objects with explicit signatures
=================================================

Using ``luabind::tag_function<>`` it is possible to export function objects
from which luabind can't automatically deduce a signature. This can be used to
slightly alter the signature of a bound function, or even to bind stateful
function objects.

Synopsis:

.. parsed-literal::

  template <class Signature, class F>
  *implementation-defined* tag_function(F f);

Where ``Signature`` is a function type describing the signature of ``F``.
It can be used like this::

  int f(int x);

  // alter the signature so that the return value is ignored
  def("f", tag_function<void(int)>(f));

  struct plus
  {
      plus(int x)
        : x(x)
      {}

      int operator()(int y) const
      {
          return x + y;
      }
  };

  // bind a stateful function object
  def("plus3", tag_function<int(int)>(plus(3)));


Object
======

Since functions have to be able to take Lua values (of variable type) we need a
wrapper around them. This wrapper is called ``luabind::object``. If the
function you register takes an object, it will match any Lua value. To use it,
you need to include ``<luabind/object.hpp>``.

.. topic:: Synopsis

    .. parsed-literal::

        class object
        {
        public:
            template<class T>
            object(lua_State\*, T const& value);
            object(from_stack const&);
            object(object const&);
            object();

            ~object();

            lua_State\* interpreter() const;
            void push() const;
            bool is_valid() const;
            operator *safe_bool_type* () const;

            template<class Key>
            *implementation-defined* operator[](Key const&);

            template<class T>
            object& operator=(T const&);
            object& operator=(object const&);

            bool operator==(object const&) const;
            bool operator<(object const&) const;
            bool operator<=(object const&) const;
            bool operator>(object const&) const;
            bool operator>=(object const&) const;
            bool operator!=(object const&) const;

            template <class T>
            *implementation-defined* operator[](T const& key) const

            void swap(object&);

            *implementation-defined* operator()();

            template<class A0>
            *implementation-defined* operator()(A0 const& a0);

            template<class A0, class A1>
            *implementation-defined* operator()(A0 const& a0, A1 const& a1);

            /\* ... \*/
        };

When you have a Lua object, you can assign it a new value with the assignment
operator (=). When you do this, the ``default_policy`` will be used to make the
conversion from C++ value to Lua. If your ``luabind::object`` is a table you
can access its members through the operator[] or the Iterators_. The value
returned from the operator[] is a proxy object that can be used both for
reading and writing values into the table (using operator=).

Note that it is impossible to know if a Lua value is indexable or not
(``lua_gettable`` doesn't fail, it succeeds or crashes). This means that if
you're trying to index something that cannot be indexed, you're on your own.
Lua will call its ``panic()`` function. See `lua panic`_.

There are also free functions that can be used for indexing the table, see
`Related functions`_.

The constructor that takes a ``from_stack`` object is used when you want to
initialize the object with a value from the lua stack. The ``from_stack``
type has the following constructor::

	 from_stack(lua_State* L, int index);

The index is an ordinary lua stack index, negative values are indexed from the
top of the stack. You use it like this::

	 object o(from_stack(L, -1));

This will create the object ``o`` and copy the value from the top of the lua stack.

The ``interpreter()`` function returns the Lua state where this object is stored.
If you want to manipulate the object with Lua functions directly you can push
it onto the Lua stack by calling ``push()``.

The operator== will call lua_equal() on the operands and return its result.

The ``is_valid()`` function tells you whether the object has been initialized
or not. When created with its default constructor, objects are invalid. To make
an object valid, you can assign it a value. If you want to invalidate an object
you can simply assign it an invalid object.

The ``operator safe_bool_type()`` is equivalent to ``is_valid()``. This means
that these snippets are equivalent::

    object o;
    // ...
    if (o)
    {
        // ...
    }

    ...

    object o;
    // ...
    if (o.is_valid())
    {
        // ...
    }

The application operator will call the value as if it was a function. You can
give it any number of parameters (currently the ``default_policy`` will be used
for the conversion). The returned object refers to the return value (currently
only one return value is supported). This operator may throw ``luabind::error``
if the function call fails. If you want to specify policies to your function
call, you can use index-operator (operator[]) on the function call, and give
the policies within the [ and ]. Like this::

    my_function_object(
        2
      , 8
      , new my_complex_structure(6)
    ) [ adopt(_3) ];

This tells luabind to make Lua adopt the ownership and responsibility for the
pointer passed in to the lua-function.

It's important that all instances of object have been destructed by the time
the Lua state is closed. The object will keep a pointer to the lua state and
release its Lua object in its destructor.

Here's an example of how a function can use a table::

    void my_function(object const& table)
    {
        if (type(table) == LUA_TTABLE)
        {
            table["time"] = std::clock();
            table["name"] = std::rand() < 500 ? "unusual" : "usual";

            std::cout << object_cast<std::string>(table[5]) << "\n";
        }
    }

If you take a ``luabind::object`` as a parameter to a function, any Lua value
will match that parameter. That's why we have to make sure it's a table before
we index into it.

::

    std::ostream& operator<<(std::ostream&, object const&);

There's a stream operator that makes it possible to print objects or use
``boost::lexical_cast`` to convert it to a string. This will use lua's string
conversion function. So if you convert a C++ object with a ``tostring``
operator, the stream operator for that type will be used.

Iterators
---------

There are two kinds of iterators. The normal iterator that will use the metamethod
of the object (if there is any) when the value is retrieved. This iterator is simply
called ``luabind::iterator``. The other iterator is called ``luabind::raw_iterator``
and will bypass the metamethod and give the true contents of the table. They have
identical interfaces, which implements the ForwardIterator_ concept. Apart from
the members of standard iterators, they have the following members and constructors:

.. _ForwardIterator: http://www.sgi.com/tech/stl/ForwardIterator.html

.. parsed-literal::

    class iterator
    {
        iterator();
        iterator(object const&);

        object key() const;

        *standard iterator members*
    };

The constructor that takes a ``luabind::object`` is actually a template that can be
used with object. Passing an object as the parameter to the iterator will
construct the iterator to refer to the first element in the object.

The default constructor will initialize the iterator to the one-past-end
iterator. This is used to test for the end of the sequence.

The value type of the iterator is an implementation defined proxy type which
supports the same operations as ``luabind::object``. Which means that in most
cases you can just treat it as an ordinary object. The difference is that any
assignments to this proxy will result in the value being inserted at the
iterators position, in the table.
 
The ``key()`` member returns the key used by the iterator when indexing the
associated Lua table.

An example using iterators::

    for (iterator i(globals(L)["a"]), end; i != end; ++i)
    {
      *i = 1;
    }

The iterator named ``end`` will be constructed using the default constructor
and hence refer to the end of the sequence. This example will simply iterate
over the entries in the global table ``a`` and set all its values to 1.

Related functions
-----------------

There are a couple of functions related to objects and tables.

::

    int type(object const&);

This function will return the lua type index of the given object.
i.e. ``LUA_TNIL``, ``LUA_TNUMBER`` etc.

::

    template<class T, class K>
    void settable(object const& o, K const& key, T const& value);
    template<class K>
    object gettable(object const& o, K const& key);
    template<class T, class K>
    void rawset(object const& o, K const& key, T const& value);
    template<class K>
    object rawget(object const& o, K const& key);

These functions are used for indexing into tables. ``settable`` and ``gettable``
translates into calls to ``lua_settable`` and ``lua_gettable`` respectively. Which
means that you could just as well use the index operator of the object.

``rawset`` and ``rawget`` will translate into calls to ``lua_rawset`` and
``lua_rawget`` respectively. So they will bypass any metamethod and give you the
true value of the table entry.

::

    template<class T>
    T object_cast<T>(object const&);
    template<class T, class Policies>
    T object_cast<T>(object const&, Policies);

    template<class T>
    boost::optional<T> object_cast_nothrow<T>(object const&);
    template<class T, class Policies>
    boost::optional<T> object_cast_nothrow<T>(object const&, Policies);

The ``object_cast`` function casts the value of an object to a C++ value.
You can supply a policy to handle the conversion from lua to C++. If the cast
cannot be made a ``cast_failed`` exception will be thrown. If you have
defined LUABIND_NO_ERROR_CHECKING (see `Build options`_) no checking will occur,
and if the cast is invalid the application may very well crash. The nothrow
versions will return an uninitialized ``boost::optional<T>`` object, to
indicate that the cast could not be performed.

The function signatures of all of the above functions are really templates
for the object parameter, but the intention is that you should only pass
objects in there, that's why it's left out of the documentation.

::

    object globals(lua_State*);
    object registry(lua_State*);

These functions return the global environment table and the registry table respectively.

::

  object newtable(lua_State*);

This function creates a new table and returns it as an object.

::

  object getmetatable(object const& obj);
  void setmetatable(object const& obj, object const& metatable);

These functions get and set the metatable of a Lua object.

::

  lua_CFunction tocfunction(object const& value);
  template <class T> T* touserdata(object const& value)

These extract values from the object at a lower level than ``object_cast()``.

::

  object getupvalue(object const& function, int index);
  void setupvalue(object const& function, int index, object const& value);

These get and set the upvalues of ``function``.

Assigning nil
-------------

To set a table entry to ``nil``, you can use ``luabind::nil``. It will avoid
having to take the detour by first assigning ``nil`` to an object and then
assign that to the table entry. It will simply result in a ``lua_pushnil()``
call, instead of copying an object.

Example::

  using luabind;
  object table = newtable(L);
  table["foo"] = "bar";
  
  // now, clear the "foo"-field
  table["foo"] = nil;


Defining classes in Lua
=======================

In addition to binding C++ functions and classes with Lua, luabind also provide
an OO-system in Lua. ::

    class 'lua_testclass'

    function lua_testclass:__init(name)
        self.name = name
    end

    function lua_testclass:print()
        print(self.name)
    end

    a = lua_testclass('example')
    a:print()


Inheritance can be used between lua-classes::

    class 'derived' (lua_testclass)

    function derived:__init()
        lua_testclass.__init(self, 'derived name')
    end

    function derived:print()
        print('Derived:print() -> ')
        lua_testclass.print(self)
    end

The base class is initialized explicitly by calling its ``__init()``
function.

As you can see in this example, you can call the base class member functions.
You can find all member functions in the base class, but you will have to give
the this-pointer (``self``) as first argument.


Deriving in lua
---------------

It is also possible to derive Lua classes from C++ classes, and override
virtual functions with Lua functions. To do this we have to create a wrapper
class for our C++ base class. This is the class that will hold the Lua object
when we instantiate a Lua class.

::

    class base
    {
    public:
        base(const char* s)
        { std::cout << s << "\n"; }

        virtual void f(int a) 
        { std::cout << "f(" << a << ")\n"; }
    };

    struct base_wrapper : base, luabind::wrap_base
    {
        base_wrapper(const char* s)
            : base(s) 
        {}

        virtual void f(int a) 
        { 
            call<void>("f", a); 
        }

        static void default_f(base* ptr, int a)
        {
            return ptr->base::f(a);
        }
    };

    ...

    module(L)
    [
        class_<base, base_wrapper>("base")
            .def(constructor<const char*>())
            .def("f", &base::f, &base_wrapper::default_f)
    ];

.. Important::
    Since MSVC6.5 doesn't support explicit template parameters
    to member functions, instead of using the member function ``call()``
    you call a free function ``call_member()`` and pass the this-pointer
    as first parameter.

Note that if you have both base classes and a base class wrapper, you must give
both bases and the base class wrapper type as template parameter to 
``class_`` (as done in the example above). The order in which you specify
them is not important. You must also register both the static version and the
virtual version of the function from the wrapper, this is necessary in order
to allow luabind to use both dynamic and static dispatch when calling the function.

.. Important::
    It is extremely important that the signatures of the static (default) function
    is identical to the virtual function. The fact that one of them is a free
    function and the other a member function doesn't matter, but the parameters
    as seen from lua must match. It would not have worked if the static function
    took a ``base_wrapper*`` as its first argument, since the virtual function
    takes a ``base*`` as its first argument (its this pointer). There's currently
    no check in luabind to make sure the signatures match.

If we didn't have a class wrapper, it would not be possible to pass a Lua class
back to C++. Since the entry points of the virtual functions would still point
to the C++ base class, and not to the functions defined in Lua. That's why we
need one function that calls the base class' real function (used if the lua
class doesn't redefine it) and one virtual function that dispatches the call
into luabind, to allow it to select if a Lua function should be called, or if
the original function should be called. If you don't intend to derive from a
C++ class, or if it doesn't have any virtual member functions, you can register
it without a class wrapper.

You don't need to have a class wrapper in order to derive from a class, but if
it has virtual functions you may have silent errors. 

.. Unnecessary? The rule of thumb is: 
  If your class has virtual functions, create a wrapper type, if it doesn't
  don't create a wrapper type.

The wrappers must derive from ``luabind::wrap_base``, it contains a Lua reference
that will hold the Lua instance of the object to make it possible to dispatch
virtual function calls into Lua. This is done through an overloaded member function::

    template<class Ret>
    Ret call(char const* name, ...)

Its used in a similar way as ``call_function``, with the exception that it doesn't
take a ``lua_State`` pointer, and the name is a member function in the Lua class.

.. warning::

	The current implementation of ``call_member`` is not able to distinguish const
	member functions from non-const. If you have a situation where you have an overloaded
	virtual function where the only difference in their signatures is their constness, the
	wrong overload will be called by ``call_member``. This is rarely the case though.

Object identity
~~~~~~~~~~~~~~~

When a pointer or reference to a registered class with a wrapper is passed
to Lua, luabind will query for it's dynamic type. If the dynamic type
inherits from ``wrap_base``, object identity is preserved.

::

    struct A { .. };
    struct A_wrap : A, wrap_base { .. };

    A* f(A* ptr) { return ptr; }

    module(L)
    [
        class_<A, A_wrap>("A"),
        def("f", &f)
    ];

::

    > class 'B' (A)
    > x = B()
    > assert(x == f(x)) -- object identity is preserved when object is
                        -- passed through C++

This functionality relies on RTTI being enabled (that ``LUABIND_NO_RTTI`` is
not defined).

Overloading operators
---------------------

You can overload most operators in Lua for your classes. You do this by simply
declaring a member function with the same name as an operator (the name of the
metamethods in Lua). The operators you can overload are:

 - ``__add``
 - ``__sub`` 
 - ``__mul`` 
 - ``__div`` 
 - ``__pow`` 
 - ``__lt`` 
 - ``__le`` 
 - ``__eq`` 
 - ``__call`` 
 - ``__unm`` 
 - ``__tostring``
 - ``__len``

``__tostring`` isn't really an operator, but it's the metamethod that is called
by the standard library's ``tostring()`` function. There's one strange behavior
regarding binary operators. You are not guaranteed that the self pointer you
get actually refers to an instance of your class. This is because Lua doesn't
distinguish the two cases where you get the other operand as left hand value or
right hand value. Consider the following examples::

    class 'my_class'

      function my_class:__init(v)
          self.val = v
      end
        
      function my_class:__sub(v)
          return my_class(self.val - v.val)
      end

      function my_class:__tostring()
          return self.val
      end

This will work well as long as you only subtracts instances of my_class with
each other. But If you want to be able to subtract ordinary numbers from your
class too, you have to manually check the type of both operands, including the
self object. ::

    function my_class:__sub(v)
        if (type(self) == 'number') then
            return my_class(self - v.val)

        elseif (type(v) == 'number') then
            return my_class(self.val - v)
        
        else
            -- assume both operands are instances of my_class
            return my_class(self.val - v.val)

        end
    end

The reason why ``__sub`` is used as an example is because subtraction is not
commutative (the order of the operands matters). That's why luabind cannot
change order of the operands to make the self reference always refer to the
actual class instance.

If you have two different Lua classes with an overloaded operator, the operator
of the right hand side type will be called. If the other operand is a C++ class
with the same operator overloaded, it will be prioritized over the Lua class'
operator. If none of the C++ overloads matches, the Lua class operator will be
called.


Finalizers
----------

If an object needs to perform actions when it's collected we provide a
``__finalize`` function that can be overridden in lua-classes. The
``__finalize`` functions will be called on all classes in the inheritance
chain, starting with the most derived type. ::

    ...

    function lua_testclass:__finalize()
        -- called when the an object is collected
    end


Slicing
-------

If your lua C++ classes don't have wrappers (see `Deriving in lua`_) and
you derive from them in lua, they may be sliced. Meaning, if an object
is passed into C++ as a pointer to its base class, the lua part will be
separated from the C++ base part. This means that if you call virtual
functions on that C++ object, they will not be dispatched to the lua
class. It also means that if you adopt the object, the lua part will be
garbage collected.

::

	+--------------------+
	| C++ object         |    <- ownership of this part is transferred
	|                    |       to c++ when adopted
	+--------------------+
	| lua class instance |    <- this part is garbage collected when
	| and lua members    |       instance is adopted, since it cannot
	+--------------------+       be held by c++. 


The problem can be illustrated by this example::

    struct A {};

    A* filter_a(A* a) { return a; }
    void adopt_a(A* a) { delete a; }


::

    using namespace luabind;

    module(L)
    [
        class_<A>("A"),
        def("filter_a", &filter_a),
        def("adopt_a", &adopt_a, adopt(_1))
    ]


In lua::

    a = A()
    b = filter_a(a)
    adopt_a(b)

In this example, lua cannot know that ``b`` actually is the same object as
``a``, and it will therefore consider the object to be owned by the C++ side.
When the ``b`` pointer then is adopted, a runtime error will be raised because
an object not owned by lua is being adopted to C++.

If you have a wrapper for your class, none of this will happen, see
`Object identity`_.


Exceptions
==========

If any of the functions you register throws an exception when called, that
exception will be caught by luabind and converted to an error string and
``lua_error()`` will be invoked. If the exception is a ``std::exception`` or a
``const char*`` the string that is pushed on the Lua stack, as error message,
will be the string returned by ``std::exception::what()`` or the string itself
respectively. If the exception is unknown, a generic string saying that the
function threw an exception will be pushed.

If you have an exception type that isn't derived from
``std::exception``, or you wish to change the error message from the
default result of ``what()``, it is possible to register custom
exception handlers::

  struct my_exception
  {};

  void translate_my_exception(lua_State* L, my_exception const&)
  {
      lua_pushstring(L, "my_exception");
  }

  …

  luabind::register_exception_handler<my_exception>(&translate_my_exception);

``translate_my_exception()`` will be called by luabind whenever a
``my_exception`` is caught. ``lua_error()`` will be called after the
handler function returns, so it is expected that the function will push
an error string on the stack.

Any function that invokes Lua code may throw ``luabind::error``. This exception
means that a Lua run-time error occurred. The error message is found on top of
the Lua stack. The reason why the exception doesn't contain the error string
itself is because it would then require heap allocation which may fail. If an
exception class throws an exception while it is being thrown itself, the
application will be terminated.

Error's synopsis is::

    class error : public std::exception
    {
    public:
        error(lua_State*);
        lua_State* state() const throw();
        virtual const char* what() const throw();
    };

The state function returns a pointer to the Lua state in which the error was
thrown. This pointer may be invalid if you catch this exception after the lua
state is destructed. If the Lua state is valid you can use it to retrieve the
error message from the top of the Lua stack.

An example of where the Lua state pointer may point to an invalid state
follows::

    struct lua_state
    {
        lua_state(lua_State* L): m_L(L) {}
        ~lua_state() { lua_close(m_L); }
        operator lua_State*() { return m_L; }
        lua_State* m_L;
    };

    int main()
    {
        try
        {
            lua_state L = lua_open();
            /* ... */
        }
        catch(luabind::error& e)
        {
            lua_State* L = e.state();
            // L will now point to the destructed
            // Lua state and be invalid
            /* ... */
        }
    }

There's another exception that luabind may throw: ``luabind::cast_failed``,
this exception is thrown from ``call_function<>`` or ``call_member<>``. It
means that the return value from the Lua function couldn't be converted to
a C++ value. It is also thrown from ``object_cast<>`` if the cast cannot
be made.

The synopsis for ``luabind::cast_failed`` is::

    class cast_failed : public std::exception
    {
    public:
        cast_failed(lua_State*);
        lua_State* state() const throw();
        LUABIND_TYPE_INFO info() const throw();
        virtual const char* what() const throw();
    };

Again, the state member function returns a pointer to the Lua state where the
error occurred. See the example above to see where this pointer may be invalid.

The info member function returns the user defined ``LUABIND_TYPE_INFO``, which
defaults to a ``const std::type_info*``. This type info describes the type that
we tried to cast a Lua value to.

If you have defined ``LUABIND_NO_EXCEPTIONS`` none of these exceptions will be
thrown, instead you can set two callback functions that are called instead.
These two functions are only defined if ``LUABIND_NO_EXCEPTIONS`` are defined.

::

    luabind::set_error_callback(void(*)(lua_State*))

The function you set will be called when a runtime-error occur in Lua code. You
can find an error message on top of the Lua stack. This function is not
expected to return, if it does luabind will call ``std::terminate()``.

::

    luabind::set_cast_failed_callback(void(*)(lua_State*, LUABIND_TYPE_INFO))

The function you set is called instead of throwing ``cast_failed``. This function
is not expected to return, if it does luabind will call ``std::terminate()``.


Policies
========

Sometimes it is necessary to control how luabind passes arguments and return
value, to do this we have policies. All policies use an index to associate
them with an argument in the function signature. These indices are ``result`` 
and ``_N`` (where ``N >= 1``). When dealing with member functions ``_1`` refers
to the ``this`` pointer.

.. contents:: Policies currently implemented
    :local:
    :depth: 1

.. include:: adopt.rst
.. include:: dependency.rst
.. include:: out_value.rst
.. include:: pure_out_value.rst
.. include:: return_reference_to.rst
.. include:: copy.rst
.. include:: discard_result.rst
.. include:: return_stl_iterator.rst
.. include:: raw.rst
.. include:: yield.rst

..  old policies section
    ===================================================

    Copy
    ----

    This will make a copy of the parameter. This is the default behavior when
    passing parameters by-value. Note that this can only be used when passing from
    C++ to Lua. This policy requires that the parameter type has a copy
    constructor.

    To use this policy you need to include ``luabind/copy_policy.hpp``.


    Adopt
    -----

    This will transfer ownership of the parameter.

    Consider making a factory function in C++ and exposing it to lua::

        base* create_base()
        {
            return new base();
        }

        ...

        module(L)
        [
            def("create_base", create_base)
        ];

    Here we need to make sure Lua understands that it should adopt the pointer
    returned by the factory-function. This can be done using the adopt-policy.

    ::

        module(L)
        [
            def(L, "create_base", adopt(return_value))
        ];

    To specify multiple policies we just separate them with '+'.

    ::

        base* set_and_get_new(base* ptr)
        {
            base_ptrs.push_back(ptr);
            return new base();
        }

        module(L)
        [
            def("set_and_get_new", &set_and_get_new, 
                adopt(return_value) + adopt(_1))
        ];

    When Lua adopts a pointer, it will call delete on it. This means that it cannot
    adopt pointers allocated with another allocator than new (no malloc for
    example).

    To use this policy you need to include ``luabind/adopt_policy.hpp``.


    Dependency
    ----------

    The dependency policy is used to create life-time dependencies between values.
    Consider the following example::

        struct A
        {
            B member;

            const B& get_member()
            {
                return member;
            }
        };

    When wrapping this class, we would do something like::

        module(L)
        [
            class_<A>("A")
                .def(constructor<>())
                .def("get_member", &A::get_member)
        ];


    However, since the return value of get_member is a reference to a member of A,
    this will create some life-time issues. For example::

        Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
        a = A()
        b = a:get_member() -- b points to a member of a
        a = nil
        collectgarbage(0)  -- since there are no references left to a, it is
                           -- removed
                           -- at this point, b is pointing into a removed object

    When using the dependency-policy, it is possible to tell luabind to tie the
    lifetime of one object to another, like this::

        module(L)
        [
            class_<A>("A")
                .def(constructor<>())
                .def("get_member", &A::get_member, dependency(result, _1))
        ];

    This will create a dependency between the return-value of the function, and the
    self-object. This means that the self-object will be kept alive as long as the
    result is still alive. ::

        Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
        a = A()
        b = a:get_member() -- b points to a member of a
        a = nil
        collectgarbage(0)  -- a is dependent on b, so it isn't removed
        b = nil
        collectgarbage(0)  -- all dependencies to a gone, a is removed

    To use this policy you need to include ``luabind/dependency_policy.hpp``.


    Return reference to
    -------------------

    It is very common to return references to arguments or the this-pointer to
    allow for chaining in C++.

    ::

        struct A
        {
            float val;

            A& set(float v)
            {
                val = v;
                return *this;
            }
        };

    When luabind generates code for this, it will create a new object for the
    return-value, pointing to the self-object. This isn't a problem, but could be a
    bit inefficient. When using the return_reference_to-policy we have the ability
    to tell luabind that the return-value is already on the Lua stack.

    ::

        module(L)
        [
            class_<A>("A")
                .def(constructor<>())
                .def("set", &A::set, return_reference_to(_1))
        ];

    Instead of creating a new object, luabind will just copy the object that is
    already on the stack.

    .. warning:: 
       This policy ignores all type information and should be used only it 
       situations where the parameter type is a perfect match to the 
       return-type (such as in the example).

    To use this policy you need to include ``luabind/return_reference_to_policy.hpp``.


    Out value
    ---------

    This policy makes it possible to wrap functions that take non const references
    as its parameters with the intention to write return values to them.

    ::

        void f(float& val) { val = val + 10.f; }

    or

    ::

        void f(float* val) { *val = *val + 10.f; }

    Can be wrapped by doing::

        module(L)
        [
            def("f", &f, out_value(_1))
        ];

    When invoking this function from Lua it will return the value assigned to its 
    parameter.

    ::

        Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
        > a = f(10)
        > print(a)
        20

    When this policy is used in conjunction with user define types we often need 
    to do ownership transfers.

    ::

        struct A;

        void f1(A*& obj) { obj = new A(); }
        void f2(A** obj) { *obj = new A(); }

    Here we need to make sure luabind takes control over object returned, for 
    this we use the adopt policy::

        module(L)
        [
            class_<A>("A"),
            def("f1", &f1, out_value(_1, adopt(_2)))
            def("f2", &f2, out_value(_1, adopt(_2)))
        ];

    Here we are using adopt as an internal policy to out_value. The index 
    specified, _2, means adopt will be used to convert the value back to Lua. 
    Using _1 means the policy will be used when converting from Lua to C++.

    To use this policy you need to include ``luabind/out_value_policy.hpp``.

    Pure out value
    --------------

    This policy works in exactly the same way as out_value, except that it 
    replaces the parameters with default-constructed objects.

    ::

        void get(float& x, float& y)
        {
            x = 3.f;
            y = 4.f;
        }

        ...

        module(L)
        [
            def("get", &get, 
                pure_out_value(_1) + pure_out_value(_2))
        ];

    ::

        Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
        > x, y = get()
        > print(x, y)
        3    5

    Like out_value, it is possible to specify an internal policy used then 
    converting the values back to Lua.

    ::

        void get(test_class*& obj)
        {
            obj = new test_class();
        }

        ...

        module(L)
        [
            def("get", &get, pure_out_value(_1, adopt(_1)))
        ];


    Discard result
    --------------

    This is a very simple policy which makes it possible to throw away 
    the value returned by a C++ function, instead of converting it to 
    Lua. This example makes sure the this reference never gets converted 
    to Lua.

    ::

        struct simple
        {
            simple& set_name(const std::string& n)
            {
                name = n;
                return *this;
            }

            std::string name;
        };

        ...

        module(L)
        [
            class_<simple>("simple")
                .def("set_name", &simple::set_name, discard_result)
        ];

    To use this policy you need to include ``luabind/discard_result_policy.hpp``.


    Return STL iterator
    -------------------

    This policy converts an STL container to a generator function that can be used
    in Lua to iterate over the container. It works on any container that defines
    ``begin()`` and ``end()`` member functions (they have to return iterators). It
    can be used like this::

        struct A
        {
            std::vector<std::string> names;
        };


        module(L)
        [
            class_<A>("A")
                .def_readwrite("names", &A::names, return_stl_iterator)
        ];

    The Lua code to iterate over the container::

        a = A()

        for name in a.names do
          print(name)
        end


    To use this policy you need to include ``luabind/iterator_policy.hpp``.


    Yield
    -----    

    This policy will cause the function to always yield the current thread when 
    returning. See the Lua manual for restrictions on yield.


Splitting up the registration
=============================

It is possible to split up a module registration into several
translation units without making each registration dependent
on the module it's being registered in.

``a.cpp``::

    luabind::scope register_a()
    {
        return 
            class_<a>("a")
                .def("f", &a::f)
                ;
    }

``b.cpp``::

    luabind::scope register_b()
    {
        return 
            class_<b>("b")
                .def("g", &b::g)
                ;
    }

``module_ab.cpp``::

    luabind::scope register_a();
    luabind::scope register_b();

    void register_module(lua_State* L)
    {
        module("b", L)
        [
            register_a(),
            register_b()
        ];
    }


Error Handling
==============

pcall errorfunc
---------------

As mentioned in the `Lua documentation`_, it is possible to pass an
error handler function to ``lua_pcall()``. Luabind makes use of 
``lua_pcall()`` internally when calling member functions and free functions.
It is possible to set the error handler function that Luabind will use
globally::

    typedef int(*pcall_callback_fun)(lua_State*);
    void set_pcall_callback(pcall_callback_fun fn);

This is primarily useful for adding more information to the error message
returned by a failed protected call. For more information on how to use the
pcall_callback function, see ``errfunc`` under the
`pcall section of the lua manual`_.

For more information on how to retrieve debugging information from lua, see
`the debug section of the lua manual`_.

The message returned by the ``pcall_callback`` is accessable as the top lua
value on the stack. For example, if you would like to access it as a luabind
object, you could do like this::

    catch(error& e)
    {
        object error_msg(from_stack(e.state(), -1));
        std::cout << error_msg << std::endl;
    }

.. _Lua documentation: http://www.lua.org/manual/5.0/manual.html
.. _`pcall section of the lua manual`: http://www.lua.org/manual/5.0/manual.html#3.15
.. _`the debug section of the lua manual`: http://www.lua.org/manual/5.0/manual.html#4

file and line numbers
---------------------

If you want to add file name and line number to the error messages generated
by luabind you can define your own `pcall errorfunc`_. You may want to modify
this callback to better suit your needs, but the basic functionality could be
implemented like this::

   int add_file_and_line(lua_State* L)
   {
      lua_Debug d;
      lua_getstack(L, 1, &d);
      lua_getinfo(L, "Sln", &d);
      std::string err = lua_tostring(L, -1);
      lua_pop(L, 1);
      std::stringstream msg;
      msg << d.short_src << ":" << d.currentline;

      if (d.name != 0)
      {
         msg << "(" << d.namewhat << " " << d.name << ")";
      }
      msg << " " << err;
      lua_pushstring(L, msg.str().c_str());
      return 1;
   }

For more information about what kind of information you can add to the error
message, see `the debug section of the lua manual`_.

Note that the callback set by ``set_pcall_callback()`` will only be used when
luabind executes lua code. Anytime when you call ``lua_pcall`` yourself, you
have to supply your function if you want error messages translated.

lua panic
---------

When lua encounters a fatal error caused by a bug from the C/C++ side, it will
call its internal panic function. This can happen, for example,  when you call
``lua_gettable`` on a value that isn't a table. If you do the same thing from
within lua, it will of course just fail with an error message.

The default panic function will ``exit()`` the application. If you want to
handle this case without terminating your application, you can define your own
panic function using ``lua_atpanic``. The best way to continue from the panic
function is to make sure lua is compiled as C++ and throw an exception from
the panic function. Throwing an exception instead of using ``setjmp`` and
``longjmp`` will make sure the stack is correctly unwound.

When the panic function is called, the lua state is invalid, and the only
allowed operation on it is to close it.

For more information, see the `lua manual section 3.19`_.

.. _`lua manual section 3.19`: http://www.lua.org/manual/5.0/manual.html#3.19

structured exceptions (MSVC)
----------------------------

Since lua is generally built as a C library, any callbacks called from lua
cannot under any circumstance throw an exception. Because of that, luabind has
to catch all exceptions and translate them into proper lua errors (by calling
``lua_error()``). This means we have a ``catch(...) {}`` in there.

In Visual Studio, ``catch (...)`` will not only catch C++ exceptions, it will
also catch structured exceptions, such as segmentation fault. This means that if
your function, that gets called from luabind, makes an invalid memory
adressing, you won't notice it. All that will happen is that lua will return
an error message saying "unknown exception".

To remedy this, you can create your own *exception translator*::

   void straight_to_debugger(unsigned int, _EXCEPTION_POINTERS*)
   { throw; }

   #ifdef _MSC_VER
      ::_set_se_translator(straight_to_debugger);
   #endif

This will make structured exceptions, like segmentation fault, to actually get
caught by the debugger.


Error messages
--------------

These are the error messages that can be generated by luabind, with a more
in-depth explanation.

- .. parsed-literal::

    the attribute '*class-name.attribute-name*' is read only

  There is no data member named *attribute-name* in the class *class-name*,
  or there's no setter-function registered on that property name. See the 
  Properties_ section.

- .. parsed-literal:: 

    the attribute '*class-name.attribute-name*' is of type: (*class-name*) and does not match (*class_name*)

  This error is generated if you try to assign an attribute with a value 
  of a type that cannot be converted to the attributes type.


- .. parsed-literal:: 

    *class-name()* threw an exception, *class-name:function-name()* threw an exception

  The class' constructor or member function threw an unknown exception.
  Known exceptions are const char*, std::exception. See the 
  `exceptions`_ section.

- .. parsed-literal::

    no overload of '*class-name:function-name*' matched the arguments (*parameter-types*)
    no match for function call '*function-name*' with the parameters (*parameter-types*)
    no constructor of *class-name* matched the arguments (*parameter-types*)
    no operator *operator-name* matched the arguments (*parameter-types*)

  No function/operator with the given name takes the parameters you gave 
  it. You have either misspelled the function name, or given it incorrect
  parameters. This error is followed by a list of possible candidate 
  functions to help you figure out what parameter has the wrong type. If
  the candidate list is empty there's no function at all with that name.
  See the signature matching section.

- .. parsed-literal::

    call of overloaded '*class-name:function-name*(*parameter-types*)' is ambiguous
    ambiguous match for function call '*function-name*' with the parameters (*parameter-types*)
    call of overloaded constructor '*class-name*(*parameter-types*)' is ambiguous
    call of overloaded operator *operator-name* (*parameter-types*) is ambiguous

  This means that the function/operator you are trying to call has at least
  one other overload that matches the arguments just as good as the first
  overload.

- .. parsed-literal::

    cannot derive from C++ class '*class-name*'. It does not have a wrapped type.



Build options
=============

There are a number of configuration options available when building luabind.
It is very important that your project has the exact same configuration 
options as the ones given when the library was build! The exceptions are the
``LUABIND_MAX_ARITY`` and ``LUABIND_MAX_BASES`` which are template-based 
options and only matters when you use the library (which means they can 
differ from the settings of the library).

The default settings which will be used if no other settings are given
can be found in ``luabind/config.hpp``.

If you want to change the settings of the library, you can modify the 
config file. It is included and used by all makefiles. You can change paths
to Lua and boost in there as well.

LUABIND_MAX_ARITY
    Controls the maximum arity of functions that are registered with luabind. 
    You can't register functions that takes more parameters than the number 
    this macro is set to. It defaults to 5, so, if your functions have greater 
    arity you have to redefine it. A high limit will increase compilation time.

LUABIND_MAX_BASES
    Controls the maximum number of classes one class can derive from in 
    luabind (the number of classes specified within ``bases<>``). 
    ``LUABIND_MAX_BASES`` defaults to 4. A high limit will increase 
    compilation time.

LUABIND_NO_ERROR_CHECKING
    If this macro is defined, all the Lua code is expected only to make legal 
    calls. If illegal function calls are made (e.g. giving parameters that 
    doesn't match the function signature) they will not be detected by luabind
    and the application will probably crash. Error checking could be disabled 
    when shipping a release build (given that no end-user has access to write 
    custom Lua code). Note that function parameter matching will be done if a 
    function is overloaded, since otherwise it's impossible to know which one 
    was called. Functions will still be able to throw exceptions when error 
    checking is disabled.

    If a function throws an exception it will be caught by luabind and 
    propagated with ``lua_error()``.

LUABIND_NO_EXCEPTIONS
    This define will disable all usage of try, catch and throw in luabind. 
    This will in many cases disable run-time errors, when performing invalid 
    casts or calling Lua functions that fails or returns values that cannot 
    be converted by the given policy. luabind requires that no function called 
    directly or indirectly by luabind throws an exception (throwing exceptions 
    through Lua has undefined behavior).

    Where exceptions are the only way to get an error report from luabind, 
    they will be replaced with calls to the callback functions set with
    ``set_error_callback()`` and ``set_cast_failed_callback()``.

LUA_API
    If you want to link dynamically against Lua, you can set this define to 
    the import-keyword on your compiler and platform. On Windows in Visual Studio 
    this should be ``__declspec(dllimport)`` if you want to link against Lua 
    as a dll.

LUABIND_DYNAMIC_LINK
    Must be defined if you intend to link against the luabind shared
    library.

LUABIND_NO_RTTI
    You can define this if you don't want luabind to use ``dynamic_cast<>``.
    It will disable `Object identity`_.

NDEBUG
    This define will disable all asserts and should be defined in a release 
    build.


Implementation notes
====================

The classes and objects are implemented as user data in Lua. To make sure that
the user data really is the internal structure it is supposed to be, we tag
their metatables. A user data who's metatable contains a boolean member named
``__luabind_classrep`` is expected to be a class exported by luabind. A user
data who's metatable contains a boolean member named ``__luabind_class`` is
expected to be an instantiation of a luabind class.

This means that if you make your own user data and tags its metatable with the
exact same names, you can very easily fool luabind and crash the application.

In the Lua registry, luabind keeps an entry called ``__luabind_classes``. It
should not be removed or overwritten.

In the global table, a variable called ``super`` is used every time a
constructor in a lua-class is called. This is to make it easy for that
constructor to call its base class' constructor. So, if you have a global
variable named super it may be overwritten. This is probably not the best
solution, and this restriction may be removed in the future.

.. note:: Deprecated

  ``super()`` has been deprecated since version 0.8 in favor of directly
  invoking the base class' ``__init()`` function::

    function Derived:__init()
        Base.__init(self)
    end

Luabind uses two upvalues for functions that it registers. The first is a
userdata containing a list of overloads for the function, the other is a light
userdata with the value 0x1337, this last value is used to identify functions
registered by luabind. It should be virtually impossible to have such a pointer
as secondary upvalue by pure chance. This means, if you are trying to replace
an existing function with a luabind function, luabind will see that the
secondary upvalue isn't the magic id number and replace it. If it can identify
the function to be a luabind function, it won't replace it, but rather add
another overload to it.

Inside the luabind namespace, there's another namespace called detail. This
namespace contains non-public classes and are not supposed to be used directly.


FAQ
===

What's up with __cdecl and __stdcall?
    If you're having problem with functions
    that cannot be converted from ``void (__stdcall *)(int,int)`` to 
    ``void (__cdecl*)(int,int)``. You can change the project settings to make the
    compiler generate functions with __cdecl calling conventions. This is
    a problem in developer studio.

What's wrong with functions taking variable number of arguments?
    You cannot register a function with ellipses in its signature. Since
    ellipses don't preserve type safety, those should be avoided anyway.

Internal structure overflow in VC
    If you, in visual studio, get fatal error C1204: compiler limit :
    internal structure overflow. You should try to split that compilation
    unit up in smaller ones. See `Splitting up the registration`_ and
    `Splitting class registrations`_.

What's wrong with precompiled headers in VC?
    Visual Studio doesn't like anonymous namespaces in its precompiled
    headers. If you encounter this problem you can disable precompiled 
    headers for the compilation unit (cpp-file) that uses luabind.

error C1076: compiler limit - internal heap limit reached in VC
    In visual studio you will probably hit this error. To fix it you have to
    increase the internal heap with a command-line option. We managed to
    compile the test suit with /Zm300, but you may need a larger heap then 
    that.

error C1055: compiler limit \: out of keys in VC
    It seems that this error occurs when too many assert() are used in a
    program, or more specifically, the __LINE__ macro. It seems to be fixed by
    changing /ZI (Program database for edit and continue) to /Zi 
    (Program database).

How come my executable is huge?
    If you're compiling in debug mode, you will probably have a lot of
    debug-info and symbols (luabind consists of a lot of functions). Also, 
    if built in debug mode, no optimizations were applied, luabind relies on 
    that the compiler is able to inline functions. If you built in release 
    mode, try running strip on your executable to remove export-symbols, 
    this will trim down the size.

    Our tests suggests that cygwin's gcc produces much bigger executables 
    compared to gcc on other platforms and other compilers.

.. HUH?! // check the magic number that identifies luabind's functions 

Can I register class templates with luabind?
    Yes you can, but you can only register explicit instantiations of the 
    class. Because there's no Lua counterpart to C++ templates. For example, 
    you can register an explicit instantiation of std::vector<> like this::

        module(L)
        [
            class_<std::vector<int> >("vector")
                .def(constructor<int>)
                .def("push_back", &std::vector<int>::push_back)
        ];

.. Again, irrelevant to docs: Note that the space between the two > is required by C++.

Do I have to register destructors for my classes?
    No, the destructor of a class is always called by luabind when an 
    object is collected. Note that Lua has to own the object to collect it.
    If you pass it to C++ and gives up ownership (with adopt policy) it will 
    no longer be owned by Lua, and not collected.

    If you have a class hierarchy, you should make the destructor virtual if 
    you want to be sure that the correct destructor is called (this apply to C++ 
    in general).

.. And again, the above is irrelevant to docs. This isn't a general C++ FAQ. But it saves us support questions.

Fatal Error C1063 compiler limit \: compiler stack overflow in VC
    VC6.5 chokes on warnings, if you are getting alot of warnings from your 
    code try suppressing them with a pragma directive, this should solve the 
    problem.

Crashes when linking against luabind as a dll in Windows
    When you build luabind, Lua and you project, make sure you link against 
    the runtime dynamically (as a dll).

I cannot register a function with a non-const parameter
    This is because there is no way to get a reference to a Lua value. Have 
    a look at out_value_ and pure_out_value_ policies.


Known issues
============

- You cannot use strings with extra nulls in them as member names that refers
  to C++ members.

- If one class registers two functions with the same name and the same
  signature, there's currently no error. The last registered function will
  be the one that's used.

- In VC7, classes can not be called test.

- If you register a function and later rename it, error messages will use the
  original function name.

- luabind does not support class hierarchies with virtual inheritance. Casts are
  done with static pointer offsets.


Acknowledgments
===============

Written by Daniel Wallin and Arvid Norberg. © Copyright 2003.
All rights reserved.

Evan Wies has contributed with thorough testing, countless bug reports
and feature ideas.

This library was highly inspired by Dave Abrahams' Boost.Python_ library.

.. _Boost.Python: http://www.boost.org/libraries/python

