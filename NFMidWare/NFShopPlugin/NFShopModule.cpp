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


#include "NFShopModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"


bool NFShopModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	
	return true;
}

bool NFShopModule::AfterInit()
{
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_BUY_FORM_SHOP, this, &NFShopModule::ClientBuyItem)) { return false; }

	return true;
}

void NFShopModule::ClientBuyItem(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    /*
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqBuyItemFromShop);

    const std::string& strShopItemID = xMsg.config_id();
    const int nCount = xMsg.config_id();

	ReqBuyItem(nPlayerID, strShopItemID, nCount);
     */
}

bool NFShopModule::ReqBuyItem(const NFGUID & self, const std::string & strShopItemID, const int nCount)
{
	if (!m_pElementModule->ExistElement(strShopItemID))
	{
		return false;
	}

    if (nCount <= 0)
    {
        return false;
    }

	const int nNeedLevel = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Level());
	if (m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()) < nNeedLevel)
	{
        //TODO SHOW A MESSAGE TO PLAYER
		return false;
	}

	const std::string& strItem = m_pElementModule->GetPropertyString(strShopItemID, NFrame::Shop::ItemID());
	if (!m_pElementModule->ExistElement(strItem))
	{
		return false;
	}

    //TODO  please remove the properties: stone steel and so on

	const int nDiamond = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Diamond());
	const int64_t nGold = m_pElementModule->GetPropertyInt(strShopItemID, NFrame::Shop::Gold());
	//const int nStone = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Stone());
	//const int nSteel = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Steel());

	if (!m_pPropertyModule->EnoughDiamond(self, nDiamond * nCount)
		|| !m_pPropertyModule->EnoughGold(self, nGold * nCount))
	{
		return false;
	}

	if (!m_pPropertyModule->ConsumeDiamond(self, nDiamond * nCount)
		|| !m_pPropertyModule->ConsumeGold(self, nGold * nCount))
	{
		return false;
	}

    //TODO  please remove the shop type in the excel file

    const int nItemType = m_pElementModule->GetPropertyInt32(strItem, NFrame::Server::Type());
    switch (nItemType)
    {
        case NFMsg::EItemType::EIT_EQUIP:
        case NFMsg::EItemType::EIT_GEM:
        case NFMsg::EItemType::EIT_SUPPLY:
        case NFMsg::EItemType::EIT_SCROLL:
            m_pPackModule->CreateItem(self, strItem, nCount);
            break;
        default:
            break;
    }

    return true;
}

bool NFShopModule::Shut()
{

	return true;
}

bool NFShopModule::Execute()
{
	return true;
}
