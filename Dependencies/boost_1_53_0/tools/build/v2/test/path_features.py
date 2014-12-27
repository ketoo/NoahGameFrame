#!/usr/bin/python

# Copyright 2003 Dave Abrahams 
# Copyright 2002, 2003, 2004 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild

t = BoostBuild.Tester()

t.write("jamroot.jam", "import gcc ;")
t.write("jamfile.jam", "lib a : a.cpp : <include>. ;")
t.write("a.cpp", """
#include <a.h>
void
# ifdef _WIN32
__declspec(dllexport)
# endif 
foo() {}
""")
t.write("a.h", "//empty file\n")

t.write("d/jamfile.jam", "exe b : b.cpp ..//a ; ")
t.write("d/b.cpp", """
void foo();
int main() { foo(); }
""")

t.run_build_system(subdir="d")

# Now test the path features with condition work as well.
t.write("jamfile.jam", "lib a : a.cpp : <variant>debug:<include>.  ;")
t.rm("bin")
t.run_build_system(subdir="d")

# Test path features with condition in usage requirements.
t.write("jamfile.jam", """
lib a : a.cpp : <include>. : : <variant>debug:<include>. ;
""")
t.write("d/b.cpp", """
#include <a.h>
void foo();
int main() { foo(); }
""")
t.rm("d/bin")
t.run_build_system(subdir="d")

# Test that absolute paths inside requirements are ok. The problems appeared
# only when building targets in subprojects.
t.write("jamroot.jam", "")
t.write("jamfile.jam", "build-project x ; ")
t.write("x/jamfile.jam", """
local pwd = [ PWD ] ;
project : requirements <include>$(pwd)/x/include ;
exe m : m.cpp : <include>$(pwd)/x/include2 ;
""")
t.write("x/m.cpp", """
#include <h1.hpp>
#include <h2.hpp>
int main() {}
""")
t.write("x/include/h1.hpp", "\n")
t.write("x/include2/h2.hpp", "\n")

t.run_build_system()
t.expect_addition("x/bin/$toolset/debug/m.exe")

# Test that "&&" in path features is handled correctly.
t.rm("bin")
t.write("jamfile.jam", "build-project sub ;")
t.write("sub/jamfile.jam", """
exe a : a.cpp : <include>../h1&&../h2 ;
""")
t.write("sub/a.cpp", """
#include <header.h>
int main() { return OK; }
""")
t.write("h2/header.h", """
const int OK = 0;
""")
t.run_build_system()
t.expect_addition("sub/bin/$toolset/debug/a.exe")        

t.cleanup()
