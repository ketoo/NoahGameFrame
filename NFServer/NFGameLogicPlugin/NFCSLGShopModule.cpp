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

bool NFCSLGShopModule::Init()
{
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

	assert( NULL != m_pKernelModule );

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_BUY_FORM_SHOP, this, &NFCSLGShopModule::OnSLGClienBuyItem))
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
    //位置呢
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
    //扣除货币
    int nGold = m_pElementInfoModule->GetPropertyInt(strID, "Gold");
    if (!m_pPropertyModule->ConsumeMoney(self, nGold))
    {
        //return false;
    }

	const std::string strItem = m_pElementInfoModule->GetPropertyString(strID, "ItemID");
	if (!m_pElementInfoModule->ExistElement(strItem))
	{
		return false;
	}

    //添加进表
    m_pSLGBuildingModule->AddBuilding(self, strItem, fX, fY, fZ);
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