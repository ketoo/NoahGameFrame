//  Copyright (c) 2003 Vladimir Prus
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org
// 

#if SELECT == 1
void z1() {}
#elif SELECT == 2
void z2() {}
#else
#error Invlid value of SELECT
#endif
