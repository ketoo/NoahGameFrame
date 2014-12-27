# Copyright 2003 Dave Abrahams 
# Copyright 2002, 2003, 2005, 2010 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import bjam
import b2.build.type as type
import b2.build.generators as generators

from b2.manager import get_manager

type.register("FOO", ["foo"])
generators.register_standard("foo.foo", ["FOO"], ["CPP", "H"])

def prepare_foo(targets, sources, properties):

    if properties.get('os') in ['windows', 'cygwin']:
        bjam.call('set-target-variable', targets, "DECL", 
                  "void __declspec(dllexport) foo(){}")
    
    pass

get_manager().engine().register_action("foo.foo",\
"""echo -e $(DECL:E="//")\\n > $(<[1])
echo -e "#include <z.h>\\n" > $(<[2])
""", function=prepare_foo)
