// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFCGuildModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_MODULE_H
#define NFC_GUILD_MODULE_H

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildModule.h"
#include "NFComm/NFPluginModule/NFIGuildDataModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
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
	virtual bool UpGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);
	virtual bool DownGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);
	virtual bool KickGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);

    virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFDataList& varMemberList, NFDataList& varGameList);
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID);
	virtual bool MemberOffline(const NFGUID& self, const NFGUID& xGuild);

	//////////////////////其次考虑参数////////////////////////////////////////////////////
	virtual bool GetGuildBaseInfo(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);

protected:
	void OnCreateGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnJoinGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnLeaveGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnOprGuildMemberProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSearchGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
    bool CheckPower(const NFGUID& self, const NFGUID& xGuildID, int nPowerType);

protected:
    NFIKernelModule* m_pKernelModule;
    NFIGuildDataModule* m_pGuildDataModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
};

#endif