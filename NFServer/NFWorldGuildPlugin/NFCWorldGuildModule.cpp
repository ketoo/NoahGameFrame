// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#include "NFWorldGuildPlugin.h"
#include "NFCWorldGuildModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"

bool NFCWorldGuildModule::Init()
{
    return true;
}

bool NFCWorldGuildModule::Shut()
{
    return true;
}

bool NFCWorldGuildModule::Execute()
{
    return true;
}

bool NFCWorldGuildModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pWorldGuildDataModule = pPluginManager->FindModule<NFIWorldGuildDataModule>("NFCWorldGuildDataModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pWorldGuildDataModule);

    return true;
}

const NFGUID& NFCWorldGuildModule::CreateGuild( const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP)
{
    if (strName.empty())
    {
        return NULL_OBJECT;
    }

    bool bExit = false;
    if (!m_pWorldGuildDataModule->ExitGuild(self, strName, bExit))
    {
        return NULL_OBJECT;
    }

    if (bExit)
    {
        return NULL_OBJECT;
    }

    return m_pWorldGuildDataModule->CreateGuild(self, strName, strRoleName, nLevel, nJob, nDonation, nVIP);
}

bool NFCWorldGuildModule::JoinGuild( const NFGUID& self, const NFGUID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    const NFINT64 nCount = m_pKernelModule->GetPropertyInt(xGuildID, "GuildMemeberCount");
    if (nCount >= m_pKernelModule->GetPropertyInt(xGuildID, "GuildMemeberMaxCount"))
    {
        //limit
        return false;
    }

    NFCDataList varData;

    std::string strName ;
    int nLevel = 0;
    int nJob = 0;
    int nDonation = 0;
    int nReceive = 0;
    int nVIP = 0;
    int nOnLine = 0;
    int nPower = NFMsg::GUILD_POWER_TYPE_NORMAL;

    const NFGUID& xPresident = pGuildObject->GetPropertyObject("PresidentID");
    if (xPresident == self)
    {
        nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT;
    }
    
    int nTitle = 0;
    int nOnlineGameID = 0;

    m_pWorldGuildDataModule->GetPlayerGameID(self, nOnlineGameID);
    m_pWorldGuildDataModule->GetPlayerInfo(self, strName, nLevel, nJob, nDonation, nVIP);

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nReceive << (NFINT64)nVIP << (NFINT64)nOnLine << (NFINT64)nPower << nTitle << nOnlineGameID ;

    if (pMemberRecord->AddRow(-1, varData) < 0)
    {
        return false;
    }
    
    m_pKernelModule->SetPropertyInt(xGuildID, "GuildMemeberCount", nCount+1);

	return true;
}

bool NFCWorldGuildModule::LeaveGuild( const NFGUID& self, const NFGUID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    
    return pMemberRecord->Remove(nRow);
}

bool NFCWorldGuildModule::UpGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_UP);

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);

    const int nPower = pMemberRecord->GetInt(nRow, NFrame::Guild::GuildMemberList_Power);

    if (nPower >= NFMsg::GUILD_POWER_TYPE_PRESIDENT)
    {
        return false;
    }

    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Power, (nPower + 1)) ;
	return true;
}

bool NFCWorldGuildModule::DownGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_DOWN);

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);

    const int nPower = pMemberRecord->GetInt(nRow, NFrame::Guild::GuildMemberList_Power);
    if (nPower == 0)
    {
        return false;
    }

    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Power, (nPower - 1));
    return true;
}

bool NFCWorldGuildModule::KickGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    if (self == xMmember)
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_KICK);

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->Remove(nRow);

    return true;
}

bool NFCWorldGuildModule::GetGuildBaseInfo( const NFGUID& self, const NFGUID& xGuildID )
{
	return true;
}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFGUID& self, const NFGUID& xGuildID )
{

	return true;
}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{

	return false;
}

bool NFCWorldGuildModule::CheckPower( const NFGUID& self, const NFGUID& xGuildID, int nPowerType )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (pGuildObject.get())
    {
        NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
        if (pMemberRecord.get())
        {
            NFCDataList varList;
            pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
            if (varList.GetCount() == 1)
            {
                const int nRow = varList.Int(0);
                const int nPower = pMemberRecord->GetInt(nRow, NFrame::Guild::GuildMemberList_Power); 
                if (nPower >= NFMsg::GUILD_POWER_TYPE_PRESIDENT)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool NFCWorldGuildModule::GetOnlineMember( const NFGUID& self, const NFGUID& xGuild, NFCDataList& varMemberList, NFCDataList& varGameList)
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, "GuildMemberList");
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

        const NFINT64 nOnline = pMemberRecord->GetInt(i, NFrame::Guild::GuildMemberList_Online);
        const NFINT64 nGameID = pMemberRecord->GetInt(i, NFrame::Guild::GuildMemberList_GameID);
        const NFGUID& xID = pMemberRecord->GetObject(i, NFrame::Guild::GuildMemberList_GUID);
        if (nOnline > 0 && !xID.IsNull())
        {

            varMemberList.Add(xID);
            varGameList.Add(nGameID);
        }
    }

    return true;
}

bool NFCWorldGuildModule::MemberOnline( const NFGUID& self, const NFGUID& xGuild , const int& nGameID)
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() <=  0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_GameID, nGameID);
    
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Online, 1);

    return true;
}

bool NFCWorldGuildModule::MemberOffeline( const NFGUID& self, const NFGUID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }


    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() <= 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Online, 0);
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_GameID, 0);
    return true;
}