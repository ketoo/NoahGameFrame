// -------------------------------------------------------------------------
//    @FileName         £º    NFIActorDataModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-12-15
//    @Module           £º    NFIActorDataModule
//
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_DATA_MODULE_H
#define NFI_ACTOR_DATA_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFIActor.h"

class NFIActorDataModule
    : public NFILogicModule
{

public:

#ifdef NF_USE_ACTOR
    //virtual void Handler(const NFIActorMessage& message, const Theron::Address from) = 0;
#endif

};

#endif