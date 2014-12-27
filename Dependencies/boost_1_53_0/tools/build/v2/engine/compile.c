/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "compile.h"
# include "variable.h"
# include "rules.h"
# include "object.h"
# include "make.h"
# include "search.h"
# include "hdrmacro.h"
# include "hash.h"
# include "modules.h"
# include "strings.h"
# include "builtins.h"
# include "class.h"
# include "constants.h"

# include <assert.h>
# include <string.h>
# include <stdarg.h>

/*
 * compile.c - compile parsed jam statements
 *
 * External routines:
 *
 *  compile_append() - append list results of two statements
 *  compile_eval() - evaluate if to determine which leg to compile
 *  compile_foreach() - compile the "for x in y" statement
 *  compile_if() - compile 'if' rule
 *  compile_while() - compile 'while' rule
 *  compile_include() - support for 'include' - call include() on file
 *  compile_list() - expand and return a list
 *  compile_local() - declare (and set) local variables
 *  compile_null() - do nothing -- a stub for parsing
 *  compile_on() - run rule under influence of on-target variables
 *  compile_rule() - compile a single user defined rule
 *  compile_rules() - compile a chain of rules
 *  compile_set() - compile the "set variable" statement
 *  compile_setcomp() - support for `rule` - save parse tree
 *  compile_setexec() - support for `actions` - save execution string
 *  compile_settings() - compile the "on =" (set variable on exec) statement
 *  compile_switch() - compile 'switch' rule
 *
 * Internal routines:
 *
 *  debug_compile() - printf with indent to show rule expansion.
 *  evaluate_rule() - execute a rule invocation
 *
 *  builtin_depends() - DEPENDS/INCLUDES rule
 *  builtin_echo() - ECHO rule
 *  builtin_exit() - EXIT rule
 *  builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *
 * 02/03/94 (seiwald) - Changed trace output to read "setting" instead of
 *          the awkward sounding "settings".
 * 04/12/94 (seiwald) - Combined build_depends() with build_includes().
 * 04/12/94 (seiwald) - actionlist() now just appends a single action.
 * 04/13/94 (seiwald) - added shorthand L0 for null list pointer
 * 05/13/94 (seiwald) - include files are now bound as targets, and thus
 *          can make use of $(SEARCH)
 * 06/01/94 (seiwald) - new 'actions existing' does existing sources
 * 08/23/94 (seiwald) - Support for '+=' (append to variable)
 * 12/20/94 (seiwald) - NOTIME renamed NOTFILE.
 * 01/22/95 (seiwald) - Exit rule.
 * 02/02/95 (seiwald) - Always rule; LEAVES rule.
 * 02/14/95 (seiwald) - NoUpdate rule.
 * 09/11/00 (seiwald) - new evaluate_rule() for headers().
 * 09/11/00 (seiwald) - compile_xxx() now return LIST *.
 *          New compile_append() and compile_list() in
 *          support of building lists here, rather than
 *          in jamgram.yy.
 * 01/10/00 (seiwald) - built-ins split out to builtin.c.
 */

static void debug_compile( int which, const char * s, FRAME * frame );
int glob( const char * s, const char * c );
/* Internal functions from builtins.c */
void backtrace( FRAME * frame );
void backtrace_line( FRAME * frame );
void print_source_line( FRAME * frame );

struct frame * frame_before_python_call;

static OBJECT * module_scope;

void frame_init( FRAME* frame )
{
    frame->prev = 0;
    frame->prev_user = 0;
    lol_init(frame->args);
    frame->module = root_module();
    frame->rulename = "module scope";
    frame->file = 0;
    frame->line = -1;
}


void frame_free( FRAME* frame )
{
    lol_free( frame->args );
}


/*
 * evaluate_rule() - execute a rule invocation.
 */

LIST *
evaluate_rule(
    OBJECT * rulename,
    FRAME  * frame )
{
    LIST          * result = L0;
    RULE          * rule;
    profile_frame   prof[1];
    module_t      * prev_module = frame->module;

    rule = bindrule( rulename, frame->module );

    if ( DEBUG_COMPILE )
    {
        /* Try hard to indicate in which module the rule is going to execute. */
        if ( rule->module != frame->module
             && rule->procedure != 0 && !object_equal( rulename, function_rulename( rule->procedure ) ) )
        {
            char buf[256] = "";
            if ( rule->module->name )
            {
                strncat( buf, object_str( rule->module->name ), sizeof( buf ) - 1 );
                strncat( buf, ".", sizeof( buf ) - 1 );
            }
            strncat( buf, object_str( rule->name ), sizeof( buf ) - 1 );
            debug_compile( 1, buf, frame );
        }
        else
        {
            debug_compile( 1, object_str( rulename ), frame );
        }

        lol_print( frame->args );
        printf( "\n" );
    }

    if ( rule->procedure && rule->module != prev_module )
    {
        /* Propagate current module to nested rule invocations. */
        frame->module = rule->module;
    }

    /* Record current rule name in frame. */
    if ( rule->procedure )
    {
        frame->rulename = object_str( rulename );
        /* And enter record profile info. */
        if ( DEBUG_PROFILE )
            profile_enter( function_rulename( rule->procedure ), prof );
    }

    /* Check traditional targets $(<) and sources $(>). */
    if ( !rule->actions && !rule->procedure )
    {
        backtrace_line( frame->prev );
        if ( frame->module->name )
        {
            printf( "rule %s unknown in module %s\n", object_str( rule->name ), object_str( frame->module->name ) );
        }
        else
        {
            printf( "rule %s unknown in module \n", object_str( rule->name ) );
        }
        backtrace( frame->prev );
        exit( 1 );
    }

    /* If this rule will be executed for updating the targets then construct the
     * action for make().
     */
    if ( rule->actions )
    {
        TARGETS * t;
        ACTION  * action;

        /* The action is associated with this instance of this rule. */
        action = (ACTION *)BJAM_MALLOC( sizeof( ACTION ) );
        memset( (char *)action, '\0', sizeof( *action ) );

        action->rule = rule;
        action->targets = targetlist( (TARGETS *)0, lol_get( frame->args, 0 ) );
        action->sources = targetlist( (TARGETS *)0, lol_get( frame->args, 1 ) );
        action->refs = 1;

        /* If we have a group of targets all being built using the same action
         * then we must not allow any of them to be used as sources unless they
         * had all already been built in the first place or their joined action
         * has had a chance to finish its work and build all of them anew.
         *
         * Without this it might be possible, in case of a multi-process build,
         * for their action, triggered by buiding one of the targets, to still
         * be running when another target in the group reports as done in order
         * to avoid triggering the same action again and gets used prematurely.
         *
         * As a quick-fix to achieve this effect we make all the targets list
         * each other as 'included targets'. More precisely, we mark the first
         * listed target as including all the other targets in the list and vice
         * versa. This makes anyone depending on any of those targets implicitly
         * depend on all of them, thus making sure none of those targets can be
         * used as sources until all of them have been built. Note that direct
         * dependencies could not have been used due to the 'circular
         * dependency' issue.
         *
         * TODO: Although the current implementation solves the problem of one
         * of the targets getting used before its action completes its work it
         * also forces the action to run whenever any of the targets in the
         * group is not up to date even though some of them might not actually
         * be used by the targets being built. We should see how we can
         * correctly recognize such cases and use that to avoid running the
         * action if possible and not rebuild targets not actually depending on
         * targets that are not up to date.
         *
         * TODO: Using the 'include' feature might have side-effects due to
         * interaction with the actual 'inclusion scanning' system. This should
         * be checked.
         */
        if ( action->targets )
        {
            TARGET * t0 = action->targets->target;
            for ( t = action->targets->next; t; t = t->next )
            {
                target_include( t->target, t0 );
                target_include( t0, t->target );
            }
        }

        /* Append this action to the actions of each target. */
        for ( t = action->targets; t; t = t->next )
            t->target->actions = actionlist( t->target->actions, action );

        action_free( action );
    }

    /* Now recursively compile any parse tree associated with this rule.
     * function_refer()/function_free() call pair added to ensure rule not freed
     * during use.
     */
    if ( rule->procedure )
    {
        FUNCTION * function = rule->procedure;

        function_refer( function );
        result = function_run( function, frame, stack_global() );
        function_free( function );
    }

    if ( DEBUG_PROFILE && rule->procedure )
        profile_exit( prof );

    if ( DEBUG_COMPILE )
        debug_compile( -1, 0, frame);

    return result;
}


/*
 * Call the given rule with the specified parameters. The parameters should be
 * of type LIST* and end with a NULL pointer. This differs from 'evaluate_rule'
 * in that frame for the called rule is prepared inside 'call_rule'.
 *
 * This function is useful when a builtin rule (in C) wants to call another rule
 * which might be implemented in Jam.
 */

LIST * call_rule( OBJECT * rulename, FRAME * caller_frame, ... )
{
    va_list va;
    LIST * result;

    FRAME       inner[1];
    frame_init( inner );
    inner->prev = caller_frame;
    inner->prev_user = caller_frame->module->user_module ?
        caller_frame : caller_frame->prev_user;
    inner->module = caller_frame->module;

    va_start( va, caller_frame );
    for ( ; ; )
    {
        LIST * l = va_arg( va, LIST* );
        if ( !l )
            break;
        lol_add( inner->args, l );
    }
    va_end( va );

    result = evaluate_rule( rulename, inner );

    frame_free( inner );

    return result;
}



/*
 * debug_compile() - printf with indent to show rule expansion.
 */

static void debug_compile( int which, const char * s, FRAME * frame )
{
    static int level = 0;
    static char indent[36] = ">>>>|>>>>|>>>>|>>>>|>>>>|>>>>|>>>>|";

    if ( which >= 0 )
    {
        int i;

        print_source_line( frame );

        i = ( level + 1 ) * 2;
        while ( i > 35 )
        {
            fputs( indent, stdout );
            i -= 35;
        }

        printf( "%*.*s ", i, i, indent );
    }

    if ( s )
        printf( "%s ", s );

    level += which;
}
