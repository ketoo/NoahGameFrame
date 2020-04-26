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

#include "NFItemConsumeManagerModule.h"

bool NFItemConsumeManagerModule::Init()
{

    return true;
}

bool NFItemConsumeManagerModule::Shut()
{

    return true;
}

bool NFItemConsumeManagerModule::Execute()
{

    return true;
}

bool NFItemConsumeManagerModule::AfterInit()
{

    return true;
}

bool NFItemConsumeManagerModule::SetConsumeModule(const int itemType, const int itemSubType, NFIItemConsumeProcessModule* pModule)
{
	mItemConsumeModule[NFGUID(itemType, itemSubType)] = pModule;

	return true;
}

NFIItemConsumeProcessModule* NFItemConsumeManagerModule::GetConsumeModule(const int itemType, const int itemSubType)
{
	auto it = mItemConsumeModule.find(NFGUID(itemType, itemSubType));
	if (it != mItemConsumeModule.end())
	{
		return it->second;
	}
	else
	{
		it = mItemConsumeModule.find(NFGUID(itemType, 0));
		if (it != mItemConsumeModule.end())
		{
			return it->second;
		}
	}

	return nullptr;
}

bool NFItemConsumeManagerModule::SetConsumeModule(const int itemType, NFIItemConsumeProcessModule* pModule)
{
	mItemConsumeModule[NFGUID(itemType, 0)] = pModule;

	return true;
}

NFIItemConsumeProcessModule* NFItemConsumeManagerModule::GetConsumeModule(const int itemType)
{
	auto it = mItemConsumeModule.find(NFGUID(itemType, 0));
	if (it != mItemConsumeModule.end())
	{
		return it->second;
	}

	return nullptr;
}