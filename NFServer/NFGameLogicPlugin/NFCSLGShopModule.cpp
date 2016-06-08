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
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
    
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pPackModule);

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_BUY_FORM_SHOP, this, &NFCSLGShopModule::OnSLGClienBuyItem))
	{
		//Log
	}

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
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>( "NFCLogModule" );
    m_pSLGBuildingModule = pPluginManager->FindModule<NFISLGBuildingModule>("NFCSLGBuildingModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>("NFCPropertyModule");

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );
    assert(NULL != m_pSLGBuildingModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pPropertyModule);

    return true;
}

bool NFCSLGShopModule::ReqBuyItem(const NFGUID& self, const std::string& strID, float fX, float fY, float fZ)
{
    if (!m_pElementInfoModule->ExistElement(strID))
    {
        return false;
    }

    int nNeedLevel = m_pElementInfoModule->GetPropertyInt(strID, "Level");
    if (m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level()) < nNeedLevel)
    {
        return false;
    }

    //¿Û³ý»õ±Ò
    int nGold = m_pElementInfoModule->GetPropertyInt(strID, "Gold");
    if (!m_pPropertyModule->ConsumeMoney(self, nGold))
    {
        return false;
    }

//     int nSteel = m_pElementInfoModule->GetPropertyInt(strID, "Steel");
//     int nStone = m_pElementInfoModule->GetPropertyInt(strID, "Stone");

    int nDiamond = m_pElementInfoModule->GetPropertyInt(strID, "Diamond");
    if (!m_pPropertyModule->ConsumeDiamond(self, nGold))
    {
        return false;
    }

	const std::string strItem = m_pElementInfoModule->GetPropertyString(strID, "ItemID");
	if (!m_pElementInfoModule->ExistElement(strItem))
	{
		return false;
	}

    const int nShopType = m_pElementInfoModule->GetPropertyInt(strID, "Type");
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
            const int nItemType = m_pElementInfoModule->GetPropertyInt(strItem, "Type");
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