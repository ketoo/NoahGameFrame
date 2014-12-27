/*
    Copyright 2007 Rene Rivera
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BJAM_OUTPUT_H
#define BJAM_OUTPUT_H

#include "object.h"
#include <time.h>

#define EXIT_OK 0
#define EXIT_FAIL 1
#define EXIT_TIMEOUT 2

void out_action(
    const char * action,
    const char * target,
    const char * command,
    const char * out_data,
    const char * err_data,
    int exit_reason
    );

OBJECT * outf_int( int value );
OBJECT * outf_double( double value );
OBJECT * outf_time( time_t value );

#endif
