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

#include "NFCItemConsumeManagerModule.h"
#include "NFCItemCardConsumeProcessModule.h"
#include "NFCItemEquipConsumeProcessModule.h"
#include "NFCItemGemConsumeProcessModule.h"
#include "NFCItemTokenConsumeProcessModule.h"
#include "NFCItemItemConsumeProcessModule.h"

bool NFCItemConsumeManagerModule::Init()
{
	ResgisterConsumeModule(NFMsg::EItemType::EIT_CARD, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemCardConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_EQUIP, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemEquipConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_GEM, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemGemConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_TOKEN, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemTokenConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_ITEM, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemItemConsumeProcessModule(pPluginManager)));

	for (NF_SHARE_PTR<NFIItemConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Init();
	}

    return true;
}

bool NFCItemConsumeManagerModule::Shut()
{
	for (NF_SHARE_PTR<NFIItemConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Shut();
	}

    return true;
}

bool NFCItemConsumeManagerModule::Execute()
{
	for (NF_SHARE_PTR<NFIItemConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Execute();
	}
    return true;
}

bool NFCItemConsumeManagerModule::AfterInit()
{
	for (NF_SHARE_PTR<NFIItemConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->AfterInit();
	}

    return true;
}

bool NFCItemConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NF_SHARE_PTR<NFIItemConsumeProcessModule> pModule )
{
	if (ExistElement(nModuleType))
	{
		assert(0);
		return false;
	}

    return AddElement( nModuleType, pModule );
}

NF_SHARE_PTR<NFIItemConsumeProcessModule> NFCItemConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
