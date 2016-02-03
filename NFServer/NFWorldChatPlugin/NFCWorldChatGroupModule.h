// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldChatGroupModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldChatGroupModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLD_CHAT_GROUP_MODULE_H
#define NFC_WORLD_CHAT_GROUP_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldChatGroupModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIDataProcessModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCWorldChatGroupModule
    : public NFIWorldChatGroupModule
{
public:
    NFCWorldChatGroupModule(NFIPluginManager* p)
    {
        pPluginManager = p;
        mstrGroupTalble = "ChatGroup";
        mContainerID = 2;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

public:
    virtual bool JoinGroup(const NFGUID& self, const NFGUID& xGroupID);
    virtual const NFGUID CreateGroup(const NFGUID& self);
    virtual bool QuitGroup(const NFGUID& self, const NFGUID& xGroupID);
    virtual bool DeleteGroup(const NFGUID& self, const NFGUID& xGroupID);
    virtual NF_SHARE_PTR<NFIObject> GetGroup(const NFGUID& self);
    virtual bool GetOnlineMember( const NFGUID& self, const NFGUID& xGroupID, NFCDataList& varMemberList, NFCDataList& varGameList);
    virtual bool Online(const NFGUID& self, const NFGUID& xGroupID, const int& nGameID);
    virtual bool Offeline(const NFGUID& self, const NFGUID& xGroupID);

protected:
	void OnReqCreateChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqJoineChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqLeaveChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqSubscriptionChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqCancelSubscriptionChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
    void CheckLoadGuild( const NFGUID& self, const NFGUID& xGuild );
    int OnGuildClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    int OnSaveGroupheartEvent( const NFGUID& self , const std::string& strHeartName, const float fTime, const int nCount );

protected:
    NFIKernelModule* m_pKernelModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIClusterModule* m_pClusterSQLModule;    
    NFIDataProcessModule* m_pDataProcessModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
	NFILogModule* m_pLogModule;

private:
    std::string mstrGroupTalble;
    int mContainerID;
};

#endif