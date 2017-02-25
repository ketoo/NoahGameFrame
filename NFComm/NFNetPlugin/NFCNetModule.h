// -------------------------------------------------------------------------
//    @FileName         :    NFCNetModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-15
//    @Module           :    NFCNetModule
//
// -------------------------------------------------------------------------

#ifndef NFC_NET_MODULE_H
#define NFC_NET_MODULE_H

#include <iostream>
#include <iosfwd>
#include "NFINet.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFCNetModule
	: public NFINetModule
{
public:
	NFCNetModule(NFIPluginManager* p);

	virtual ~NFCNetModule();

	//as client
	virtual void Initialization(const char* strIP, const unsigned short nPort);

	//as server
	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4);
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20);

	virtual void RemoveReceiveCallBack(const int nMsgID);

	virtual bool AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb);

	virtual bool AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb);

	virtual bool AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb);

	virtual bool Execute();

	
	virtual bool SendMsgWithOutHead(const int nMsgID, const std::string& msg, const int nSockIndex);

	virtual bool SendMsgToAllClientWithOutHead(const int nMsgID, const std::string& msg);

	virtual bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex);

	virtual bool SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message& xData);

	virtual bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList = NULL);

	virtual bool SendMsgPB(const uint16_t nMsgID, const std::string& strData, const uint32_t nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList = NULL);

	virtual NFINet* GetNet();

protected:
	void OnReceiveNetPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnSocketNetEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	void KeepAlive();

private:

	int mnBufferSize;
	NFINet* m_pNet;
	NFINT64 nLastTime;
	std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
	std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBackList;
	std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
};
#endif