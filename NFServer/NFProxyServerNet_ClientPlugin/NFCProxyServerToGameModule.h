// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyServerToGameModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyServerToGameModule
//
//     @Changed         : NFCProxyConnectObject Add HeartBeat by hahaya 2013-11-1
// -------------------------------------------------------------------------

#ifndef NFC_PROXYSERVER_TO_GAME_MODULE_H
#define NFC_PROXYSERVER_TO_GAME_MODULE_H

#include <string>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFCHeartBeatManager.h"
#include "NFComm/NFPluginModule/NFIProxyServerToGameModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"


class NFCProxyServerToGameModule : public NFIProxyServerToGameModule
{
public:

    NFCProxyServerToGameModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}


protected:

	void OnReciveGSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSocketGSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	void Register(NFINet* pNet);

	void OnAckEnterGame(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	virtual void LogServerInfo( const std::string& strServerInfo );

private:

    NFILogModule* m_pLogModule;
    NFIProxyLogicModule* m_pProxyLogicModule;
    NFIKernelModule* m_pKernelModule;
    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;

};

#endif