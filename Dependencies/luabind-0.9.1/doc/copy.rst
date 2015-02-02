copy
----------------

Motivation
~~~~~~~~~~

This will make a copy of the parameter. This is the default behavior when
passing parameters by-value. Note that this can only be used when passing from
C++ to Lua. This policy requires that the parameter type has an accessible copy
constructor.

Defined in
~~~~~~~~~~

.. parsed-literal::

    #include <luabind/copy_policy.hpp>

Synopsis
~~~~~~~~

.. parsed-literal::

    copy(index)

Parameters
~~~~~~~~~~

============= ===============================================================
Parameter     Purpose
============= ===============================================================
``index``     The index to copy. ``result`` when used while wrapping C++
              functions. ``_N`` when passing arguments to Lua.
============= ===============================================================

Example
~~~~~~~

.. parsed-literal::

    X* get()
    {
        static X instance;
        return &instance;
    }

    ...

    module(L)
    [
        def("create", &create, **copy(result)**)
    ];

