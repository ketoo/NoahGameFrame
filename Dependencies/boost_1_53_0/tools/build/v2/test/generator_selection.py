#!/usr/bin/python

# Copyright 2008 Jurko Gospodnetic
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Tests that generators get selected correctly.

import BoostBuild


################################################################################
#
# test_generator_added_after_already_building_a_target_of_its_target_type()
# -------------------------------------------------------------------------
#
################################################################################

def test_generator_added_after_already_building_a_target_of_its_target_type():
    """Regression test for a Boost Build bug causing it to not use a generator
    if it got added after already building a targer of its target type.
    """

    t = BoostBuild.Tester()

    t.write("dummy.cpp", "void f() {}\n")

    t.write("jamroot.jam", """
# Building this dummy target must not cause a later defined CPP target type
# generator not to be recognized as viable.
obj dummy : dummy.cpp ;
alias the-other-obj : Other//other-obj ;
""")

    t.write("Other/source.extension", "A dummy source file.")

    t.write("Other/mygen.jam", """
import generators ;
import os ;
import type ;
type.register MY_TYPE : extension ;
generators.register-standard mygen.generate-a-cpp-file : MY_TYPE : CPP ;
rule generate-a-cpp-file { ECHO Generating a CPP file... ; }
if [ os.name ] = NT
{
    actions generate-a-cpp-file { echo void g() {} > "$(<)" }
}
else
{
    actions generate-a-cpp-file { echo "void g() {}" > "$(<)" }
}
""")

    t.write("Other/mygen.py", """
import b2.build.generators as generators
import b2.build.type as type

from b2.manager import get_manager

import os


type.register('MY_TYPE', ['extension'])
generators.register_standard('mygen.generate-a-cpp-file', ['MY_TYPE'], ['CPP'])
if os.name == 'nt':
    action = 'echo void g() {} > "$(<)"'
else:
    action = 'echo "void g() {}" > "$(<)"'
def f(*args):
    print "Generating a CPP file..."

get_manager().engine().register_action("mygen.generate-a-cpp-file",
                                       action, function=f)    
""")

    t.write("Other/jamfile.jam", """
import mygen ;
obj other-obj : source.extension ;
""")

    t.run_build_system()
    t.expect_output_line("Generating a CPP file...")
    t.expect_addition("bin/$toolset/debug/dummy.obj")
    t.expect_addition("Other/bin/$toolset/debug/other-obj.obj")

    t.cleanup()


################################################################################
#
# test_using_a_derived_source_type_created_after_generator_already_used()
# -----------------------------------------------------------------------
#
################################################################################

def test_using_a_derived_source_type_created_after_generator_already_used():
    """Regression test for a Boost Build bug causing it to not use a generator
    with a source type derived from one of the generator's sources but created
    only after already using the generateor.
    """

    t = BoostBuild.Tester()

    t.write("dummy.cpp", "void f() {}\n")

    t.write("jamroot.jam", """
# Building this dummy target must not cause a later defined UNGA_CPP target type
# not to be recognized as a viable source type for building OBJ targets.
obj dummy : dummy.cpp ;
alias the-test-output : Other//other-obj ;
""")

    t.write("Other/source.unga_cpp", "void g() {}\n")

    t.write("Other/jamfile.jam", """
import type ;
type.register UNGA_CPP : unga_cpp : CPP ;
# We are careful not to do anything between defining our new UNGA_CPP target
# type and using the CPP --> OBJ generator that could potentially cover the
# Boost Build bug by clearing its internal viable source target type state.
obj other-obj : source.unga_cpp ;
""")

    t.run_build_system()
    t.expect_addition("bin/$toolset/debug/dummy.obj")
    t.expect_addition("Other/bin/$toolset/debug/other-obj.obj")
    t.expect_nothing_more()

    t.cleanup()


################################################################################
#
# main()
# ------
#
################################################################################

test_generator_added_after_already_building_a_target_of_its_target_type()
test_using_a_derived_source_type_created_after_generator_already_used()
