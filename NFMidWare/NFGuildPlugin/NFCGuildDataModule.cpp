// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildDataModule.cpp
//    @Author           :    Chuanbo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFCGuildDataModule
//
// -------------------------------------------------------------------------

#include "NFCGuildDataModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCGuildDataModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();

    return true;
}

bool NFCGuildDataModule::Shut()
{
    return true;
}

bool NFCGuildDataModule::Execute()
{
    return true;
}

bool NFCGuildDataModule::AfterInit()
{
    //m_pDataProcessModule->RegisterAutoSave(NFrame::Guild::ThisName());
    m_pKernelModule->AddClassCallBack(NFrame::Guild::ThisName(), this, &NFCGuildDataModule::OnGuildClassEvent);

    return true;
}

bool NFCGuildDataModule::ExitGuild( const NFGUID& self, const std::string& strName, bool& bExit)
{
    //if (!m_pClusterSQLModule->Exists(NFrame::GuildName::ThisName(), strName, bExit))
    {
        return false;
    }

    return true;
}

void NFCGuildDataModule::CheckLoadGuild( const NFGUID& self, const NFGUID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pObejct = m_pKernelModule->GetObject(xGuild);
    if (!pObejct.get())
    {
        //if (m_pDataProcessModule->LoadDataFormSql(xGuild, "Guild"))
        {
            m_pKernelModule->CreateObject(xGuild, 1, 0, "Guild", "", NFDataList());
        }
    }
}

int NFCGuildDataModule::OnGuildClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
{
    if ( strClassName == "Guild" )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
			m_pScheduleModule->AddSchedule(self, "OnSaveGuildheartEvent", this, &NFCGuildDataModule::OnSaveGuildheartEvent, 9999999, -1);
        }
    }

    return 0;
}


const NFGUID NFCGuildDataModule::CreateGuild(const NFGUID& xPlayeID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine /*= 1*/, const int nPower/* = NFMsg::GUILD_POWER_TYPE_PRESIDENT*/)
{
    bool bExit = false;
    //if (!m_pClusterSQLModule->Exists(NFrame::GuildName::ThisName(), strName, bExit))
    {
        return NULL_OBJECT;
    }

    if (bExit)
    {
        return NULL_OBJECT;
    }

    const NFGUID xGuidID = m_pKernelModule->CreateGUID();

    //Guild
    std::vector<std::string> vGuildFieldVec;
    std::vector<std::string> vGuildValueVec;

    vGuildFieldVec.push_back(NFrame::Guild::Name());
    vGuildValueVec.push_back(strName);

    vGuildFieldVec.push_back(NFrame::Guild::GuildLevel());
    vGuildValueVec.push_back("1");

    vGuildFieldVec.push_back(NFrame::Guild::PresidentID());
    vGuildValueVec.push_back(xPlayeID.ToString());

    vGuildFieldVec.push_back(NFrame::Guild::PresidentName());
    vGuildValueVec.push_back(strRoleName);

    vGuildFieldVec.push_back(NFrame::Guild::GuildMemeberMaxCount());
    vGuildValueVec.push_back("50");

    //if (!m_pClusterSQLModule->Updata(NFrame::Guild::ThisName(), xGuidID.ToString(), vGuildFieldVec, vGuildValueVec))
    {
        return NULL_OBJECT;
    }

    //Name
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back(NFrame::GuildName::GuildID());
    vValueVec.push_back(xGuidID.ToString());

    //m_pClusterSQLModule->Updata(NFrame::GuildName::ThisName(), strName, vFieldVec, vValueVec);

    return xGuidID;
}

const bool NFCGuildDataModule::DeleteGuild(const NFGUID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pGuilD = m_pKernelModule->GetObject(xGuild);
    if (pGuilD.get())
    {
        m_pKernelModule->DestroyObject(xGuild);
    }

    bool bExit = false;
    //if (!m_pClusterSQLModule->Exists(NFrame::Guild::ThisName(), xGuild.ToString(), bExit) || !bExit)
    {
        return false;
    }    

    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;
    vFieldVec.push_back(NFrame::Guild::Name());
    
    //if (!m_pClusterSQLModule->Query(NFrame::Guild::ThisName(), xGuild.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildName = vValueVec[0];
    bExit = false;

    //if (!m_pClusterSQLModule->Exists(NFrame::GuildName::ThisName(), strGuildName, bExit) || !bExit)
    {
        return false;
    }

    //if (!m_pClusterSQLModule->Delete(NFrame::Guild::ThisName(), xGuild.ToString()))
    {
        return false;
    }

    return true;
}

NF_SHARE_PTR<NFIObject> NFCGuildDataModule::GetGuild( const NFGUID& xGuild )
{
    CheckLoadGuild(NFGUID(), xGuild);

    return m_pKernelModule->GetObject(xGuild);
}

bool NFCGuildDataModule::GetPlayerGuild( const NFGUID& self, NFGUID& xGuild )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;
    vFieldVec.push_back(NFrame::Guild::GuildID());

    //if (!m_pClusterSQLModule->Query(NFrame::Player::ThisName(), self.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildID = vValueVec[0];
    return xGuild.FromString(strGuildID);
}

bool NFCGuildDataModule::SearchGuild( const NFGUID& self, const std::string& strName, std::vector<SearchGuildObject>& xList )
{   
    std::vector<std::string> vValueName;
    //if (!m_pClusterSQLModule->Keys(NFrame::GuildName::ThisName(), strName, vValueName))
    {
        return false;
    }

    for (int i = 0; i < vValueName.size(); ++i)
    {
        const std::string& strGuildName = vValueName[i];

        std::vector<std::string> vFieldVec;
        std::vector<std::string> vValueVec;

        vFieldVec.push_back(NFrame::GuildName::GuildID());

        //if (!m_pClusterSQLModule->Query(NFrame::GuildName::ThisName(), strGuildName, vFieldVec, vValueVec))
        {
            continue;
        }

        NFGUID xGuild ;
        xGuild.FromString(vValueVec[0]);

        SearchGuildObject xGuildInfo;
        if (GetGuildInfo(self, xGuild, xGuildInfo))
        {
            xList.push_back(xGuildInfo);
        }
    }

    return true;
}

bool NFCGuildDataModule::GetGuildInfo( const NFGUID& self, const NFGUID& xGuild, SearchGuildObject& xGuildInfo )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back(NFrame::Guild::Name());
    vFieldVec.push_back(NFrame::Guild::GuildIcon());
    vFieldVec.push_back(NFrame::Guild::GuildMemeberCount());
    vFieldVec.push_back(NFrame::Guild::GuildMemeberMaxCount());
    vFieldVec.push_back(NFrame::Guild::GuildHonor());
    vFieldVec.push_back(NFrame::Guild::Rank());

    //if (!m_pClusterSQLModule->Query(NFrame::Guild::ThisName(), xGuild.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildName = vValueVec[0];

    int nGuildIconn = 0;
    int nGuildMemeberCount = 0;
    int nGuildMemeberMaxCount = 0;
    int nGuildHonor = 0;
    int nGuildRank = 0;

    NF_StrTo(vValueVec[1], nGuildIconn);
    NF_StrTo(vValueVec[2], nGuildMemeberCount);
    NF_StrTo(vValueVec[3], nGuildMemeberMaxCount);
    NF_StrTo(vValueVec[4], nGuildHonor);
    NF_StrTo(vValueVec[5], nGuildRank);

    xGuildInfo.mxGuildID = xGuild    ;
    xGuildInfo.mstrGuildName = strGuildName  ;
    xGuildInfo.mnGuildIcon = nGuildIconn  ;
    xGuildInfo.mnGuildMemberCount = nGuildMemeberCount  ;
    xGuildInfo.mnGuildMemberMaxCount = nGuildMemeberMaxCount  ;
    xGuildInfo.mnGuildHonor = nGuildHonor ;
    xGuildInfo.mnGuildRank = nGuildRank  ;  

    return true;
}

bool NFCGuildDataModule::GetPlayerInfo( const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP )
{
    nDonation = 0;//这什么字段，还没加？
    nVIP = 1;
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("Name");
    vFieldVec.push_back("Level");
    vFieldVec.push_back("Job");

    //if (!m_pClusterSQLModule->Query(NFrame::Player::ThisName(), self.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    if (!NF_StrTo(vValueVec[0], strRoleName))
    {
        return false;
    }

    if (!NF_StrTo(vValueVec[1], nLevel))
    {
        return false;
    }

    if (!NF_StrTo(vValueVec[2], nJob))
    {
        return false;
    }

    return true;
}

bool NFCGuildDataModule::GetPlayerGameID( const NFGUID& self, int& nGameID )
{
    std::vector<std::string> xVecFeild;
    std::vector<std::string> xVecValue;

    xVecFeild.push_back("GameID");
    //if (!m_pClusterSQLModule->Query(NFrame::Player::ThisName(), self.ToString(), xVecFeild, xVecValue))
    {
        return false;
    }

    const std::string& strGameID = xVecValue[0];
    if (!NF_StrTo(strGameID, nGameID))
    {
        return false;
    }

    return true;
}

int NFCGuildDataModule::OnSaveGuildheartEvent( const NFGUID& self , const std::string& strHeartName, const float fTime, const int nCount )
{
    //m_pDataProcessModule->SaveDataToSql(self);

    return 0;
}
