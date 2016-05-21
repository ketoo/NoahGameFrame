// -------------------------------------------------------------------------
//    @FileName         :    NFCTeamDataModule.h
//    @Author           :    chuanbo.guo
//    @Date             :    2015-06-06 16:51
//    @Module           :    NFCTeamDataModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TEAM_DATA_MODULE_H
#define NFC_TEAM_DATA_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFITeamDataModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFMessageDefine/NFMsgBase.pb.h"
#include "NFComm/NFPluginModule/NFIDataProcessModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"

class NFCTeamDataModule
    : public NFITeamDataModule
{
public:
    NFCTeamDataModule(NFIPluginManager* p)
    {
        pPluginManager = p;
		mnScenceID = 0;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual NF_SHARE_PTR<NFIObject> GetTeam(const NFGUID& xTeam);

    virtual bool GetPlayerInfo(const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP);
    virtual bool GetPlayerGameID( const NFGUID& self, int& nGameID );

    virtual const NFGUID CreateTeam(const NFGUID& xPlayeID, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine = 1);
    virtual const bool DeleteTeam(const NFGUID& xTeam);

protected:
    NFIKernelModule* m_pKernelModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIClusterModule* m_pClusterSQLModule;    
    NFICommonConfigModule* m_pCommonConfigModule;

	int mnScenceID;
    
private:
};

#endif