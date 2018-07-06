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

#include "NFCItemCardConsumeProcessModule.h"

bool NFCItemCardConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_CARD, this);

    return true;
}

bool NFCItemCardConsumeProcessModule::AfterInit()
{


    return true;
}

bool NFCItemCardConsumeProcessModule::Shut()
{
    return true;
}

bool NFCItemCardConsumeProcessModule::Execute()
{
    return true;
}


int NFCItemCardConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemID, const NFDataList& targetID )
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return  1;
	}

	NFDataList varList;
	if (pHeroRecord->FindString(NFrame::Player::PlayerHero::ConfigID, strItemID, varList) <= 0)
	{
		return 0;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());

    return 100;
}

int NFCItemCardConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemID, const NFDataList& targetID )
{
	NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHero)
	{
		return  1;
	}

	NFDataList varList;
	if (pHero->FindString(NFrame::Player::PlayerHero::ConfigID, strItemID, varList) > 0)
	{
		return 2;
	}

	const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());
	const std::string& strExtendHeroCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());

	NFGUID xHeroID = m_pHeroModule->AddHero(self, strExtendHeroCnfID);
	if (xHeroID.IsNull())
	{
		return 0;
	}

    return 100;
}