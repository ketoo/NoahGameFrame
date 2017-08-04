// -------------------------------------------------------------------------
//    @FileName			:    NFCNetLogic.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCNetLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_NETLOGIC_H
#define NFC_NETLOGIC_H

#include "NFCLogicBase.h"

class NFCNetLogic
    : public NFCLogicBase, public NFSingleton<NFCNetLogic>
{
public:
	NFCNetLogic() {};
	virtual ~NFCNetLogic() {};
    NFCNetLogic(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

public:	
	template<typename BaseType>
	bool AddReceiveCallBack(const int nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		return GetNetModule()->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_NONE, nMsgID, pBase, handleRecieve);
	}

	void ConnectServer(const std::string strIp, int nPort);	
	void SendToServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData);

protected:
	NFINetClientModule *GetNetModule();
	void OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	void OnEventResult(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnMsgRecive(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:	
	std::list<std::pair<uint16_t, std::string>> m_listDelayMsg;
	bool m_bSocketReady;
};

#define g_pNetLogic (NFCNetLogic::Instance())

#endif