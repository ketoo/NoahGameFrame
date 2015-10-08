// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldChatModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldChatModule
//
// -------------------------------------------------------------------------

#include "NFWorldChatPlugin.h"
#include "NFCWorldChatGroupModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFMessageDefine/NFRecordDefine.pb.h"

bool NFCWorldChatGroupModule::Init()
{
    return true;
}

bool NFCWorldChatGroupModule::Shut()
{
    return true;
}

bool NFCWorldChatGroupModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCWorldChatGroupModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pUUIDModule = dynamic_cast<NFIUUIDModule*>(pPluginManager->FindModule("NFCUUIDModule"));
    m_pClusterSQLModule = dynamic_cast<NFIClusterModule*>(pPluginManager->FindModule("NFCMysqlClusterModule"));
    m_pDataProcessModule = dynamic_cast<NFIDataProcessModule*>(pPluginManager->FindModule("NFCDataProcessModule"));

    assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pClusterSQLModule);
	assert(NULL != m_pDataProcessModule);

    m_pDataProcessModule->RegisterAutoSave(mstrGroupTalble);
    m_pKernelModule->AddClassCallBack(mstrGroupTalble, this, &NFCWorldChatGroupModule::OnGuildClassEvent);

    m_pKernelModule->CreateContainer(mContainerID, "");
    return true;
}

bool NFCWorldChatGroupModule::JoinGroup( const NFIDENTID& self, const NFIDENTID& xGroupID )
{
    NF_SHARE_PTR<NFIObject> pGroup = GetGroup(xGroupID);
    if (NULL == pGroup)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord>pRecord = pGroup->GetRecordManager()->GetElement("GroupMemberList");
    if (NULL == pRecord)
    {
        return false;
    }

    NFCDataList varFind;
    if (pRecord->FindObject(0, self, varFind) > 0)
    {
        return false;
    }

    NFCDataList varRow;
    
    varRow << self << 0 << 0;
    return pRecord->AddRow(-1, varRow);
}

const NFIDENTID& NFCWorldChatGroupModule::CreateGroup( const NFIDENTID& self )
{
    const NFIDENTID xGuidID = m_pUUIDModule->CreateGUID();

    //Guild
    std::vector<std::string> vGroupFieldVec;
    std::vector<std::string> vGroupValueVec;

    vGroupFieldVec.push_back("Name");
    vGroupValueVec.push_back("");

    vGroupFieldVec.push_back("CreateObject");
    vGroupValueVec.push_back(self.ToString());

    if (!m_pClusterSQLModule->Updata(mstrGroupTalble, xGuidID.ToString(), vGroupFieldVec, vGroupValueVec))
    {
        return NFIDENTID();
    }

    return xGuidID;
}

bool NFCWorldChatGroupModule::QuitGroup( const NFIDENTID& self, const NFIDENTID& xGroupID )
{
    NF_SHARE_PTR<NFIObject> pChatGroup = GetGroup(xGroupID);
    if (pChatGroup == NULL)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pChatGroup->GetRecordManager()->GetElement("GroupMemberList");
    if (pRecord == NULL)
    {
        return false;
    }

    NFCDataList varFind;
    if (pRecord->FindObject(NFMsg::GroupMemberList_GUID, self, varFind) > 0)
    {
        pRecord->Remove(varFind);
    }

    return true;
}

bool NFCWorldChatGroupModule::DeleteGroup( const NFIDENTID& self, const NFIDENTID& xGroupID )
{
    NF_SHARE_PTR<NFIObject> pGuilD = m_pKernelModule->GetObject(xGroupID);
    if (pGuilD.get())
    {
        m_pKernelModule->DestroyObject(xGroupID);
    }

    bool bExit = false;
    if (!m_pClusterSQLModule->Exists(mstrGroupTalble, xGroupID.ToString(), bExit)
        || !bExit)
    {
        return false;
    }   

    if (!m_pClusterSQLModule->Delete(mstrGroupTalble, xGroupID.ToString()))
    {
        return false;
    }

    return true;
}

NF_SHARE_PTR<NFIObject> NFCWorldChatGroupModule::GetGroup( const NFIDENTID& self )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (pObject == NULL )
    {
        if (m_pDataProcessModule->LoadDataFormNoSql(self, mstrGroupTalble))
        {
            m_pKernelModule->CreateObject(self, mContainerID, 0, mstrGroupTalble, "", NFCDataList());
        }

        pObject = m_pKernelModule->GetObject(self);
    }

    return pObject;
}


bool NFCWorldChatGroupModule::GetOnlineMember( const NFIDENTID& self, const NFIDENTID& xGroupID, NFCDataList& varMemberList, NFCDataList& varGameList)
{
    NF_SHARE_PTR<NFIObject> pGuildObject = GetGroup(xGroupID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGroupID, "GroupMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    for (int i = 0; i< pMemberRecord->GetRows(); i++)
    {
        if (!pMemberRecord->IsUsed(i))
        {
            continue;
        }

        const NFINT64 nOnline = pMemberRecord->GetInt(i, NFMsg::GroupMemberList_Online);
        const NFINT64 nGameID = pMemberRecord->GetInt(i, NFMsg::GroupMemberList_GameID);
        const NFIDENTID& xID = pMemberRecord->GetObject(i, NFMsg::GroupMemberList_GUID);
        if (nOnline > 0 && !xID.IsNull())
        {

            varMemberList.Add(xID);
            varGameList.Add(nGameID);
        }
    }

    return true;
}

int NFCWorldChatGroupModule::OnGuildClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == mstrGroupTalble )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            m_pKernelModule->AddHeartBeat(self, "OnSaveGroupheartEvent", this, &NFCWorldChatGroupModule::OnSaveGroupheartEvent, 30, 99999);
        }
    }

    return 0;
}


int NFCWorldChatGroupModule::OnSaveGroupheartEvent( const NFIDENTID& self , const std::string& strHeartName, const float fTime, const int nCount )
{
    m_pDataProcessModule->SaveDataToNoSql(self);

    return 0;
}

bool NFCWorldChatGroupModule::Online( const NFIDENTID& self, const NFIDENTID& xGroupID, const int& nGameID )
{
    NF_SHARE_PTR<NFIObject> pGroup = GetGroup(xGroupID);
    if (NULL == pGroup)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pGroup->GetRecordManager()->GetElement("GroupMemberList");
    if (NULL == pRecord)
    {
        return false;
    }

    NFCDataList varList;
    if (pRecord->FindObject(NFMsg::GroupMemberList_GUID, self, varList) <= 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pRecord->SetInt(nRow, NFMsg::GroupMemberList_GameID, nGameID);

    pRecord->SetInt(nRow, NFMsg::GroupMemberList_Online, 1);
    
    return true;
}

bool NFCWorldChatGroupModule::Offeline( const NFIDENTID& self, const NFIDENTID& xGroupID )
{
    NF_SHARE_PTR<NFIObject> pGroup = GetGroup(xGroupID);
    if (NULL == pGroup)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pGroup->GetRecordManager()->GetElement("GroupMemberList");
    if (NULL == pRecord)
    {
        return false;
    }

    NFCDataList varList;
    if (pRecord->FindObject(NFMsg::GroupMemberList_GUID, self, varList) <= 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pRecord->SetInt(nRow, NFMsg::GroupMemberList_Online, 0);
    pRecord->SetInt(nRow, NFMsg::GroupMemberList_GameID, 0);
    return true;
}
