// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_GAMESERVER_SERVER_MODULE_H
#define NFC_GAMESERVER_SERVER_MODULE_H

//#include "GameServerPCH.h"
//#include "NW_Helper.h"
//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include <memory>
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFSLGDefine.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFISLGShopModule.h"
#include "NFComm/NFPluginModule/NFISLGBuildingModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPVPModule.h"
#include "NFComm/NFPluginModule/NFISkillModule.h"
#include "NFComm/NFPluginModule/NFIDataProcessModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIEquipModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
////////////////////////////////////////////////////////////////////////////



class NFCGameServerNet_ServerModule
    : public NFIGameServerNet_ServerModule
{
public:
    NFCGameServerNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual void LogRecive(const char* str) {}
    virtual void LogSend(const char* str) {}
    virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self);
    virtual void SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self);

protected:

    void OnRecivePSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSocketPSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    //���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
    void OnClientDisconnect(const int nSockIndex);
    //������
    void OnClientConnected(const int nSockIndex);

protected:
    void OnProxyServerRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnProxyServerUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnRefreshProxyServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

    void OnReqiureRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnCreateRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnDeleteRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienCommand(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    //////////////////////////////////////////////////////////////////////////

    void OnClienEnterGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienLeaveGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienGMProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////

    void OnClienSwapSceneProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnClienPickItem(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienMove(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienMoveImmune(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienAcceptTask(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienPushTask(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienPushCustom(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienChatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////

    void OnClientExitPVP(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnClientEndBattle(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    ///////////WORLD_START///////////////////////////////////////////////////////////////
    void OnTransWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnTransWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen, const int nWorldKey);

    ///////////WORLD_END///////////////////////////////////////////////////////////////

protected:
    //��self��ȫ�����Թ㲥��argVar[Ӧ���Ƕ�Զ�]
    int OnPropertyEnter(const NFIDataList& argVar, const NFGUID& self);
    int OnRecordEnter(const NFIDataList& argVar, const NFGUID& self);

    //��argVar��Щ�˵ĳ��ֻ�����ȥ�㲥��self��Щ��
    int OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar);
    int OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar);

    //����ͬ��
    int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    int OnGroupEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnContainerEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

    int GetBroadCastObject(const NFGUID& self, const std::string& strPropertyName, const bool bTable, NFIDataList& valueObject);
    int GetBroadCastObject(const int nObjectContainerID, const int nGroupID, NFIDataList& valueObject);
    //////////////////////////////////////////////////////////////////////////
    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
    int OnObjectNPCClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
    //////////////////////////////////////////////////////////////////////////
    // �����¼�
    int OnReturnEvent(const NFGUID& self, const int nEventID, const NFIDataList& var);
    // �ƶ��㲥
    int OnMoveEvent(const NFGUID& self, const int nEventID, const NFIDataList& var);
    // ���ܽ������¼�
    int OnUseSkillResultEvent(const NFGUID& self, const int nEventID, const NFIDataList& var);
    // �糡������¼�
    int OnSwapSceneResultEvent(const NFGUID& self, const int nEventID, const NFIDataList& var);
    // ����������
    int OnChatResultEvent(const NFGUID& self, const int nEventID, const NFIDataList& var);
    // ֪ͨ�����������
    int OnNoticeEctypeAward(const NFGUID& self, const int nEventID, const NFIDataList& var);

    void PlayerLeaveGameServer(const NFGUID& self);

    template<class PBClass>
    NFGUID GetGuildID(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
    {
        NFGUID nPlayerID;
        PBClass xMsg;
        if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
        {
            return NULL_OBJECT;
        }

        return PBToNF(xMsg.guild_id());
    }

private:

    struct GateData
    {
        ServerData xServerData;
        //�����������еĶ���<��ɫID,gate_FD>
        std::map<NFGUID, int> xRoleInfo;
    };

    //Ҫ����ǰ���еĶ������ڵ�actor,gateid,fd��
    struct BaseData
    {
        BaseData()
        {
            nActorID = 0;
            nGateID = 0;
        }

        BaseData(const int gateID, const NFGUID xIdent)
        {
            nActorID = 0;
            nGateID = gateID;
            xClientID = xIdent;
        }

        int nActorID;
        int nGateID;
        NFGUID xClientID;
    };

private:
    //<��ɫid,��ɫ���ػ�����Ϣ>//��ʵ������objectϵͳ�б�����
    NFMapEx<NFGUID, BaseData> mRoleBaseData;

    //gateid,data
    NFMapEx<int, GateData> mProxyMap;

    //////////////////////////////////////////////////////////////////////////
    NFIUUIDModule* m_pUUIDModule;
    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILogModule* m_pLogModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIElementInfoModule* m_pElementInfoModule;

    NFIDataProcessModule* m_pDataProcessModule;
    //////////////////////////////////////////////////////////////////////////
    NFIGameServerToWorldModule* m_pGameServerToWorldModule;
};
#endif
