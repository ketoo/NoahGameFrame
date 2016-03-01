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
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCWorldChatGroupModule::Init()
{
    return true;
}

bool NFCWorldChatGroupModule::Shut()
{
    m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>("NFCWorldNet_ServerModule");

    assert(NULL != m_pWorldNet_ServerModule);

    if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_CREATE_CHATGROUP, this, &NFCWorldChatGroupModule::OnReqCreateChatGroupProcess))
    {
        return false;
    }
    if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_JOIN_CHATGROUP, this, &NFCWorldChatGroupModule::OnReqJoineChatGroupProcess))
    {
        return false;
    }
    if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_LEAVE_CHATGROUP, this, &NFCWorldChatGroupModule::OnReqLeaveChatGroupProcess))
    {
        return false;
    }
    if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_SUBSCRIPTION_CHATGROUP, this, &NFCWorldChatGroupModule::OnReqSubscriptionChatGroupProcess))
    {
        return false;
    }
    if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_CANCELSUBSCRIPTION_CHATGROUP, this, &NFCWorldChatGroupModule::OnReqCancelSubscriptionChatGroupProcess))
    {
        return false;
    }
    return true;
}

bool NFCWorldChatGroupModule::Execute()
{
    return true;
}

bool NFCWorldChatGroupModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pClusterSQLModule = pPluginManager->FindModule<NFIClusterModule>("NFCMysqlClusterModule");
    m_pDataProcessModule = pPluginManager->FindModule<NFIDataProcessModule>("NFCDataProcessModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pClusterSQLModule);
    assert(NULL != m_pDataProcessModule);
    assert(NULL != m_pLogModule);

    m_pDataProcessModule->RegisterAutoSave(mstrGroupTalble);
    m_pKernelModule->AddClassCallBack(mstrGroupTalble, this, &NFCWorldChatGroupModule::OnGuildClassEvent);

    m_pKernelModule->CreateScene(mContainerID);
    return true;
}

bool NFCWorldChatGroupModule::JoinGroup(const NFGUID& self, const NFGUID& xGroupID)
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
    int nRet = pRecord->AddRow(-1, varRow);

    if (nRet >= 0)
    {
        return true;
    }

    return false;
}

const NFGUID NFCWorldChatGroupModule::CreateGroup(const NFGUID& self)
{
    const NFGUID xGuidID = m_pUUIDModule->CreateGUID();

    //Guild
    std::vector<std::string> vGroupFieldVec;
    std::vector<std::string> vGroupValueVec;

    vGroupFieldVec.push_back("Name");
    vGroupValueVec.push_back("");

    vGroupFieldVec.push_back("CreateObject");
    vGroupValueVec.push_back(self.ToString());

    if (!m_pClusterSQLModule->Updata(mstrGroupTalble, xGuidID.ToString(), vGroupFieldVec, vGroupValueVec))
    {
        return NULL_OBJECT;
    }

    return xGuidID;
}

bool NFCWorldChatGroupModule::QuitGroup(const NFGUID& self, const NFGUID& xGroupID)
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
    if (pRecord->FindObject(NFrame::ChatGroup::GroupMemberList_GUID, self, varFind) > 0)
    {
        pRecord->Remove(varFind);
    }

    return true;
}

bool NFCWorldChatGroupModule::DeleteGroup(const NFGUID& self, const NFGUID& xGroupID)
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

NF_SHARE_PTR<NFIObject> NFCWorldChatGroupModule::GetGroup(const NFGUID& self)
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (pObject == NULL)
    {
        if (m_pDataProcessModule->LoadDataFormSql(self, mstrGroupTalble))
        {
            m_pKernelModule->CreateObject(self, mContainerID, 0, mstrGroupTalble, "", NFCDataList());
        }

        pObject = m_pKernelModule->GetObject(self);
    }

    return pObject;
}


bool NFCWorldChatGroupModule::GetOnlineMember(const NFGUID& self, const NFGUID& xGroupID, NFCDataList& varMemberList, NFCDataList& varGameList)
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

    for (int i = 0; i < pMemberRecord->GetRows(); i++)
    {
        if (!pMemberRecord->IsUsed(i))
        {
            continue;
        }

        const NFINT64 nOnline = pMemberRecord->GetInt(i, NFrame::ChatGroup::GroupMemberList_Online);
        const NFINT64 nGameID = pMemberRecord->GetInt(i, NFrame::ChatGroup::GroupMemberList_GameID);
        const NFGUID& xID = pMemberRecord->GetObject(i, NFrame::ChatGroup::GroupMemberList_GUID);
        if (nOnline > 0 && !xID.IsNull())
        {

            varMemberList.Add(xID);
            varGameList.Add(nGameID);
        }
    }

    return true;
}

int NFCWorldChatGroupModule::OnGuildClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == mstrGroupTalble)
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
        {
            m_pKernelModule->AddHeartBeat(self, "OnSaveGroupheartEvent", this, &NFCWorldChatGroupModule::OnSaveGroupheartEvent, 30, 99999);
        }
    }

    return 0;
}


int NFCWorldChatGroupModule::OnSaveGroupheartEvent(const NFGUID& self , const std::string& strHeartName, const float fTime, const int nCount)
{
    m_pDataProcessModule->SaveDataToSql(self);

    return 0;
}

bool NFCWorldChatGroupModule::Online(const NFGUID& self, const NFGUID& xGroupID, const int& nGameID)
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
    if (pRecord->FindObject(NFrame::ChatGroup::GroupMemberList_GUID, self, varList) <= 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pRecord->SetInt(nRow, NFrame::ChatGroup::GroupMemberList_GameID, nGameID);

    pRecord->SetInt(nRow, NFrame::ChatGroup::GroupMemberList_Online, 1);

    return true;
}

bool NFCWorldChatGroupModule::Offeline(const NFGUID& self, const NFGUID& xGroupID)
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
    if (pRecord->FindObject(NFrame::ChatGroup::GroupMemberList_GUID, self, varList) <= 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pRecord->SetInt(nRow, NFrame::ChatGroup::GroupMemberList_Online, 0);
    pRecord->SetInt(nRow, NFrame::ChatGroup::GroupMemberList_GameID, 0);
    return true;
}

void NFCWorldChatGroupModule::OnReqCreateChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckCreateChatGroup);

    const NFGUID xGroup = CreateGroup(nPlayerID);
    if (!xGroup.IsNull())
    {
        NFMsg::ReqAckCreateChatGroup xAck;
        xAck.set_name(xMsg.name());
        *xAck.mutable_selfid() = NFINetModule::NFToPB(nPlayerID);
        *xAck.mutable_xchatgroupid() = NFINetModule::NFToPB(xGroup);
        m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGEC_ACK_CREATE_CHATGROUP, xAck, nSockIndex, nPlayerID);
    }
    else
    {
        NFMsg::ReqAckCreateChatGroup xAck;
        xAck.set_name(xMsg.name());
        *xAck.mutable_selfid() = NFINetModule::NFToPB(nPlayerID);
        *xAck.mutable_xchatgroupid() = NFINetModule::NFToPB(xGroup);
        m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGEC_ACK_CREATE_CHATGROUP, xAck, nSockIndex, nPlayerID);
    }
}

void NFCWorldChatGroupModule::OnReqJoineChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckjoinChatGroup);

    const NFGUID xGroup = NFINetModule::PBToNF(xMsg.xchatgroupid());
    if (JoinGroup(nPlayerID, xGroup))
    {
        NFMsg::ReqAckjoinChatGroup xAck;

        *xAck.mutable_selfid() = NFINetModule::NFToPB(nPlayerID);
        *xAck.mutable_xchatgroupid() = NFINetModule::NFToPB(xGroup);
        xAck.set_result(1);
        m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGEC_ACK_JOIN_CHATGROUP, xAck, nSockIndex, nPlayerID);
    }
    else
    {
        NFMsg::ReqAckjoinChatGroup xAck;

        *xAck.mutable_selfid() = NFINetModule::NFToPB(nPlayerID);
        *xAck.mutable_xchatgroupid() = NFINetModule::NFToPB(xGroup);
        xAck.set_result(0);
        m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGEC_ACK_JOIN_CHATGROUP, xAck, nSockIndex, nPlayerID);
    }
}

void NFCWorldChatGroupModule::OnReqLeaveChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckQuitChatGroup);

    const NFGUID xGroup = NFINetModule::PBToNF(xMsg.xchatgroupid());
    if (QuitGroup(nPlayerID, xGroup))
    {
        NFMsg::ReqAckQuitChatGroup xAck;

        *xAck.mutable_selfid() = NFINetModule::NFToPB(nPlayerID);
        *xAck.mutable_xchatgroupid() = NFINetModule::NFToPB(xGroup);
        xAck.set_result(1);
        m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGEC_ACK_LEAVE_CHATGROUP, xAck, nSockIndex, nPlayerID);
    }
    else
    {
        NFMsg::ReqAckQuitChatGroup xAck;

        *xAck.mutable_selfid() = NFINetModule::NFToPB(nPlayerID);
        *xAck.mutable_xchatgroupid() = NFINetModule::NFToPB(xGroup);
        xAck.set_result(0);
        m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGEC_ACK_LEAVE_CHATGROUP, xAck, nSockIndex, nPlayerID);
    }
}

void NFCWorldChatGroupModule::OnReqSubscriptionChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSubscriptionChatGroup);

    int nGameID = 0;
    //  if (!m_pWorldGuildDataModule->GetPlayerGameID(nPlayerID, nGameID))
    //  {
    //      return ;
    //  }

    for (int i = 0; i < xMsg.xchatgroupid_size(); ++ i)
    {
        const NFGUID xGroup = NFINetModule::PBToNF(xMsg.xchatgroupid(i));

        Online(nPlayerID, xGroup, nGameID);
    }
}

void NFCWorldChatGroupModule::OnReqCancelSubscriptionChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCancelSubscriptionChatGroup);

    for (int i = 0; i < xMsg.xchatgroupid_size(); ++ i)
    {
        const NFGUID xGroup = NFINetModule::PBToNF(xMsg.xchatgroupid(i));

        Offeline(nPlayerID, xGroup);
    }
}
