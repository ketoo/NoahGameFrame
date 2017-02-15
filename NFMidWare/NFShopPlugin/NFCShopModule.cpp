// -------------------------------------------------------------------------
//    @FileName			:    NFCShopModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCShopModule
//    @Desc             :

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

void NFCShopModule::OnClienBuyItem(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	/*
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckBuyObjectFormShop);

	const std::string& strItemID = xMsg.config_id();
	NFVector3 v;
	v.SetX(xMsg.x());
	v.SetY(xMsg.y());
	v.SetZ(xMsg.z());

	ReqBuyItem(nPlayerID, strItemID, v);
	*/
}

bool NFCShopModule::ReqBuyItem(const NFGUID & self, const std::string & strID)
{
	return false;
}

bool NFCShopModule::ReqBuyItem(const NFGUID & self, const std::string & strID, const NFVector3 & v)
{
	if (!m_pElementModule->ExistElement(strID))
	{
		return false;
	}

	int nNeedLevel = m_pElementModule->GetPropertyInt(strID, NFrame::Shop::Level());
	if (m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()) < nNeedLevel)
	{
		return false;
	}

	int nDiamond = m_pElementModule->GetPropertyInt(strID, NFrame::Shop::Diamond());
	if (!m_pPropertyModule->ConsumeDiamond(self, nDiamond))
	{
		return false;
	}

	const std::string& strItem = m_pElementModule->GetPropertyString(strID, NFrame::Shop::ItemID());
	if (!m_pElementModule->ExistElement(strItem))
	{
		return false;
	}

	const int nShopType = m_pElementModule->GetPropertyInt(strID, NFrame::Shop::Type());
	switch (nShopType)
	{
	case NFMsg::EShopType::EST_GOLD:
	case NFMsg::EShopType::EST_DIAMOND:
	case NFMsg::EShopType::EST_SP:
	{
		//m_pPackModule->CreateItem(self, strItem, 1);
	}
	break;
	case NFMsg::EShopType::EST_BUILDING:
	{
		//m_pSLGBuildingModule->AddBuilding(self, strItem, fX, fY, fZ);
	}
	break;
	default:
	{
		const int nItemType = m_pElementModule->GetPropertyInt(strItem, NFrame::Server::Type());
		if (nItemType == NFMsg::EItemType::EIT_EQUIP)
		{
			//m_pPackModule->CreateEquip(self, strItem);
		}
		else
		{
			//m_pPackModule->CreateItem(self, strItem, 1);
		}
	}
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
