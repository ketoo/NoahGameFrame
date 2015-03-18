// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_PROXYSERVER_SERVER_MODULE_H_
#define _NFC_PROXYSERVER_SERVER_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"

class NFCProxyServerNet_ServerModule : public NFIProxyServerNet_ServerModule
{
public:
    NFCProxyServerNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

    virtual int Transpond(const NFIPacket& msg);

protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	//连接丢失,删2层(连接对象，帐号对象)
	void OnClientDisconnect(const int nAddress);
	//有连接
	void OnClientConnected(const int nAddress);

    int OnConnectKeyProcess(const NFIPacket& msg);
    int OnReqServerListProcess(const NFIPacket& msg);
    int OnSelectServerProcess(const NFIPacket& msg);
    int OnReqRoleListProcess(const NFIPacket& msg);
    int OnReqCreateRoleProcess(const NFIPacket& msg);
    int OnReqDelRoleProcess(const NFIPacket& msg);
    int OnReqEnterGameServer(const NFIPacket& msg);


    int OnTranspondProcess(const NFIPacket& msg);

    //客户端的连接60秒删掉
    int HB_OnConnectCheckTime( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var );
    //////////////////////////////////////////////////////////////////////////

    //保存的世界服务器发过来的信息对象
    int OnWantToConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    //进入游戏成功
    int OnEnterGameSuccessEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

protected:

	NFMapEx<NFIDENTID, int> mxClientIdent;

protected:
    NFIProxyServerToWorldModule* m_pProxyToWorldModule;
	NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;
	NFIUUIDModule* m_pUUIDModule;

};

#endif