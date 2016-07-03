// -------------------------------------------------------------------------
//    @FileName			:    NFCActorModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_ACTOR_MODULE_H
#define NFC_ACTOR_MODULE_H

#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCActorModule
    : public NFIActorModule
{
public:

	NFCActorModule(NFIPluginManager* p);
    virtual ~NFCActorModule() {}

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute();

};

#endif
