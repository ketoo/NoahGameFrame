#ifndef _NFI_CRASH_MODULE_H
#define _NFI_CRASH_MODULE_H

#include "NFILogicModule.h"

class NFICrashModule : public NFILogicModule
{

public:
    virtual void TestCrash() = 0;
};

#endif // !_NFI_CRASH_MODULE_H