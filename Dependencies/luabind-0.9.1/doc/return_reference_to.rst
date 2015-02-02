return_reference_to
-------------------

Motivation
~~~~~~~~~~

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
to tell luabind that the return-value is already on the lua stack.

Defined in
~~~~~~~~~~

.. parsed-literal::

    #include <luabind/return_reference_to_policy.hpp>

Synopsis
~~~~~~~~

.. parsed-literal::

    return_reference_to(index)

Parameters
~~~~~~~~~~

========= =============================================================
Parameter Purpose
========= =============================================================
``index`` The argument index to return a reference to, any argument but
          not ``result``.
========= =============================================================

Example
~~~~~~~

.. parsed-literal::

    struct A
    {
        float val;

        A& set(float v)
        {
            val = v;
            return \*this;
        }
    };

    module(L)
    [
        class_<A>("A")
            .def(constructor<>())
            .def("set", &A::set, **return_reference_to(_1)**)
    ];

.. warning:: 
   This policy ignores all type information and should be used only it 
   situations where the parameter type is a perfect match to the 
   return-type (such as in the example).

