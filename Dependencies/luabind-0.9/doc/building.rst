Building luabind
================

Prerequisites
-------------

Luabind depends on a number of Boost 1.34 libraries. It also depends on
Boost Jam and Boost Build V2 to build the library and run the tests.
Boost provides `precompiled bjam binaries`__ for a number of platforms.
If there isn't a precompiled binary available for your platform, you may
need to `build it yourself`__.

__ http://sourceforge.net/project/showfiles.php?group_id=7586&package_id=72941
__ http://www.boost.org/doc/libs/1_36_0/doc/html/jam/building.html


Windows
-------

The environment varaible ``LUA_PATH`` needs to be set to point to a
directory containing the Lua include directory and built libraries. At
least for the purpose of running the test suite, the recommended way to
get these is the `Lua Binaries`_ *Windows x86 DLL and Includes* package.

Furthermore, the environment variable ``BOOST_ROOT`` must point to
a Boost installation directory.

.. _`Lua Binaries`: http://luabinaries.luaforge.net


Linux and other \*nix flavors
-----------------------------

If your system already has Lua installed, it is very likely that the
build system will automatically find it and just work. If you have
Lua installed in a non-standard location, you may need to set
``LUA_PATH`` to point to the installation prefix.

``BOOST_ROOT`` can be set to a Boost installation directory. If left
unset, the build system will try to use boost headers from the standard
include path.

MacOSX
~~~~~~

If you have both the 10.4 and 10.5 SDK installed, Boost Build seems to
default to 10.4. Lua, at least when installed from MacPorts, will be
linked with the 10.5 SDK. If the luabind build fails with link errors,
you may need to explicitly build with the 10.5 SDK::

  $ bjam macosx-version=10.5


Building and testing
--------------------

Building the default variant of the library, which is a shared debug
library, is simply done by invoking ``bjam`` in the luabind root
directory::

  $ bjam
  ...patience...
  ...found 714 targets...
  ...updating 23 targets...

When building with GCC on Linux, this results in::

  bin/gcc-4.2.3/debug/libluabind.so

On Windows a dll and matching import library would be produced.

To run the unit tests, invoke ``bjam`` with the ``test`` target::

  $ bjam test

This will build and run the unit tests in four different variants:
debug, release, debug-static-lib, release-static-lib. A clean test run
output should end with something like:

.. parsed-literal::

  ... updated *xxx* targets...

A failed run would end with something like:

.. parsed-literal::

  ...failed updating *xxx* target...
  ...skipped *xxx* targets...

If you are not using Boost Build to build your application, and want to
use the shared library variant, ``LUABIND_DYNAMIC_LINK`` needs to be
defined to properly import symbols.
