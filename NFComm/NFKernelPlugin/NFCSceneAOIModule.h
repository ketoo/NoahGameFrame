// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneAOIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCSceneAOIModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SCENE_AOI__MODULE_H
#define NFC_SCENE_AOI__MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"

class NFCSceneAOIModule
    : public NFISceneAOIModule
{
public:
	NFCSceneAOIModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~NFCSceneAOIModule()
    {
    }

    virtual bool Init();
    virtual bool AfterInit();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Execute();



protected:
private:
};

#endif