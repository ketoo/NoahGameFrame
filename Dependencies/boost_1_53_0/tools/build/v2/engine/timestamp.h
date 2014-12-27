/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * timestamp.h - get the timestamp of a file or archive member
 */

#ifndef TIMESTAMP_H_SW_2011_11_18
#define TIMESTAMP_H_SW_2011_11_18

#include "object.h"
#include "time.h"

void timestamp( OBJECT * target, time_t * time );
void stamps_done();

#endif
