// -------------------------------------------------------------------------
//    @FileName			:    NFITeamModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFITeamModule
//
// -------------------------------------------------------------------------

#ifndef NFI_TEAM_MODULE_H
#define NFI_TEAM_MODULE_H

#include "NFIModule.h"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif

class NFITeamModule
    : public NFIModule
{
public:
	virtual NFGUID CreateTeam(const NFGUID& self, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob, const int nDonation, const int nVIP) = 0;
	virtual bool JoinTeam(const NFGUID& self, const NFGUID& xTeamID) = 0;
	virtual bool LeaveTeam(const NFGUID& self, const NFGUID& xTeamID) = 0;
	virtual bool KickTeamMmember(const NFGUID& self, const NFGUID& xTeamID, const NFGUID& xMmember) = 0;

	virtual bool MemberOnline(const NFGUID& self, const NFGUID& xTeam, const int& nGameID) = 0;
	virtual bool MemberOffline(const NFGUID& self, const NFGUID& xTeam) = 0;
	virtual bool GetMemberList(const NFGUID& self, const NFGUID& xTeam, std::vector<NFGUID>& xmemberList) = 0;
	virtual bool BroadcastMsgToTeam(const NFGUID& self, const NFGUID& xTeam, const uint16_t nMsgID, google::protobuf::Message& xData) = 0;
private:
};

#endif