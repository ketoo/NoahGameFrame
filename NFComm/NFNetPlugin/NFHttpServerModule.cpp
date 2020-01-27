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

#include "NFHttpServer.h"
#include "NFHttpServerModule.h"

NFHttpServerModule::NFHttpServerModule(NFIPluginManager* p)
{
    pPluginManager = p;
    m_pHttpServer = NULL;
}

NFHttpServerModule::~NFHttpServerModule()
{
    if (m_pHttpServer)
    {
        delete m_pHttpServer;
        m_pHttpServer = NULL;
    }
}

bool NFHttpServerModule::Execute()
{
    if (m_pHttpServer)
    {
        m_pHttpServer->Execute();
    }

    return true;
}

int NFHttpServerModule::InitServer(const unsigned short nPort)
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pHttpServer = new NFHttpServer(this, &NFHttpServerModule::OnReceiveNetPack, &NFHttpServerModule::OnFilterPack);
    std::cout << "Open http port:" << nPort << std::endl;

    return m_pHttpServer->InitServer(nPort);
}

bool NFHttpServerModule::OnReceiveNetPack(NF_SHARE_PTR<NFHttpRequest> req)
{
	if (req == nullptr)
	{
		return false;
	}

	NFPerformance performance;
	auto it = mMsgCBMap.GetElement(req->type);
	if (it)
	{
		auto itPath = it->find(req->path);
		if (it->end() != itPath)
		{
			HTTP_RECEIVE_FUNCTOR_PTR& pFunPtr = itPath->second;
			HTTP_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
			try
			{
				pFunc->operator()(req);
			}
			catch (const std::exception&)
			{
				ResponseMsg(req, "unknow error", NFWebStatus::WEB_INTER_ERROR);
			}
			return true;
		}
	}

	if (performance.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "---------------net module performance problem------------------- ";
		os << performance.TimeScope();
		os << "---------- ";
		os << req->path;
		m_pLogModule->LogWarning(NFGUID(), os, __FUNCTION__, __LINE__);
	}

	return ResponseMsg(req, "", NFWebStatus::WEB_ERROR);
}

NFWebStatus NFHttpServerModule::OnFilterPack(NF_SHARE_PTR<NFHttpRequest> req)
{
	if (req == nullptr)
	{
		return NFWebStatus::WEB_INTER_ERROR;
	}

	auto itPath = mMsgFliterMap.find(req->path);
	if (mMsgFliterMap.end() != itPath)
	{
		HTTP_FILTER_FUNCTOR_PTR& pFunPtr = itPath->second;
		HTTP_FILTER_FUNCTOR* pFunc = pFunPtr.get();
		return pFunc->operator()(req);
	}

	return NFWebStatus::WEB_OK;
}

bool NFHttpServerModule::AddMsgCB(const std::string& strCommand, const NFHttpType eRequestType, const HTTP_RECEIVE_FUNCTOR_PTR& cb)
{
	auto it = mMsgCBMap.GetElement(eRequestType);
	if (!it)
	{
		mMsgCBMap.AddElement(eRequestType, NF_SHARE_PTR<std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>>(NF_NEW std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>()));
	}

	it = mMsgCBMap.GetElement(eRequestType);
	auto itPath = it->find(strCommand);
	if (it->end() == itPath)
	{
		it->insert(std::map<std::string, HTTP_RECEIVE_FUNCTOR_PTR>::value_type(strCommand, cb));

		return true;
	}
	else
	{
		std::cout << eRequestType << " " << strCommand << "" << std::endl;
	}

    return false;
}

bool NFHttpServerModule::AddFilterCB(const std::string& strCommand, const HTTP_FILTER_FUNCTOR_PTR& cb)
{
	auto it = mMsgFliterMap.find(strCommand);
	if (it == mMsgFliterMap.end())
	{
		mMsgFliterMap.insert(std::map<std::string, HTTP_FILTER_FUNCTOR_PTR>::value_type(strCommand, cb));
	}

    return true;
}

bool NFHttpServerModule::ResponseMsg(NF_SHARE_PTR<NFHttpRequest> req, const std::string& strMsg, NFWebStatus code,
                                      const std::string& strReason)
{
    return m_pHttpServer->ResponseMsg(req, strMsg, code, strReason);
}