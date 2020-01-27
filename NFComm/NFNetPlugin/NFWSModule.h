/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NF_WS_SERVER_MODULE_H
#define NF_WS_SERVER_MODULE_H

#include <iostream>
#include "NFNetModule.h"
#include "NFComm/NFPluginModule/NFIWSModule.h"

class NFWSModule: public NFIWSModule
{
public:
    NFWSModule(NFIPluginManager* p);

    virtual ~NFWSModule();

	virtual bool Init();
	virtual bool AfterInit();

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

    virtual bool SendMsg(const std::string& msg, const NFSOCK nSockIndex, const bool text = true);
    virtual bool SendMsgToAllClient(const std::string& msg, const bool text = true);
    virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const size_t nLen, const NFSOCK nSockIndex /*= 0*/);
    virtual bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const NFSOCK nSockIndex);
    int EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData);
    virtual NFINet* GetNet();

    virtual void OnError(const NFSOCK nSockIndex, const std::error_code& e);
protected:
    bool SendRawMsg(const std::string& msg, const NFSOCK nSockIndex);

    void OnReceiveNetPack(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnSocketNetEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void KeepAlive();

    std::error_code HandShake(const NFSOCK nSockIndex, const char* msg, const uint32_t nLen);

    std::error_code DecodeFrame(const NFSOCK nSockIndex,NetObject* pNetObject);
    int DeCode(const char* strData, const uint32_t unAllLen, NFMsgHead& xHead);

    std::string EncodeFrame(const char* data, size_t size, bool text);
    

    std::string HashKey(const char* key, size_t len);
private:

    int mnBufferSize;
    NFINet* m_pNet;
    NFINT64 nLastTime;
	std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>> mxReceiveCallBack;
    std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBackList;
    std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;

	NFILogModule* m_pLogModule;
};

#endif
