/*
    Copyright 2007 Rene Rivera
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

#include "jam.h"
#include "output.h"
#include "object.h"
#include <stdio.h>

#define bjam_out (stdout)
#define bjam_err (stderr)

static void out_
(
    char const * data,
    FILE       * io
)
{
    while ( *data )
    {
        size_t len = strcspn(data,"\r");
        data += fwrite(data,1,len,io);
        if ( *data == '\r' ) ++data;
    }
}


void out_action
(
    char const * action,
    char const * target,
    char const * command,
    char const * out_data,
    char const * err_data,
    int          exit_reason
)
{
    /* Print out the action+target line, if the action is quite the action
     * should be null.
     */
    if ( action )
    {
        fprintf( bjam_out, "%s %s\n", action, target );
    }

    /* Print out the command executed if given -d+2. */
    if ( DEBUG_EXEC )
    {
        fputs( command, bjam_out );
        fputc( '\n', bjam_out );
    }

    /* Print out the command executed to the command stream. */
    if ( globs.cmdout )
    {
        fputs( command, globs.cmdout );
    }

    switch ( exit_reason )
    {
        case EXIT_OK:
            break;
        case EXIT_FAIL:
            break;
        case EXIT_TIMEOUT:
        {
            /* Process expired, make user aware with explicit message. */
            if ( action )
            {
                /* But only output for non-quietly actions. */
                fprintf( bjam_out, "%ld second time limit exceeded\n", globs.timeout );
            }
            break;
        }
        default:
          break;
    }

    /* Print out the command output, if requested, or if the program failed. */
    if ( action || exit_reason != EXIT_OK)
    {
        /* But only output for non-quietly actions. */
        if ( ( 0 != out_data ) &&
           ( ( globs.pipe_action & 1 /* STDOUT_FILENO */ ) ||
             ( globs.pipe_action == 0 ) ) )
        {
            out_( out_data, bjam_out );
        }
        if ( ( 0 != err_data ) &&
            ( globs.pipe_action & 2 /* STDERR_FILENO */ ) )
        {
            out_( err_data, bjam_err );
        }
    }

    fflush( bjam_out );
    fflush( bjam_err );
    fflush( globs.cmdout );
}


OBJECT * outf_int( int value )
{
    char buffer[50];
    sprintf( buffer, "%i", value );
    return object_new( buffer );
}


OBJECT * outf_double( double value )
{
    char buffer[50];
    sprintf( buffer, "%f", value );
    return object_new( buffer );
}


OBJECT * outf_time( time_t value )
{
    char buffer[50];
    strftime( buffer, 49, "%Y-%m-%d %H:%M:%SZ", gmtime( &value ) );
    return object_new( buffer );
}
