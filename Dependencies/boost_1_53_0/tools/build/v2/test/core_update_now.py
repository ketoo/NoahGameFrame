#!/usr/bin/python

# Copyright 2011 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild
import os

def basic():
    # Basic test

    t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

    t.write("file.jam", """

actions do-print
{
    echo updating $(<)
}

NOTFILE target1 ;
ALWAYS target1 ;
do-print target1 ;

UPDATE_NOW target1 ;

DEPENDS all : target1 ;
""")

    t.run_build_system("-ffile.jam", stdout="""...found 1 target...
...updating 1 target...
do-print target1
updating target1
...updated 1 target...
...found 1 target...
""")

    t.cleanup()

def ignore_minus_n():
    # ignore-minus-n

    t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

    t.write("file.jam", """

actions do-print
{
    echo updating $(<)
}

NOTFILE target1 ;
ALWAYS target1 ;
do-print target1 ;

UPDATE_NOW target1 : : ignore-minus-n ;

DEPENDS all : target1 ;
""")

    t.run_build_system("-ffile.jam -n", stdout="""...found 1 target...
...updating 1 target...
do-print target1

    echo updating target1

updating target1
...updated 1 target...
...found 1 target...
""")

    t.cleanup()

def failed_target():

    t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

    t.write("file.jam", """

actions fail
{
    exit 1
}

NOTFILE target1 ;
ALWAYS target1 ;
fail target1 ;

actions do-print
{
    echo updating $(<)
}

NOTFILE target2 ;
do-print target2 ;
DEPENDS target2 : target1 ;

UPDATE_NOW target1 : : ignore-minus-n ;

DEPENDS all : target1 target2 ;
""")

    t.run_build_system("-ffile.jam -n", stdout="""...found 1 target...
...updating 1 target...
fail target1

    exit 1

...failed fail target1...
...failed updating 1 target...
...found 2 targets...
...updating 1 target...
do-print target2

    echo updating target2

...updated 1 target...
""")

    t.cleanup()

def missing_target():
    t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

    t.write("file.jam", """

actions do-print
{
    echo updating $(<)
}

NOTFILE target2 ;
do-print target2 ;
DEPENDS target2 : target1 ;

UPDATE_NOW target1 : : ignore-minus-n ;

DEPENDS all : target1 target2 ;
""")

    t.run_build_system("-ffile.jam -n", status=1, stdout="""don't know how to make target1
...found 1 target...
...can't find 1 target...
...found 2 targets...
...can't make 1 target...
""")

    t.cleanup()

# Make sure that if we call UPDATE_NOW with ignore-minus-n,
# the target gets updated exactly once regardless of previous
# calls to UPDATE_NOW with -n in effect.

def build_once():
    t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

    t.write("file.jam", """

actions do-print
{
    echo updating $(<)
}

NOTFILE target1 ;
ALWAYS target1 ;
do-print target1 ;

UPDATE_NOW target1 ;
UPDATE_NOW target1 : : ignore-minus-n ;
UPDATE_NOW target1 : : ignore-minus-n ;

DEPENDS all : target1 ;
""")

    t.run_build_system("-ffile.jam -n", stdout="""...found 1 target...
...updating 1 target...
do-print target1

    echo updating target1

...updated 1 target...
do-print target1

    echo updating target1

updating target1
...updated 1 target...
...found 1 target...
""")

    t.cleanup()

basic()
ignore_minus_n()
failed_target()
missing_target()
build_once()
