#!/usr/bin/python

# Copyright 2001 Dave Abrahams 
# Copyright 2011 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild
import os

t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

t.write("file.jam", """
if $(NT)
{
    #
    # Build a really long commandline. (> 10K characters).
    #
    ten = 0 1 2 3 4 5 6 7 8 9 ;
    1x7chars = 0_____ ;
    # add a digit and multiply by 10
    10x8chars = $(ten)$(1x7chars) ;
    # add a digit to each of 10 strings and multiply by 10
    100x9chars = $(ten)$(10x8chars) ;
    # add a digit to each of 100 strings and multiply by 10
    1000x10chars = $(ten)$(100x9chars) ;

    #
    # Cause line_length_test to be built
    #
    actions do_echo
    {
        echo $(text)
    }

    400x10chars = $(ten[1-4])$(100x9chars) ;

    text on line_length_test = $(400x10chars) 40$(10x8chars[1-9]) 01234 ;
    text on line_length_test = $(1000x10chars) $(1000x10chars) ;
    JAMSHELL on line_length_test = % ;
    DEPENDS all : line_length_test ;

    do_echo line_length_test ;
}
else
{
    NOCARE all ;
}
""")
t.run_build_system("-ffile.jam")

t.cleanup()
