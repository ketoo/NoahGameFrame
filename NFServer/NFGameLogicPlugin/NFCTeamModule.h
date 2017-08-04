// -------------------------------------------------------------------------
//    @FileName      :    NFITeamModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFITeamModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TEAM_MODULE_H
#define NFC_TEAM_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFITeamModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"

class NFCTeamModule
    : public NFITeamModule
{
public:
	NFCTeamModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

	virtual NFGUID CreateTeam( const NFGUID& self, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP);
	virtual bool JoinTeam(const NFGUID& self, const NFGUID& xTeamID);
	virtual bool LeaveTeam(const NFGUID& self, const NFGUID& xTeamID);
	virtual bool KickTeamMmember(const NFGUID& self, const NFGUID& xTeamID, const NFGUID& xMmember);

    virtual bool GetPlayerGameID(const NFGUID& self, int& nGameID);
    virtual bool GetMemberList(const NFGUID& self, const NFGUID& xTeam, std::vector<NFGUID>& xmemberList);
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xTeam, const int& nGameID);
    virtual bool MemberOffline(const NFGUID& self, const NFGUID& xTeam);
    virtual bool BroadcastMsgToTeam(const NFGUID& self, const NFGUID& xTeam, const uint16_t nMsgID, google::protobuf::Message& xData);

protected:
	void OnCreateTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnJoinTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnLeaveTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnOprTeamMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnTeamEnterEctypeProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    bool GetTeamInfo(const NFGUID& self, const NFGUID& xTeam, NFMsg::TeamInfo& xTeamInfo);

protected:
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
	NFILogModule* m_pLogModule;
    NFICommonRedisModule* m_pCommonRedisModule;
    NFIPlayerRedisModule* m_pPlayerRedisModule;
};

#endif