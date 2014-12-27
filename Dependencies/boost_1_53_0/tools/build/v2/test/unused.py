#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

# Test that unused sources are at least reported.

import BoostBuild
from string import find

t = BoostBuild.Tester()

t.set_tree("unused")

t.run_build_system()
# The second invocation should do nothing, and produce no warning. The previous
# invocation might have printed executed actions and other things, so it is not
# easy to check if warning was issued or not.
t.run_build_system(stdout="")

t.run_build_system("-sGENERATE_ONLY_UNUSABLE=1", stdout="")

# Now check that even if main target generates nothing, its usage requirements
# are still propagated to dependants.
t.write("a.cpp", """
#ifdef FOO
int main() {}
#endif
""")
t.run_build_system("-sGENERATE_NOTHING=1")

t.cleanup()
