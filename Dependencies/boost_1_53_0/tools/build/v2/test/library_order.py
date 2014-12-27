#!/usr/bin/python

# Copyright 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test that on compilers sensitive to library order on linker's command line, we
# generate the correct order.

import BoostBuild
import string


t = BoostBuild.Tester()

t.write("a.cpp", """
void b();
void a() { b(); }
""")

t.write("b.cpp", """
void c();
void b() { c(); }
""")

t.write("c.cpp", """
void d();
void c() { d(); }
""")

t.write("d.cpp", """
void d() {}
""")

# The order of libraries in 'main' is crafted so that we get error unless we do
# something about the order ourselves.
t.write("jamfile.jam", """
exe main : main.cpp libd libc libb liba ;
lib libd : d.cpp ;
lib libc : c.cpp : <link>static <use>libd ;
lib libb : b.cpp : <use>libc ;
lib liba : a.cpp : <use>libb ;
""")

t.write("main.cpp", """
void a();
int main() { a(); }
""")

t.write("jamroot.jam", """
""")

t.run_build_system("-d2")
t.expect_addition("bin/$toolset/debug/main.exe")


# Test the order between searched libraries.
t.write("jamfile.jam", """
exe main : main.cpp png z ;
lib png : z : <name>png ;
lib z : : <name>zzz ;
""")

t.run_build_system("-a -n -d+2")
t.fail_test(string.find(t.stdout(), "png") > string.find(t.stdout(), "zzz"))

t.write("jamfile.jam", """
exe main : main.cpp png z ;
lib png : : <name>png ;
lib z : png : <name>zzz ;
""")

t.run_build_system("-a -n -d+2")
t.fail_test(string.find(t.stdout(), "png") < string.find(t.stdout(), "zzz"))


# Test the order between prebuilt libraries.
t.write("first.a", "")

t.write("second.a", "")

t.write("jamfile.jam", """
exe main : main.cpp first second ;
lib first : second : <file>first.a ;
lib second : : <file>second.a ;
""")

t.run_build_system("-a -n -d+2")
t.fail_test(string.find(t.stdout(), "first") > string.find(t.stdout(), "second"))

t.write("jamfile.jam", """
exe main : main.cpp first second ;
lib first : : <file>first.a ;
lib second : first : <file>second.a ;
""")

t.run_build_system("-a -n -d+2")
t.fail_test(string.find(t.stdout(), "first") < string.find(t.stdout(), "second"))

t.cleanup()
