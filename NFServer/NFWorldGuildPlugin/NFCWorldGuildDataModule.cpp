// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildDataModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildDataModule
//
// -------------------------------------------------------------------------

#include "NFWorldGuildPlugin.h"
#include "NFCWorldGuildDataModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCWorldGuildDataModule::Init()
{
    return true;
}

bool NFCWorldGuildDataModule::Shut()
{
    return true;
}

bool NFCWorldGuildDataModule::Execute()
{
    return true;
}

bool NFCWorldGuildDataModule::AfterInit()
{
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pClusterSQLModule = pPluginManager->FindModule<NFIClusterModule>("NFCMysqlClusterModule");
    m_pDataProcessModule = pPluginManager->FindModule<NFIDataProcessModule>("NFCDataProcessModule");

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pClusterSQLModule);
    assert(NULL != m_pDataProcessModule);

    m_pDataProcessModule->RegisterAutoSave("Guild");
    m_pKernelModule->AddClassCallBack("Guild", this, &NFCWorldGuildDataModule::OnGuildClassEvent);

    m_pKernelModule->CreateScene(mContainerID, "");

    m_pUUIDModule->SetIdentID(pPluginManager->AppID());
    return true;
}

bool NFCWorldGuildDataModule::ExitGuild( const NFGUID& self, const std::string& strName, bool& bExit)
{
    if (!m_pClusterSQLModule->Exists(mstrGuildNameTalble, strName, bExit))
    {
        return false;
    }

    return true;
}

void NFCWorldGuildDataModule::CheckLoadGuild( const NFGUID& self, const NFGUID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pObejct = m_pKernelModule->GetObject(xGuild);
    if (!pObejct.get())
    {
        if (m_pDataProcessModule->LoadDataFormSql(xGuild, "Guild"))
        {
            m_pKernelModule->CreateObject(xGuild, 1, 0, "Guild", "", NFCDataList());
        }
    }
}

int NFCWorldGuildDataModule::OnGuildClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == "Guild" )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            m_pKernelModule->AddHeartBeat(self, "OnSaveGuildheartEvent", this, &NFCWorldGuildDataModule::OnSaveGuildheartEvent, 30, 99999);
        }
    }

    return 0;
}


const NFGUID& NFCWorldGuildDataModule::CreateGuild(const NFGUID& xPlayeID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine /*= 1*/, const int nPower/* = NFMsg::GUILD_POWER_TYPE_PRESIDENT*/)
{
    bool bExit = false;
    if (!m_pClusterSQLModule->Exists(mstrGuildNameTalble, strName, bExit))
    {
        return NULL_OBJECT;
    }

    if (bExit)
    {
        return NULL_OBJECT;
    }

    const NFGUID xGuidID = m_pUUIDModule->CreateGUID();

    //Guild
    std::vector<std::string> vGuildFieldVec;
    std::vector<std::string> vGuildValueVec;

    vGuildFieldVec.push_back("Name");
    vGuildValueVec.push_back(strName);

    vGuildFieldVec.push_back("GuildLevel");
    vGuildValueVec.push_back("1");

    vGuildFieldVec.push_back("PresidentID");
    vGuildValueVec.push_back(xPlayeID.ToString());

    vGuildFieldVec.push_back("PresidentName");
    vGuildValueVec.push_back(strRoleName);

    vGuildFieldVec.push_back("GuildMemeberMaxCount");
    vGuildValueVec.push_back("200");

    if (!m_pClusterSQLModule->Updata(mstrGuildTalble, xGuidID.ToString(), vGuildFieldVec, vGuildValueVec))
    {
        return NULL_OBJECT;
    }

    //Name
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("GuildID");
    vValueVec.push_back(xGuidID.ToString());

    m_pClusterSQLModule->Updata(mstrGuildNameTalble, strName, vFieldVec, vValueVec);

    return xGuidID;
}

const bool NFCWorldGuildDataModule::DeleteGuild(const NFGUID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pGuilD = m_pKernelModule->GetObject(xGuild);
    if (pGuilD.get())
    {
        m_pKernelModule->DestroyObject(xGuild);
    }

    bool bExit = false;
    if (!m_pClusterSQLModule->Exists(mstrGuildTalble, xGuild.ToString(), bExit)
        || !bExit)
    {
        return false;
    }    

    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;
    vFieldVec.push_back("Name");
    
    if (!m_pClusterSQLModule->Query(mstrGuildTalble, xGuild.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildName = vValueVec[0];
    bExit = false;

    if (!m_pClusterSQLModule->Exists(mstrGuildNameTalble, strGuildName, bExit)
        || !bExit)
    {
        return false;
    }

    if (!m_pClusterSQLModule->Delete(mstrGuildTalble, xGuild.ToString()))
    {
        return false;
    }

    return true;
}

NF_SHARE_PTR<NFIObject> NFCWorldGuildDataModule::GetGuild( const NFGUID& xGuild )
{
    CheckLoadGuild(NFGUID(), xGuild);

    return m_pKernelModule->GetObject(xGuild);
}

bool NFCWorldGuildDataModule::GetPlayerGuild( const NFGUID& self, NFGUID& xGuild )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;
    vFieldVec.push_back("GuildID");

    if (!m_pClusterSQLModule->Query("Player", self.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildID = vValueVec[0];
    return xGuild.FromString(strGuildID);
}

bool NFCWorldGuildDataModule::SearchGuild( const NFGUID& self, const std::string& strName, std::vector<SearchGuildObject>& xList )
{   
    std::vector<std::string> vValueName;
    if (!m_pClusterSQLModule->Keys(mstrGuildNameTalble, strName, vValueName))
    {
        return false;
    }

    for (int i = 0; i < vValueName.size(); ++i)
    {
        const std::string& strGuildName = vValueName[i];

        std::vector<std::string> vFieldVec;
        std::vector<std::string> vValueVec;

        vFieldVec.push_back("GuildID");

        if (!m_pClusterSQLModule->Query(mstrGuildNameTalble, strGuildName, vFieldVec, vValueVec))
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

bool NFCWorldGuildDataModule::GetGuildInfo( const NFGUID& self, const NFGUID& xGuild, SearchGuildObject& xGuildInfo )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("Name");
    vFieldVec.push_back("GuilIDIcon");
    vFieldVec.push_back("GuildMemeberCount");
    vFieldVec.push_back("GuildMemeberMaxCount");
    vFieldVec.push_back("GuildHonor");
    vFieldVec.push_back("Rank");

    if (!m_pClusterSQLModule->Query(mstrGuildTalble, xGuild.ToString(), vFieldVec, vValueVec))
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

bool NFCWorldGuildDataModule::GetPlayerInfo( const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP )
{
    nDonation = 0;//这什么字段，还没加？
    nVIP = 1;
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("Name");
    vFieldVec.push_back("Level");
    vFieldVec.push_back("Job");

    if (!m_pClusterSQLModule->Query( "Player", self.ToString(), vFieldVec, vValueVec))
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

bool NFCWorldGuildDataModule::GetPlayerGameID( const NFGUID& self, int& nGameID )
{
    std::vector<std::string> xVecFeild;
    std::vector<std::string> xVecValue;

    xVecFeild.push_back("GameID");
    if (!m_pClusterSQLModule->Query("Player", self.ToString(), xVecFeild, xVecValue))
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

int NFCWorldGuildDataModule::OnSaveGuildheartEvent( const NFGUID& self , const std::string& strHeartName, const float fTime, const int nCount )
{
    m_pDataProcessModule->SaveDataToSql(self);

    return 0;
}
