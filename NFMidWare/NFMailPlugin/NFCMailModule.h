/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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


#ifndef NFC_MAIL_MODULE_H
#define NFC_MAIL_MODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIMailModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
////////////////////////////////////////////////////////////////////////////



class NFCMailModule
    : public NFIMailModule
{
public:
	explicit NFCMailModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

	virtual bool Init() override;
	virtual bool Shut() override;
	virtual bool Execute() override;

	virtual bool AfterInit() override;

public:
	virtual bool SendMail(const NFGUID& reciever, const int nMailType, const NFDataList& xItemList, const std::string& strNailContent = NULL_STR) { return false; }
	virtual int OnDrawMail(const NFGUID& self, const NFDataList& var) { return 0; }
	virtual int OnOpenMail(const NFGUID& self, const NFDataList& var) { return 0; }
	virtual int OnDeleteMail(const NFGUID& self, const NFDataList& var) { return 0; }

protected:

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIEventModule* m_pEventModule;
	NFISceneAOIModule* m_pSceneAOIModule;
    //////////////////////////////////////////////////////////////////////////
};
#endif
