// -------------------------------------------------------------------------
//    @FileName     :    NFCGameServerPublicProxyModule.h
//    @Author          :    LvSheng.Huang
//    @Date              :    2013-07-27
//    @Module         :    NFCGameServerPublicProxyModule
//    @Desc             :       工会，拍卖行，邮箱，银行，商城，商店等公共可延时数据处理的代理，他们将由此转发到1号服务器
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVER_PUBLICPROXY_MODULE_H_
#define _NFC_GAMESERVER_PUBLICPROXY_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ClientModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCGameServerPublicProxyModule
{
public:
    //  NFCGameServerPublicProxyModule(NFIPluginManager* p)
    //  {
    //      pPluginManager = p;
    //  }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

protected:


private:

    NFIGameServerNet_ServerModule* m_pGameServerModule;
    NFIGameLogicModule* m_pGameLogicModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;

};

#endif