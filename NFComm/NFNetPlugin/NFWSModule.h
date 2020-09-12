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
    virtual void Initialization(const char* ip, const unsigned short nPort);

    //as server
    virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4);

    virtual unsigned int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20) override;

    virtual void RemoveReceiveCallBack(const int msgID);

    virtual bool AddReceiveCallBack(const int msgID, const NET_RECEIVE_FUNCTOR_PTR& cb);

    virtual bool AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb);

    virtual bool AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb);

    virtual bool Execute();

    virtual bool SendMsg(const std::string& msg, const NFSOCK sockIndex, const bool text = true);
    virtual bool SendMsgToAllClient(const std::string& msg, const bool text = true);
    virtual bool SendMsgWithOutHead(const int16_t msgID, const char* msg, const size_t len, const NFSOCK sockIndex /*= 0*/);
    virtual bool SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex);
    int EnCode(const uint16_t umsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData);
    virtual NFINet* GetNet();

    virtual void OnError(const NFSOCK sockIndex, const std::error_code& e);
protected:
    bool SendRawMsg(const std::string& msg, const NFSOCK sockIndex);

    void OnReceiveNetPack(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

    void OnSocketNetEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void KeepAlive();

    std::error_code HandShake(const NFSOCK sockIndex, const char* msg, const uint32_t len);

    std::error_code DecodeFrame(const NFSOCK sockIndex,NetObject* pNetObject);
    int DeCode(const char* strData, const uint32_t unAllLen, NFMsgHead& xHead);

    std::string EncodeFrame(const char* data, size_t size, bool text);
    

    std::string HashKey(const char* key, size_t len);
private:

    unsigned int mnBufferSize;
    NFINet* m_pNet;
    NFINT64 mLastTime;
	std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>> mxReceiveCallBack;
    std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBackList;
    std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;

	NFILogModule* m_pLogModule;
};

#endif
