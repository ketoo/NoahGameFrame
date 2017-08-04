// -------------------------------------------------------------------------
//    @FileName			:    NFCChatLogic.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCChatLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_ChatLogic_H
#define NFC_ChatLogic_H

#include "NFCLogicBase.h"

enum ChatLogicEvent
{
	E_ChatEvent_ChatWorld, // iID iType sName sInfo iContainersize [iType, sData]
	E_ChatEvent_ChatGuild,
	E_ChatEvent_ChatPrivate,
	E_ChatEvent_ChatTeam,
};

class NFCChatLogic
    : public NFCLogicBase, public NFSingleton<NFCChatLogic>
{
public:
	NFCChatLogic() {};
	virtual ~NFCChatLogic() {};
    NFCChatLogic(NFIPluginManager* p)
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
	void RequireChatWorld(std::string strContent);
	void RequireChatGuild(std::string strContent);
	void RequireChatPrivate(const NFGUID &target, std::string strContent);
	void RequireChatTeam(std::string strContent);

	// 接收消息
private:
	void OnChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

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

#define g_pChatLogic (NFCChatLogic::Instance())

#endif