// -------------------------------------------------------------------------
//    @FileName			:    NFITeamModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFITeamModule
//
// -------------------------------------------------------------------------

#ifndef NFI_TEAM_MODULE_H
#define NFI_TEAM_MODULE_H

#include "NFILogicModule.h"

class NFITeamModule
    : public NFILogicModule
{
public:
	virtual const NFGUID& CreateTeam(const NFGUID& self, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob, const int nDonation, const int nVIP) = 0;
	virtual bool JoinTeam(const NFGUID& self, const NFGUID& xTeamID) = 0;
	virtual bool LeaveTeam(const NFGUID& self, const NFGUID& xTeamID) = 0;
	virtual bool KickTeamMmember(const NFGUID& self, const NFGUID& xTeamID, const NFGUID& xMmember) = 0;

	virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xTeam, NFCDataList& varMemberList, NFCDataList& varGameList) = 0;
	virtual bool MemberOnline(const NFGUID& self, const NFGUID& xTeam, const int& nGameID) = 0;
	virtual bool MemberOffeline(const NFGUID& self, const NFGUID& xTeam) = 0;
private:
};

#endif