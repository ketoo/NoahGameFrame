discard_result
----------------

Motivation
~~~~~~~~~~

This is a very simple policy which makes it possible to throw away 
the value returned by a C++ function, instead of converting it to 
Lua.

Defined in
~~~~~~~~~~

.. parsed-literal::

    #include <luabind/discard_result_policy.hpp>

Synopsis
~~~~~~~~

.. parsed-literal::

    discard_result

Example
~~~~~~~

.. parsed-literal::

    struct X
    {
        X& set(T n)
        {
            ...
            return \*this;
        }
    };

    ...

    module(L)
    [
        class_<X>("X")
            .def("set", &simple::set, **discard_result**)
    ];

