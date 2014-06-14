// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVER_SERVER_MODULE_H_
#define _NFC_GAMESERVER_SERVER_MODULE_H_

//#include "GameServerPCH.h"
//#include "NW_Helper.h"
//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCGameServerNet_ServerModule
    : public NFINetModule
{
public:
    NFCGameServerNet_ServerModule(NFIPluginManager* p)
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

	//���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
	void OnClientDisconnect(const uint16_t& nAddress);
	//������
	void OnClientConnected(const uint16_t& nAddress);


    
protected:

    //Ҫ����ǰ���еĶ������ڵ�actor���Ա�ת��
    struct BaseData 
    {
        std::string strAccount;
        int32_t nAccountID;
        NFIDENTID NGateID;
        NFIDENTID nPlayerID;
    };


private:

    NFMap<NFIDENTID, BaseData> mBaseData;

	int mnRoleHallContainer;
	int mnProxyContainer;;

    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILogModule* m_pLogModule;
    NFIEventProcessModule* m_pEventProcessModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFIElementInfoModule* m_pElementInfoModule;
    NFIGameLogicModule* m_pGameLogicModule;
};

#endif
