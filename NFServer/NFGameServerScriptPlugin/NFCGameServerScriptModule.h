// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFGameServerScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVER_SCRIPT_MODULE_H_
#define _NFC_GAMESERVER_SCRIPT_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIGameServerScriptModule.h"

class NFCGameServerScriptModule
    : public NFIGameServerScriptModule
{
public:
    NFCGameServerScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();


protected:


    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif