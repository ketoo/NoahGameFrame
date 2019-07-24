/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "NFWSModule.h"


NFWSModule::NFWSModule(NFIPluginManager* p)
{
    pPluginManager = p;

    mnBufferSize = 0;
    nLastTime = GetPluginManager()->GetNowTime();
    m_pNet = NULL;
}

NFWSModule::~NFWSModule()
{
    if (m_pNet)
    {
        m_pNet->Final();
    }

    delete m_pNet;
    m_pNet = NULL;
}

bool NFWSModule::Init()
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	return true;
}

bool NFWSModule::AfterInit()
{
	return true;
}

void NFWSModule::Initialization(const char* strIP, const unsigned short nPort)
{
    m_pNet = NF_NEW NFNet(this, &NFWSModule::OnReceiveNetPack, &NFWSModule::OnSocketNetEvent, true);
    m_pNet->ExpandBufferSize(mnBufferSize);
    m_pNet->Initialization(strIP, nPort);
}

int NFWSModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
    m_pNet = NF_NEW NFNet(this, &NFWSModule::OnReceiveNetPack, &NFWSModule::OnSocketNetEvent, true);
    m_pNet->ExpandBufferSize(mnBufferSize);
    return m_pNet->Initialization(nMaxClient, nPort, nCpuCount);
}

int NFWSModule::ExpandBufferSize(const unsigned int size)
{
    if (size > 0)
    {
        mnBufferSize = size;
        if (m_pNet)
        {
            m_pNet->ExpandBufferSize(mnBufferSize);
        }
    }

    return mnBufferSize;
}

void NFWSModule::RemoveReceiveCallBack(const int nMsgID)
{
    std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(nMsgID);
    if (mxReceiveCallBack.end() != it)
    {
        mxReceiveCallBack.erase(it);
    }
}

bool NFWSModule::AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb)
{
    if (mxReceiveCallBack.find(nMsgID) == mxReceiveCallBack.end())
    {
		std::list<NET_RECEIVE_FUNCTOR_PTR> xList;
		xList.push_back(cb);
		mxReceiveCallBack.insert(std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::value_type(nMsgID, xList));
        return true;
    }

	std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(nMsgID);
	it->second.push_back(cb);

    return true;
}

bool NFWSModule::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb)
{
    mxCallBackList.push_back(cb);

    return true;
}

bool NFWSModule::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb)
{
    mxEventCallBackList.push_back(cb);

    return true;
}

bool NFWSModule::Execute()
{
    if (!m_pNet)
    {
        return false;
    }


    KeepAlive();

    return m_pNet->Execute();
}

bool NFWSModule::SendMsg(const std::string& msg, const NFSOCK nSockIndex)
{
    bool bRet = m_pNet->SendMsg(msg.c_str(), (uint32_t) msg.length(), nSockIndex);
	if (!bRet)
	{
		std::ostringstream stream;
		stream << " SendMsg failed fd " << nSockIndex;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);
	}

	return bRet;
}

bool NFWSModule::SendMsgToAllClient(const std::string& msg)
{
	bool bRet = m_pNet->SendMsgToAllClient(msg.c_str(), (uint32_t) msg.length());
	if (!bRet)
	{
		std::ostringstream stream;
		stream << " SendMsgToAllClient failed";
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);
	}

	return bRet;
}

NFINet* NFWSModule::GetNet()
{
    return m_pNet;
}

void NFWSModule::OnReceiveNetPack(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    if (nMsgID <= 0)
    {
        NetObject* pNetObject =  m_pNet->GetNetObject(nSockIndex);
        //pNetObject->GetBuffLen
    }
    else
    {

    }

	m_pLogModule->LogInfo("OnReceiveNetPack " + std::to_string(nMsgID), __FUNCTION__, __LINE__);

	NFPerformance performance;

    std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(nMsgID);
    if (mxReceiveCallBack.end() != it)
    {
		std::list<NET_RECEIVE_FUNCTOR_PTR>& xFunList = it->second;
		for (std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator itList = xFunList.begin(); itList != xFunList.end(); ++itList)
		{
			NET_RECEIVE_FUNCTOR_PTR& pFunPtr = *itList;
			NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
            //NF_CRASH_TRY
			pFunc->operator()(nSockIndex, nMsgID, msg, nLen);
    		//NF_CRASH_END_TRY
		}
    } 
	else
    {
        for (std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator itList = mxCallBackList.begin(); itList != mxCallBackList.end(); ++itList)
        {
            NET_RECEIVE_FUNCTOR_PTR& pFunPtr = *itList;
            NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
            //NF_CRASH_TRY
            pFunc->operator()(nSockIndex, nMsgID, msg, nLen);
    		//NF_CRASH_END_TRY
        }
    }

	if (performance.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "---------------net module performance problem------------------- ";
		os << performance.TimeScope();
		os << "---------- ";
		m_pLogModule->LogWarning(NFGUID(0, nMsgID), os, __FUNCTION__, __LINE__);
	}
}

void NFWSModule::OnSocketNetEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    for (std::list<NET_EVENT_FUNCTOR_PTR>::iterator it = mxEventCallBackList.begin();
         it != mxEventCallBackList.end(); ++it)
    {
        NET_EVENT_FUNCTOR_PTR& pFunPtr = *it;
        NET_EVENT_FUNCTOR* pFunc = pFunPtr.get();
        pFunc->operator()(nSockIndex, eEvent, pNet);
    }
}

void NFWSModule::KeepAlive()
{
    if (!m_pNet)
    {
        return;
    }

    if (m_pNet->IsServer())
    {
        return;
    }

    if (nLastTime + 10 > GetPluginManager()->GetNowTime())
    {
        return;
    }

    nLastTime = GetPluginManager()->GetNowTime();
}