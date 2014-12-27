#!/usr/bin/python

# Copyright 2003 Dave Abrahams 
# Copyright 2006 Rene Rivera 
# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild
import os
import re

spaces_re = re.compile("\ \ +")
trailing_spaces_re = re.compile("\ +\n")

t = BoostBuild.Tester("-d+1", pass_toolset=0)

t.set_tree('module-actions')

#   Note that the following string contains some trailing spaces that should not
# be removed.
expected_output = """...found 4 targets...
...updating 3 targets...
A.act t1
A.act t1: X1-t1   
B.act t1
B.act t1: X1-t1 X2-B  
act t1
act t1: X1-t1 X2-global X3-global 
A.act t2
A.act t2: X1-A X2-t2  
B.act t2
B.act t2:  X2-t2  
act t2
act t2: X1-global X2-t2 X3-global 
A.act t3
A.act t3: X1-A  X3-t3 
B.act t3
B.act t3:  X2-B X3-t3 
act t3
act t3: X1-global X2-global X3-t3 
...updated 3 targets...
"""

# On Unixes, call to 'echo 1     2      3' produces '1 2 3' (note the spacing)
# Accomodate for that fact.
if os.name != 'nt':
    expected_output = re.sub(spaces_re, " ", expected_output)
    expected_output = re.sub(trailing_spaces_re, "\n", expected_output)

t.run_build_system(stdout=expected_output)
t.expect_nothing_more()
t.cleanup()
