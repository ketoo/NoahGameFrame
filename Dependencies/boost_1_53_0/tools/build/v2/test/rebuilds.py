#!/usr/bin/python

# Copyright 2005 Dave Abrahams
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)

t.write('file.jam', '''
rule make
{
    DEPENDS $(<) : $(>) ;
    DEPENDS all : $(<) ;
}
actions make
{
    echo "******" making $(<) from $(>) "******"
    echo made from $(>) > $(<)
}

make aux1 : bar ;
make foo : bar ;
REBUILDS foo : bar ;
make bar : baz ;
make aux2 : bar ;
''')

t.write('baz', 'nothing\n')

t.run_build_system('-ffile.jam bar')
t.expect_addition('bar')
t.expect_nothing_more()

t.wait_for_time_change_since_last_build()
t.run_build_system('-ffile.jam foo')
t.expect_touch('bar')
t.expect_addition('foo')
t.expect_nothing_more()

t.run_build_system('-ffile.jam')
t.expect_addition(['aux1', 'aux2'])
t.expect_nothing_more()

t.touch('bar')
t.run_build_system('-ffile.jam')
t.expect_touch(['foo', 'aux1', 'aux2'])
t.expect_nothing_more()

t.cleanup()
