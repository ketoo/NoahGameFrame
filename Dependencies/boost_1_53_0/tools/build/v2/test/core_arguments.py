#!/usr/bin/python

# Copyright 2001 Dave Abrahams 
# Copyright 2011 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild
import os

t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

t.write("echo_args.jam", """
rule echo_args ( a b ? c ? : d + : e * )
{
    ECHO a= $(a) b= $(b) c= $(c) ":" d= $(d) ":" e= $(e) ;
}

rule echo_varargs ( a b ? c ? : d + : e * : * )
{
    ECHO a= $(a) b= $(b) c= $(c) ":" d= $(d) ":" e= $(e)
      ": rest= "$(4[1]) $(4[2])
      ": "$(5[1]) $(5[2])
      ": "$(6[1]) $(6[2])
      ": "$(7[1]) $(7[2])
      ": "$(8[1]) $(8[2])
      ": "$(9[1]) $(9[2]) ;
}
""")

t.write("file.jam", "include echo_args.jam ; echo_args ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("* missing argument a");

t.write("file.jam", "include echo_args.jam ; echo_args 1 2 : 3 : 4 : 5 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("* extra argument 5");

t.write("file.jam", "include echo_args.jam ; echo_args a b c1 c2 : d ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("* extra argument c2");

# Check modifier '?'

t.write("file.jam", "include echo_args.jam ; echo_args 1 2 3 : 4 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= 2 c= 3 : d= 4 : e=");

t.write("file.jam", "include echo_args.jam ; echo_args 1 2 : 3 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= 2 c= : d= 3 : e=");

t.write("file.jam", "include echo_args.jam ; echo_args 1 : 2 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e=");

# Check modifier '+'

t.write("file.jam", "include echo_args.jam ; echo_args 1 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("* missing argument d");

t.write("file.jam", "include echo_args.jam ; echo_args 1 : 2 3 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 3 : e=");

t.write("file.jam", "include echo_args.jam ; echo_args 1 : 2 3 4 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 3 4 : e=");

# Check modifier '*'

t.write("file.jam", "include echo_args.jam ; echo_args 1 : 2 : 3 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3");

t.write("file.jam", "include echo_args.jam ; echo_args 1 : 2 : 3 4 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3 4");

t.write("file.jam", "include echo_args.jam ; echo_args 1 : 2 : 3 4 5 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3 4 5");

#
# Check varargs
#

t.write("file.jam", "include echo_args.jam ; echo_varargs 1 : 2 : 3 4 5 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3 4 5");

t.write("file.jam", "include echo_args.jam ; echo_varargs 1 : 2 : 3 4 5 : 6 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6");

t.write("file.jam", "include echo_args.jam ; echo_varargs 1 : 2 : 3 4 5 : 6 7 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6 7");

t.write("file.jam", "include echo_args.jam ; echo_varargs 1 : 2 : 3 4 5 : 6 7 : 8 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6 7 : 8");

t.write("file.jam", "include echo_args.jam ; echo_varargs 1 : 2 : 3 4 5 : 6 7 : 8 : 9 ;")
t.run_build_system("-ffile.jam", status=1)
t.expect_output_line("a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6 7 : 8 : 9");

t.cleanup()
