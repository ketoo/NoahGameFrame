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

#ifndef NF_HTTP_CLIENT_MODULE_H
#define NF_HTTP_CLIENT_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFNetPlugin/NFIHttpClient.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFHttpClientModule
        : public NFIHttpClientModule
{
public:
    NFHttpClientModule(NFIPluginManager* p);

    virtual ~NFHttpClientModule();

	virtual bool Init();

	virtual bool AfterInit();

    virtual bool Execute();

    virtual bool Shut();

	//actually, sync post method
	//NFWebStatus
	virtual int Post(const std::string& strUri, const std::string& strData, std::string& strResData);
	virtual int Post(const std::string& strUri, const std::map<std::string, std::string>& xHeaders, const std::string& strData, std::string& strResData);

	//actually, sync get method
	//NFWebStatus
	virtual int Get(const std::string& strUri, std::string& strResData);
	virtual int Get(const std::string& strUri, const std::map<std::string, std::string>& xHeaders, std::string& strResData);
protected:
    virtual bool DoGet(const std::string& strUri,
                            const std::map<std::string, std::string>& xHeaders,
                            HTTP_RESP_FUNCTOR_PTR pCB);

    virtual bool DoPost(const std::string& strUri,
        const std::map<std::string, std::string>& xHeaders,
        const std::string& strPostData,
        HTTP_RESP_FUNCTOR_PTR pCB, const std::string& strMemo = "");


protected:

	class RespData
	{
	public:
		RespData()
		{
			resp = false;
			time = 0;
			state_code = 0;
		}

		bool resp;
		int time;
		int state_code;
		std::string strRespData;
	};

	void CallBack(const NFGUID id, const int state_code, const std::string& strRespData);

private:
	NFIKernelModule* m_pKernelModule;
	NFIHttpClient* m_pHttpClient;
    std::map<std::string, std::string> m_xDefaultHttpHeaders;
	NFMapEx<NFGUID, RespData> mxRespDataMap;
};

#endif
