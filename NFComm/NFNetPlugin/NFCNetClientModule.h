// -------------------------------------------------------------------------
//    @FileName         :    NFCNetClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-15
//    @Module           :    NFCNetClientModule
//
// -------------------------------------------------------------------------

#ifndef NFC_NET_CLIENT_MODULE_H
#define NFC_NET_CLIENT_MODULE_H

#include <iostream>
#include <iosfwd>
#include "NFCNet.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include "NFComm/NFCore/NFConsistentHash.hpp"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCNetClientModule : public NFINetClientModule
{
public:
	NFCNetClientModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

	virtual void AddServer(const ConnectData& xInfo);
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20);

	virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	virtual int AddEventCallBack(const NF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr);

	virtual void RemoveReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID);

	////////////////////////////////////////////////////////////////////////////////
	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const std::string& strData);
	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const char* msg, const uint32_t nLen);

	virtual void SendToAllServer(const uint16_t nMsgID, const std::string& strData);
	virtual void SendToAllServer(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData);

	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, const google::protobuf::Message& xData);

	virtual void SendToAllServerByPB(const uint16_t nMsgID, const google::protobuf::Message& xData);
	virtual void SendToAllServerByPB(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const google::protobuf::Message& xData);

	////////////////////////////////////////////////////////////////////////////////

	//SendBySuit & SendSuitByPB  suit by (int32)nHashKey32
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData);
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const char* msg, const uint32_t nLen);
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData);
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const char* msg, const uint32_t nLen);
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const google::protobuf::Message& xData);
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const google::protobuf::Message& xData);

	////////////////////////////////////////////////////////////////////////////////

	virtual NFMapEx<int, ConnectData>& GetServerList();
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NF_SERVER_TYPES eType);
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID);
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NFINet* pNet);

protected:

	void InitCallBacks(NF_SHARE_PTR<ConnectData> pServerData);
	void ProcessExecute();

	void KeepReport(NF_SHARE_PTR<ConnectData> pServerData) {};
	void LogServerInfo(const std::string& strServerInfo) {};

private:
	void LogServerInfo();

	void KeepState(NF_SHARE_PTR<ConnectData> pServerData);

	void OnSocketEvent(const NFSOCK fd, const NF_NET_EVENT eEvent, NFINet* pNet);

	int OnConnected(const NFSOCK fd, NFINet* pNet);

	int OnDisConnected(const NFSOCK fd, NFINet* pNet);

	void ProcessAddNetConnect();

private:
	int mnBufferSize;
	//server_id, server_data
	NFConsistentHashMapEx<int, ConnectData> mxServerMap;
	//server_type, server_id, server_data
	NFMapEx<int, NFConsistentHashMapEx<int, ConnectData>> mxServerTypeMap;

	std::list<ConnectData> mxTempNetList;

	struct CallBack
	{
		//call back
		std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
		std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBack;
		std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
	};

	NFMapEx<int, CallBack> mxCallBack;
};
#endif
