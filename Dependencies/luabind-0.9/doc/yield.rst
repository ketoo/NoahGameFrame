yield
----------------

Motivation
~~~~~~~~~~

Makes a C++ function yield when returning.

Defined in
~~~~~~~~~~

.. parsed-literal::

    #include <luabind/yield_policy.hpp>

Synopsis
~~~~~~~~

.. parsed-literal::

    yield

Example
~~~~~~~

.. parsed-literal::

    void do_thing_that_takes_time()
    {
        ...
    }

    ...

    module(L)
    [
        def("do_thing_that_takes_time", &do_thing_that_takes_time, **yield**)
    ];

