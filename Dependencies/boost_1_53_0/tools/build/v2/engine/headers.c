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
# include "rules.h"
# include "modules.h"
# include "variable.h"
# include "regexp.h"
# include "headers.h"
# include "hdrmacro.h"
# include "object.h"

#ifdef OPT_HEADER_CACHE_EXT
# include "hcache.h"
#endif

/*
 * headers.c - handle #includes in source files
 *
 * Using regular expressions provided as the variable $(HDRSCAN),
 * headers() searches a file for #include files and phonies up a
 * rule invocation:
 *
 *  $(HDRRULE) <target> : <include files> ;
 *
 * External routines:
 *    headers() - scan a target for include files and call HDRRULE
 *
 * Internal routines:
 *    headers1() - using regexp, scan a file and build include LIST
 *
 * 04/13/94 (seiwald) - added shorthand L0 for null list pointer
 * 09/10/00 (seiwald) - replaced call to compile_rule with evaluate_rule,
 *      so that headers() doesn't have to mock up a parse structure
 *      just to invoke a rule.
 */

#ifndef OPT_HEADER_CACHE_EXT
static LIST * headers1( LIST * l, OBJECT * file, int rec, regexp * re[]);
#endif

/*
 * headers() - scan a target for include files and call HDRRULE
 */

# define MAXINC 10

void
headers( TARGET * t )
{
    LIST   * hdrscan;
    LIST   * hdrrule;
	#ifndef OPT_HEADER_CACHE_EXT
    LIST   * headlist = L0;
	#endif
    regexp * re[ MAXINC ];
    int rec = 0;
    LISTITER iter, end;

    hdrscan = var_get( root_module(), constant_HDRSCAN );
    if ( list_empty( hdrscan ) )
        return;

    hdrrule = var_get( root_module(), constant_HDRRULE );
    if ( list_empty( hdrrule ) )
        return;

    if ( DEBUG_HEADER )
        printf( "header scan %s\n", object_str( t->name ) );

    /* Compile all regular expressions in HDRSCAN */
    iter = list_begin( hdrscan ), end = list_end( hdrscan );
    for ( ; ( rec < MAXINC ) && iter != end; iter = list_next( iter ) )
    {
        re[ rec++ ] = regex_compile( list_item( iter ) );
    }

    /* Doctor up call to HDRRULE rule */
    /* Call headers1() to get LIST of included files. */
    {
        FRAME   frame[1];
        frame_init( frame );
        lol_add( frame->args, list_new( object_copy( t->name ) ) );
#ifdef OPT_HEADER_CACHE_EXT
        lol_add( frame->args, hcache( t, rec, re, hdrscan ) );
#else
        lol_add( frame->args, headers1( headlist, t->boundname, rec, re ) );
#endif

        if ( lol_get( frame->args, 1 ) )
        {
            /* The third argument to HDRRULE is the bound name of
             * $(<) */
            lol_add( frame->args, list_new( object_copy( t->boundname ) ) );

            list_free( evaluate_rule( list_front( hdrrule ), frame ) );
        }

        /* Clean up. */
        frame_free( frame );
    }
}


/*
 * headers1() - using regexp, scan a file and build include LIST.
 */

#ifdef OPT_HEADER_CACHE_EXT
LIST *
#else
static LIST *
#endif
headers1(
    LIST    * l,
    OBJECT  * file,
    int rec,
    regexp  * re[] )
{
    FILE * f;
    char buf[ 1024 ];
    int  i;
    static regexp * re_macros = 0;

#ifdef OPT_IMPROVED_PATIENCE_EXT
    static int count = 0;
    ++count;
    if ( ((count == 100) || !( count % 1000 )) && DEBUG_MAKE )
        printf("...patience...\n");
#endif

    /* the following regexp is used to detect cases where a  */
    /* file is included through a line line "#include MACRO" */
    if ( re_macros == 0 )
    {
        OBJECT * re_str = object_new(
            "^[     ]*#[    ]*include[  ]*([A-Za-z][A-Za-z0-9_]*).*$" );
        re_macros = regex_compile( re_str );
        object_free( re_str );
    }

    if ( !( f = fopen( object_str( file ), "r" ) ) )
        return l;

    while ( fgets( buf, sizeof( buf ), f ) )
    {
        int size = strlen( buf );
        /* Remove trailing \r and \n, if any. */
        while ( ( size > 0 ) &&
            ( buf[ size - 1 ] == '\n' ) &&
            ( buf[ size - 1 ] == '\r' ) )
        {
            buf[ size - 1 ] = '\0';
            --size;
        }

        for ( i = 0; i < rec; ++i )
            if ( regexec( re[i], buf ) && re[i]->startp[1] )
            {
                ((char *)re[i]->endp[1])[0] = '\0';

                if ( DEBUG_HEADER )
                    printf( "header found: %s\n", re[i]->startp[1] );

                l = list_push_back( l, object_new( re[i]->startp[1] ) );
            }

        /* special treatment for #include MACRO */
        if ( regexec( re_macros, buf ) && re_macros->startp[1] )
        {
            OBJECT *  header_filename;
            OBJECT * macro_name;

            ((char *)re_macros->endp[1])[0] = '\0';

            if ( DEBUG_HEADER )
                printf( "macro header found: %s", re_macros->startp[1] );

            macro_name = object_new( re_macros->startp[1] );
            header_filename = macro_header_get( macro_name );
            object_free( macro_name );
            if ( header_filename )
            {
                if ( DEBUG_HEADER )
                    printf( " resolved to '%s'\n", object_str( header_filename ) );
                l = list_push_back( l, object_copy( header_filename ) );
            }
            else
            {
                if ( DEBUG_HEADER )
                    printf( " ignored !!\n" );
            }
        }
    }

    fclose( f );

    return l;
}


void regerror( const char * s )
{
    printf( "re error %s\n", s );
}
