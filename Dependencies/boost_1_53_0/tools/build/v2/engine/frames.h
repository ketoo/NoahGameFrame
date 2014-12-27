/*
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef FRAMES_DWA20011021_H
#define FRAMES_DWA20011021_H

#include "lists.h"
#include "object.h"
#include "modules.h"

typedef struct _PARSE PARSE;
typedef struct frame FRAME;

struct frame
{
    FRAME      * prev;
    /* The nearest enclosing frame for which module->user_module is true. */
    FRAME      * prev_user;
    LOL          args[ 1 ];
    module_t   * module;
    OBJECT     * file;
    int          line;
    const char * rulename;
};


/* When call into Python is in progress, this variable points to the bjam frame
 * that was current at the moment of call. When the call completes, the variable
 * is not defined. Further, if Jam calls Python which calls Jam and so on, this
 * variable only keeps the most recent Jam frame.
 */
extern struct frame * frame_before_python_call;

void frame_init( FRAME * );  /* implemented in compile.c */
void frame_free( FRAME * );  /* implemented in compile.c */

#endif
