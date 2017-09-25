// -------------------------------------------------------------------------
//    @FileName         :    NFITeamDataModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-30 16:51
//    @Module           :    NFITeamDataModule
//
// -------------------------------------------------------------------------

#ifndef NFI_TEAM_DATA_MODULE_H
#define NFI_TEAM_DATA_MODULE_H

#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif

class NFITeamDataModule
    : public NFIModule
{
public:    

    virtual bool GetPlayerInfo(const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP) = 0;
	virtual const NFGUID CreateTeam(const NFGUID& xPlayeID, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob, const int nDonation, const int nVIP, const int nOffLine = 1) = 0;
	virtual const bool DeleteTeam(const NFGUID& xTeam) = 0;
    virtual NF_SHARE_PTR<NFIObject> GetTeam(const NFGUID& xTeam) = 0;
    virtual NF_SHARE_PTR<NFIPropertyManager> GetTeamPropertyManager(const NFGUID& xTeam) = 0;
    virtual NF_SHARE_PTR<NFIRecordManager> GetTeamRecordManager(const NFGUID& xTeam) = 0;
    virtual bool GetPlayerGameID(const NFGUID& self, int& nGameID) = 0;

protected:
private:
};

#endif