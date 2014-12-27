dependency
----------------

Motivation
~~~~~~~~~~

The dependency policy is used to create life-time dependencies between values.
This is needed for example when returning internal references to some class.

Defined in
~~~~~~~~~~

.. parsed-literal::

    #include <luabind/dependency_policy.hpp>

Synopsis
~~~~~~~~

.. parsed-literal::

    dependency(nurse_index, patient_index)

Parameters
~~~~~~~~~~

================= ==========================================================
Parameter         Purpose
================= ==========================================================
``nurse_index``   The index which will keep the patient alive. 
``patient_index`` The index which will be kept alive.
================= ==========================================================

Example
~~~~~~~

.. parsed-literal::

    struct X
    {
        B member;
        B& get() { return member; }
    };

    module(L)
    [
        class_<X>("X")
            .def("get", &X::get, **dependency(result, _1)**)
    ];

