#!/usr/bin/python

# Copyright 2005 David Abrahams
# Copyright 2008 Jurko Gospodnetic
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Tests the build step timing facilities.


import BoostBuild
import re


################################################################################
#
# basic_jam_action_test()
# -----------------------
#
################################################################################

def basic_jam_action_test():
    """Tests basic Jam action timing support."""

    t = BoostBuild.Tester(pass_toolset=0)

    t.write("file.jam", """
rule time
{
    DEPENDS $(<) : $(>) ;
    __TIMING_RULE__ on $(>) = record_time $(<) ;
    DEPENDS all : $(<) ;
}

actions time
{
    echo $(>) user: $(__USER_TIME__) system: $(__SYSTEM_TIME__)
    echo timed from $(>) >> $(<)
}

rule record_time ( target : source : start end user system )
{
    __USER_TIME__ on $(target) = $(user) ;
    __SYSTEM_TIME__ on $(target) = $(system) ;
}

rule make
{
    DEPENDS $(<) : $(>) ;
}

actions make
{
    echo made from $(>) >> $(<)
}

time foo : bar ;
make bar : baz ;
""")

    t.write("baz", "nothing\n")

    expected_output = """\.\.\.found 4 targets\.\.\.
\.\.\.updating 2 targets\.\.\.
make bar
time foo
bar +user: [0-9\.]+ +system: +[0-9\.]+ *
\.\.\.updated 2 targets\.\.\.$
"""

    t.run_build_system("-ffile.jam -d+1", stdout=expected_output, match=lambda
        actual, expected: re.search(expected, actual, re.DOTALL))
    t.expect_addition("foo")
    t.expect_addition("bar")
    t.expect_nothing_more()

    t.cleanup()


################################################################################
#
# boost_build_testing_support_timing_rule():
# ------------------------------------------
#
################################################################################

def boost_build_testing_support_timing_rule():
    """Tests the target build timing rule provided by the Boost Build testing
    support system.
    """

    t = BoostBuild.Tester()

    t.write("aaa.cpp", "int main() {}\n")

    t.write("jamroot.jam", """
import testing ;
exe my-exe : aaa.cpp ;
time my-time : my-exe ;
""")

    t.run_build_system()
    t.expect_addition("bin/$toolset/debug/aaa.obj")
    t.expect_addition("bin/$toolset/debug/my-exe.exe")
    t.expect_addition("bin/$toolset/debug/my-time.time")

    t.expect_content_line("bin/$toolset/debug/my-time.time", "user: *")
    t.expect_content_line("bin/$toolset/debug/my-time.time", "system: *")

    t.cleanup()


################################################################################
#
# boost_build_testing_support_timing_rule_with_spaces_in_names()
# --------------------------------------------------------------
#
################################################################################

def boost_build_testing_support_timing_rule_with_spaces_in_names():
    """Tests the target build timing rule provided by the Boost Build testing
    support system when used with targets contining spaces in their names.
    """

    t = BoostBuild.Tester()

    t.write("aaa bbb.cpp", "int main() {}\n")

    t.write("jamroot.jam", """
import testing ;
exe "my exe" : "aaa bbb.cpp" ;
time "my time" : "my exe" ;
""")

    t.run_build_system()
    t.expect_addition("bin/$toolset/debug/aaa bbb.obj")
    t.expect_addition("bin/$toolset/debug/my exe.exe")
    t.expect_addition("bin/$toolset/debug/my time.time")

    t.expect_content_line("bin/$toolset/debug/my time.time", "user: *")
    t.expect_content_line("bin/$toolset/debug/my time.time", "system: *")

    t.cleanup()


################################################################################
#
# main()
# ------
#
################################################################################

basic_jam_action_test()
boost_build_testing_support_timing_rule()
boost_build_testing_support_timing_rule_with_spaces_in_names()