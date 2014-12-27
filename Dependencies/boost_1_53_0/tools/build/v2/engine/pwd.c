/* Copyright Vladimir Prus 2002, Rene Rivera 2005. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#include "jam.h"
#include "lists.h"
#include "object.h"
#include "pathsys.h"
#include "mem.h"

#include <limits.h>
#include <errno.h>

/* MinGW on windows declares PATH_MAX in limits.h */
#if defined(NT) && ! defined(__GNUC__)
#include <direct.h>
#define PATH_MAX _MAX_PATH
#else
#include <unistd.h>
#if defined(__COMO__)
     #include <linux/limits.h>
#endif
#endif

#ifndef PATH_MAX
	#define PATH_MAX 1024
#endif

/* The current directory can't change in bjam, so optimize this to cache
** the result.
*/
static OBJECT * pwd_result = NULL;


LIST*
pwd(void)
{
    if (!pwd_result)
    {
		int buffer_size = PATH_MAX;
		char * result_buffer = 0;
		do
		{
			char * buffer = BJAM_MALLOC_RAW(buffer_size);
			result_buffer = getcwd(buffer,buffer_size);
			if (result_buffer)
			{
				#ifdef NT
                OBJECT * result = object_new(result_buffer);
				pwd_result = short_path_to_long_path(result);
                object_free( result );
				#else
				pwd_result = object_new(result_buffer);
				#endif
			}
			buffer_size *= 2;
			BJAM_FREE_RAW(buffer);
		}
		while (!pwd_result && errno == ERANGE);
		
		if (!pwd_result)
		{
            perror("can not get current directory");
            return L0;
        }
    }
    return list_new( object_copy( pwd_result ) );
}

void pwd_done( void )
{
    if( pwd_result )
    {
        object_free( pwd_result );
    }
}
