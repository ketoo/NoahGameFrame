#!/usr/bin/python

# Copyright (C) Vladimir Prus 2006.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild

t = BoostBuild.Tester()

t.write("jamroot.jam", """
exe a1 : a1.cpp : <conditional>@a1-rule ;

rule a1-rule ( properties * )
{
    if <variant>debug in $(properties)
    {
        return <define>OK ;
    }
}

exe a2 : a2.cpp : <conditional>@$(__name__).a2-rule
    <variant>debug:<optimization>speed ;

rule a2-rule ( properties * )
{
    if <optimization>speed in $(properties)
    {
        return <define>OK ;
    }
}

exe a3 : a3.cpp : <conditional>@$(__name__).a3-rule-1
    <conditional>@$(__name__).a3-rule-2 ;

rule a3-rule-1 ( properties * )
{
    if <optimization>speed in $(properties)
    {
        return <define>OK ;
    }
}

rule a3-rule-2 ( properties * )
{
    if <variant>debug in $(properties)
    {
        return <optimization>speed ;
    }
}
""")

t.write("a1.cpp", """
#ifdef OK
int main() {}
#endif
""")

t.write("a2.cpp", """
#ifdef OK
int main() {}
#endif
""")

t.write("a3.cpp", """
#ifdef OK
int main() {}
#endif
""")

t.run_build_system()

t.expect_addition("bin/$toolset/debug/a1.exe")
t.expect_addition("bin/$toolset/debug/optimization-speed/a2.exe")
t.expect_addition("bin/$toolset/debug/optimization-speed/a3.exe")

t.cleanup()
