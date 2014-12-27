#!/usr/bin/python

# Copyright 2007 Rene Rivera.
# Copyright 2011 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

t.write("sleep.bat","""@setlocal
@echo off
@REM timeout /T %1 /NOBREAK >nul
ping 127.0.0.1 -n 2 -w 1000 >nul
ping 127.0.0.1 -n %1 -w 1000 >nul
@endlocal
@exit /B 0
""")

t.write("file.jam", """

    if $(NT)
    {
        SLEEP = @call sleep.bat ;
    }
    else
    {
        SLEEP = sleep ;
    }

    actions .gen. {
echo 001
$(SLEEP) 4
echo 002
}
    rule .use.1 { DEPENDS $(<) : $(>) ; }
    actions .use.1 {
echo 003
}
    rule .use.2 { DEPENDS $(<) : $(>) ; }
    actions .use.2 {
$(SLEEP) 1
echo 004
}

    .gen. g1.generated g2.generated ;
    .use.1 u1.user : g1.generated ;
    .use.2 u2.user : g2.generated ;

    NOTFILE root ;
    DEPENDS g1.generated g2.generated : root ;
    DEPENDS all : u1.user u2.user ;
""")

t.run_build_system("-ffile.jam -j2", stdout="""...found 6 targets...
...updating 4 targets...
.gen. g1.generated
001
002
.use.1 u1.user
003
.use.2 u2.user
004
...updated 4 targets...
""")

t.cleanup()
