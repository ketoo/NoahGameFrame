/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * pathsys.h - PATHNAME struct
 */

/*
 * PATHNAME - a name of a file, broken into <grist>dir/base/suffix(member)
 *
 * <grist> is salt to distinguish between targets that otherwise would
 * have the same name:  it never appears in the bound name of a target.
 * (member) is an archive member name: the syntax is arbitrary, but must
 * agree in path_parse(), path_build() and the Jambase.
 */

#ifndef PATHSYS_VP_20020211_H
# define PATHSYS_VP_20020211_H

#include "jam.h"
#include "strings.h"
#include "object.h"

typedef struct _pathname PATHNAME;
typedef struct _pathpart PATHPART;

struct _pathpart
{
    const char * ptr;
    int          len;
};

struct _pathname
{
    PATHPART    part[6];

#define f_grist    part[0]
#define f_root     part[1]
#define f_dir      part[2]
#define f_base     part[3]
#define f_suffix   part[4]
#define f_member   part[5]
};

void path_build( PATHNAME * f, string * file, int binding );
void path_build1( PATHNAME * f, string * file );

void path_parse( const char * file, PATHNAME * f );
void path_parent( PATHNAME * f );

#ifdef NT

/** Returns object_new-allocated string with long equivivalent of 'short_name'.
    If none exists -- i.e. 'short_path' is already long path, it's returned
    unaltered. */
OBJECT * short_path_to_long_path( OBJECT * short_path );

#endif

/** Given a path, returns an object that can be
    used as a unique key for that path.  Equivalent
    paths such as a/b, A\B, and a\B on NT all yield the
    same key.
 */
OBJECT * path_as_key( OBJECT * path );
void path_add_key( OBJECT * path );

#ifdef USE_PATHUNIX
/** Returns a static pointer to the system dependent path to the temporary
    directory. NOTE: *without* a trailing path separator.
*/
const char * path_tmpdir( void );

/** Returns a new temporary name.
*/
OBJECT * path_tmpnam( void );

/** Returns a new temporary path.
*/
OBJECT * path_tmpfile( void );
#endif

/** Give the first argument to 'main', return a full path to
    our executable.  Returns null in the unlikely case it
    cannot be determined. Caller is responsible for freeing
    the string.

    Implemented in jam.c
*/
char * executable_path (const char *argv0);

void path_done( void );

#endif
