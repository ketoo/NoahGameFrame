/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * search.h - find a target along $(SEARCH) or $(LOCATE)
 */

#ifndef SEARCH_SW20111118_H
#define SEARCH_SW20111118_H

#include "object.h"
#include <time.h>

OBJECT * search( OBJECT * target, time_t * time, OBJECT * * another_target, int file );
void search_done( void );

#endif
