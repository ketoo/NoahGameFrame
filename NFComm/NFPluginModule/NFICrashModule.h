#ifndef __NFI_CRASH_MODULE_H__
#define __NFI_CRASH_MODULE_H__

#include "NFILogicModule.h"

class NFICrashModule : public NFILogicModule
{

public:
    virtual void TestCrash() = 0;
};

#endif // !__NFI_CRASH_MODULE_H__