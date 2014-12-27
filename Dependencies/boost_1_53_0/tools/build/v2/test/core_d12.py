#!/usr/bin/python

# Copyright 2002, 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# This tests correct handling of "-d1" and "-d2" options.

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)

t.write("file.jam", """
actions a { }
actions quietly b { }
ALWAYS all ;
a all ;
b all ;
""")

t.run_build_system("-ffile.jam -d0", stdout="")

t.run_build_system("-ffile.jam -d1", stdout=
"""...found 1 target...
...updating 1 target...
a all
...updated 1 target...
""")

t.run_build_system("-ffile.jam -d2")

t.fail_test(t.stdout().find("a all") == -1)
t.fail_test(t.stdout().find("b all") == -1)

t.cleanup()
