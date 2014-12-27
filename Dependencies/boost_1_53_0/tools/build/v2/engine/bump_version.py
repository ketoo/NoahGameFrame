#!/usr/bin/python

# This script is used to bump version of bjam. It takes a single argument, e.g
#
#    ./bump_version.py 3.1.9
#
# and updates all necessary files. For the time being, it's assumes presense
# of 'perl' executable and Debian-specific 'dch' executable.
#


import os
import os.path
import re
import string
import sys

srcdir = os.path.abspath(os.path.dirname(__file__ ))
docdir = os.path.abspath(os.path.join(srcdir,"..","doc"))

def edit(file,replacements):
    print "  '%s'..." %(file)
    text = open(file,'r').read()
    while len(replacements) > 0:
        #~ print  "  '%s' ==> '%s'" % (replacements[0],replacements[1])
        text = re.compile(replacements[0],re.M).subn(replacements[1],text)[0]
        replacements = replacements[2:]
    #~ print text
    open(file,'w').write(text)

def make_edits(version):
    edit(os.path.join(srcdir,"boost-jam.spec"), [
        '^Version:.*$','Version: %s' % string.join(version, "."),
        ])

    edit(os.path.join(srcdir,"build.jam"), [
        '^_VERSION_ = .* ;$','_VERSION_ = %s %s %s ;' % (version[0], version[1], version[2]),
        ])

    edit(os.path.join(docdir,"bjam.qbk"), [
        '\[version.*\]','[version: %s]' % string.join(version, '.'),
        '\[def :version:.*\]','[def :version: %s]' % string.join(version, '.'),
        ])

    edit(os.path.join(srcdir,"patchlevel.h"), [
        '^#define VERSION_MAJOR .*$',
            '#define VERSION_MAJOR %s' % (version[0]),
        '^#define VERSION_MINOR .*$',
            '#define VERSION_MINOR %s' % (version[1]),
        '^#define VERSION_PATCH .*$',
            '#define VERSION_PATCH %s' % (version[2]),
        '^#define VERSION_MAJOR_SYM .*$',
            '#define VERSION_MAJOR_SYM "0%s"' % (version[0]),
        '^#define VERSION_MINOR_SYM .*$',
            '#define VERSION_MINOR_SYM "%s"' % (version[1]),
        '^#define VERSION_PATCH_SYM .*$',
            '#define VERSION_PATCH_SYM "%s"' % (version[2]),
        '^#define VERSION .*$',
            '#define VERSION "%s"' % string.join(version, '.'),
        '^#define JAMVERSYM .*$',
            '#define JAMVERSYM "JAMVERSION=%s.%s"' % (version[0],version[1]),
        ])

def main():

    if len(sys.argv) < 2:
        print "Expect new version as argument"
        sys.exit(1)

    version = string.split(sys.argv[1], ".")
    print "Setting version to", version
    make_edits(version)

if __name__ == '__main__':
    main()

#~ Copyright 2006 Rene Rivera.
#~ Copyright 2005-2006 Vladimir Prus.
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
