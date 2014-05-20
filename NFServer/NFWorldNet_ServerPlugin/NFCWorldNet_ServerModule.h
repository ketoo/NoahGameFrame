// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_WORLDNET_SERVER_MODULE_H_
#define _NFC_WORLDNET_SERVER_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFNet/NFIPacket.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

class NFCWorldNet_ServerModule
    : public NFINetModule,
  public NFMap<NFIDENTID,NFList<std::string>>//帐号--应该是网关->帐号列表
{
public:
    NFCWorldNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

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

    int OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);
    int OnClientExitWorldEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);
protected:

	bool InThisWorld(const std::string& strAccount);

    //////////////////////////////////////////////////////////////////////////
    int GetGameServerObject(const int nServerID, NFIValueList& varObjectList);
    //NFIDENTID GetDataBaseObject(const int nDataBaseID);
    int GetProxyObject(const int nProxyID, NFIValueList& varObjectList);

    int OnGameServerRegisteredProcess(const NFIPacket& msg);
    int OnGameServerUnRegisteredProcess(const NFIPacket& msg);
    int OnRefreshGameServerInfoProcess(const NFIPacket& msg);

    //     int OnDataBaseRegisteredProcess(const NFIPacket& msg);    //
    //     int OnDataBaseUnRegisteredProcess(const NFIPacket& msg);

    int OnProxyServerRegisteredProcess(const NFIPacket& msg);
    int OnProxyServerUnRegisteredProcess(const NFIPacket& msg);
    int OnRefreshProxyServerInfoProcess(const NFIPacket& msg);

    int OnLeaveGameProcess(const NFIPacket& msg);
    //////////////////////////////////////////////////////////////////////////

    //     int OnLoadRoleDataBeginProcess(const NFIPacket& msg);
    //     int OnLoadRoleDataFinalProcess(const NFIPacket& msg);
    //     int OnSaveRoleDataProcess(const NFIPacket& msg);
    //     int OnWantToSwapGSProcess(const NFIPacket& msg);
	void SynGameToProxy();
	void SynGameToProxy( const int nFD );
private:
    //world server container:-3
    int mnGameServerContainerID;

    //data server container:-4
    int mnDataBaseContainerID;

    //login server container:-10
    int mnProxyContainerID;

	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;
    NFIWorldLogicModule* m_pWorldLogicModule;
	NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif