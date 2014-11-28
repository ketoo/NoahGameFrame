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
#include "NFComm/NFPluginModule/NFISLGShopModule.h"
#include "NFComm/NFPluginModule/NFISLGBuildingModule.h"
#include "NFComm/NFMessageDefine/NFSLGDefine.pb.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"

////////////////////////////////////////////////////////////////////////////
// �ͻ�����Ϣ�����
#define CLIENT_MSG_PROCESS(packet, msg)                 \
    NFIDENTID nPlayerID = 0;                              \
    msg xMsg;                                           \
    if (!RecivePB(packet, xMsg, nPlayerID))              \
    {                                                   \
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, 0, "ReqAckBuyObjectFormShop", "Parse msg error", __FUNCTION__, __LINE__); \
        return;                                         \
    }                                                   \
                                                        \
    NFIObject* pObject = m_pKernelModule->GetObject(nPlayerID); \
    if ( NULL == pObject )                              \
    {                                                   \
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "FromClient Object do not Exist", "", __FUNCTION__, __LINE__); \
        return;                                         \
    }
//////////////////////////////////////////////////////////////////////////


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
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent);

	//���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
	void OnClientDisconnect(const int nSockIndex);
	//������
	void OnClientConnected(const int nSockIndex);

protected:
    int OnProxyServerRegisteredProcess(const NFIPacket& msg);
    int OnProxyServerUnRegisteredProcess(const NFIPacket& msg);
    int OnRefreshProxyServerInfoProcess(const NFIPacket& msg);

protected:

    void OnReqiureRoleListProcess(const NFIPacket& msg);
    void OnCreateRoleGameProcess(const NFIPacket& msg);
    void OnDeleteRoleGameProcess(const NFIPacket& msg);
    void OnClienCommand(const NFIPacket& msg);
    //////////////////////////////////////////////////////////////////////////

    void OnClienEnterGameProcess(const NFIPacket& msg);
    void OnClienLeaveGameProcess(const NFIPacket& msg);
    void OnClienGMProcess(const NFIPacket& msg);

    //////////////////////////////////////////////////////////////////////////
    
    void OnClienSwapSceneProcess(const NFIPacket& msg);
    void OnClienUseSkill(const NFIPacket& msg);
    void OnClienUseItem(const NFIPacket& msg);
    void OnClienPickItem(const NFIPacket& msg);
    void OnClienMove(const NFIPacket& msg);
    void OnClienMoveImmune(const NFIPacket& msg);
    void OnClienAcceptTask(const NFIPacket& msg);
    void OnClienPushTask(const NFIPacket& msg);
    void OnClienPushCustom(const NFIPacket& msg);
    void OnClienChatProcess(const NFIPacket& msg);

	/////////SLG_START/////////////////////////////////////////////////////////////////
	void OnSLGClienBuyItem(const NFIPacket& msg);
	void OnSLGClienMoveObject(const NFIPacket& msg);
	void OnSLGClienUpgradeBuilding(const NFIPacket& msg);
	void OnSLGClienCreateItem(const NFIPacket& msg);

	/////////SLG_END/////////////////////////////////////////////////////////////////
protected:
    //��self��ȫ�����Թ㲥��argVar[Ӧ���Ƕ�Զ�]
    int OnPropertyEnter( const NFIDENTID& self, const NFIDataList& argVar );
    int OnRecordEnter( const NFIDENTID& self, const NFIDataList& argVar );

    //��argVar��Щ�˵ĳ��ֻ�����ȥ�㲥��self��Щ��
    int OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar );
    int OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar );

    //����ͬ��
    int OnPropertyCommonEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar );
    int OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar );
    int OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    int OnGroupEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar );
    int OnContainerEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar );

    int GetBroadCastObject( const NFIDENTID& self, const std::string& strPropertyName, const bool bTable, NFIDataList& valueObject );
    int GetBroadCastObject( const int nObjectContainerID, const int nGroupID, NFIDataList& valueObject );
    //////////////////////////////////////////////////////////////////////////
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    //////////////////////////////////////////////////////////////////////////
    // �����¼�
    int OnReturnEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var );
    // �ƶ��㲥
    int OnMoveEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var );
    // ���ܽ������¼�
    int OnUseSkillResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var );
    // �糡������¼�
    int OnSwapSceneResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var );
    // ����������
    int OnChatResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var );
private:

    struct ServerData 
    {
        ServerData()
        {
            pData = new NFMsg::ServerInfoReport();
            nFD = 0;
        }

        ~ServerData()
        {
            nFD = 0;
            delete pData;
            pData = NULL;
        }

        int nFD;
        NFMsg::ServerInfoReport* pData;

        //�����������еĶ���<��ɫID,gate_FD>
        std::map<NFIDENTID, int> xRoleInfo;
    };

    //Ҫ����ǰ���еĶ������ڵ�actor,gateid,fd��
    struct BaseData 
    {
        BaseData()
        {
            nActorID = 0;
            nGateID = 0;
            nFD = 0;
        }

        int nActorID;
        int nGateID;
        int nFD;
    };

private:
    //<��ɫid,��ɫ���ػ�����Ϣ>//��ʵ������objectϵͳ�б�����
    NFMap<NFIDENTID, BaseData> mRoleBaseData;
    //gateid,data
    NFMap<int, ServerData> mProxyMap;

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILogModule* m_pLogModule;
    NFIEventProcessModule* m_pEventProcessModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFIElementInfoModule* m_pElementInfoModule;
	NFIUUIDModule* m_pUUIDModule;
    //////////////////////////////////////////////////////////////////////////
    //SLGģ��
	NFISLGShopModule* m_pSLGShopModule;
    NFISLGBuildingModule* m_pSLGBuildingModule;
};

#endif
