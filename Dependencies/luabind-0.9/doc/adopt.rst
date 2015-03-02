adopt
----------------

Motivation
~~~~~~~~~~

Used to transfer ownership across language boundaries.

Defined in
~~~~~~~~~~

.. parsed-literal::

    #include <luabind/adopt_policy.hpp>

Synopsis
~~~~~~~~

.. parsed-literal::

    adopt(index)

Parameters
~~~~~~~~~~

============= ===============================================================
Parameter     Purpose
============= ===============================================================
``index``     The index which should transfer ownership, ``_N`` or ``result``
============= ===============================================================

Example
~~~~~~~

.. parsed-literal::

    X* create()
    {
        return new X;
    }

    ...

    module(L)
    [
        def("create", &create, **adopt(result)**)
    ];

