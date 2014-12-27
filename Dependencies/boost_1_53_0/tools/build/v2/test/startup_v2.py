#!/usr/bin/python

# Copyright 2002 Dave Abrahams
# Copyright 2003, 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild
import os
import re


def match_re(actual,expected):
    return re.match(expected,actual,re.DOTALL) != None

# Test the v1 startup behavior.
t = BoostBuild.Tester(match=match_re, boost_build_path='', pass_toolset=0)

t.set_tree('startup')

t.run_build_system(
    status=1, stdout=r'''Unable to load Boost\.Build: could not find "boost-build.jam"
.*Attempted search from .* up to the root''', match = match_re)

os.chdir('no-bootstrap1')

t.run_build_system(
    status=1
    , stdout=r'''Unable to load Boost\.Build: could not find build system\.'''
    + r'''.*attempted to load the build system by invoking'''
    + r'''.*'boost-build ;'.*'''
    + r'''but we were unable to find "bootstrap\.jam"'''
    )

# Descend to a subdirectory which /doesn't/ contain a boost-build.jam file, and
# try again to test the crawl-up behavior.
os.chdir('subdir')

t.run_build_system(
    status=1
    , stdout=r'''Unable to load Boost\.Build: could not find build system\.'''
    + r'''.*attempted to load the build system by invoking'''
    + r'''.*'boost-build ;'.*'''
    + r'''but we were unable to find "bootstrap\.jam"'''
    )

os.chdir('../../no-bootstrap2')

t.run_build_system(
    status=1
    , stdout=r'''Unable to load Boost\.Build: could not find build system\.'''
    + r'''.*attempted to load the build system by invoking'''
    + r'''.*'boost-build \. ;'.*'''
    + r'''but we were unable to find "bootstrap\.jam"'''
    )

os.chdir('../no-bootstrap3')

t.run_build_system(
    status=1
    , stdout=r'''Unable to load Boost.Build
.*boost-build.jam" was found.*
However, it failed to call the "boost-build" rule'''
    )

# Test bootstrapping based on BOOST_BUILD_PATH.
os.chdir('../bootstrap-env')
t.run_build_system(
    extra_args = '-sBOOST_BUILD_PATH=../boost-root/build'
    , stdout = 'build system bootstrapped'
    )

# Test bootstrapping based on an explicit path in boost-build.jam.
os.chdir('../bootstrap-explicit')
t.run_build_system(
    stdout = 'build system bootstrapped'
    )

t.cleanup()
