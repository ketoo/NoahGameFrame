// -------------------------------------------------------------------------
//    @FileName         :    NFIMemory.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-11
//    @Module           :    NFIMemory
//
// -------------------------------------------------------------------------

#ifndef _NFI_MEMORY_H_
#define _NFI_MEMORY_H_

#include "NFPlatform.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include <vector>
#include <boost/lexical_cast.hpp>

#pragma pack(push, 1)

class NFIMemory
{
public:
    NFIMemory() {};
    virtual ~NFIMemory() {};
    virtual void Add(void* p, char const* file, int line) = 0;
    virtual void Remove(void* p) = 0;
    virtual void Dump() = 0;


};

#pragma pack(pop)
#endif
