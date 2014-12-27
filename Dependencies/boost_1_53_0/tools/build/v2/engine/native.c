/* Copyright Vladimir Prus 2003. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#include "native.h"
#include "hash.h"
#include "object.h"
#include "assert.h"

void declare_native_rule( const char * module, const char * rule, const char * * args,
                          LIST * (*f)( FRAME *, int ), int version )
{
    OBJECT * module_obj = 0;
    module_t * m;
    if ( module )
    {
        module_obj = object_new( module );
    }
    m = bindmodule( module_obj );
    if ( module_obj )
    {
        object_free( module_obj );
    }
    if (m->native_rules == 0)
    {
        m->native_rules = hashinit( sizeof( native_rule_t ), "native rules");
    }

    {
        native_rule_t *np;
        OBJECT * name = object_new( rule );
        int found;
        np = (native_rule_t *)hash_insert( m->native_rules, name, &found );
        np->name = name;
        assert( !found );
        np->procedure = function_builtin( f, 0, args );
        np->version = version;
    }
}
