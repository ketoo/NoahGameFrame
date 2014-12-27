"""
TestCmd.py:  a testing framework for commands and scripts.

The TestCmd module provides a framework for portable automated testing of
executable commands and scripts (in any language, not just Python), especially
commands and scripts that require file system interaction.

In addition to running tests and evaluating conditions, the TestCmd module
manages and cleans up one or more temporary workspace directories, and provides
methods for creating files and directories in those workspace directories from
in-line data, here-documents), allowing tests to be completely self-contained.

A TestCmd environment object is created via the usual invocation:

    test = TestCmd()

The TestCmd module provides pass_test(), fail_test(), and no_result() unbound
methods that report test results for use with the Aegis change management
system. These methods terminate the test immediately, reporting PASSED, FAILED
or NO RESULT respectively and exiting with status 0 (success), 1 or 2
respectively. This allows for a distinction between an actual failed test and a
test that could not be properly evaluated because of an external condition (such
as a full file system or incorrect permissions).
"""

# Copyright 2000 Steven Knight
# This module is free software, and you may redistribute it and/or modify
# it under the same terms as Python itself, so long as this copyright message
# and disclaimer are retained in their original form.
#
# IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
# SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF
# THIS CODE, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
#
# THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE.  THE CODE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS,
# AND THERE IS NO OBLIGATION WHATSOEVER TO PROVIDE MAINTENANCE,
# SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

# Copyright 2002-2003 Vladimir Prus.
# Copyright 2002-2003 Dave Abrahams.
# Copyright 2006 Rene Rivera.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#         http://www.boost.org/LICENSE_1_0.txt)


from string import join, split

__author__ = "Steven Knight <knight@baldmt.com>"
__revision__ = "TestCmd.py 0.D002 2001/08/31 14:56:12 software"
__version__ = "0.02"

from types import *

import os
import os.path
import popen2
import re
import shutil
import stat
import sys
import tempfile
import traceback


tempfile.template = 'testcmd.'

_Cleanup = []

def _clean():
    global _Cleanup
    list = _Cleanup[:]
    _Cleanup = []
    list.reverse()
    for test in list:
        test.cleanup()

sys.exitfunc = _clean


def caller(tblist, skip):
    string = ""
    arr = []
    for file, line, name, text in tblist:
        if file[-10:] == "TestCmd.py":
                break
        arr = [(file, line, name, text)] + arr
    atfrom = "at"
    for file, line, name, text in arr[skip:]:
        if name == "?":
            name = ""
        else:
            name = " (" + name + ")"
        string = string + ("%s line %d of %s%s\n" % (atfrom, line, file, name))
        atfrom = "\tfrom"
    return string


def fail_test(self=None, condition=True, function=None, skip=0):
    """Cause the test to fail.

    By default, the fail_test() method reports that the test FAILED and exits
    with a status of 1. If a condition argument is supplied, the test fails only
    if the condition is true.
    """
    if not condition:
        return
    if not function is None:
        function()
    of = ""
    desc = ""
    sep = " "
    if not self is None:
        if self.program:
            of = " of " + join(self.program, " ")
            sep = "\n\t"
        if self.description:
            desc = " [" + self.description + "]"
            sep = "\n\t"

    at = caller(traceback.extract_stack(), skip)

    sys.stderr.write("FAILED test" + of + desc + sep + at + """
in directory: """ + os.getcwd() )
    sys.exit(1)


def no_result(self=None, condition=True, function=None, skip=0):
    """Causes a test to exit with no valid result.

    By default, the no_result() method reports NO RESULT for the test and exits
    with a status of 2. If a condition argument is supplied, the test fails only
    if the condition is true.
    """
    if not condition:
        return
    if not function is None:
        function()
    of = ""
    desc = ""
    sep = " "
    if not self is None:
        if self.program:
            of = " of " + self.program
            sep = "\n\t"
        if self.description:
            desc = " [" + self.description + "]"
            sep = "\n\t"

    at = caller(traceback.extract_stack(), skip)
    sys.stderr.write("NO RESULT for test" + of + desc + sep + at)
    sys.exit(2)


def pass_test(self=None, condition=True, function=None):
    """Causes a test to pass.

    By default, the pass_test() method reports PASSED for the test and exits
    with a status of 0. If a condition argument is supplied, the test passes
    only if the condition is true.
    """
    if not condition:
        return
    if not function is None:
        function()
    sys.stderr.write("PASSED\n")
    sys.exit(0)


def match_exact(lines=None, matches=None):
    """Returns whether the given lists or strings containing lines separated
    using newline characters contain exactly the same data.
    """
    if not type(lines) is ListType:
        lines = split(lines, "\n")
    if not type(matches) is ListType:
        matches = split(matches, "\n")
    if len(lines) != len(matches):
        return
    for i in range(len(lines)):
        if lines[i] != matches[i]:
            return
    return 1


def match_re(lines=None, res=None):
    """Given lists or strings contain lines separated using newline characters.
    This function matches those lines one by one, interpreting the lines in the
    res parameter as regular expressions.
    """
    if not type(lines) is ListType:
        lines = split(lines, "\n")
    if not type(res) is ListType:
        res = split(res, "\n")
    if len(lines) != len(res):
        return
    for i in range(len(lines)):
        if not re.compile("^" + res[i] + "$").search(lines[i]):
            return
    return 1


class TestCmd:
    """Class TestCmd.
    """

    def __init__(self, description=None, program=None, workdir=None,
        subdir=None, verbose=False, match=None, inpath=None):

        self._cwd = os.getcwd()
        self.description_set(description)
        if inpath:
            self.program = program
        else:
            self.program_set(program)
        self.verbose_set(verbose)
        if not match is None:
            self.match_func = match
        else:
            self.match_func = match_re
        self._dirlist = []
        self._preserve = {'pass_test': 0, 'fail_test': 0, 'no_result': 0}
        if os.environ.has_key('PRESERVE') and not os.environ['PRESERVE'] is '':
            self._preserve['pass_test'] = os.environ['PRESERVE']
            self._preserve['fail_test'] = os.environ['PRESERVE']
            self._preserve['no_result'] = os.environ['PRESERVE']
        else:
            try:
                self._preserve['pass_test'] = os.environ['PRESERVE_PASS']
            except KeyError:
                pass
            try:
                self._preserve['fail_test'] = os.environ['PRESERVE_FAIL']
            except KeyError:
                pass
            try:
                self._preserve['no_result'] = os.environ['PRESERVE_NO_RESULT']
            except KeyError:
                pass
        self._stdout = []
        self._stderr = []
        self.status = None
        self.condition = 'no_result'
        self.workdir_set(workdir)
        self.subdir(subdir)

    def __del__(self):
        self.cleanup()

    def __repr__(self):
        return "%x" % id(self)

    def cleanup(self, condition=None):
        """Removes any temporary working directories for the specified TestCmd
        environment. If the environment variable PRESERVE was set when the
        TestCmd environment was created, temporary working directories are not
        removed. If any of the environment variables PRESERVE_PASS,
        PRESERVE_FAIL or PRESERVE_NO_RESULT were set when the TestCmd
        environment was created, then temporary working directories are not
        removed if the test passed, failed or had no result, respectively.
        Temporary working directories are also preserved for conditions
        specified via the preserve method.

        Typically, this method is not called directly, but is used when the
        script exits to clean up temporary working directories as appropriate
        for the exit status.
        """
        if not self._dirlist:
            return
        if condition is None:
            condition = self.condition
        if self._preserve[condition]:
            for dir in self._dirlist:
                print "Preserved directory", dir
        else:
            list = self._dirlist[:]
            list.reverse()
            for dir in list:
                self.writable(dir, 1)
                shutil.rmtree(dir, ignore_errors = 1)

        self._dirlist = []
        self.workdir = None
        os.chdir(self._cwd)
        try:
            global _Cleanup
            _Cleanup.remove(self)
        except (AttributeError, ValueError):
            pass

    def description_set(self, description):
        """Set the description of the functionality being tested.
        """
        self.description = description

    def fail_test(self, condition=True, function=None, skip=0):
        """Cause the test to fail.
        """
        if not condition:
            return
        self.condition = 'fail_test'
        fail_test(self = self,
                  condition = condition,
                  function = function,
                  skip = skip)

    def match(self, lines, matches):
        """Compare actual and expected file contents.
        """
        return self.match_func(lines, matches)

    def match_exact(self, lines, matches):
        """Compare actual and expected file contents.
        """
        return match_exact(lines, matches)

    def match_re(self, lines, res):
        """Compare actual and expected file contents.
        """
        return match_re(lines, res)

    def no_result(self, condition=True, function=None, skip=0):
        """Report that the test could not be run.
        """
        if not condition:
            return
        self.condition = 'no_result'
        no_result(self = self,
                  condition = condition,
                  function = function,
                  skip = skip)

    def pass_test(self, condition=True, function=None):
        """Cause the test to pass.
        """
        if not condition:
            return
        self.condition = 'pass_test'
        pass_test(self = self, condition = condition, function = function)

    def preserve(self, *conditions):
        """Arrange for the temporary working directories for the specified
        TestCmd environment to be preserved for one or more conditions. If no
        conditions are specified, arranges for the temporary working directories
        to be preserved for all conditions.
        """
        if conditions is ():
            conditions = ('pass_test', 'fail_test', 'no_result')
        for cond in conditions:
            self._preserve[cond] = 1

    def program_set(self, program):
        """Set the executable program or script to be tested.
        """
        if program and program[0] and not os.path.isabs(program[0]):
            program[0] = os.path.join(self._cwd, program[0])
        self.program = program

    def read(self, file, mode='rb'):
        """Reads and returns the contents of the specified file name. The file
        name may be a list, in which case the elements are concatenated with the
        os.path.join() method. The file is assumed to be under the temporary
        working directory unless it is an absolute path name. The I/O mode for
        the file may be specified; it must begin with an 'r'. The default is
        'rb' (binary read).
        """
        if type(file) is ListType:
            file = apply(os.path.join, tuple(file))
        if not os.path.isabs(file):
            file = os.path.join(self.workdir, file)
        if mode[0] != 'r':
            raise ValueError, "mode must begin with 'r'"
        return open(file, mode).read()

    def run(self, program=None, arguments=None, chdir=None, stdin=None):
        """Runs a test of the program or script for the test environment.
        Standard output and error output are saved for future retrieval via the
        stdout() and stderr() methods.
        """
        if chdir:
            oldcwd = os.getcwd()
            if not os.path.isabs(chdir):
                chdir = os.path.join(self.workpath(chdir))
            if self.verbose:
                sys.stderr.write("chdir(" + chdir + ")\n")
            os.chdir(chdir)
        cmd = []
        if program and program[0]:
            if program[0] != self.program[0] and not os.path.isabs(program[0]):
                program[0] = os.path.join(self._cwd, program[0])
            cmd += program
        else:
            cmd += self.program
        if arguments:
            cmd += arguments.split(" ")
        if self.verbose:
            sys.stderr.write(join(cmd, " ") + "\n")
        try:
            p = popen2.Popen3(cmd, 1)
        except AttributeError:
            # We end up here in case the popen2.Popen3 class is not available
            # (e.g. on Windows). We will be using the os.popen3() Python API
            # which takes a string parameter and so needs its executable quoted
            # in case its name contains spaces.
            cmd[0] = '"' + cmd[0] + '"'
            command_string = join(cmd, " ")
            if ( os.name == 'nt' ):
                # This is a workaround for a longstanding Python bug on Windows
                # when using os.popen(), os.system() and similar functions to
                # execute a command containing quote characters. The bug seems
                # to be related to the quote stripping functionality used by the
                # Windows cmd.exe interpreter when its /S is not specified.
                #
                # Cleaned up quote from the cmd.exe help screen as displayed on
                # Windows XP SP2:
                #
                #   1. If all of the following conditions are met, then quote
                #      characters on the command line are preserved:
                #
                #       - no /S switch
                #       - exactly two quote characters
                #       - no special characters between the two quote
                #         characters, where special is one of: &<>()@^|
                #       - there are one or more whitespace characters between
                #         the two quote characters
                #       - the string between the two quote characters is the
                #         name of an executable file.
                #
                #   2. Otherwise, old behavior is to see if the first character
                #      is a quote character and if so, strip the leading
                #      character and remove the last quote character on the
                #      command line, preserving any text after the last quote
                #      character.
                #
                # This causes some commands containing quotes not to be executed
                # correctly. For example:
                #
                #   "\Long folder name\aaa.exe" --name="Jurko" --no-surname
                #
                # would get its outermost quotes stripped and would be executed
                # as:
                #
                #   \Long folder name\aaa.exe" --name="Jurko --no-surname
                #
                # which would report an error about '\Long' not being a valid
                # command.
                #
                # cmd.exe help seems to indicate it would be enough to add an
                # extra space character in front of the command to avoid this
                # but this does not work, most likely due to the shell first
                # stripping all leading whitespace characters from the command.
                #
                # Solution implemented here is to quote the whole command in
                # case it contains any quote characters. Note thought this will
                # not work correctly should Python ever fix this bug.
                #                                    (01.05.2008.) (Jurko)
                if command_string.find('"') != -1:
                    command_string = '"' + command_string + '"'
            (tochild, fromchild, childerr) = os.popen3(command_string)
            if stdin:
                if type(stdin) is ListType:
                    for line in stdin:
                        tochild.write(line)
                else:
                    tochild.write(stdin)
            tochild.close()
            self._stdout.append(fromchild.read())
            self._stderr.append(childerr.read())
            fromchild.close()
            self.status = childerr.close()
            if not self.status:
                self.status = 0
        except:
            raise
        else:
            if stdin:
                if type(stdin) is ListType:
                    for line in stdin:
                        p.tochild.write(line)
                else:
                    p.tochild.write(stdin)
            p.tochild.close()
            self._stdout.append(p.fromchild.read())
            self._stderr.append(p.childerr.read())
            self.status = p.wait()

        if self.verbose:
            sys.stdout.write(self._stdout[-1])
            sys.stderr.write(self._stderr[-1])

        if chdir:
            os.chdir(oldcwd)

    def stderr(self, run=None):
        """Returns the error output from the specified run number. If there is
        no specified run number, then returns the error output of the last run.
        If the run number is less than zero, then returns the error output from
        that many runs back from the current run.
        """
        if not run:
            run = len(self._stderr)
        elif run < 0:
            run = len(self._stderr) + run
        run = run - 1
        if (run < 0):
            return ''
        return self._stderr[run]

    def stdout(self, run=None):
        """Returns the standard output from the specified run number. If there
        is no specified run number, then returns the standard output of the last
        run. If the run number is less than zero, then returns the standard
        output from that many runs back from the current run.
        """
        if not run:
            run = len(self._stdout)
        elif run < 0:
            run = len(self._stdout) + run
        run = run - 1
        if (run < 0):
            return ''
        return self._stdout[run]

    def subdir(self, *subdirs):
        """Create new subdirectories under the temporary working directory, one
        for each argument. An argument may be a list, in which case the list
        elements are concatenated using the os.path.join() method.
        Subdirectories multiple levels deep must be created using a separate
        argument for each level:

                test.subdir('sub', ['sub', 'dir'], ['sub', 'dir', 'ectory'])

        Returns the number of subdirectories actually created.
        """
        count = 0
        for sub in subdirs:
            if sub is None:
                continue
            if type(sub) is ListType:
                sub = apply(os.path.join, tuple(sub))
            new = os.path.join(self.workdir, sub)
            try:
                os.mkdir(new)
            except:
                pass
            else:
                count = count + 1
        return count

    def unlink (self, file):
        """Unlinks the specified file name. The file name may be a list, in
        which case the elements are concatenated using the os.path.join()
        method. The file is assumed to be under the temporary working directory
        unless it is an absolute path name.
        """
        if type(file) is ListType:
            file = apply(os.path.join, tuple(file))
        if not os.path.isabs(file):
            file = os.path.join(self.workdir, file)
        os.unlink(file)

    def verbose_set(self, verbose):
        """Set the verbose level.
        """
        self.verbose = verbose

    def workdir_set(self, path):
        """Creates a temporary working directory with the specified path name.
        If the path is a null string (''), a unique directory name is created.
        """

        if os.path.isabs(path):
            self.workdir = path
        else:
            if (path != None):
                if path == '':
                    path = tempfile.mktemp()
                if path != None:
                    os.mkdir(path)
                self._dirlist.append(path)
                global _Cleanup
                try:
                    _Cleanup.index(self)
                except ValueError:
                    _Cleanup.append(self)
                # We'd like to set self.workdir like this:
                #        self.workdir = path
                # But symlinks in the path will report things differently from
                # os.getcwd(), so chdir there and back to fetch the canonical
                # path.
                cwd = os.getcwd()
                os.chdir(path)
                self.workdir = os.getcwd()
                os.chdir(cwd)
            else:
                self.workdir = None

    def workpath(self, *args):
        """Returns the absolute path name to a subdirectory or file within the
        current temporary working directory. Concatenates the temporary working
        directory name with the specified arguments using the os.path.join()
        method.
        """
        return apply(os.path.join, (self.workdir,) + tuple(args))

    def writable(self, top, write):
        """Make the specified directory tree writable (write == 1) or not
        (write == None).
        """

        def _walk_chmod(arg, dirname, names):
            st = os.stat(dirname)
            os.chmod(dirname, arg(st[stat.ST_MODE]))
            for name in names:
                n = os.path.join(dirname, name)
                st = os.stat(n)
                os.chmod(n, arg(st[stat.ST_MODE]))

        def _mode_writable(mode):
            return stat.S_IMODE(mode|0200)

        def _mode_non_writable(mode):
            return stat.S_IMODE(mode&~0200)

        if write:
            f = _mode_writable
        else:
            f = _mode_non_writable
        try:
            os.path.walk(top, _walk_chmod, f)
        except:
            pass  # Ignore any problems changing modes.

    def write(self, file, content, mode='wb'):
        """Writes the specified content text (second argument) to the specified
        file name (first argument). The file name may be a list, in which case
        the elements are concatenated using the os.path.join() method. The file
        is created under the temporary working directory. Any subdirectories in
        the path must already exist. The I/O mode for the file may be specified;
        it must begin with a 'w'. The default is 'wb' (binary write).
        """
        if type(file) is ListType:
            file = apply(os.path.join, tuple(file))
        if not os.path.isabs(file):
            file = os.path.join(self.workdir, file)
        if mode[0] != 'w':
            raise ValueError, "mode must begin with 'w'"
        open(file, mode).write(content)
