# Niklaus Giger, 2005-03-15
# Testing whether we may run a test in absolute directories. There are no tests
# for temporary directories as this is implictly tested in a lot of other cases.

import BoostBuild
import os
import string

t = BoostBuild.Tester(arguments="pwd", executable="jam", workdir=os.getcwd(),
    pass_toolset=0)

t.write("jamroot.jam", """
actions print_pwd { pwd ; }
print_pwd pwd ;
ALWAYS pwd ;
""")

t.run_build_system(status=0)

if 'TMP' in os.environ:
  tmp_dir = os.environ.get('TMP')
else:
  tmp_dir = "/tmp"

if string.rfind(t.stdout(), tmp_dir) != -1:
    t.fail_test(1)

if string.rfind(t.stdout(), 'build/v2/test') == -1:
    t.fail_test(1)

t.run_build_system(status=1, subdir="/must/fail/with/absolute/path",
    stderr=None)

t.cleanup()
