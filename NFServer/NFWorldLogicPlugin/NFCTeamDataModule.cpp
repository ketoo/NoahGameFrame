// -------------------------------------------------------------------------
//    @FileName      :    NFCTeamDataModule.cpp
//    @Author           :    Chuanbo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFCTeamDataModule
//
// -------------------------------------------------------------------------

#include "NFCTeamDataModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCTeamDataModule::Init()
{
    return true;
}

bool NFCTeamDataModule::Shut()
{
    return true;
}

bool NFCTeamDataModule::Execute()
{
    return true;
}

bool NFCTeamDataModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pClusterSQLModule = pPluginManager->FindModule<NFIClusterModule>("NFCMysqlClusterModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pClusterSQLModule);


    m_pUUIDModule->SetIdentID(pPluginManager->AppID());

//     mnScenceID = m_pCommonConfigModule->GetAttributeInt("TeamEctype", "TeamEctypeInfo", "ScenceID");
//     if (mnScenceID <=0)
//     {
		mnScenceID = 400;
        m_pKernelModule->CreateScene(mnScenceID);
//     }

    return true;
}



const NFGUID NFCTeamDataModule::CreateTeam(const NFGUID& xPlayeID, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob, const int nDonation, const int nVIP, const int nOffLine )
{
	NFGUID xTeam = xDefaultTeamID;
	if (xTeam.IsNull())
	{
		xTeam = m_pUUIDModule->CreateGUID();
	}
	
	m_pKernelModule->CreateObject(xTeam, mnScenceID, 0, "Team", "", NFCDataList());

    return xTeam;
}

const bool NFCTeamDataModule::DeleteTeam(const NFGUID& xTeam )
{
    NF_SHARE_PTR<NFIObject> pTeam = m_pKernelModule->GetObject(xTeam);
    if (pTeam.get())
    {
        m_pKernelModule->DestroyObject(xTeam);
    }
    
    return true;
}

NF_SHARE_PTR<NFIObject> NFCTeamDataModule::GetTeam( const NFGUID& xTeam )
{
    return m_pKernelModule->GetObject(xTeam);
}

bool NFCTeamDataModule::GetPlayerInfo( const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP )
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

bool NFCTeamDataModule::GetPlayerGameID( const NFGUID& self, int& nGameID )
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
