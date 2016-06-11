// -------------------------------------------------------------------------
//    @FileName      :    NFCSLGShopModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-10-05
//    @Module           :    NFCSLGShopModule
//
// -------------------------------------------------------------------------

#include "NFCSLGShopModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFSLGDefine.pb.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCSLGShopModule::Init()
{
    

	
    return true;
}


bool NFCSLGShopModule::Shut()
{
    return true;
}

bool NFCSLGShopModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCSLGShopModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pSLGBuildingModule = pPluginManager->FindModule<NFISLGBuildingModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_BUY_FORM_SHOP, this, &NFCSLGShopModule::OnSLGClienBuyItem))
	{
		//Log
	}

    return true;
}

bool NFCSLGShopModule::ReqBuyItem(const NFGUID& self, const std::string& strID, float fX, float fY, float fZ)
{
    if (!m_pElementModule->ExistElement(strID))
    {
        return false;
    }

    int nNeedLevel = m_pElementModule->GetPropertyInt(strID, "Level");
    if (m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()) < nNeedLevel)
    {
        return false;
    }

    //¿Û³ý»õ±Ò
    int nGold = m_pElementModule->GetPropertyInt(strID, "Gold");
    if (!m_pPropertyModule->ConsumeMoney(self, nGold))
    {
        return false;
    }

//     int nSteel = m_pElementModule->GetPropertyInt(strID, "Steel");
//     int nStone = m_pElementModule->GetPropertyInt(strID, "Stone");

    int nDiamond = m_pElementModule->GetPropertyInt(strID, "Diamond");
    if (!m_pPropertyModule->ConsumeDiamond(self, nGold))
    {
        return false;
    }

	const std::string strItem = m_pElementModule->GetPropertyString(strID, "ItemID");
	if (!m_pElementModule->ExistElement(strItem))
	{
		return false;
	}

    const int nShopType = m_pElementModule->GetPropertyInt(strID, "Type");
    switch (nShopType)
    {
        case NFMsg::EShopType::EST_GOLD:
        case NFMsg::EShopType::EST_DIAMOND:
        case NFMsg::EShopType::EST_SP:
        {
            m_pPackModule->CreateItem(self, strItem, 1);
        }
        break;
        case NFMsg::EShopType::EST_BUILDING:
        {
            m_pSLGBuildingModule->AddBuilding(self, strItem, fX, fY, fZ);
        }
        break;
        default:
        {
            const int nItemType = m_pElementModule->GetPropertyInt(strItem, "Type");
            if (nItemType == NFMsg::EItemType::EIT_EQUIP)
            {
                m_pPackModule->CreateEquip(self, strItem);
            }
            else
            {
                m_pPackModule->CreateItem(self, strItem, 1);
            }
        }
    }

	return true;
}

void NFCSLGShopModule::OnSLGClienBuyItem(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckBuyObjectFormShop);

	const std::string strItemID = xMsg.config_id();
	const float fX = xMsg.x();
	const float fY = xMsg.y();
	const float fZ = xMsg.z();
	ReqBuyItem(nPlayerID, strItemID, fX, fY, fZ);
}