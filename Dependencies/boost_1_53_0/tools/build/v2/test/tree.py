# Copyright 2003 Dave Abrahams
# Copyright 2001, 2002 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# This file is based in part on the content of svn_tree.py.

import svn_tree;

class Trees_difference:

    def __init__(self):
        self.added_files = []
        self.removed_files = []
        self.modified_files = []
        self.touched_files = []

    def append(self, other):
        self.added_files.extend(other.added_files)
        self.removed_files.extend(other.removed_files)
        self.modified_files.extend(other.modified_files)
        self.touched_files.extend(other.touched_files)

    def ignore_directories(self):
        "Removes directories for list of found differences"

        def not_dir(x):
            return x[-1] != "/"
        self.added_files = filter(not_dir, self.added_files)
        self.removed_files = filter(not_dir, self.removed_files)
        self.modified_files = filter(not_dir, self.modified_files)
        self.touched_files = filter(not_dir, self.touched_files)

    def pprint(self, f=None):
        print >> f, "Added files   :", self.added_files
        print >> f, "Removed files :", self.removed_files
        print >> f, "Modified files:", self.modified_files
        print >> f, "Touched files :", self.touched_files

    def empty(self):
        return ( len(self.added_files) == 0 ) and \
            ( len(self.removed_files) == 0 ) and \
            ( len(self.modified_files) == 0 ) and \
            ( len(self.touched_files) == 0 )

def build_tree(dir):
    return svn_tree.build_tree_from_wc(dir, load_props=0, ignore_svn=1)

def trees_difference(a, b, current_name=""):
    """Compare SVNTreeNodes A and B, and create Trees_difference class."""

    assert a.name == b.name

    result = Trees_difference()
    try:
        # A and B are both files.
        if ((a.children is None) and (b.children is None)):
            assert a.name == b.name
            if svn_tree.compare_file_nodes(a, b):
                result.modified_files.append(current_name)
            elif (a.mtime != b.mtime):
                result.touched_files.append(current_name)

        # One is a file, one is a directory.
        # this case is disabled because svn_tree doesn't distinguish
        # empty directories from files, at least on Cygwin.
        elif 0 and (((a.children is None) and (b.children is not None))
            or ((a.children is not None) and (b.children is None))):
            a.pprint()
            b.pprint()
            raise svn_tree.SVNTypeMismatch
        # They're both directories.
        else:
            # accounted_for holds childrens present in both trees
            accounted_for = []
            for a_child in (a.children or []):
                b_child = svn_tree.get_child(b, a_child.name)
                if b_child:
                    accounted_for.append(b_child)
                    if current_name:
                        result.append(trees_difference(a_child, b_child, current_name + "/" + a_child.name))
                    else:
                        result.append(trees_difference(a_child, b_child, a_child.name))
                else:
                    if current_name:
                        result.removed_files.append(current_name + "/" + a_child.name)
                    else:
                        result.removed_files.append(a_child.name)
            for b_child in (b.children or []):
                if (b_child not in accounted_for):
                    result.added_files.extend(traverse_tree(b_child, current_name))

    except svn_tree.SVNTypeMismatch:
        print 'Unequal Types: one Node is a file, the other is a directory'
        raise svn_tree.SVNTreeUnequal
    except svn_tree.SVNTreeIsNotDirectory:
        print "Error: Foolish call to get_child."
        sys.exit(1)
    except IndexError:
        print "Error: unequal number of children"
        raise svn_tree.SVNTreeUnequal
    return result

def dump_tree(t):
    svn_tree.dump_tree(t)

def traverse_tree(t, parent_name=""):
    """ Returns the list of all names in tree. """
    if parent_name:
        full_node_name = parent_name + "/" + t.name
    else:
        full_node_name = t.name

    if (t.children is None):
        result = [full_node_name]
    else:
        result = [full_node_name + "/"]
        for i in t.children:
            result.extend(traverse_tree(i, full_node_name))
    return result
