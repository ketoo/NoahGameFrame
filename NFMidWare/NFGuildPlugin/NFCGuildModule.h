// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildModle.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-07-21
//    @Module           :    NFCGuildComponent
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_MODULE_H
#define NFC_GUILD_MODULE_H

#include "NFCGuildComponent.h"
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCGuildModule
    : public NFIGuildModule
{
public:
    NFCGuildModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

	virtual const NFGUID& CreateGuild( const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP);
	virtual bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool PromotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);
	virtual bool DemotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);
	virtual bool KickMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);

    virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFDataList& varMemberList, NFDataList& varGameList);
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID);
	virtual bool MemberOffline(const NFGUID& self, const NFGUID& xGuild);

	virtual bool GetGuildBaseInfo(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);

protected:
	void OnGuildOnlineProcess(const NFGUID& xGuildID);
	void OnGuildOfflineProcess(const NFGUID& xGuildID);

	int ComponentAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData);

protected:

	void OnCreateGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnJoinGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnLeaveGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnOprGuildMemberProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSearchGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
    bool CheckPower(const NFGUID& self, const NFGUID& xGuildID, int nPowerType);

protected:
	NFIActorModule* m_pActorModule;
	NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFIKernelModule* m_pKernelModule;
	NFIGuildRedisModule* m_pGuildRedisModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;

};

#endif