// -------------------------------------------------------------------------
//    @FileName			:    NFCShopModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCShopModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCShopModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCShopModule::Init()
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

bool NFCShopModule::AfterInit()
{
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_BUY_FORM_SHOP, this, &NFCShopModule::OnClienBuyItem))
	{
		//Log
	}
	return true;
}

void NFCShopModule::OnClienBuyItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	/*
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckBuyObjectFormShop);

	const std::string& strShopItemID = xMsg.config_id();
	NFVector3 v;
	v.SetX(xMsg.x());
	v.SetY(xMsg.y());
	v.SetZ(xMsg.z());

	ReqBuyItem(nPlayerID, strShopItemID, v);
	*/
}

bool NFCShopModule::ReqBuyItem(const NFGUID & self, const std::string & strShopItemID)
{
	return false;
}

bool NFCShopModule::ReqBuyItem(const NFGUID & self, const std::string & strShopItemID, const NFVector3 & v)
{
	if (!m_pElementModule->ExistElement(strShopItemID))
	{
		return false;
	}

	const int nNeedLevel = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Level());
	if (m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()) < nNeedLevel)
	{
		return false;
	}

	const std::string& strItem = m_pElementModule->GetPropertyString(strShopItemID, NFrame::Shop::ItemID());
	if (!m_pElementModule->ExistElement(strItem))
	{
		return false;
	}

	const int nDiamond = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Diamond());
	const int64_t nGold = m_pElementModule->GetPropertyInt(strShopItemID, NFrame::Shop::Gold());
	const int nStone = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Stone());
	const int nSteel = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Steel());
	
	if (!m_pPropertyModule->EnoughDiamond(self, nDiamond)
		&& !m_pPropertyModule->EnoughGold(self, nGold))
	{
		return false;
	}
	
	if (!m_pPropertyModule->ConsumeDiamond(self, nDiamond)
		&& !m_pPropertyModule->ConsumeGold(self, nGold))
	{
		return false;
	}

	const int nShopType = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Type());
	const int nShopCount = m_pElementModule->GetPropertyInt32(strShopItemID, NFrame::Shop::Count());
	switch (nShopType)
	{
		//EST_BUILDING 	= 1;//
		//EST_GOLD = 2;//
		//EST_ITEM = 3;//
		//EST_EQUIP = 5;//
		//EST_GEM = 6;//
		//EST_CARD = 7;//
		case NFMsg::EShopType::EST_BUILDING:
		{
			//m_pSLGBuildingModule->AddBuilding(self, strItem, fX, fY, fZ);
		}
		break;
		case NFMsg::EShopType::EST_GOLD:
		{
			m_pPropertyModule->AddGold(self, nShopCount);
		}
		break;
		default:
		{
			const int nItemType = m_pElementModule->GetPropertyInt32(strItem, NFrame::Server::Type());
			switch (nItemType)
			{
			case NFMsg::EItemType::EIT_EQUIP:
			case NFMsg::EItemType::EIT_GEM:
			case NFMsg::EItemType::EIT_ITEM:
			case NFMsg::EItemType::EIT_CARD:
			case NFMsg::EItemType::EIT_TOKEN:
				m_pPackModule->CreateEquip(self, strItem);
				break;
			default:
				break;
			}
		}
		break;
	}

	return false;
}

bool NFCShopModule::Shut()
{

	return true;
}

bool NFCShopModule::Execute()
{
	return true;
}
