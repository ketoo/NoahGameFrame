// -------------------------------------------------------------------------
//    @FileName         :    NFINetClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-4
//    @Module           :    NFINetClientModule
//
// -------------------------------------------------------------------------

#ifndef NFI_CLUSTER_CLIENT_MODULE_H
#define NFI_CLUSTER_CLIENT_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"
#include "NFComm/NFCore/NFConsistentHash.hpp"

enum ConnectDataState
{
	DISCONNECT,
    CONNECTING,
	NORMAL,
    RECONNECT,
};

struct ConnectData
{
	ConnectData()
	{
		nGameID = 0;
		nPort = 0;
		strName = "";
		strIP = "";
		eServerType = NF_ST_NONE;
		eState = ConnectDataState::DISCONNECT;
		mnLastActionTime = 0;
	}

	int nGameID;
	NF_SERVER_TYPES eServerType;
	std::string strIP;
	int nPort;
	std::string strName;
	ConnectDataState eState;
	NFINT64 mnLastActionTime;

	NF_SHARE_PTR<NFINetModule> mxNetModule;
};

class NFINetClientModule : public NFIModule
{
public:
    enum EConstDefine
    {
        EConstDefine_DefaultWeith = 500,
    };

	template<typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const NFSOCK, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		AddReceiveCallBack(eType, nMsgID, functorPtr);

		return true;
	}

	template<typename BaseType>
	int AddReceiveCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handleRecieve)(const NFSOCK, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		AddReceiveCallBack(eType, functorPtr);

		return true;
	}

	template<typename BaseType>
	bool AddEventCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handler)(const NFSOCK, const NF_NET_EVENT, NFINet*))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr(new NET_EVENT_FUNCTOR(functor));

		AddEventCallBack(eType, functorPtr);

		return true;
	}

	virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddEventCallBack(const NF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr) = 0;

	virtual void RemoveReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID) = 0;
	////////////////////////////////////////////////////////////////////////////////

	virtual void AddServer(const ConnectData& xInfo) = 0;
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20) = 0;

	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const char* msg, const uint32_t nLen) = 0;

	virtual void SendToAllServer(const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendToAllServer(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData) = 0;

	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;

	virtual void SendToAllServerByPB(const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;
	virtual void SendToAllServerByPB(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;

	////////////////////////////////////////////////////////////////////////////////

	//SendBySuit & SendSuitByPB  suit by (int32)nHashKey32
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const char* msg, const uint32_t nLen) = 0;
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const char* msg, const uint32_t nLen) = 0;
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;

	////////////////////////////////////////////////////////////////////////////////

	virtual NFMapEx<int, ConnectData>& GetServerList() = 0;
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NF_SERVER_TYPES eType) = 0;
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID) = 0;
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NFINet* pNet) = 0;
};
#endif
