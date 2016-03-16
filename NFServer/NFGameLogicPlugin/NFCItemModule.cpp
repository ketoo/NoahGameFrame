// -------------------------------------------------------------------------
//    @FileName         :    NFCMoveModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCMoveModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCItemModule.h"

bool NFCItemModule::Init()
{
    return true;
}

bool NFCItemModule::Shut()
{
    return true;
}

bool NFCItemModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCItemModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>("NFCItemConsumeManagerModule");
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>( "NFCHeroModule" );
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>( "NFCCommonConfigModule" );
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

	assert( NULL != m_pKernelModule );
	assert( NULL != m_pItemConsumeManagerModule );
	assert( NULL != m_pPackModule );
	assert( NULL != m_pElementInfoModule );
	assert(	NULL != m_pLogicClassModule);
	assert(	NULL != m_pPropertyModule);
	assert(	NULL != m_pHeroModule);
	assert(	NULL != m_pCommonConfigModule);
	assert(	NULL != m_pGameServerNet_ServerModule);

	std::string strPlayerPath = pPluginManager->GetConfigPath();
	strPlayerPath += "NFDataCfg/Ini/Common/AwardPackConfig.xml";
	m_pCommonConfigModule->LoadConfig(strPlayerPath);

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCItemModule::OnClassObjectEvent );
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_ITEM_OBJECT, this, &NFCItemModule::OnClienUseItem)){ return false; }


    return true;
}

int NFCItemModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
        m_pKernelModule->AddEventCallBack( self, NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS, this, &NFCItemModule::OnRequireUseItemPosEvent );
    }

    return 0;
}

int NFCItemModule::OnRequireUseItemPosEvent( const NFGUID& self, const int nEventID, const NFIDataList& var )
{
    return 0;
}

bool NFCItemModule::CheckConfig()
{
	NF_SHARE_PTR<NFILogicClass> pLogicCLass = m_pLogicClassModule->GetElement(NFrame::Item::ThisName());
	if (nullptr == pLogicCLass)
	{
		assert(0);
	}

	NFList<std::string>& configList = pLogicCLass->GetConfigNameList();
	std::string strConfigID;
	bool bRet = configList.First(strConfigID);

	while (bRet)
	{
		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigID);
		if (!pPropertyManager)
		{
			assert(0);
		}
		int nType = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::ItemType());

		if (nType < 0)
		{
			assert(0);
		}
		int nSubType = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::ItemSubType());
		if (nSubType < 0)
		{
			assert(0);
		}
		
		int nLevel = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::Level());
		if (nLevel < 0)
		{
			assert(0);
		}

		int nQuality = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::Quality());
		if (nQuality < 0)
		{
			assert(0);
		}

 		//int nCoolDown = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::CoolDownTime());
 		//if (nCoolDown <= 0)
 		//{
 		//	assert(0);
 		//}

		int nOverlayCount = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::OverlayCount());
		if (nOverlayCount <= 0)
		{
			assert(0);
		}


		int nBuyPrice = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::BuyPrice());
		int nSalePrice = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::Item::SalePrice());
		
		if (nSalePrice < 0 || nBuyPrice < 0)
		{
			assert(0);
		}

		 bRet = configList.Next(strConfigID);
	}
	return true;
}

int NFCItemModule::AddItemEffectDataProperty( const NFGUID& self, const NFGUID& xTarget, const std::string& strItemID)
{
    if (strItemID.empty())
    {
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( xTarget );
    if ( NULL == pObject )
    {
        //m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager(strItemID);
    if (!pPropertyManager)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIProperty> pEffectDataProperty = pPropertyManager->GetElement("EffectData");
    if (!pEffectDataProperty)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIPropertyManager> pEffectDataPropertyManager = m_pElementInfoModule->GetPropertyManager(pEffectDataProperty->GetString());
    if (!pEffectDataPropertyManager)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIProperty> pProperty = pEffectDataPropertyManager->First();
    while (pProperty)
    {
        if (pProperty->GetInt() != 0)
        {
            m_pPropertyModule->AddPropertyValue( xTarget, pProperty->GetKey(), NFIPropertyModule::NPG_EQUIP, pProperty->GetInt());
        }

        pProperty = pEffectDataPropertyManager->Next();
    }

    return 0;
}

bool NFCItemModule::ConsumeDataItemProperty( const NFGUID& self, const std::string& strID )
{
    if (strID.empty())
    {
        return false;
    }

    const int nVIPEXP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::VIPEXP());
    const int nEXP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::EXP());
    const int nHP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::HP());
    const int nSP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::SP());
    const int nMP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::MP());
    const int nGold = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::Gold());
    const int nMoney = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::Money());
   
    if (nGold > 0 && !m_pPropertyModule->EnoughDiamond(self, nGold))
    {
        return false;
    }

    if (nHP > 0 && !m_pPropertyModule->EnoughHP(self, nHP))
    {
        return false;
    }

    if (nMoney > 0 && !m_pPropertyModule->EnoughMoney(self, nMoney))
    {
        return false;
    }

    if (nMP > 0 && !m_pPropertyModule->EnoughMP(self, nMP))
    {
        return false;
    }

    if (nSP > 0 && !m_pPropertyModule->EnoughSP(self, nSP))
    {
        return false;
    }

    if (nHP > 0 && !m_pPropertyModule->ConsumeHP(self, nHP))
    {
        return false;
    }

    if (nMoney > 0 && !m_pPropertyModule->ConsumeMoney(self, nMoney))
    {
        return false;
    }

    if (nMP > 0 && !m_pPropertyModule->ConsumeMP(self, nMP))
    {
        return false;
    }

    if (nSP > 0 && !m_pPropertyModule->ConsumeSP(self, nSP))
    {
        return false;
    }

    return true;
}
  
bool NFCItemModule::DoAwardPack( const NFGUID& self, const std::string& strAwardPack )
{
    std::vector<std::string> xList;
    m_pCommonConfigModule->GetStructItemList(strAwardPack, xList);

    for (int i = 0; i < xList.size(); ++i)
    {
        const std::string& strItemID = xList[i];
        const int nCout = m_pCommonConfigModule->GetAttributeInt(strAwardPack, strItemID, "Count");
        const int nIsHero = m_pCommonConfigModule->GetAttributeInt(strAwardPack, strItemID, "IsHero");
        if (m_pElementInfoModule->ExistElement(strItemID))
        {
            if (nIsHero > 0)
            {
                m_pHeroModule->AddHero(self, strItemID);
                continue;
            }

            const int nItemType = m_pElementInfoModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
            switch (nItemType)
            {
            case NFMsg::EIT_EQUIP:
                {
                    m_pPackModule->CreateEquip(self, strItemID);
                }
                break;
            default:
                m_pPackModule->CreateItem(self, strItemID, nCout);
                break;
            }
        }
    }

    return true;
}

void NFCItemModule::OnClienUseItem(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckUseItem);

	const NFGUID& self = NFINetModule::PBToNF(xMsg.user());
	const std::string strItemID = xMsg.item().item_id();
	const NFGUID xTargetID = NFINetModule::PBToNF(xMsg.targetid());

	if(!m_pElementInfoModule->ExistElement(strItemID) || !m_pKernelModule->GetObject(xTargetID))
	{
		return;
	}

	NF_SHARE_PTR<NFIRecord> pRecordPack = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagItemList());
	if (nullptr == pRecordPack)
	{
		return;
	}

    if (!ConsumeDataItemProperty(self, strItemID))
    {
        return;
    }

    if (!m_pPackModule->DeleteItem(self, strItemID, 1))
    {
        return;
    }

	int nItemType = m_pElementInfoModule->GetPropertyInt(strItemID, "ItemType");
	NFIItemConsumeProcessModule* pConsumeProcessModule = m_pItemConsumeManagerModule->GetConsumeModule(nItemType);
	if (pConsumeProcessModule)
	{
		if (pConsumeProcessModule->ConsumeLegal(self, strItemID, xTargetID) > 0)
		{
			pConsumeProcessModule->ConsumeProcess(self, strItemID, xTargetID);
		}
	}

	AddItemEffectDataProperty(self, xTargetID, strItemID);

	const std::string& strAwardPackID = m_pElementInfoModule->GetPropertyString(strItemID, "AwardData");
	if (!strAwardPackID.empty())
	{
		DoAwardPack( self, strAwardPackID);
	}
         
}