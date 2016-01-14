#ifndef __NFI_CRASH_MODULE_H_
#define __NFI_CRASH_MODULE_H_

#include "NFILogicModule.h"

class NFICrashModule : public NFILogicModule
{

public:
    virtual void TestCrash() = 0;
};

#endif // !__NFI_CRASH_MODULE_H_