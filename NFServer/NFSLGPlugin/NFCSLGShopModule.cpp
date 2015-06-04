// -------------------------------------------------------------------------
//    @FileName      :    NFCSLGShopModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-10-05
//    @Module           :    NFCSLGShopModule
//
// -------------------------------------------------------------------------

#include "NFCSLGShopModule.h"

bool NFCSLGShopModule::Init()
{


    return true;
}


bool NFCSLGShopModule::Shut()
{
    return true;
}

bool NFCSLGShopModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //位置呢
    return true;
}

bool NFCSLGShopModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );
    m_pSLGBuildingModule = dynamic_cast<NFISLGBuildingModule*>(pPluginManager->FindModule("NFCSLGBuildingModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>(pPluginManager->FindModule("NFCPropertyModule"));

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );
    assert(NULL != m_pSLGBuildingModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pPropertyModule);

    return true;
}

bool NFCSLGShopModule::ReqBuyItem(const NFIDENTID& self, const std::string& strID, float fX, float fY, float fZ)
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