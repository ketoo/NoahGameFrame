/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "jam.h"
#include "lists.h"
#include "search.h"
#include "timestamp.h"
#include "pathsys.h"
#include "variable.h"
#include "object.h"
#include "compile.h"
#include "strings.h"
#include "hash.h"
#include "filesys.h"
#include <string.h>


typedef struct _binding
{
    OBJECT * binding;
    OBJECT * target;
} BINDING;

static struct hash *explicit_bindings = 0;


void call_bind_rule
(
    OBJECT * target_,
    OBJECT * boundname_
)
{
    LIST * bind_rule = var_get( root_module(), constant_BINDRULE );
    if ( !list_empty( bind_rule ) )
    {
        OBJECT * target = object_copy( target_ );
        OBJECT * boundname = object_copy( boundname_ );
        if ( boundname && target )
        {
            /* Prepare the argument list. */
            FRAME frame[1];
            frame_init( frame );

            /* First argument is the target name. */
            lol_add( frame->args, list_new( target ) );

            lol_add( frame->args, list_new( boundname ) );
            if ( lol_get( frame->args, 1 ) )
                list_free( evaluate_rule( list_front( bind_rule ), frame ) );

            /* Clean up */
            frame_free( frame );
        }
        else
        {
            if ( boundname )
                object_free( boundname );
            if ( target )
                object_free( target );
        }
    }
}

/*
 * search.c - find a target along $(SEARCH) or $(LOCATE)
 * First, check if LOCATE is set. If so, use it to determine
 * the location of target and return, regardless of whether anything
 * exists on that location.
 *
 * Second, examine all directories in SEARCH. If there's file already
 * or there's another target with the same name which was placed
 * to this location via LOCATE setting, stop and return the location.
 * In case of previous target, return it's name via the third argument.
 *
 * This bevahiour allow to handle dependency on generated files. If
 * caller does not expect that target is generated, 0 can be passed as
 * the third argument.
 */

OBJECT *
search(
    OBJECT * target,
    time_t *time,
    OBJECT * * another_target,
    int file
)
{
    PATHNAME f[1];
    LIST   * varlist;
    string   buf[1];
    int      found = 0;
    /* Will be set to 1 if target location is specified via LOCATE. */
    int      explicitly_located = 0;
    OBJECT * boundname = 0;

    if ( another_target )
        *another_target = 0;

    if (! explicit_bindings )
        explicit_bindings = hashinit( sizeof(BINDING),
                                     "explicitly specified locations");

    string_new( buf );
    /* Parse the filename */

    path_parse( object_str( target ), f );

    f->f_grist.ptr = 0;
    f->f_grist.len = 0;

    varlist = var_get( root_module(), constant_LOCATE );
    if ( !list_empty( varlist ) )
    {
        OBJECT * key;
        f->f_root.ptr = object_str( list_front( varlist ) );
        f->f_root.len = strlen( object_str( list_front( varlist ) ) );

        path_build( f, buf, 1 );

        if ( DEBUG_SEARCH )
            printf( "locate %s: %s\n", object_str( target ), buf->value );

        explicitly_located = 1;

        key = object_new( buf->value );
        timestamp( key, time );
        object_free( key );
        found = 1;
    }
    else if ( varlist = var_get( root_module(), constant_SEARCH ), !list_empty( varlist ) )
    {
        LISTITER iter = list_begin( varlist ), end = list_end( varlist );
        for ( ; iter != end; iter = list_next( iter ) )
        {
            BINDING * ba;
            file_info_t *ff;
            OBJECT * key;
            OBJECT * test_path;

            f->f_root.ptr = object_str( list_item( iter ) );
            f->f_root.len = strlen( object_str( list_item( iter ) ) );

            string_truncate( buf, 0 );
            path_build( f, buf, 1 );

            if ( DEBUG_SEARCH )
                printf( "search %s: %s\n", object_str( target ), buf->value );

            test_path = object_new( buf->value );
            key = path_as_key( test_path );
            object_free( test_path );
            ff = file_query( key );
            timestamp( key, time );

            if ( ( ba = (BINDING *)hash_find( explicit_bindings, key ) ) )
            {
                if ( DEBUG_SEARCH )
                    printf(" search %s: found explicitly located target %s\n",
                           object_str( target ), object_str( ba->target ) );
                if ( another_target )
                    *another_target = ba->target;
                found = 1;
                object_free( key );
                break;
            }
            else if ( ff && ff->time )
            {
                if ( !file || ff->is_file )
                {
                    found = 1;
                    object_free( key );
                    break;
                }
            }
            object_free( key );
        }
    }

    if ( !found )
    {
        /* Look for the obvious */
        /* This is a questionable move.  Should we look in the */
        /* obvious place if SEARCH is set? */
        OBJECT * key;

        f->f_root.ptr = 0;
        f->f_root.len = 0;

        string_truncate( buf, 0 );
        path_build( f, buf, 1 );

        if ( DEBUG_SEARCH )
            printf( "search %s: %s\n", object_str( target ), buf->value );

        key = object_new( buf->value );
        timestamp( key, time );
        object_free( key );
    }

    boundname = object_new( buf->value );
    string_free( buf );

    if ( explicitly_located )
    {
        int found;
        BINDING * ba;
        OBJECT * key = path_as_key( boundname );
        /* CONSIDER: we probably should issue a warning is another file
           is explicitly bound to the same location. This might break
           compatibility, though. */
        ba = (BINDING *)hash_insert( explicit_bindings, key, &found );
        if ( !found )
        {
            ba->binding = key;
            ba->target = target;
        }
        else
        {
            object_free( key );
        }
    }

    /* prepare a call to BINDRULE if the variable is set */
    call_bind_rule( target, boundname );

    return boundname;
}


static void free_binding( void * xbinding, void * data )
{
    BINDING * binding = (BINDING *)xbinding;
    object_free( binding->binding );
}

void search_done( void )
{
    if ( explicit_bindings )
    {
        hashenumerate( explicit_bindings, free_binding, (void *)0 );
        hashdone( explicit_bindings );
    }
}
