// -------------------------------------------------------------------------
//    @FileName      :    NFCSceneModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCSceneModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SCENE_MODULE_H
#define NFC_SCENE_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"

class NFCSceneModule
    : public NFISceneModule
{
public:
    NFCSceneModule(NFIPluginManager* p)
    {
        //pPluginManager = p;
    }


    virtual ~NFCSceneModule()
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