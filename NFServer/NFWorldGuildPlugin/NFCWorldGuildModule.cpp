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

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);

    return true;
}

NFIDENTID NFCWorldGuildModule::CreateGuild( const NFIDENTID& self, const std::string& strName )
{
    if (strName.empty() || mmGuidMap.GetElement(strName).get())
    {
        return NFIDENTID();
    }

    const NFIDENTID xGuidID = m_pUUIDModule->CreateGUID();

    if (!mmGuidMap.AddElement(strName, NF_SHARE_PTR<NFIDENTID>( NF_NEW NFIDENTID(xGuidID) ) ))
    {
        return NFIDENTID();
    }

    NF_SHARE_PTR<NFIObject> pGuildObejct = m_pKernelModule->CreateObject(xGuidID, 1, 0, "Guild", "", NFCDataList());
    if (!pGuildObejct.get())
    {
        mmGuidMap.RemoveElement(strName);
        return NFIDENTID();
    }

    const std::string& strRoleName = m_pKernelModule->GetPropertyString(self, "Name");

    pGuildObejct->SetPropertyString("Name", strName);
    pGuildObejct->SetPropertyInt("GuildLevel", 1);

    if (!JoinGuild(self, xGuidID))
    {
        mmGuidMap.RemoveElement(strName);
        return NFIDENTID();
    }

    if (!SetPresidentInfo(self, xGuidID))
    {
        mmGuidMap.RemoveElement(strName);
        return NFIDENTID();
    }
    
	return xGuidID;
}

bool NFCWorldGuildModule::JoinGuild( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pKernelModule->GetObject(xGuildID);
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

    varData << self << strName << nLevel << nJob << nDonation << nVIP << nOffLine << nPower;

    pMemberRecord->AddRow(-1, varData);
	return true;
}

bool NFCWorldGuildModule::LeaveGuild( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pKernelModule->GetObject(xGuildID);
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
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pKernelModule->GetObject(xGuildID);
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

	return true;
}

bool NFCWorldGuildModule::DownGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pKernelModule->GetObject(xGuildID);
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

    return true;
}

bool NFCWorldGuildModule::KickGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pKernelModule->GetObject(xGuildID);
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
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pKernelModule->GetObject(xGuildID);
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
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pKernelModule->GetObject(xGuildID);
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
