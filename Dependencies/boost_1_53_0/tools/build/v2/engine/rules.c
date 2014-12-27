/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "lists.h"
# include "parse.h"
# include "variable.h"
# include "rules.h"
# include "object.h"
# include "hash.h"
# include "modules.h"
# include "search.h"
# include "lists.h"
# include "pathsys.h"
# include "timestamp.h"

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * rules.c - access to RULEs, TARGETs, and ACTIONs
 *
 * External routines:
 *
 *    bindrule()     - return pointer to RULE, creating it if necessary.
 *    bindtarget()   - return pointer to TARGET, creating it if necessary.
 *    touch_target() - mark a target to simulate being new.
 *    targetlist()   - turn list of target names into a TARGET chain.
 *    targetentry()  - add a TARGET to a chain of TARGETS.
 *    actionlist()   - append to an ACTION chain.
 *    addsettings()  - add a deferred "set" command to a target.
 *    pushsettings() - set all target specific variables.
 *    popsettings()  - reset target specific variables to their pre-push values.
 *    freesettings() - delete a settings list.
 *    rules_done()   - free RULE and TARGET tables.
 *
 * 04/12/94 (seiwald) - actionlist() now just appends a single action.
 * 08/23/94 (seiwald) - Support for '+=' (append to variable)
 */

static void set_rule_actions( RULE *, rule_actions * );
static void set_rule_body   ( RULE *, FUNCTION * procedure );

static struct hash * targethash = 0;


/*
 * target_include() - adds the 'included' TARGET to the list of targets included
 * by the 'including' TARGET. Such targets are modeled as dependencies of the
 * internal include node belonging to the 'including' TARGET.
 */

void target_include( TARGET * including, TARGET * included )
{
    TARGET * internal;
    if ( !including->includes )
    {
        including->includes = copytarget( including );
        including->includes->original_target = including;
    }
    internal = including->includes;
    internal->depends = targetentry( internal->depends, included );
}


/*
 * enter_rule() - return pointer to RULE, creating it if necessary in
 * target_module.
 */

static RULE * enter_rule( OBJECT * rulename, module_t * target_module )
{
    int found;
    RULE * r;

    r = (RULE *)hash_insert( demand_rules(target_module), rulename, &found );
    if ( !found )
    {
        r->name = object_copy( rulename );
        r->procedure = 0;
        r->module = 0;
        r->actions = 0;
        r->exported = 0;
        r->module = target_module;
    }
    return r;
}


/*
 * define_rule() - return pointer to RULE, creating it if necessary in
 * target_module. Prepare it to accept a body or action originating in
 * src_module.
 */

static RULE * define_rule
(
    module_t * src_module,
    OBJECT   * rulename,
    module_t * target_module
)
{
    RULE * r = enter_rule( rulename, target_module );
    if ( r->module != src_module ) /* if the rule was imported from elsewhere, clear it now */
    {
        set_rule_body( r, 0 );
        set_rule_actions( r, 0 );
        r->module = src_module; /* r will be executed in the source module */
    }
    return r;
}


void rule_free( RULE * r )
{
    object_free( r->name );
    r->name = 0;
    if ( r->procedure )
        function_free( r->procedure );
    r->procedure = 0;
    if ( r->actions )
        actions_free( r->actions );
    r->actions = 0;
}


/*
 * bindtarget() - return pointer to TARGET, creating it if necessary.
 */

TARGET * bindtarget( OBJECT * target_name )
{
    int found;
    TARGET * t;

    if ( !targethash )
        targethash = hashinit( sizeof( TARGET ), "targets" );

    t = (TARGET *)hash_insert( targethash, target_name, &found );
    if ( !found )
    {
        memset( (char *)t, '\0', sizeof( *t ) );
        t->name = object_copy( target_name );
        t->boundname = object_copy( t->name );  /* default for T_FLAG_NOTFILE */
    }

    return t;
}


static void bind_explicitly_located_target( void * xtarget, void * data )
{
    TARGET * t = (TARGET *)xtarget;
    if ( !( t->flags & T_FLAG_NOTFILE ) )
    {
        /* Check if there's a setting for LOCATE */
        SETTINGS * s = t->settings;
        for ( ; s ; s = s->next )
        {
            if ( strcmp( object_str( s->symbol ), "LOCATE" ) == 0 )
            {
                pushsettings( root_module(), t->settings );
                /* We are binding a target with explicit LOCATE. So third
                 * argument is of no use: nothing will be returned through it.
                 */
                object_free( t->boundname );
                t->boundname = search( t->name, &t->time, 0, 0 );
                popsettings( root_module(), t->settings );
                break;
            }
        }
    }
}


void bind_explicitly_located_targets()
{
    if ( targethash )
        hashenumerate( targethash, bind_explicitly_located_target, (void *)0 );
}


/*
 * copytarget() - make a new target with the old target's name.
 *
 * Not entered into hash table -- for internal nodes.
 */

TARGET * copytarget( const TARGET * ot )
{
    TARGET * t = (TARGET *)BJAM_MALLOC( sizeof( *t ) );
    memset( (char *)t, '\0', sizeof( *t ) );
    t->name = object_copy( ot->name );
    t->boundname = object_copy( t->name );

    t->flags |= T_FLAG_NOTFILE | T_FLAG_INTERNAL;

    return t;
}


/*
 * touch_target() - mark a target to simulate being new.
 */

void touch_target( OBJECT * t )
{
    bindtarget( t )->flags |= T_FLAG_TOUCHED;
}


/*
 * targetlist() - turn list of target names into a TARGET chain.
 *
 * Inputs:
 *  chain   existing TARGETS to append to
 *  targets list of target names
 */

TARGETS * targetlist( TARGETS * chain, LIST * target_names )
{
    LISTITER iter = list_begin( target_names ), end = list_end( target_names );
    for ( ; iter != end; iter = list_next( iter ) )
        chain = targetentry( chain, bindtarget( list_item( iter ) ) );
    return chain;
}


/*
 * targetentry() - add a TARGET to a chain of TARGETS.
 *
 * Inputs:
 *  chain   existing TARGETS to append to
 *  target  new target to append
 */

TARGETS * targetentry( TARGETS * chain, TARGET * target )
{
    TARGETS * c = (TARGETS *)BJAM_MALLOC( sizeof( TARGETS ) );
    c->target = target;

    if ( !chain ) chain = c;
    else chain->tail->next = c;
    chain->tail = c;
    c->next = 0;

    return chain;
}


/*
 * targetchain() - append two TARGET chains.
 *
 * Inputs:
 *  chain   exisitng TARGETS to append to
 *  target  new target to append
 */

TARGETS * targetchain( TARGETS * chain, TARGETS * targets )
{
    if ( !targets ) return chain;
    if ( !chain   ) return targets;

    chain->tail->next = targets;
    chain->tail = targets->tail;

    return chain;
}

/*
 * action_free - decrement the ACTIONs refrence count
 * and (maybe) free it.
 */

void action_free ( ACTION * action )
{
    if ( --action->refs == 0 )
    {
        freetargets( action->targets );
        freetargets( action->sources );
        BJAM_FREE( action );
    }
}

/*
 * actionlist() - append to an ACTION chain.
 */

ACTIONS * actionlist( ACTIONS * chain, ACTION * action )
{
    ACTIONS * actions = (ACTIONS *)BJAM_MALLOC( sizeof( ACTIONS ) );

    actions->action = action;

    ++action->refs;
    if ( !chain ) chain = actions;
    else chain->tail->next = actions;
    chain->tail = actions;
    actions->next = 0;

    return chain;
}

static SETTINGS * settings_freelist;


/*
 * addsettings() - add a deferred "set" command to a target.
 *
 * Adds a variable setting (varname=list) onto a chain of settings for a
 * particular target. 'flag' controls the relationship between new and old
 * values in the same way as in var_set() function (see variable.c). Returns
 * the head of the settings chain.
 */

SETTINGS * addsettings( SETTINGS * head, int flag, OBJECT * symbol, LIST * value )
{
    SETTINGS * v;

    /* Look for previous settings. */
    for ( v = head; v; v = v->next )
        if ( object_equal( v->symbol, symbol ) )
            break;

    /* If not previously set, alloc a new. */
    /* If appending, do so. */
    /* Else free old and set new. */
    if ( !v )
    {
        v = settings_freelist;

        if ( v )
            settings_freelist = v->next;
        else
            v = (SETTINGS *)BJAM_MALLOC( sizeof( *v ) );

        v->symbol = object_copy( symbol );
        v->value = value;
        v->next = head;
        head = v;
    }
    else if ( flag == VAR_APPEND )
    {
        v->value = list_append( v->value, value );
    }
    else if ( flag != VAR_DEFAULT )
    {
        list_free( v->value );
        v->value = value;
    }
    else
        list_free( value );

    /* Return (new) head of list. */
    return head;
}


/*
 * pushsettings() - set all target specific variables.
 */

void pushsettings( struct module_t * module, SETTINGS * v )
{
    for ( ; v; v = v->next )
        v->value = var_swap( module, v->symbol, v->value );
}


/*
 * popsettings() - reset target specific variables to their pre-push values.
 */

void popsettings( struct module_t * module, SETTINGS * v )
{
    pushsettings( module, v );  /* just swap again */
}


/*
 * copysettings() - duplicate a settings list, returning the new copy.
 */

SETTINGS * copysettings( SETTINGS * head )
{
    SETTINGS * copy = 0;
    SETTINGS * v;
    for ( v = head; v; v = v->next )
        copy = addsettings( copy, VAR_SET, v->symbol, list_copy( v->value ) );
    return copy;
}


/*
 * freetargets() - delete a targets list.
 */

void freetargets( TARGETS * chain )
{
    while ( chain )
    {
        TARGETS * n = chain->next;
        BJAM_FREE( chain );
        chain = n;
    }
}


/*
 * freeactions() - delete an action list.
 */

void freeactions( ACTIONS * chain )
{
    while ( chain )
    {
        ACTIONS * n = chain->next;
        action_free( chain->action );
        BJAM_FREE( chain );
        chain = n;
    }
}


/*
 * freesettings() - delete a settings list.
 */

void freesettings( SETTINGS * v )
{
    while ( v )
    {
        SETTINGS * n = v->next;
        object_free( v->symbol );
        list_free( v->value );
        v->next = settings_freelist;
        settings_freelist = v;
        v = n;
    }
}


static void freetarget( void * xt, void * data )
{
    TARGET * t = (TARGET *)xt;
    if ( t->name       ) object_free ( t->name       );
    if ( t->boundname  ) object_free ( t->boundname  );
    if ( t->settings   ) freesettings( t->settings   );
    if ( t->depends    ) freetargets ( t->depends    );
    if ( t->dependants ) freetargets ( t->dependants );
    if ( t->parents    ) freetargets ( t->parents    );
    if ( t->actions    ) freeactions ( t->actions    );

    if ( t->includes   )
    {
        freetarget( t->includes, (void *)0 );
        BJAM_FREE( t->includes );
    }
}


/*
 * rules_done() - free RULE and TARGET tables.
 */

void rules_done()
{
    if ( targethash )
    {
        hashenumerate( targethash, freetarget, 0 );
        hashdone( targethash );
    }
    while ( settings_freelist )
    {
        SETTINGS * n = settings_freelist->next;
        BJAM_FREE( settings_freelist );
        settings_freelist = n;
    }
}


/*
 * actions_refer() - add a new reference to the given actions.
 */

void actions_refer( rule_actions * a )
{
    ++a->reference_count;
}


/*
 * actions_free() - release a reference to the given actions.
 */

void actions_free( rule_actions * a )
{
    if ( --a->reference_count <= 0 )
    {
        function_free( a->command );
        list_free( a->bindlist );
        BJAM_FREE( a );
    }
}

/*
 * set_rule_body() - set the argument list and procedure of the given rule.
 */

static void set_rule_body( RULE * rule, FUNCTION * procedure )
{
    if ( procedure )
        function_refer( procedure );
    if ( rule->procedure )
        function_free( rule->procedure );
    rule->procedure = procedure;
}


/*
 * global_name() - given a rule, return the name for a corresponding rule in the
 * global module.
 */

static OBJECT * global_rule_name( RULE * r )
{
    if ( r->module == root_module() )
        return object_copy( r->name );

    {
        char name[4096] = "";
        if ( r->module->name )
        {
            strncat( name, object_str( r->module->name ), sizeof( name ) - 1 );
            strncat( name, ".", sizeof( name ) - 1 );
        }
        strncat( name, object_str( r->name ), sizeof( name ) - 1 );
        return object_new( name );
    }
}


/*
 * global_rule() - given a rule, produce the corresponding entry in the global
 * module.
 */

static RULE * global_rule( RULE * r )
{
    if ( r->module == root_module() )
        return r;

    {
        OBJECT * name = global_rule_name( r );
        RULE * result = define_rule( r->module, name, root_module() );
        object_free( name );
        return result;
    }
}


/*
 * new_rule_body() - make a new rule named rulename in the given module, with
 * the given argument list and procedure. If exported is true, the rule is
 * exported to the global module as modulename.rulename.
 */

RULE * new_rule_body( module_t * m, OBJECT * rulename, FUNCTION * procedure, int exported )
{
    RULE * local = define_rule( m, rulename, m );
    local->exported = exported;
    set_rule_body( local, procedure );

    /* Mark the procedure with the global rule name, regardless of whether the
     * rule is exported. That gives us something reasonably identifiable that we
     * can use, e.g. in profiling output. Only do this once, since this could be
     * called multiple times with the same procedure.
     */
    if ( function_rulename( procedure ) == 0 )
        function_set_rulename( procedure, global_rule_name( local ) );

    return local;
}


static void set_rule_actions( RULE * rule, rule_actions * actions )
{
    if ( actions )
        actions_refer( actions );
    if ( rule->actions )
        actions_free( rule->actions );
    rule->actions = actions;
}


static rule_actions * actions_new( FUNCTION * command, LIST * bindlist, int flags )
{
    rule_actions * result = (rule_actions *)BJAM_MALLOC( sizeof( rule_actions ) );
    function_refer( command );
    result->command = command;
    result->bindlist = bindlist;
    result->flags = flags;
    result->reference_count = 0;
    return result;
}


RULE * new_rule_actions( module_t * m, OBJECT * rulename, FUNCTION * command, LIST * bindlist, int flags )
{
    RULE * local = define_rule( m, rulename, m );
    RULE * global = global_rule( local );
    set_rule_actions( local, actions_new( command, bindlist, flags ) );
    set_rule_actions( global, local->actions );
    return local;
}


/*
 * Looks for a rule in the specified module, and returns it, if found. First
 * checks if the rule is present in the module's rule table. Second, if name of
 * the rule is in the form name1.name2 and name1 is in the list of imported
 * modules, look in module 'name1' for rule 'name2'.
 */

RULE * lookup_rule( OBJECT * rulename, module_t * m, int local_only )
{
    RULE     * r;
    RULE     * result = 0;
    module_t * original_module = m;

    if ( m->class_module )
        m = m->class_module;

    if ( m->rules && ( r = (RULE *)hash_find( m->rules, rulename ) ) )
        result = r;
    else if ( !local_only && m->imported_modules )
    {
        /* Try splitting the name into module and rule. */
        char *p = strchr( object_str( rulename ), '.' ) ;
        if ( p )
        {
            string buf[1];
            OBJECT * module_part;
            OBJECT * rule_part;
            string_new( buf );
            string_append_range( buf, object_str( rulename ), p );
            module_part = object_new( buf->value );
            rule_part = object_new( p + 1 );
            /* Now, r->name keeps the module name, and p+1 keeps the rule name.
             */
            if ( hash_find( m->imported_modules, module_part ) )
                result = lookup_rule( rule_part, bindmodule( module_part ), 1 );
            object_free( rule_part );
            object_free( module_part );
            string_free( buf );
        }
    }

    if ( result )
    {
        if ( local_only && !result->exported )
            result = 0;
        else
        {
            /* Lookup started in class module. We have found a rule in class
             * module, which is marked for execution in that module, or in some
             * instances. Mark it for execution in the instance where we started
             * the lookup.
             */
            int execute_in_class = ( result->module == m );
            int execute_in_some_instance = ( result->module->class_module &&
                ( result->module->class_module == m ) );
            if ( ( original_module != m ) &&
                ( execute_in_class || execute_in_some_instance ) )
                result->module = original_module;
        }
    }

    return result;
}


RULE * bindrule( OBJECT * rulename, module_t * m )
{
    RULE * result = lookup_rule( rulename, m, 0 );
    if ( !result )
        result = lookup_rule( rulename, root_module(), 0 );
    /* We have only one caller, 'evaluate_rule', which will complain about
     * calling an undefined rule. We could issue the error here, but we do not
     * have the necessary information, such as frame.
     */
    if ( !result )
        result = enter_rule( rulename, m );
    return result;
}


RULE * import_rule( RULE * source, module_t * m, OBJECT * name )
{
    RULE * dest = define_rule( source->module, name, m );
    set_rule_body( dest, source->procedure );
    set_rule_actions( dest, source->actions );
    return dest;
}


void rule_localize( RULE * rule, module_t * m )
{
    rule->module = m;
    if ( rule->procedure )
    {
        FUNCTION * procedure = function_unbind_variables( rule->procedure );
        function_refer( procedure );
        function_free( rule->procedure );
        rule->procedure = procedure;
    }
}

