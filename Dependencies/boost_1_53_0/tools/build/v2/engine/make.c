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
 * make.c - bring a target up to date, once rules are in place.
 *
 * This modules controls the execution of rules to bring a target and its
 * dependencies up to date. It is invoked after the targets, rules, et. al.
 * described in rules.h are created by the interpreting jam files.
 *
 * This file contains the main make() entry point and the first pass make0().
 * The second pass, make1(), which actually does the command execution, is in
 * make1.c.
 *
 * External routines:
 *  make() - make a target, given its name
 *
 * Internal routines:
 *  make0() - bind and scan everything to make a TARGET
 *  make0sort() - reorder TARGETS chain by their time (newest to oldest)
 *
 * 12/26/93 (seiwald) - allow NOTIME targets to be expanded via $(<), $(>).
 * 01/04/94 (seiwald) - print all targets, bounded, when tracing commands.
 * 04/08/94 (seiwald) - progress report now reflects only targets with actions.
 * 04/11/94 (seiwald) - Combined deps & headers into deps[2] in TARGET.
 * 12/20/94 (seiwald) - NOTIME renamed NOTFILE.
 * 12/20/94 (seiwald) - make0() headers after determining fate of target, so
 *                      that headers are not seen as being dependent on
 *                      themselves.
 * 01/19/95 (seiwald) - distinguish between CANTFIND/CANTMAKE targets.
 * 02/02/95 (seiwald) - propagate leaf source time for new LEAVES rule.
 * 02/14/95 (seiwald) - NOUPDATE rule means don't update existing target.
 * 08/22/95 (seiwald) - NOUPDATE targets immune to anyhow (-a) flag.
 * 09/06/00 (seiwald) - NOCARE affects targets with sources/actions.
 * 03/02/01 (seiwald) - reverse NOCARE change.
 * 03/14/02 (seiwald) - TEMPORARY targets no longer take on parents age.
 * 03/16/02 (seiwald) - support for -g (reorder builds by source time).
 */

#include "jam.h"

#include "lists.h"
#include "parse.h"
#include "variable.h"
#include "rules.h"

#ifdef OPT_HEADER_CACHE_EXT
    #include "hcache.h"
#endif

#include "search.h"
#include "object.h"
#include "make.h"
#include "headers.h"
#include "command.h"
#include <assert.h>

#ifndef max
    #define max( a,b ) ((a)>(b)?(a):(b))
#endif

static TARGETS * make0sort( TARGETS * c );

#ifdef OPT_GRAPH_DEBUG_EXT
    static void dependGraphOutput( TARGET * t, int depth );
#endif

static const char * target_fate[] =
{
    "init",     /* T_FATE_INIT     */
    "making",   /* T_FATE_MAKING   */
    "stable",   /* T_FATE_STABLE   */
    "newer",    /* T_FATE_NEWER    */
    "temp",     /* T_FATE_ISTMP    */
    "touched",  /* T_FATE_TOUCHED  */
    "rebuild",  /* T_FATE_REBUILD  */
    "missing",  /* T_FATE_MISSING  */
    "needtmp",  /* T_FATE_NEEDTMP  */
    "old",      /* T_FATE_OUTDATED */
    "update",   /* T_FATE_UPDATE   */
    "nofind",   /* T_FATE_CANTFIND */
    "nomake"    /* T_FATE_CANTMAKE */
};

static const char * target_bind[] =
{
    "unbound",
    "missing",
    "parents",
    "exists",
};

# define spaces(x) ( "                    " + ( x > 20 ? 0 : 20-x ) )


/*
 * make() - make a target, given its name.
 */

int make( LIST * targets, int anyhow )
{
    COUNTS counts[ 1 ];
    int    status = 0;     /* 1 if anything fails */

#ifdef OPT_HEADER_CACHE_EXT
    hcache_init();
#endif

    memset( (char *)counts, 0, sizeof( *counts ) );

    /* First bind all targets with LOCATE_TARGET setting. This is needed to
     * correctly handle dependencies to generated headers.
     */
    bind_explicitly_located_targets();

    {
        LISTITER iter, end;
        PROFILE_ENTER( MAKE_MAKE0 );
        for ( iter = list_begin( targets ), end = list_end( targets ); iter != end; iter = list_next( iter ) )
        {
            TARGET * t = bindtarget( list_item( iter ) );
            if ( t->fate == T_FATE_INIT )
                make0( t, 0, 0, counts, anyhow );
        }
        PROFILE_EXIT( MAKE_MAKE0 );
    }

#ifdef OPT_GRAPH_DEBUG_EXT
    if ( DEBUG_GRAPH )
    {
        LISTITER iter, end;
        for ( iter = list_begin( targets ), end = list_end( targets ); iter != end; iter = list_next( iter ) )
           dependGraphOutput( bindtarget( list_item( iter ) ), 0 );
    }
#endif

    if ( DEBUG_MAKE )
    {
        if ( counts->targets )
            printf( "...found %d target%s...\n", counts->targets,
                counts->targets > 1 ? "s" : "" );
        if ( counts->temp )
            printf( "...using %d temp target%s...\n", counts->temp,
                counts->temp > 1 ? "s" : "" );
        if ( counts->updating )
            printf( "...updating %d target%s...\n", counts->updating,
                counts->updating > 1 ? "s" : "" );
        if ( counts->cantfind )
            printf( "...can't find %d target%s...\n", counts->cantfind,
                counts->cantfind > 1 ? "s" : "" );
        if ( counts->cantmake )
            printf( "...can't make %d target%s...\n", counts->cantmake,
                counts->cantmake > 1 ? "s" : "" );
    }

    status = counts->cantfind || counts->cantmake;

    {
        LISTITER iter, end;
        PROFILE_ENTER( MAKE_MAKE1 );
        for ( iter = list_begin( targets ), end = list_end( targets ); iter != end; iter = list_next( iter ) )
            status |= make1( bindtarget( list_item( iter ) ) );
        PROFILE_EXIT( MAKE_MAKE1 );
    }

    return status;
}


/* Force any dependants of t that have already at least begun being visited by
 * make0() to be updated.
 */

static void update_dependants( TARGET * t )
{
    TARGETS * q;

    for ( q = t->dependants; q; q = q->next )
    {
        TARGET * p = q->target;
        char fate0 = p->fate;

        /* If we have already at least begun visiting it and we are not already
         * rebuilding it for other reasons.
         */
        if ( ( fate0 != T_FATE_INIT ) && ( fate0 < T_FATE_BUILD ) )
        {
            p->fate = T_FATE_UPDATE;

            if ( DEBUG_FATE )
            {
                printf( "fate change  %s from %s to %s (as dependant of %s)\n",
                        object_str( p->name ), target_fate[ (int) fate0 ], target_fate[ (int) p->fate ], object_str( t->name ) );
            }

            /* If we are done visiting it, go back and make sure its dependants
             * get rebuilt.
             */
            if ( fate0 > T_FATE_MAKING )
                update_dependants( p );
        }
    }
}


/*
 * Make sure that all of t's rebuilds get rebuilt.
 */

static void force_rebuilds( TARGET * t )
{
    TARGETS * d;
    for ( d = t->rebuilds; d; d = d->next )
    {
        TARGET * r = d->target;

        /* If it is not already being rebuilt for other reasons. */
        if ( r->fate < T_FATE_BUILD )
        {
            if ( DEBUG_FATE )
                printf( "fate change  %s from %s to %s (by rebuild)\n",
                        object_str( r->name ), target_fate[ (int) r->fate ], target_fate[ T_FATE_REBUILD ] );

            /* Force rebuild it. */
            r->fate = T_FATE_REBUILD;

            /* And make sure its dependants are updated too. */
            update_dependants( r );
        }
    }
}


/*
 * make0() - bind and scan everything to make a TARGET.
 *
 * Recursively binds a target, searches for #included headers, calls itself on
 * those headers and any dependencies.
 */

void make0
(
    TARGET * t,
    TARGET * p,       /* parent */
    int      depth,   /* for display purposes */
    COUNTS * counts,  /* for reporting */
    int      anyhow
)  /* forcibly touch all (real) targets */
{
    TARGETS    * c;
    TARGET     * ptime = t;
    time_t       last;
    time_t       leaf;
    time_t       hlast;
    int          fate;
    char const * flag = "";
    SETTINGS   * s;

#ifdef OPT_GRAPH_DEBUG_EXT
    int savedFate, oldTimeStamp;
#endif

    if ( DEBUG_MAKEPROG )
        printf( "make\t--\t%s%s\n", spaces( depth ), object_str( t->name ) );

    /*
     * Step 1: initialize
     */

    if ( DEBUG_MAKEPROG )
        printf( "make\t--\t%s%s\n", spaces( depth ), object_str( t->name ) );

    t->fate = T_FATE_MAKING;

    /*
     * Step 2: under the influence of "on target" variables,
     * bind the target and search for headers.
     */

    /* Step 2a: set "on target" variables. */
    s = copysettings( t->settings );
    pushsettings( root_module(), s );

    /* Step 2b: find and timestamp the target file (if it is a file). */
    if ( ( t->binding == T_BIND_UNBOUND ) && !( t->flags & T_FLAG_NOTFILE ) )
    {
        OBJECT * another_target;
        object_free( t->boundname );
        t->boundname = search( t->name, &t->time, &another_target,
                               t->flags & T_FLAG_ISFILE );
        /* If it was detected that this target refers to an already existing and
         * bound one, we add an include dependency, so that every target
         * depending on us will depend on that other target as well.
         */
        if ( another_target )
            target_include( t, bindtarget( another_target ) );

        t->binding = t->time ? T_BIND_EXISTS : T_BIND_MISSING;
    }

    /* INTERNAL, NOTFILE header nodes have the time of their parents. */
    if ( p && ( t->flags & T_FLAG_INTERNAL ) )
        ptime = p;

    /* If temp file does not exist but parent does, use parent. */
    if ( p && ( t->flags & T_FLAG_TEMP ) &&
        ( t->binding == T_BIND_MISSING ) &&
        ( p->binding != T_BIND_MISSING ) )
    {
        t->binding = T_BIND_PARENTS;
        ptime = p;
    }

#ifdef OPT_SEMAPHORE
    {
        LIST * var = var_get( root_module(), constant_JAM_SEMAPHORE );
        if ( !list_empty( var ) )
        {
            TARGET * semaphore = bindtarget( list_front( var ) );
            semaphore->progress = T_MAKE_SEMAPHORE;
            t->semaphore = semaphore;
        }
    }
#endif

    /* Step 2c: If its a file, search for headers. */
    if ( t->binding == T_BIND_EXISTS )
        headers( t );

    /* Step 2d: reset "on target" variables. */
    popsettings( root_module(), s );
    freesettings( s );

    /*
     * Pause for a little progress reporting .
     */

    if ( DEBUG_BIND )
    {
        if ( ! object_equal( t->name, t->boundname ) )
            printf( "bind\t--\t%s%s: %s\n",
                spaces( depth ), object_str( t->name ), object_str( t->boundname ) );

        switch ( t->binding )
        {
        case T_BIND_UNBOUND:
        case T_BIND_MISSING:
        case T_BIND_PARENTS:
            printf( "time\t--\t%s%s: %s\n",
                spaces( depth ), object_str( t->name ), target_bind[ (int) t->binding ] );
            break;

        case T_BIND_EXISTS:
            printf( "time\t--\t%s%s: %s",
                spaces( depth ), object_str( t->name ), ctime( &t->time ) );
            break;
        }
    }

    /*
     * Step 3: recursively make0() dependencies & headers.
     */

    /* Step 3a: recursively make0() dependencies. */
    for ( c = t->depends; c; c = c->next )
    {
        int internal = t->flags & T_FLAG_INTERNAL;

        /* Warn about circular deps, except for includes, which include each
         * other alot.
         */
        if ( c->target->fate == T_FATE_INIT )
            make0( c->target, ptime, depth + 1, counts, anyhow );
        else if ( c->target->fate == T_FATE_MAKING && !internal )
            printf( "warning: %s depends on itself\n", object_str( c->target->name ) );
    }

    /* Step 3b: recursively make0() internal includes node. */
    if ( t->includes )
        make0( t->includes, p, depth + 1, counts, anyhow );

    /* Step 3c: add dependencies' includes to our direct dependencies. */
    {
        TARGETS * incs = 0;
        for ( c = t->depends; c; c = c->next )
            if ( c->target->includes )
                incs = targetentry( incs, c->target->includes );
        t->depends = targetchain( t->depends, incs );
    }

    /*
     * Step 4: compute time & fate
     */

    /* Step 4a: pick up dependencies' time and fate */
    last = 0;
    leaf = 0;
    fate = T_FATE_STABLE;
    for ( c = t->depends; c; c = c->next )
    {
        /* If LEAVES has been applied, we only heed the timestamps of the leaf
         * source nodes.
         */
        leaf = max( leaf, c->target->leaf );

        if ( t->flags & T_FLAG_LEAVES )
        {
            last = leaf;
            continue;
        }

        last = max( last, c->target->time );
        fate = max( fate, c->target->fate );

#ifdef OPT_GRAPH_DEBUG_EXT
        if ( DEBUG_FATE )
            if ( fate < c->target->fate )
                printf( "fate change %s from %s to %s by dependency %s\n",
                    object_str( t->name ), target_fate[(int) fate], target_fate[(int) c->target->fate],
                    object_str( c->target->name ) );
#endif
    }

    /* Step 4b: pick up included headers time */

    /*
     * If a header is newer than a temp source that includes it,
     * the temp source will need building.
     */

    hlast = t->includes ? t->includes->time : 0;

    /* Step 4c: handle NOUPDATE oddity.
     *
     * If a NOUPDATE file exists, mark it as having eternally old dependencies.
     * Do not inherit our fate from our dependencies. Decide fate based only on
     * other flags and our binding (done later).
     */
    if ( t->flags & T_FLAG_NOUPDATE )
    {
#ifdef OPT_GRAPH_DEBUG_EXT
        if ( DEBUG_FATE )
            if ( fate != T_FATE_STABLE )
                printf( "fate change  %s back to stable, NOUPDATE.\n",
                    object_str( t->name ) );
#endif

        last = 0;
        t->time = 0;

        /* Do not inherit our fate from our dependencies. Decide fate based only
         * upon other flags and our binding (done later).
         */
        fate = T_FATE_STABLE;
    }

    /* Step 4d: determine fate: rebuild target or what? */

    /*
        In English:
        If can not find or make child, can not make target.
        If children changed, make target.
        If target missing, make it.
        If children newer, make target.
        If temp's children newer than parent, make temp.
        If temp's headers newer than parent, make temp.
        If deliberately touched, make it.
        If up-to-date temp file present, use it.
        If target newer than non-notfile parent, mark target newer.
        Otherwise, stable!

        Note this block runs from least to most stable:
        as we make it further down the list, the target's
        fate is getting stabler.
    */

#ifdef OPT_GRAPH_DEBUG_EXT
    savedFate = fate;
    oldTimeStamp = 0;
#endif

    if ( fate >= T_FATE_BROKEN )
    {
        fate = T_FATE_CANTMAKE;
    }
    else if ( fate >= T_FATE_SPOIL )
    {
        fate = T_FATE_UPDATE;
    }
    else if ( t->binding == T_BIND_MISSING )
    {
        fate = T_FATE_MISSING;
    }
    else if ( ( t->binding == T_BIND_EXISTS ) && ( last > t->time ) )
    {
#ifdef OPT_GRAPH_DEBUG_EXT
        oldTimeStamp = 1;
#endif
        fate = T_FATE_OUTDATED;
    }
    else if ( ( t->binding == T_BIND_PARENTS ) && ( last > p->time ) )
    {
#ifdef OPT_GRAPH_DEBUG_EXT
        oldTimeStamp = 1;
#endif
        fate = T_FATE_NEEDTMP;
    }
    else if ( ( t->binding == T_BIND_PARENTS ) && ( hlast > p->time ) )
    {
        fate = T_FATE_NEEDTMP;
    }
    else if ( t->flags & T_FLAG_TOUCHED )
    {
        fate = T_FATE_TOUCHED;
    }
    else if ( anyhow && !( t->flags & T_FLAG_NOUPDATE ) )
    {
        fate = T_FATE_TOUCHED;
    }
    else if ( ( t->binding == T_BIND_EXISTS ) && ( t->flags & T_FLAG_TEMP ) )
    {
        fate = T_FATE_ISTMP;
    }
    else if ( ( t->binding == T_BIND_EXISTS ) && p &&
         ( p->binding != T_BIND_UNBOUND ) && ( t->time > p->time ) )
    {
#ifdef OPT_GRAPH_DEBUG_EXT
        oldTimeStamp = 1;
#endif
        fate = T_FATE_NEWER;
    }
    else
    {
        fate = T_FATE_STABLE;
    }
#ifdef OPT_GRAPH_DEBUG_EXT
    if ( DEBUG_FATE && ( fate != savedFate ) )
	{
        if ( savedFate == T_FATE_STABLE )
            printf( "fate change  %s set to %s%s\n", object_str( t->name ),
                target_fate[ fate ], oldTimeStamp ? " (by timestamp)" : "" );
        else
            printf( "fate change  %s from %s to %s%s\n", object_str( t->name ),
                target_fate[ savedFate ], target_fate[ fate ],
                oldTimeStamp ? " (by timestamp)" : "" );
	}
#endif

    /* Step 4e: handle missing files */
    /* If it is missing and there are no actions to create it, boom. */
    /* If we can not make a target we do not care about it, okay. */
    /* We could insist that there are updating actions for all missing */
    /* files, but if they have dependencies we just pretend it is a NOTFILE. */

    if ( ( fate == T_FATE_MISSING ) && !t->actions && !t->depends )
    {
        if ( t->flags & T_FLAG_NOCARE )
        {
#ifdef OPT_GRAPH_DEBUG_EXT
            if ( DEBUG_FATE )
                printf( "fate change %s to STABLE from %s, "
                    "no actions, no dependencies and do not care\n",
                    object_str( t->name ), target_fate[ fate ] );
#endif
            fate = T_FATE_STABLE;
        }
        else
        {
            printf( "don't know how to make %s\n", object_str( t->name ) );
            fate = T_FATE_CANTFIND;
        }
    }

    /* Step 4f: propagate dependencies' time & fate. */
    /* Set leaf time to be our time only if this is a leaf. */

    t->time = max( t->time, last );
    t->leaf = leaf ? leaf : t->time ;
    /* This target's fate may have been updated by virtue of following some
     * target's rebuilds list, so only allow it to be increased to the fate we
     * have calculated. Otherwise, grab its new fate.
     */
    if ( fate > t->fate )
        t->fate = fate;
    else
        fate = t->fate;

    /* Step 4g: if this target needs to be built, force rebuild everything in
     * this target's rebuilds list.
     */
    if ( ( fate >= T_FATE_BUILD ) && ( fate < T_FATE_BROKEN ) )
        force_rebuilds( t );

    /*
     * Step 5: sort dependencies by their update time.
     */

    if ( globs.newestfirst )
        t->depends = make0sort( t->depends );

    /*
     * Step 6: a little harmless tabulating for tracing purposes
     */

    /* Do not count or report interal includes nodes. */
    if ( t->flags & T_FLAG_INTERNAL )
        return;

    if ( counts )
    {
#ifdef OPT_IMPROVED_PATIENCE_EXT
        ++counts->targets;
#else
        if ( !( ++counts->targets % 1000 ) && DEBUG_MAKE )
            printf( "...patience...\n" );
#endif

        if ( fate == T_FATE_ISTMP )
            ++counts->temp;
        else if ( fate == T_FATE_CANTFIND )
            ++counts->cantfind;
        else if ( ( fate == T_FATE_CANTMAKE ) && t->actions )
            ++counts->cantmake;
        else if ( ( fate >= T_FATE_BUILD ) && ( fate < T_FATE_BROKEN ) &&
            t->actions )
            ++counts->updating;
    }

    if ( !( t->flags & T_FLAG_NOTFILE ) && ( fate >= T_FATE_SPOIL ) )
        flag = "+";
    else if ( ( t->binding == T_BIND_EXISTS ) && p && ( t->time > p->time ) )
        flag = "*";

    if ( DEBUG_MAKEPROG )
        printf( "made%s\t%s\t%s%s\n", flag, target_fate[ (int) t->fate ],
            spaces( depth ), object_str( t->name ) );
}


#ifdef OPT_GRAPH_DEBUG_EXT

static const char * target_name( TARGET * t )
{
    static char buf[ 1000 ];
    if ( t->flags & T_FLAG_INTERNAL )
    {
        sprintf( buf, "%s (internal node)", object_str( t->name ) );
        return buf;
    }
    return object_str( t->name );
}


/*
 * dependGraphOutput() - output the DG after make0 has run.
 */

static void dependGraphOutput( TARGET * t, int depth )
{
    TARGETS * c;

    if ( ( t->flags & T_FLAG_VISITED ) || !t->name || !t->boundname )
        return;

    t->flags |= T_FLAG_VISITED;

    switch ( t->fate )
    {
    case T_FATE_TOUCHED:
    case T_FATE_MISSING:
    case T_FATE_OUTDATED:
    case T_FATE_UPDATE:
        printf( "->%s%2d Name: %s\n", spaces( depth ), depth, target_name( t ) );
        break;
    default:
        printf( "  %s%2d Name: %s\n", spaces( depth ), depth, target_name( t ) );
        break;
    }

    if ( ! object_equal( t->name, t->boundname ) )
        printf( "  %s    Loc: %s\n", spaces( depth ), object_str( t->boundname ) );

    switch ( t->fate )
    {
    case T_FATE_STABLE:
        printf( "  %s       : Stable\n", spaces( depth ) );
        break;
    case T_FATE_NEWER:
        printf( "  %s       : Newer\n", spaces( depth ) );
        break;
    case T_FATE_ISTMP:
        printf( "  %s       : Up to date temp file\n", spaces( depth ) );
        break;
    case T_FATE_NEEDTMP:
        printf( "  %s       : Temporary file, to be updated\n", spaces( depth ) );
        break;
    case T_FATE_TOUCHED:
        printf( "  %s       : Been touched, updating it\n", spaces( depth ) );
        break;
    case T_FATE_MISSING:
        printf( "  %s       : Missing, creating it\n", spaces( depth ) );
        break;
    case T_FATE_OUTDATED:
        printf( "  %s       : Outdated, updating it\n", spaces( depth ) );
        break;
    case T_FATE_REBUILD:
        printf( "  %s       : Rebuild, updating it\n", spaces( depth ) );
        break;
    case T_FATE_UPDATE:
        printf( "  %s       : Updating it\n", spaces( depth ) );
        break;
    case T_FATE_CANTFIND:
        printf( "  %s       : Can not find it\n", spaces( depth ) );
        break;
    case T_FATE_CANTMAKE:
        printf( "  %s       : Can make it\n", spaces( depth ) );
        break;
    }

    if ( t->flags & ~T_FLAG_VISITED )
    {
        printf( "  %s       : ", spaces( depth ) );
        if ( t->flags & T_FLAG_TEMP     ) printf( "TEMPORARY " );
        if ( t->flags & T_FLAG_NOCARE   ) printf( "NOCARE "    );
        if ( t->flags & T_FLAG_NOTFILE  ) printf( "NOTFILE "   );
        if ( t->flags & T_FLAG_TOUCHED  ) printf( "TOUCHED "   );
        if ( t->flags & T_FLAG_LEAVES   ) printf( "LEAVES "    );
        if ( t->flags & T_FLAG_NOUPDATE ) printf( "NOUPDATE "  );
        printf( "\n" );
    }

    for ( c = t->depends; c; c = c->next )
    {
        printf( "  %s       : Depends on %s (%s)", spaces( depth ),
               target_name( c->target ), target_fate[ (int) c->target->fate ] );
        if ( c->target->time == t->time )
            printf( " (max time)");
        printf( "\n" );
    }

    for ( c = t->depends; c; c = c->next )
        dependGraphOutput( c->target, depth + 1 );
}
#endif


/*
 * make0sort() - reorder TARGETS chain by their time (newest to oldest).
 *
 * We walk chain, taking each item and inserting it on the sorted result, with
 * newest items at the front. This involves updating each of the TARGETS'
 * c->next and c->tail. Note that we make c->tail a valid prev pointer for every
 * entry. Normally, it is only valid at the head, where prev == tail. Note also
 * that while tail is a loop, next ends at the end of the chain.
 */

static TARGETS * make0sort( TARGETS * chain )
{
    PROFILE_ENTER( MAKE_MAKE0SORT );

    TARGETS * result = 0;

    /* Walk the current target list. */
    while ( chain )
    {
        TARGETS * c = chain;
        TARGETS * s = result;

        chain = chain->next;

        /* Find point s in result for c. */
        while ( s && ( s->target->time > c->target->time ) )
            s = s->next;

        /* Insert c in front of s (might be 0). Do not even think of deciphering
         * this.
         */
        c->next = s;                           /* good even if s = 0       */
        if ( result == s ) result = c;         /* new head of chain?       */
        if ( !s ) s = result;                  /* wrap to ensure a next    */
        if ( result != c ) s->tail->next = c;  /* not head? be prev's next */
        c->tail = s->tail;                     /* take on next's prev      */
        s->tail = c;                           /* make next's prev us      */
    }

    PROFILE_EXIT( MAKE_MAKE0SORT );
    return result;
}


static LIST * targets_to_update_ = L0;


void mark_target_for_updating( OBJECT * target )
{
    targets_to_update_ = list_push_back( targets_to_update_, object_copy( target ) );
}


LIST * targets_to_update()
{
    return targets_to_update_;
}


void clear_targets_to_update()
{
    list_free( targets_to_update_ );
    targets_to_update_ = L0;
}
