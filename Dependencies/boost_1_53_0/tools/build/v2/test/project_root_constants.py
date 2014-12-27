#!/usr/bin/python

# Copyright 2003, 2004, 2005 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild
import string

# Create a temporary working directory.
t = BoostBuild.Tester()

# Create the needed files.
t.write("jamroot.jam", """
constant FOO : foobar gee ;
ECHO $(FOO) ;
""")

t.run_build_system()
t.fail_test(string.find(t.stdout(), "foobar gee") == -1)

# Regression test: when absolute paths were passed to path-constant rule,
# Boost.Build failed to recognize path as absolute and prepended the current
# dir.
t.write("jamroot.jam", """
import path ;
local here = [ path.native [ path.pwd ] ] ;
path-constant HERE : $(here) ;
if $(HERE) != $(here) 
{
    ECHO "PWD           =" $(here) ;
    ECHO "path constant =" $(HERE) ;
    EXIT ;
}
""")
t.write("jamfile.jam", "")

t.run_build_system()

t.write("jamfile.jam", """
# This tests that rule 'hello' will be imported to children unlocalized, and
# will still access variables in this Jamfile.
x = 10 ;
constant FOO : foo ;
rule hello ( ) { ECHO "Hello $(x)" ; }
""")

t.write("d/jamfile.jam", """
ECHO "d: $(FOO)" ;
constant BAR : bar ;
""")

t.write("d/d2/jamfile.jam", """
ECHO "d2: $(FOO)" ;
ECHO "d2: $(BAR)" ;
hello ;
""")

t.run_build_system(subdir="d/d2")
t.fail_test(t.stdout().find("""d: foo
d2: foo
d2: bar
Hello 10
""") == -1)

t.cleanup()
