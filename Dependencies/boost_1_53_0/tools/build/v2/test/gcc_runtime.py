#!/usr/bin/python

# Copyright 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Tests that on gcc, we correctly report problem when static runtime is
# requested for building a shared library.

import BoostBuild
import string

t = BoostBuild.Tester()

# Create the needed files.
t.write("jamroot.jam", "")

t.write("jamfile.jam", """
lib hello : hello.cpp ;
""")

t.write("hello.cpp", """
int main() { }
""")

t.run_build_system("runtime-link=static")
t.fail_test(string.find(t.stdout(),
    "On gcc, DLL can't be build with '<runtime-link>static'") == -1)

t.run_build_system("link=static runtime-link=static")
t.expect_addition("bin/$toolset/debug/link-static/runtime-link-static/hello.lib")

t.cleanup()
