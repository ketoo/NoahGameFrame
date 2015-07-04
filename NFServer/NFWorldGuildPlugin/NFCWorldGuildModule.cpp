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

bool NFCWorldGuildModule::Execute(const float fLasFrametime, const float fStartedTime)
{


    return true;
}

bool NFCWorldGuildModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pUUIDModule = dynamic_cast<NFIUUIDModule*>(pPluginManager->FindModule("NFCUUIDModule"));
    m_pWorldGuildDataModule = dynamic_cast<NFIWorldGuildDataModule*>(pPluginManager->FindModule("NFCWorldGuildDataModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pWorldGuildDataModule);

    return true;
}

NFIDENTID NFCWorldGuildModule::CreateGuild( const NFIDENTID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP)
{
    if (strName.empty())
    {
        return NFIDENTID();
    }

    bool bExit = false;
    if (!m_pWorldGuildDataModule->ExitGuild(self, strName, bExit))
    {
        return NFIDENTID();
    }

    if (bExit)
    {
        return NFIDENTID();
    }

    return m_pWorldGuildDataModule->CreateGuild(self, strName, strRoleName, nLevel, nJob, nDonation, nVIP);
}

bool NFCWorldGuildModule::JoinGuild( const NFIDENTID& self, const NFIDENTID& xGuildID )
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    NFCDataList varData;

    std::string strName ;
    int nLevel = 0;
    int nJob = 0;
    int nDonation = 0;
    int nVIP = 0;
    int nOffLine = 1;
    int nPower = NFMsg::GUILD_POWER_TYPE_NORMAL;
    int nTitle = 0;
    int nOnlineGameID = 0;

    m_pWorldGuildDataModule->GetGameID(self, nOnlineGameID);
    m_pWorldGuildDataModule->GetPlayerInfo(self, strName, nLevel, nJob, nDonation, nVIP);

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nVIP << (NFINT64)nOffLine << (NFINT64)nPower << nTitle << nOnlineGameID ;

    pMemberRecord->AddRow(-1, varData);

    m_pWorldGuildDataModule->SetGuild(self, xGuildID);
	return true;
}

bool NFCWorldGuildModule::LeaveGuild( const NFIDENTID& self, const NFIDENTID& xGuildID )
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    
    return pMemberRecord->Remove(nRow);
}

bool NFCWorldGuildModule::UpGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);

    const int nPower = pMemberRecord->GetInt(nRow, "Power");

    if (nPower >= NFMsg::GUILD_POWER_TYPE_PRESIDENT)
    {
        return false;
    }

    pMemberRecord->SetInt(nRow, "Power", (nPower + 1)) ;

    m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_SHOW_RECORD, NFCDataList() << xGuildID << self << "GuildMemberList" << nRow);

	return true;
}

bool NFCWorldGuildModule::DownGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);

    const int nPower = pMemberRecord->GetInt(nRow, "Power");
    if (nPower == 0)
    {
        return false;
    }

    pMemberRecord->SetInt(nRow, "Power", (nPower - 1));

    m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_SHOW_RECORD, NFCDataList() << xGuildID << self << "GuildMemberList" << nRow);

    return true;
}

bool NFCWorldGuildModule::KickGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->Remove(nRow);

    return true;
}

bool NFCWorldGuildModule::GetGuildBaseInfo( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_SHOW_PROPERTY, NFCDataList() << xGuildID << self);
	return true;
}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_SHOW_PROPERTY, NFCDataList() << xGuildID << self << "GuildMemberList" << -1);
	return true;
}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
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
    pMemberRecord->FindObject("GUID", xMmember, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }
    
    const int nRow = varList.Int(0);

    m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_SHOW_PROPERTY, NFCDataList() << xGuildID << self << "GuildMemberList" << nRow);
	return false;
}

bool NFCWorldGuildModule::CheckPower( const NFIDENTID& self, const NFIDENTID& xGuildID, int nPowerType )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (pGuildObject.get())
    {
        NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
        if (pMemberRecord.get())
        {
            NFCDataList varList;
            pMemberRecord->FindObject("GUID", self, varList);
            if (varList.GetCount() == 1)
            {
                const int nRow = varList.Int(0);
                const int nPower = pMemberRecord->GetInt(nRow, "Power"); 
                if (nPower >= NFMsg::GUILD_POWER_TYPE_PRESIDENT)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool NFCWorldGuildModule::SetPresidentInfo( const NFIDENTID& self, const NFIDENTID& xGuildID )
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    const std::string& strRoleName = m_pKernelModule->GetPropertyString(self, "Name");
    pGuildObject->SetPropertyString("PresidentName", strRoleName);

    pMemberRecord->SetInt(nRow, "Power", NFMsg::GUILD_POWER_TYPE_PRESIDENT);

    return true;
}

bool NFCWorldGuildModule::GetOnlineMember( const NFIDENTID& self, const NFIDENTID& xGuild, NFCDataList& varMemberList, NFCDataList& varGameList)
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

        const NFINT64 nOnline = pMemberRecord->GetInt(i, "Offline");
        const NFINT64 nGameID = pMemberRecord->GetInt(i, "GameID");
        if (nOnline >=0)
        {
            const NFIDENTID& xID = pMemberRecord->GetObject(i, "GUID");

            varMemberList.Add(xID);
            varGameList.Add(nGameID);
        }
    }

    return true;
}

bool NFCWorldGuildModule::MemberOnline( const NFIDENTID& self, const NFIDENTID& xGuild , const int& nGameID)
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, "Offline", 1);

    pMemberRecord->SetInt(nRow, "GameID", nGameID);
    return true;
}

bool NFCWorldGuildModule::MemberOffeline( const NFIDENTID& self, const NFIDENTID& xGuild )
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
    pMemberRecord->FindObject("GUID", self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, "Offline", 0);
    pMemberRecord->SetInt(nRow, "GameID", 0);
    return true;
}

bool NFCWorldGuildModule::GetGuildID( const NFIDENTID& self, NFIDENTID& xGuild )
{
    return m_pWorldGuildDataModule->GetGuild(self, xGuild);
}