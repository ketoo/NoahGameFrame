// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLoginLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGINLOGIC_MODULE_H
#define NFC_LOGINLOGIC_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"


class NFCLoginLogicModule
    : public NFILoginLogicModule
{
public:
    NFCLoginLogicModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual void OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

protected:

    NFILoginNet_ServerModule* m_pLoginNet_ServerModule;
	NFILogModule* m_pLogModule;
private:
};

#endif