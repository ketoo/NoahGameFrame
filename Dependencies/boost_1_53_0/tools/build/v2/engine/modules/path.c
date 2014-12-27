/* Copyright Vladimir Prus 2003. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#include "../native.h"
#include "../timestamp.h"
#include "../object.h"

LIST *path_exists( FRAME *frame, int flags )
{
    LIST* l = lol_get( frame->args, 0 );    

    time_t time;
    timestamp(list_front(l), &time);
    if (time != 0)
    {
        return list_new(object_new("true"));
    }
    else
    {
        return L0;
    }
}

void init_path()
{
    {
        const char* args[] = { "location", 0 };
        declare_native_rule("path", "exists", args, path_exists, 1);
    }

}
