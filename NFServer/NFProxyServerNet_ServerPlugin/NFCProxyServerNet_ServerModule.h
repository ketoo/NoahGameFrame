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
#include "NFComm/NFPluginModule/NFIProxyServerNet_ClientModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

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


    virtual int GetServerPort();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const uint16_t nSockIndex, const NF_NET_EVENT eEvent);

	//���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
	void OnClientDisconnect(const uint16_t& nAddress);
	//������
	void OnClientConnected(const uint16_t& nAddress);

protected:
//     bool CheckPacketLegal(RakNet::Packet* pMsgPacket);
// 
//     void CreateConnectObject(const std::string& strAddress);
// 
//     void DestoryConnectObject(const std::string& strAddress);
// 
//     void OnDisAccount(const std::string& strAddress);
//     //////////////////////////////////////////////////////////////////////////
// 
//     int OnConnectKeyProcessTCP(RakNet::Packet* pMsgPacket, const std::string& strInfo);
// 
//     //�ͻ��˵�����
//     int HB_OnConnectCheckTime(const NFIDENTID& self, const NFIValueList& var);
// 
//     //������������������������Ϣ����
//     //int HB_OnPlayerWantToConnect( const NFIDENTID& self, const NFIValueList& var );
// 
//     //////////////////////////////////////////////////////////////////////////
// 
//     //�ͻ��˳������ӣ�����һ�����ȴ����ٴη�����Կ,��ʱ������ɾ��
//     int OnConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);
// 
//     //������������������������Ϣ����
//     int OnWantToConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);

protected:
    //�½��������Ӷ��󣬵ȴ������Լ�����֤KEY��KEY��֤��ɾ��
    //-1
    int mnConnectContainer;

    //�����������������˭���¼�˷��������Ǹ����󣬰����ʺź�KEY��Ϣ��KEY��֤��ɾ����
    //-2
    int mnWantToConnectContainer;

    //�����������������˭���¼�˷��������Ǹ����󣬰����ʺź�KEY��Ϣ��KEY��֤��ɾ����
    //-3
    int mnRoleHallContainer;

    NFIProxyServerNet_ClientModule* m_pProxyServerNet_ClientModule;
	NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif