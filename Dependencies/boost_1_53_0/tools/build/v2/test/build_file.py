#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2006.
#  Copyright (C) Jurko Gospodnetic 2008.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

# Tests that we explicitly request a file (not target) to be built by specifying
# its name on the command line.

import BoostBuild


################################################################################
#
# test_building_file_from_specific_project()
# ------------------------------------------
#
################################################################################

def test_building_file_from_specific_project():
    t = BoostBuild.Tester()

    t.write("jamroot.jam", """
exe hello : hello.cpp ;
exe hello2 : hello.cpp ;
build-project sub ;
""")
    t.write("hello.cpp", "int main() {}")
    t.write("sub/jamfile.jam", """
exe hello : hello.cpp ;
exe hello2 : hello.cpp ;
exe sub : hello.cpp ;
""")
    t.write("sub/hello.cpp", "int main() {}")

    t.run_build_system("sub " + t.adjust_suffix("hello.obj"))
    t.expect_output_line("*depends on itself*", False)
    t.expect_addition("sub/bin/$toolset/debug/hello.obj")
    t.expect_nothing_more()

    t.cleanup()


################################################################################
#
# test_building_file_from_specific_target()
# -----------------------------------------
#
################################################################################

def test_building_file_from_specific_target():
    t = BoostBuild.Tester()

    t.write("jamroot.jam", """
exe hello1 : hello1.cpp ;
exe hello2 : hello2.cpp ;
exe hello3 : hello3.cpp ;
""")
    t.write("hello1.cpp", "int main() {}")
    t.write("hello2.cpp", "int main() {}")
    t.write("hello3.cpp", "int main() {}")

    t.run_build_system("hello1 " + t.adjust_suffix("hello1.obj"))
    t.expect_addition("bin/$toolset/debug/hello1.obj")
    t.expect_nothing_more()

    t.cleanup()


################################################################################
#
# test_building_missing_file_from_specific_target()
# -------------------------------------------------
#
################################################################################

def test_building_missing_file_from_specific_target():
    t = BoostBuild.Tester()

    t.write("jamroot.jam", """
exe hello1 : hello1.cpp ;
exe hello2 : hello2.cpp ;
exe hello3 : hello3.cpp ;
""")
    t.write("hello1.cpp", "int main() {}")
    t.write("hello2.cpp", "int main() {}")
    t.write("hello3.cpp", "int main() {}")

    t.run_build_system("hello1 " + t.adjust_suffix("hello2.obj"), status=1)
    t.expect_output_line("don't know how to make*" + t.adjust_suffix("hello2.obj"))
    t.expect_nothing_more()

    t.cleanup()


################################################################################
#
# test_building_multiple_files_with_different_names()
# ---------------------------------------------------
#
################################################################################

def test_building_multiple_files_with_different_names():
    t = BoostBuild.Tester()

    t.write("jamroot.jam", """
exe hello1 : hello1.cpp ;
exe hello2 : hello2.cpp ;
exe hello3 : hello3.cpp ;
""")
    t.write("hello1.cpp", "int main() {}")
    t.write("hello2.cpp", "int main() {}")
    t.write("hello3.cpp", "int main() {}")

    t.run_build_system(
        t.adjust_suffix("hello1.obj") + " " +
        t.adjust_suffix("hello2.obj"))
    t.expect_addition("bin/$toolset/debug/hello1.obj")
    t.expect_addition("bin/$toolset/debug/hello2.obj")
    t.expect_nothing_more()

    t.cleanup()


################################################################################
#
# test_building_multiple_files_with_the_same_name()
# -------------------------------------------------
#
################################################################################

def test_building_multiple_files_with_the_same_name():
    t = BoostBuild.Tester()

    t.write("jamroot.jam", """
exe hello : hello.cpp ;
exe hello2 : hello.cpp ;
build-project sub ;
""")
    t.write("hello.cpp", "int main() {}")
    t.write("sub/jamfile.jam", """
exe hello : hello.cpp ;
exe hello2 : hello.cpp ;
exe sub : hello.cpp ;
""")
    t.write("sub/hello.cpp", "int main() {}")

    t.run_build_system(t.adjust_suffix("hello.obj"))
    t.expect_output_line("*depends on itself*", False)
    t.expect_addition("bin/$toolset/debug/hello.obj")
    t.expect_addition("sub/bin/$toolset/debug/hello.obj")
    t.expect_nothing_more()

    t.cleanup()


################################################################################
#
# main()
# ------
#
################################################################################

test_building_file_from_specific_project()
test_building_file_from_specific_target()
test_building_missing_file_from_specific_target()
test_building_multiple_files_with_different_names()
test_building_multiple_files_with_the_same_name()
