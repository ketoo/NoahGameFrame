// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginLogic.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCLoginLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGINLOGIC_H
#define NFC_LOGINLOGIC_H

#include "NFCLogicBase.h"

enum LoginLogicEvent
{
	E_LoginEvent_LoginSuccess,
	E_LoginEvent_WorldList,
	E_LoginEvent_ServerList,
	E_LoginEvent_RoleList,
};

class NFCLoginLogic
    : public NFCLogicBase, public NFSingleton<NFCLoginLogic>
{
public:
	NFCLoginLogic() {};
	virtual ~NFCLoginLogic() {};
    NFCLoginLogic(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();
	
    virtual bool AfterInit();

	// 发送消息
public:
	void LoginPB(const std::string &strAccount, const std::string &strPwd, const std::string &strKey);
	void RequireWorldList();
	void RequireConnectWorld(int nWorldID);
	void RequireVerifyWorldKey(const std::string &strAccount, const std::string &strKey);
	void RequireServerList();
	void RequireSelectServer(int nServerID);

	void RequireRoleList();

	// 接收消息
private:
	void OnLoginProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnWorldList(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnConnectWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnConnectKey(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSelectServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

public:
	std::vector<NFMsg::ServerInfo> GetWorldList() { return m_WorldServerList; }
	std::vector<NFMsg::ServerInfo> GetServerList() { return m_GameServerList; }
	const std::string& GetAccount() { return m_strAccount; }
	int GetServerID() { return m_nServerID; }

private:
	std::string m_strAccount;
	std::string m_strKey;
	int m_nServerID;
	std::vector<NFMsg::ServerInfo> m_WorldServerList;
	std::vector<NFMsg::ServerInfo> m_GameServerList;
};

#define g_pLoginLogic (NFCLoginLogic::Instance())

#endif