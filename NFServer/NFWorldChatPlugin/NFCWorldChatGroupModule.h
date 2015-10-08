// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldChatGroupModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldChatGroupModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_WORLD_CHAT_GROUP_MODULE_H_
#define _NFC_WORLD_CHAT_GROUP_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldChatGroupModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIDataProcessModule.h"

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
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

public:
    virtual bool JoinGroup(const NFIDENTID& self, const NFIDENTID& xGroupID);
    virtual const NFIDENTID& CreateGroup(const NFIDENTID& self);
    virtual bool QuitGroup(const NFIDENTID& self, const NFIDENTID& xGroupID);
    virtual bool DeleteGroup(const NFIDENTID& self, const NFIDENTID& xGroupID);
    virtual NF_SHARE_PTR<NFIObject> GetGroup(const NFIDENTID& self);
    virtual bool GetOnlineMember( const NFIDENTID& self, const NFIDENTID& xGroupID, NFCDataList& varMemberList, NFCDataList& varGameList);
    virtual bool Online(const NFIDENTID& self, const NFIDENTID& xGroupID, const int& nGameID);
    virtual bool Offeline(const NFIDENTID& self, const NFIDENTID& xGroupID);

protected:
    void CheckLoadGuild( const NFIDENTID& self, const NFIDENTID& xGuild );
    int OnGuildClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    int OnSaveGroupheartEvent( const NFIDENTID& self , const std::string& strHeartName, const float fTime, const int nCount );

protected:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIClusterModule* m_pClusterSQLModule;    
    NFIDataProcessModule* m_pDataProcessModule;

private:
    std::string mstrGroupTalble;
    int mContainerID;
};

#endif