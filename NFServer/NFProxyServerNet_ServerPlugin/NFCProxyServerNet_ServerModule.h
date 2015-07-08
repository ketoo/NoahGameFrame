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
#include "NFComm/NFPluginModule/NFIProxyServerToGameModule.h"

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

	//������Ϸ�ɹ�
	virtual int EnterGameSuccessEvent(const NFIDENTID xClientID, const NFIDENTID xPlayerID);

protected:

	int OnReciveClientPack(const NFIPacket& msg);
	int OnSocketClientEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	//���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
	void OnClientDisconnect(const int nAddress);
	//������
	void OnClientConnected(const int nAddress);

    int OnConnectKeyProcess(const NFIPacket& msg);
    int OnReqServerListProcess(const NFIPacket& msg);
    int OnSelectServerProcess(const NFIPacket& msg);
    int OnReqRoleListProcess(const NFIPacket& msg);
    int OnReqCreateRoleProcess(const NFIPacket& msg);
    int OnReqDelRoleProcess(const NFIPacket& msg);
    int OnReqEnterGameServer(const NFIPacket& msg);


    //�ͻ��˵�����60��ɾ��
    int HB_OnConnectCheckTime( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var );
    //////////////////////////////////////////////////////////////////////////
protected:

	NFMapEx<NFIDENTID, int> mxClientIdent;

protected:
	NFIProxyServerToWorldModule* m_pProxyToWorldModule;
	NFIProxyServerToGameModule* m_pProxyServerToGameModule;
	NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;
	NFIUUIDModule* m_pUUIDModule;

};

#endif