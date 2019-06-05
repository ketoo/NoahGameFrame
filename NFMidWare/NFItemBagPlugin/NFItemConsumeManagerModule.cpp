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

#include "NFItemConsumeManagerModule.h"
#include "NFItemScrollConsumeProcessModule.h"
#include "NFItemEquipConsumeProcessModule.h"
#include "NFItemGemConsumeProcessModule.h"
#include "NFItemSupplyConsumeProcessModule.h"

bool NFItemConsumeManagerModule::Init()
{
	ResgisterConsumeModule(NFMsg::EItemType::EIT_SCROLL, NF_SHARE_PTR<NFIItemConsumeocessModule>(NF_NEW NFItemScrollConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_EQUIP, NF_SHARE_PTR<NFIItemConsumeocessModule>(NF_NEW NFItemEquipConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_GEM, NF_SHARE_PTR<NFIItemConsumeocessModule>(NF_NEW NFItemGemConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_SUPPLY, NF_SHARE_PTR<NFIItemConsumeocessModule>(NF_NEW NFItemSupplyConsumeProcessModule(pPluginManager)));

	for (NF_SHARE_PTR<NFIItemConsumeocessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Init();
	}

    return true;
}

bool NFItemConsumeManagerModule::Shut()
{
	for (NF_SHARE_PTR<NFIItemConsumeocessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Shut();
	}

    return true;
}

bool NFItemConsumeManagerModule::Execute()
{
	for (NF_SHARE_PTR<NFIItemConsumeocessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Execute();
	}
    return true;
}

bool NFItemConsumeManagerModule::AfterInit()
{
	for (NF_SHARE_PTR<NFIItemConsumeocessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->AfterInit();
	}

    return true;
}

bool NFItemConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NF_SHARE_PTR<NFIItemConsumeocessModule> pModule )
{
	if (ExistElement(nModuleType))
	{
		assert(0);
		return false;
	}

    return AddElement( nModuleType, pModule );
}

NF_SHARE_PTR<NFIItemConsumeocessModule> NFItemConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
