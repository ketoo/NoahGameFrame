// -------------------------------------------------------------------------
//    @FileName         £º    NFIActorDataModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-12-15
//    @Module           £º    NFIActorDataModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_ACTOR_DATA_MODULE_H_
#define _NFI_ACTOR_DATA_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFIActor.h"

class NFIActorDataModule
    : public NFILogicModule
{

public:
    virtual void Handler(const NFIActorMessage& message, const Theron::Address from) = 0;


};

#endif