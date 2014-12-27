/* Copyright Vladimir Prus 2003. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#include "../native.h"
#include "../timestamp.h"
#include "../object.h"
#include "../strings.h"
#include "../lists.h"
#include "../variable.h"
#include "../compile.h"

LIST* get_grist(char* f)
{
    char* end = strchr(f, '>');
    string s[1];
    LIST* result;

    string_new(s);

    string_append_range(s, f, end+1);
    result = list_new(object_new(s->value));

    string_free(s);
    return result;
}

/*
rule create ( raw-properties * )
{
    raw-properties = [ sequence.unique
        [ sequence.insertion-sort $(raw-properties) ] ] ;

    local key = $(raw-properties:J=-:E=) ;

    if ! $(.ps.$(key))
    {
        .ps.$(key) = [ new property-set $(raw-properties) ] ;
    }
    return $(.ps.$(key)) ;
}
*/

LIST *property_set_create( FRAME *frame, int flags )
{
    LIST* properties = lol_get( frame->args, 0 );
    LIST* sorted = L0;
#if 0
    LIST* order_sensitive = 0;
#endif
    LIST* unique;
    LIST* val;
    string var[1];
    OBJECT* name;
    LISTITER iter, end;

#if 0
    /* Sort all properties which are not order sensitive */
    for(tmp = properties; tmp; tmp = tmp->next) {
        LIST* g = get_grist(tmp->string);
        LIST* att = call_rule("feature.attributes", frame, g, 0);
        if (list_in(att, "order-sensitive")) {
            order_sensitive = list_new( order_sensitive, copystr(tmp->string));
        } else {
            sorted = list_new( sorted, copystr(tmp->string));
        }
        list_free(att);
    }

    sorted = list_sort(sorted);
    sorted = list_append(sorted, order_sensitive);
    unique = list_unique(sorted);
#endif
    sorted = list_sort(properties);
    unique = list_unique(sorted);

    string_new(var);
    string_append(var, ".ps.");

    iter = list_begin( unique ), end = list_end( unique );
    for( ; iter != end; iter = list_next( iter ) ) {
        string_append(var, object_str( list_item( iter ) ));
        string_push_back(var, '-');
    }
    name = object_new(var->value);
    val = var_get(frame->module, name);
    if (list_empty(val))
    {
        OBJECT* rulename = object_new("new");
        val = call_rule(rulename, frame,
                        list_append(list_new(object_new("property-set")), unique), 0);
        object_free(rulename);

        var_set(frame->module, name, list_copy(val), VAR_SET);
    }
    else
    {
        list_free(unique);
        val = list_copy(val);
    }
    object_free(name);

    string_free(var);
    /* The 'unique' variable is freed in 'call_rule'. */
    list_free(sorted);

    return val;

}

void init_property_set()
{
    {
        const char* args[] = { "raw-properties", "*", 0 };
        declare_native_rule("property-set", "create", args, property_set_create, 1);
    }
}
