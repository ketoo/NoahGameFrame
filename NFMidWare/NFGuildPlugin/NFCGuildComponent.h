// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-07-21
//    @Module           :    NFCGuildComponent
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_COMPONENT_H
#define NFC_GUILD_COMPONENT_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCGuildComponent : public NFIComponent
{
public:
	NFCGuildComponent() : NFIComponent(GET_CLASS_NAME(NFCGuildComponent))
	{
	}

	virtual ~NFCGuildComponent()
	{

	}

	virtual bool Init();

	virtual bool AfterInit();

	virtual int OnASyncEvent(const NFGUID& self, const int from, const int event, std::string& arg);

	const NFGUID& CreateGuild(const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob, const int nDonation, const int nVIP);
	bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID);
	bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID);
	bool PromotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);
	bool DemotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);
	bool KickMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);

	bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFDataList& varMemberList, NFDataList& varGameList);
	bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID);
	bool MemberOffline(const NFGUID& self, const NFGUID& xGuild);

	bool GetGuildBaseInfo(const NFGUID& self, const NFGUID& xGuildID);
	bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID);
	bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);

	void OnGuildOnlineProcess(const NFGUID& xGuildID);
	void OnGuildOfflineProcess(const NFGUID& xGuildID);

	bool CheckPower(const NFGUID& self, const NFGUID& xGuildID, int nPowerType);

protected:
	
	int OnCreateGuildProcess(const NFGUID& self, const int from, const int event, std::string& arg);
	int OnJoinGuildProcess(const NFGUID& self, const int from, const int event, std::string& arg);
	int OnLeaveGuildProcess(const NFGUID& self, const int from, const int event, std::string& arg);
	int OnOprGuildMemberProcess(const NFGUID& self, const int from, const int event, std::string& arg);
	int OnSearchGuildProcess(const NFGUID& self, const int from, const int event, std::string& arg);

private:
	NFMapEx<NFGUID, NFIObject> mxGuildObject;
};

#endif