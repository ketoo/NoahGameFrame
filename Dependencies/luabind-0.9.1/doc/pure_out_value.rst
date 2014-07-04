pure_out_value
----------------

Motivation
~~~~~~~~~~

This works exactly like ``out_value``, except that it will pass a
default constructed object instead of converting an argument from
Lua. This means that the parameter will be removed from the lua
signature.

Defined in
~~~~~~~~~~

.. parsed-literal::

    #include <luabind/out_value_policy.hpp>

Synopsis
~~~~~~~~

.. parsed-literal::

    pure_out_value(index, policies = none)


Parameters
~~~~~~~~~~

=============== =============================================================
Parameter       Purpose
=============== =============================================================
``index``       The index of the parameter to be used as an out parameter.
``policies``    The policies used internally to convert the out parameter
                to Lua. ``_1`` is used as the internal index.
=============== =============================================================

Example
~~~~~~~

Note that no values are passed to the calls to ``f1`` and ``f2``.

.. parsed-literal::

    void f1(float& val) { val = 10.f; }
    void f2(float\* val) { \*val = 10.f; }

    module(L)
    [
        def("f", &f, **pure_out_value(_1)**)
    ];

    Lua 5.0  Copyright (C) 1994-2003 Tecgraf, PUC-Rio
    > print(f1())
    10
    > print(f2())
    10

