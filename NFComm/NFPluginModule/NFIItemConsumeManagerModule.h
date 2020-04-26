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

#ifndef NFI_ITEM_CONSUME_MANAGER_MODULE_H
#define NFI_ITEM_CONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIItemConsumeProcessModule
		: public NFIModule
{
public:

	// > 0, error code
	virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID, const NFVector3& vector) = 0;

	//> 0, error code
	virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID, const NFVector3& vector) = 0;

};

class NFIItemConsumeManagerModule
    : public NFIModule
{
public:
	virtual bool SetConsumeModule(const int itemType, NFIItemConsumeProcessModule* pModule) = 0;
    virtual bool SetConsumeModule(const int itemType, const int itemSubType, NFIItemConsumeProcessModule* pModule) = 0;

    virtual NFIItemConsumeProcessModule* GetConsumeModule(const int itemType) = 0;
	virtual NFIItemConsumeProcessModule* GetConsumeModule(const int itemType, const int itemSubType) = 0;
};

#endif
