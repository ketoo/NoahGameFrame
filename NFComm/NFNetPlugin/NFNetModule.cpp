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

#include "NFNetModule.h"

NFNetModule::NFNetModule(NFIPluginManager* p)
{
    m_bIsExecute = true;
    pPluginManager = p;
    
    mnBufferSize = 0;
    nLastTime = GetPluginManager()->GetNowTime();
    m_pNet = NULL;
}

NFNetModule::~NFNetModule()
{
    if (m_pNet)
    {
        m_pNet->Final();
    }

    delete m_pNet;
    m_pNet = NULL;
}

bool NFNetModule::Init()
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	return true;
}

bool NFNetModule::AfterInit()
{
	return true;
}

void NFNetModule::Initialization(const char* ip, const unsigned short nPort)
{
    m_pNet = NF_NEW NFNet(this, &NFNetModule::OnReceiveNetPack, &NFNetModule::OnSocketNetEvent);
    m_pNet->ExpandBufferSize(mnBufferSize);
    m_pNet->Initialization(ip, nPort);
}

int NFNetModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
    m_pNet = NF_NEW NFNet(this, &NFNetModule::OnReceiveNetPack, &NFNetModule::OnSocketNetEvent);
    m_pNet->ExpandBufferSize(mnBufferSize);
    return m_pNet->Initialization(nMaxClient, nPort, nCpuCount);
}

unsigned int NFNetModule::ExpandBufferSize(const unsigned int size)
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

void NFNetModule::RemoveReceiveCallBack(const int msgID)
{
    std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(msgID);
    if (mxReceiveCallBack.end() != it)
    {
        mxReceiveCallBack.erase(it);
    }
}

bool NFNetModule::AddReceiveCallBack(const int msgID, const NET_RECEIVE_FUNCTOR_PTR& cb)
{
    if (mxReceiveCallBack.find(msgID) == mxReceiveCallBack.end())
    {
		std::list<NET_RECEIVE_FUNCTOR_PTR> xList;
		xList.push_back(cb);
		mxReceiveCallBack.insert(std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::value_type(msgID, xList));
        return true;
    }

	std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(msgID);
	it->second.push_back(cb);

    return true;
}

bool NFNetModule::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb)
{
    mxCallBackList.push_back(cb);

    return true;
}

bool NFNetModule::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb)
{
    mxEventCallBackList.push_back(cb);

    return true;
}

bool NFNetModule::Execute()
{
    if (!m_pNet)
    {
        return false;
    }


    KeepAlive();

	m_pNet->Execute();

	return true;
}

bool NFNetModule::SendMsgWithOutHead(const int msgID, const std::string& msg, const NFSOCK sockIndex)
{
    bool bRet = m_pNet->SendMsgWithOutHead(msgID, msg.c_str(), (uint32_t) msg.length(), sockIndex);
	if (!bRet)
	{
		std::ostringstream stream;
		stream << " SendMsgWithOutHead failed fd " << sockIndex;
		stream << " msg id " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);
	}

	return bRet;
}

bool NFNetModule::SendMsgToAllClientWithOutHead(const int msgID, const std::string& msg)
{
	bool bRet = m_pNet->SendMsgToAllClientWithOutHead(msgID, msg.c_str(), (uint32_t) msg.length());
	if (!bRet)
	{
		std::ostringstream stream;
		stream << " SendMsgToAllClientWithOutHead failed";
		stream << " msg id " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);
	}

	return bRet;
}

bool NFNetModule::SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex, const NFGUID id)
{
    NFMsg::MsgBase xMsg;
    if (!xData.SerializeToString(xMsg.mutable_msg_data()))
    {
		std::ostringstream stream;
		stream << " SendMsgPB Message to  " << sockIndex;
		stream << " Failed For Serialize of MsgData, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

    NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
    *pPlayerID = NFToPB(id);

    std::string msg;
    if (!xMsg.SerializeToString(&msg))
    {
		std::ostringstream stream;
		stream << " SendMsgPB Message to  " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

	return SendMsgWithOutHead(msgID, msg, sockIndex);
}

bool NFNetModule::SendMsg(const uint16_t msgID, const std::string & xData, const NFSOCK sockIndex)
{
	return SendMsgWithOutHead(msgID, xData, sockIndex);
}

bool NFNetModule::SendMsg(const uint16_t msgID, const std::string & xData, const NFSOCK sockIndex, const NFGUID id)
{
	NFMsg::MsgBase xMsg;
	xMsg.set_msg_data(xData.data(), xData.length());

	NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = NFToPB(id);

	std::string msg;
	if (!xMsg.SerializeToString(&msg))
	{
		std::ostringstream stream;
		stream << " SendMsgPB Message to  " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

		return false;
	}

	return SendMsgWithOutHead(msgID, msg, sockIndex);
}

bool NFNetModule::SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex)
{
	NFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		std::ostringstream stream;
		stream << " SendMsgPB Message to  " << sockIndex;
		stream << " Failed For Serialize of MsgData, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

		return false;
	}

	NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = NFToPB(NFGUID());

	std::string msg;
	if (!xMsg.SerializeToString(&msg))
	{
		std::ostringstream stream;
		stream << " SendMsgPB Message to  " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

		return false;
	}

	SendMsgWithOutHead(msgID, msg, sockIndex);

	return true;
}

bool NFNetModule::SendMsgPBToAllClient(const uint16_t msgID, const google::protobuf::Message& xData)
{
    NFMsg::MsgBase xMsg;
    if (!xData.SerializeToString(xMsg.mutable_msg_data()))
    {
		std::ostringstream stream;
		stream << " SendMsgPBToAllClient";
		stream << " Failed For Serialize of MsgData, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

    std::string msg;
    if (!xMsg.SerializeToString(&msg))
    {
		std::ostringstream stream;
		stream << " SendMsgPBToAllClient";
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

    return SendMsgToAllClientWithOutHead(msgID, msg);
}

bool NFNetModule::SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex, const std::vector<NFGUID>* pClientIDList)
{
    if (!m_pNet)
    {
		std::ostringstream stream;
		stream << " m_pNet SendMsgPB faailed fd " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

    NFMsg::MsgBase xMsg;
    if (!xData.SerializeToString(xMsg.mutable_msg_data()))
    {
		std::ostringstream stream;
		stream << " SendMsgPB faailed fd " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }


    NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
    *pPlayerID = NFToPB(NFGUID());
    if (pClientIDList)
    {
        for (int i = 0; i < pClientIDList->size(); ++i)
        {
            const NFGUID& ClientID = (*pClientIDList)[i];

            NFMsg::Ident* pData = xMsg.add_player_client_list();
            if (pData)
            {
                *pData = NFToPB(ClientID);
            }
        }
    }

    std::string msg;
    if (!xMsg.SerializeToString(&msg))
    {
		std::ostringstream stream;
		stream << " SendMsgPB faailed fd " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

    return SendMsgWithOutHead(msgID, msg, sockIndex);
}

bool NFNetModule::SendMsgPB(const uint16_t msgID, const std::string& strData, const NFSOCK sockIndex, const std::vector<NFGUID>* pClientIDList)
{
    if (!m_pNet)
    {
		std::ostringstream stream;
		stream << " SendMsgPB NULL Of Net faailed fd " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

    NFMsg::MsgBase xMsg;
    xMsg.set_msg_data(strData.data(), strData.length());

    NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
    *pPlayerID = NFToPB(NFGUID());
    if (pClientIDList)
    {
        for (int i = 0; i < pClientIDList->size(); ++i)
        {
            const NFGUID& ClientID = (*pClientIDList)[i];

            NFMsg::Ident* pData = xMsg.add_player_client_list();
            if (pData)
            {
                *pData = NFToPB(ClientID);
            }
        }
    }

    std::string msg;
    if (!xMsg.SerializeToString(&msg))
    {
		std::ostringstream stream;
		stream << " SendMsgPB failed fd " << sockIndex;
		stream << " Failed For Serialize of MsgBase, MessageID " << msgID;
		m_pLogModule->LogError(stream, __FUNCTION__, __LINE__);

        return false;
    }

    return SendMsgWithOutHead(msgID, msg, sockIndex);
}

NFINet* NFNetModule::GetNet()
{
    return m_pNet;
}

void NFNetModule::OnReceiveNetPack(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	//m_pLogModule->LogInfo(pPluginManager->GetAppName() + std::to_string(pPluginManager->GetAppID()) + " NFNetModule::OnReceiveNetPack " + std::to_string(msgID), __FILE__, __LINE__);

	NFPerformance performance;

#if NF_PLATFORM != NF_PLATFORM_WIN
	NF_CRASH_TRY
#endif

    std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>>::iterator it = mxReceiveCallBack.find(msgID);
    if (mxReceiveCallBack.end() != it)
    {
		std::list<NET_RECEIVE_FUNCTOR_PTR>& xFunList = it->second;
		for (std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator itList = xFunList.begin(); itList != xFunList.end(); ++itList)
		{
			NET_RECEIVE_FUNCTOR_PTR& pFunPtr = *itList;
			NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();

			pFunc->operator()(sockIndex, msgID, msg, len);
		}
    } 
	else
    {
        for (std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator itList = mxCallBackList.begin(); itList != mxCallBackList.end(); ++itList)
        {
            NET_RECEIVE_FUNCTOR_PTR& pFunPtr = *itList;
            NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();

            pFunc->operator()(sockIndex, msgID, msg, len);
        }
    }

#if NF_PLATFORM != NF_PLATFORM_WIN
	NF_CRASH_END
#endif
/*
	if (performance.CheckTimePoint(5))
	{
		std::ostringstream os;
		os << "---------------net module performance problem------------------- ";
		os << performance.TimeScope();
		os << "---------- MsgID: ";
		os << msgID;
		m_pLogModule->LogWarning(NFGUID(0, msgID), os, __FUNCTION__, __LINE__);
	}
 */
}

void NFNetModule::OnSocketNetEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
    for (std::list<NET_EVENT_FUNCTOR_PTR>::iterator it = mxEventCallBackList.begin();
         it != mxEventCallBackList.end(); ++it)
    {
        NET_EVENT_FUNCTOR_PTR& pFunPtr = *it;
        NET_EVENT_FUNCTOR* pFunc = pFunPtr.get();
        pFunc->operator()(sockIndex, eEvent, pNet);
    }
}

void NFNetModule::KeepAlive()
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

    NFMsg::ServerHeartBeat xMsg;
    xMsg.set_count(0);

    SendMsgPB(NFMsg::EGameMsgID::STS_HEART_BEAT, xMsg, 0);

}