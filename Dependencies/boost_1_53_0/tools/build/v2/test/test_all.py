#!/usr/bin/python

# Copyright 2002-2005 Dave Abrahams.
# Copyright 2002-2006 Vladimir Prus.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#         http://www.boost.org/LICENSE_1_0.txt)

import os
import sys
import string
import BoostBuild

xml = "--xml" in sys.argv
toolset = BoostBuild.get_toolset()


# Clear environment for testing.
#
for s in ('BOOST_ROOT', 'BOOST_BUILD_PATH', 'JAM_TOOLSET', 'BCCROOT', 'MSVCDir',
    'MSVC', 'MSVCNT', 'MINGW', 'watcom' ):
    try:
        del os.environ[s]
    except:
        pass

BoostBuild.set_defer_annotations(1)


def run_tests(critical_tests, other_tests):
    """Runs first critical tests and then other_tests.

       Stops on first error, and write the name of failed test to
       test_results.txt. Critical tests are run in the specified order, other
       tests are run starting with the one that failed the last time.
    """
    last_failed = last_failed_test()
    other_tests = reorder_tests(other_tests, last_failed)
    all_tests = critical_tests + other_tests

    invocation_dir = os.getcwd()

    pass_count = 0
    failures_count = 0

    for i in all_tests:
        passed = 1
        if not xml:
            print ("%-25s : " %(i)),
        try:
            __import__(i)
        except SystemExit:
            passed = 0;
            if failures_count == 0:
                f = open(os.path.join(invocation_dir, 'test_results.txt'), 'w')
                f.write(i)
                f.close()
            failures_count = failures_count + 1
            # Restore the current directory, which might be changed by the test.
            os.chdir(invocation_dir)

        if not xml:
            if passed:
                print "PASSED"
            else:
                print "FAILED"

            if i == "regression":
                BoostBuild.flush_annotations()
            BoostBuild.clear_annotations()
        else:
            rs = "succeed"
            if not passed:
                rs = "fail"
            print """
<test-log library="build" test-name="%s" test-type="run" toolset="%s" test-program="%s" target-directory="%s">
<run result="%s">""" % (i, toolset, "tools/build/v2/test/" + i + ".py",
                        "boost/bin.v2/boost.build.tests/" + toolset + "/" + i, rs)

            if not passed:
                BoostBuild.flush_annotations(1)

            print """
</run>
</test-log>
"""
        if passed:
            pass_count = pass_count + 1
        sys.stdout.flush()  # Makes testing under emacs more entertaining.

    # Erase the file on success.
    if failures_count == 0:
        open('test_results.txt', 'w')

    if not xml:
        print """
        === Test summary ===
        PASS: %d
        FAIL: %d
        """ % (pass_count, failures_count)


def last_failed_test():
    "Returns the name of last failed test or None"
    try:
        f = open("test_results.txt")
        s = string.strip(f.read())
        return s
    except:
        return None


def reorder_tests(tests, first_test):
    try:
        n = tests.index(first_test)
        return [first_test] + tests[:n] + tests[n+1:]
    except ValueError:
        return tests


critical_tests = ["unit_tests", "module_actions", "startup_v2"]

critical_tests += ["core_d12", "core_typecheck", "core_delete_module",
                   "core_language", "core_arguments", "core_varnames", "core_import_module"]

tests = [ "absolute_sources",
          "alias",
          "alternatives",
          "bad_dirname",
          "build_dir",
          "build_file",
          "build_no",
          "builtin_echo",
          "builtin_exit",
          "c_file",
          "chain",
          "clean",
          "composite",
          "conditionals",
          "conditionals2",
          "conditionals3",
          "conditionals_multiple",
          "configuration",
          "copy_time",
          "core_action_status",
          "core_actions_quietly",
          "core_at_file",
          "core_bindrule",
          "core_nt_line_length",
          "core_option_d2",
          "core_option_l",
          "core_option_n",
          "core_parallel_actions",
          "core_parallel_multifile_actions_1",
          "core_parallel_multifile_actions_2",
          "core_update_now",
          "custom_generator",
          "default_build",
          "default_features",
# This test is known to be broken itself.
#          "default_toolset",
          "dependency_property",
          "dependency_test",
          "direct_request_test",
          "disambiguation",
          "dll_path",
          "double_loading",
          "duplicate",
          "example_libraries",
          "example_make",
          "expansion",
          "explicit",
          "free_features_request",
          "generator_selection",
          "generators_test",
          "implicit_dependency",
          "indirect_conditional",
          "inherit_toolset",
          "inherited_dependency",
          "inline",
          "lib_source_property",
          "library_chain",
          "library_property",
          "load_order",
          "loop",
          "make_rule",
          "ndebug",
          "no_type",
          "notfile",
          "ordered_include",
          "out_of_tree",
          "path_features",
          "prebuilt",
          "print",
          "project_dependencies",
          "project_glob",
          "project_root_constants",
          "project_root_rule",
          "project_test3",
          "project_test4",
          "property_expansion",
          "rebuilds",
          "regression",
          "relative_sources",
          "remove_requirement",
          "resolution",
          "searched_lib",
          "skipping",
          "sort_rule",
          "source_locations",
          "stage",
          "standalone",
          "suffix",
          "tag",
          "test_result_dumping",
          "testing_support",
          "timedata",
          "unit_test",
          "unused",
          "use_requirements",
          "using",
          "wrapper",
          "wrong_project",
          "exit_status",
          ]

if os.name == 'posix':
    tests.append("symlink")
    # On windows, library order is not important, so skip this test. Besides, it
    # fails ;-). Further, the test relies on the fact that on Linux, one can
    # build a shared library with unresolved symbols. This is not true on
    # Windows (even with cygwin gcc).
    if string.find(os.uname()[0], "CYGWIN") == -1:
        tests.append("library_order")

if string.find(BoostBuild.get_toolset(), 'gcc') == 0:
    tests.append("gcc_runtime")

if ( string.find(BoostBuild.get_toolset(), 'gcc') == 0 )or  \
    ( string.find(BoostBuild.get_toolset(), 'msvc') == 0 ):
    tests.append("pch")

if "--extras" in sys.argv:
    tests.append("boostbook")
    tests.append("qt4")
    tests.append("qt5")
    tests.append("example_qt4")
    # Requires ./whatever.py to work, so is not guaranted to work everywhere.
    tests.append("example_customization")
    # Requires gettext tools.
    tests.append("example_gettext")

elif not xml:
    print 'Note: skipping extra tests'

run_tests(critical_tests, tests)
