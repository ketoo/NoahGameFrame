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

class NFCProxyServerNet_ServerModule : public NFINetModule
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

	//连接丢失,删2层(连接对象，帐号对象)
	void OnClientDisconnect(const uint16_t& nAddress);
	//有连接
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
//     //客户端的连接
//     int HB_OnConnectCheckTime(const NFIDENTID& self, const NFIValueList& var);
// 
//     //保存的世界服务器发过来的信息对象
//     //int HB_OnPlayerWantToConnect( const NFIDENTID& self, const NFIValueList& var );
// 
//     //////////////////////////////////////////////////////////////////////////
// 
//     //客户端初次连接，设置一心跳等待他再次发送密钥,超时不发则删掉
//     int OnConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);
// 
//     //保存的世界服务器发过来的信息对象
//     int OnWantToConnectObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);

protected:
    //新建立的连接对象，等待他们自己发验证KEY，KEY验证后删掉
    //-1
    int mnConnectContainer;

    //世界服务器发过来，谁想登录此服务器的那个对象，包含帐号和KEY信息，KEY验证后删掉？
    //-2
    int mnWantToConnectContainer;

    //世界服务器发过来，谁想登录此服务器的那个对象，包含帐号和KEY信息，KEY验证后删掉？
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