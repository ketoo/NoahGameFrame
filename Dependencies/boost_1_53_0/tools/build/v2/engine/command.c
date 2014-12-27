/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * command.c - maintain lists of commands
 */

#include "jam.h"

#include "lists.h"
#include "parse.h"
#include "variable.h"
#include "rules.h"

#include "command.h"
#include <limits.h>
#include <string.h>


/*
 * cmd_new() - return a new CMD or 0 if too many args
 */

CMD * cmd_new( RULE * rule, LIST * targets, LIST * sources, LIST * shell )
{
    CMD * cmd = (CMD *)BJAM_MALLOC( sizeof( CMD ) );
    LISTITER iter = list_begin( shell ), end = list_end( shell );
    /* Lift line-length limitation entirely when JAMSHELL is just "%". */
    int no_limit = ( iter != end && !strcmp( object_str( list_item( iter ) ), "%") && list_next( iter ) == end );
    int max_line = MAXLINE;
    FRAME frame[1];

    cmd->rule = rule;
    cmd->shell = shell;
    cmd->next = 0;

    lol_init( &cmd->args );
    lol_add( &cmd->args, targets );
    lol_add( &cmd->args, sources );
    string_new( cmd->buf );

    frame_init( frame );
    frame->module = rule->module;
    lol_init( frame->args );
    lol_add( frame->args, list_copy( targets ) );
    lol_add( frame->args, list_copy( sources ) );
    function_run_actions( rule->actions->command, frame, stack_global(), cmd->buf );
    frame_free( frame );

    if ( !no_limit )
    {
        /* Bail if the result will not fit in MAXLINE. */
        char * s = cmd->buf->value;
        while ( *s )
        {
            size_t l = strcspn( s, "\n" );

            if ( l > MAXLINE )
            {
                /* We do not free targets/sources/shell if bailing. */
                cmd_free( cmd );
                return 0;
            }

            s += l;
            if ( *s )
                ++s;
        }
    }

    return cmd;
}


/*
 * cmd_free() - free a CMD
 */

void cmd_free( CMD * cmd )
{
    lol_free( &cmd->args );
    list_free( cmd->shell );
    string_free( cmd->buf );
    BJAM_FREE( (void *)cmd );
}
