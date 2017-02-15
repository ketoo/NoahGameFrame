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
#include "NFComm/NFCore/NFCConsistentHash.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
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

	virtual int AddReceiveCallBack(NF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	virtual int AddReceiveCallBack(NF_SERVER_TYPES eType, const int nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	virtual int AddEventCallBack(NF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr);

	virtual void RemoveReceiveCallBack(NF_SERVER_TYPES eType, const int nMsgID);

	////////////////////////////////////////////////////////////////////////////////
	virtual void SendByServerID(const int nServerID, const int nMsgID, const std::string& strData);
	virtual void SendByServerID(const int nServerID, const int nMsgID, const char* msg, const uint32_t nLen);

	virtual void SendToAllServer(const int nMsgID, const std::string& strData);
	virtual void SendToAllServer(NF_SERVER_TYPES eType, const int nMsgID, const std::string& strData);

	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, google::protobuf::Message& xData);

	virtual void SendToAllServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData);
	virtual void SendToAllServerByPB(NF_SERVER_TYPES eType, const uint16_t nMsgID, google::protobuf::Message& xData);

	////////////////////////////////////////////////////////////////////////////////

	virtual void SendBySuit(const std::string& strHashKey, const int nMsgID, const std::string& strData);
	virtual void SendBySuit(NF_SERVER_TYPES eType, const std::string& strHashKey, const int nMsgID, const std::string& strData);

	virtual void SendBySuit(const std::string& strHashKey, const int nMsgID, const char* msg, const uint32_t nLen);
	virtual void SendBySuit(NF_SERVER_TYPES eType, const std::string& strHashKey, const int nMsgID, const char* msg, const uint32_t nLen);

	virtual void SendBySuit(const int& nHashKey, const int nMsgID, const std::string& strData);
	virtual void SendBySuit(NF_SERVER_TYPES eType, const int& nHashKey, const int nMsgID, const std::string& strData);

	virtual void SendBySuit(const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen);
	virtual void SendBySuit(NF_SERVER_TYPES eType, const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen);

	virtual void SendSuitByPB(const std::string& strHashKey, const uint16_t nMsgID, google::protobuf::Message& xData);
	virtual void SendSuitByPB(NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, google::protobuf::Message& xData);

	virtual void SendSuitByPB(const int& nHashKey, const uint16_t nMsgID, google::protobuf::Message& xData);
	virtual void SendSuitByPB(NF_SERVER_TYPES eType, const int& nHashKey, const uint16_t nMsgID, google::protobuf::Message& xData);

	////////////////////////////////////////////////////////////////////////////////

	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID);

	virtual NFMapEx<int, ConnectData>& GetServerList();
	virtual NFMapEx<int, ConnectData>& GetServerList(NF_SERVER_TYPES eType);

	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NFINet* pNet);

protected:

	void InitCallBacks(NF_SHARE_PTR<ConnectData> pServerData);
	void ProcessExecute();

	void KeepReport(NF_SHARE_PTR<ConnectData> pServerData) {};
	void LogServerInfo(const std::string& strServerInfo) {};

private:
	void LogServerInfo();

	void KeepState(NF_SHARE_PTR<ConnectData> pServerData);

	void OnSocketEvent(const int fd, const NF_NET_EVENT eEvent, NFINet* pNet);

	int OnConnected(const int fd, NFINet* pNet);

	int OnDisConnected(const int fd, NFINet* pNet);

	void ProcessAddNetConnect();

private:
	NFCConsistentHashMapEx<int, ConnectData> mxServerMap;

	std::list<ConnectData> mxTempNetList;

	struct CallBack
	{
		//call back
		std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
		std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBack;
		std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
	};

	NFMapEx<int, CallBack> mxCallBack;

	//call back
	//std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
	//std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBack;
	//std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
};
#endif
