// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildModle.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-07-21
//    @Module           :    NFCGuildModle
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_MODULE_H
#define NFC_GUILD_MODULE_H

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
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

protected:

    bool CreateGuild(const NFGUID& self, const NFGUID& xGuildID, const std::string& strName, const std::string& strGuildName);
    bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID);
    bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID);
    bool PromotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);
    bool DemotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);
    bool KickMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember);

    bool MemberOnline(const NFGUID& self, const NFGUID& xGuild);
    bool MemberOffline(const NFGUID& self, const NFGUID& xGuild);


    void OnGuildOnlineProcess(const NFGUID& xGuildID);
    void OnGuildOfflineProcess(const NFGUID& xGuildID);

    NFIGuildModule::MEMBER_TYPE CheckPower(const NFGUID& self, const NFGUID& xGuildID);

protected:

	void OnCreateGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnJoinGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnLeaveGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnOprGuildMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSearchGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

protected:

	NFConsistentHashMapEx<int, int> mActorList; //actorid <-->Used

protected:
	NFIActorModule* m_pActorModule;
	NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFIKernelModule* m_pKernelModule;
	NFIGuildRedisModule* m_pGuildRedisModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
};

#endif