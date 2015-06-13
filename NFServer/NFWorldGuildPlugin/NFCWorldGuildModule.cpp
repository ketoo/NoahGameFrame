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

NFIDENTID NFCWorldGuildModule::CreateGuild( const NFIDENTID& self, const std::string& strName )
{
    if (strName.empty())
    {
        return NFIDENTID();
    }

    bool bExit = false;
    if (!m_pWorldGuildDataModule->ExitGuild(self, strName, bExit))
    {
        m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_SHOW_STRING, NFCDataList() << self << (int)NFMsg::EGEC_INVALID_GANGS_NAME);
        return NFIDENTID();
    }

    if (bExit)
    {
        return NFIDENTID();
    }

    NFIDENTID xGuidID  = m_pWorldGuildDataModule->CreateGuild(self, strName);
    if (xGuidID.IsNull())
    {
        return xGuidID;
    }

    if (!JoinGuild(self, xGuidID))
    {
        m_pWorldGuildDataModule->DeleteGuild(xGuidID);
        return NFIDENTID();
    }

    if (!SetPresidentInfo(self, xGuidID))
    {
        m_pWorldGuildDataModule->DeleteGuild(xGuidID);
        return NFIDENTID();
    }
    
	return xGuidID;
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

    const std::string& strName = m_pKernelModule->GetPropertyString(self, "Name");
    const int nLevel = m_pKernelModule->GetPropertyInt(self, "Level");
    const int nJob = m_pKernelModule->GetPropertyInt(self, "Job");
    const int nDonation = 0;
    const int nVIP = m_pKernelModule->GetPropertyInt(self, "VIP");
    const int nOffLine = m_pKernelModule->GetPropertyInt(self, "OffLine");
    const int nPower = NFMsg::GUILD_POWER_TYPE_NORMAL;

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nVIP << (NFINT64)nOffLine << (NFINT64)nPower;

    pMemberRecord->AddRow(-1, varData);
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

	return false;
}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
	return false;

}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
{
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
