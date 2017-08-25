// -------------------------------------------------------------------------
//    @FileName				:    NFCItemModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCItemModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCItemModule.h"

bool NFCItemModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>();
	
	return true;
}

bool NFCItemModule::Shut()
{
	return true;
}

bool NFCItemModule::Execute()
{
	
	return true;
}

bool NFCItemModule::AfterInit()
{

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCItemModule::OnClassObjectEvent);
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ITEM_OBJECT, this, &NFCItemModule::OnClientUseItem)) { return false; }


	return true;
}

bool NFCItemModule::UseItem(const NFGUID & self, const std::string & strItemID, const NFGUID & xTargetID)
{
	int nCount = 1;
	if (!m_pElementModule->ExistElement(strItemID) || !m_pKernelModule->GetObject(xTargetID))
	{
		return false;
	}

	if (!ConsumeDataItemProperty(self, strItemID))
	{
		return false;
	}

	if (!m_pPackModule->DeleteItem(self, strItemID, nCount))
	{
		return false;
	}

	NFMsg::EItemType eItemType = (NFMsg::EItemType)m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
	NF_SHARE_PTR<NFIItemConsumeProcessModule> pConsumeProcessModule = m_pItemConsumeManagerModule->GetConsumeModule(eItemType);
	if (!pConsumeProcessModule)
	{
		return false;
	}

	switch (eItemType)
	{
	case NFMsg::EItemType::EIT_CARD:
	case NFMsg::EItemType::EIT_EQUIP:
	case NFMsg::EItemType::EIT_GEM:
	case NFMsg::EItemType::EIT_TOKEN:
	{
		if (pConsumeProcessModule->ConsumeLegal(self, strItemID, NFDataList()) == 0)
		{
			pConsumeProcessModule->ConsumeProcess(self, strItemID, NFDataList());
		}
	}
	break;
	case NFMsg::EItemType::EIT_ITEM:
	{
		NFDataList xTarget;
		xTarget.AddObject(xTargetID);
		xTarget.AddString(strItemID);	//this is Item Config ID
		xTarget.AddInt(nCount);	//this is Item Count to Consume

		if (pConsumeProcessModule->ConsumeLegal(self, strItemID, xTarget) == 0)
		{
			pConsumeProcessModule->ConsumeProcess(self, strItemID, xTarget);
		}
	}
	break;
	break;
	default:
		break;
	}

	/*	AddItemEffectDataProperty(self, xTargetID, strItemID);

	const std::string& strAwardPackID = m_pElementModule->GetPropertyString(strItemID, "AwardData");
	if (!strAwardPackID.empty())
	{
	DoAwardPack( self, strAwardPackID);
	}
	*/
	return false;
}

int NFCItemModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{
		m_pEventModule->AddEventCallBack(self, NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS, this, &NFCItemModule::OnRequireUseItemPosEvent);
	}

	return 0;
}

int NFCItemModule::OnRequireUseItemPosEvent(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& var)
{
	return 0;
}

bool NFCItemModule::CheckConfig()
{
	NF_SHARE_PTR<NFIClass> pLogicCLass = m_pLogicClassModule->GetElement(NFrame::Item::ThisName());
	if (nullptr == pLogicCLass)
	{
		assert(0);
	}

	const std::vector<std::string>& strIdList = pLogicCLass->GetIDList();
	for (int i = 0; i < strIdList.size(); ++i)
	{
		const std::string& strConfigID = strIdList[i];

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager(strConfigID);
		if (!pPropertyManager)
		{
			assert(0);
		}
		int nType = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::ItemType());

		if (nType < 0)
		{
			assert(0);
		}
		int nSubType = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::ItemSubType());
		if (nSubType < 0)
		{
			assert(0);
		}

		int nLevel = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::Level());
		if (nLevel < 0)
		{
			assert(0);
		}

		int nQuality = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::Quality());
		if (nQuality < 0)
		{
			assert(0);
		}

		//int nCoolDown = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::CoolDownTime());
		//if (nCoolDown <= 0)
		//{
		//	assert(0);
		//}

		int nOverlayCount = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::OverlayCount());
		if (nOverlayCount <= 0)
		{
			assert(0);
		}


		int nBuyPrice = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::BuyPrice());
		int nSalePrice = m_pElementModule->GetPropertyInt32(strConfigID, NFrame::Item::SalePrice());

		//if (nSalePrice <= 0 || nBuyPrice <= 0)
		if (nSalePrice < 0 || nBuyPrice < 0)
		{
			assert(0);
		}
	}

	return true;
}

int NFCItemModule::AddItemEffectDataProperty(const NFGUID& self, const NFGUID& xTarget, const std::string& strItemID)
{
	if (strItemID.empty())
	{
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(xTarget);
	if (NULL == pObject)
	{
		//m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////
	NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager(strItemID);
	if (!pPropertyManager)
	{
		return 1;
	}

	NF_SHARE_PTR<NFIProperty> pEffectDataProperty = pPropertyManager->GetElement("EffectData");
	if (!pEffectDataProperty)
	{
		return 1;
	}

	NF_SHARE_PTR<NFIPropertyManager> pEffectDataPropertyManager = m_pElementModule->GetPropertyManager(pEffectDataProperty->GetString());
	if (!pEffectDataPropertyManager)
	{
		return 1;
	}

	NF_SHARE_PTR<NFIProperty> pProperty = pEffectDataPropertyManager->First();
	while (pProperty)
	{
		if (pProperty->GetInt() != 0)
		{
			m_pPropertyModule->CalculatePropertyValue(xTarget, pProperty->GetKey(), NFIPropertyModule::NPG_EQUIP, pProperty->GetInt());
		}

		pProperty = pEffectDataPropertyManager->Next();
	}

	return 0;
}

bool NFCItemModule::ConsumeDataItemProperty(const NFGUID& self, const std::string& strID)
{
	if (strID.empty())
	{
		return false;
	}

	const int nVIPEXP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::VIPEXP());
	const int64_t nEXP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::EXP());
	const int nHP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::HP());
	const int nSP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::SP());
	const int nMP = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::MP());
	const int64_t nGold = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::Gold());
	const int nDiamond = m_pElementModule->GetPropertyInt32(strID, NFrame::ConsumeData::Diamond());

	if (nGold > 0 && !m_pPropertyModule->EnoughGold(self, nGold))
	{
		return false;
	}

	if (nHP > 0 && !m_pPropertyModule->EnoughHP(self, nHP))
	{
		return false;
	}

	if (nDiamond > 0 && !m_pPropertyModule->EnoughDiamond(self, nDiamond))
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

	//////

	if (nGold > 0 && !m_pPropertyModule->ConsumeGold(self, nGold))
	{
		return false;
	}

	if (nHP > 0 && !m_pPropertyModule->ConsumeHP(self, nHP))
	{
		return false;
	}

	if (nDiamond > 0 && !m_pPropertyModule->ConsumeDiamond(self, nDiamond))
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

bool NFCItemModule::DoAwardPack(const NFGUID& self, const std::string& strAwardPack)
{
	std::vector<std::string> xList;
	m_pCommonConfigModule->GetStructItemList(strAwardPack, xList);

	for (int i = 0; i < xList.size(); ++i)
	{
		const std::string& strItemID = xList[i];
		const int nCout = m_pCommonConfigModule->GetAttributeInt(strAwardPack, strItemID, "Count");
		const int nIsHero = m_pCommonConfigModule->GetAttributeInt(strAwardPack, strItemID, "IsHero");
		if (m_pElementModule->ExistElement(strItemID))
		{
			if (nIsHero > 0)
			{
				m_pHeroModule->ActiviteHero(self, strItemID);
				continue;
			}

			const int nItemType = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemType());
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

void NFCItemModule::OnClientUseItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckUseItem);

	const NFGUID& self = NFINetModule::PBToNF(xMsg.user());
	const std::string& strItemID = xMsg.item().item_id();
	const NFGUID xTargetID = NFINetModule::PBToNF(xMsg.targetid());
	const int nCount = xMsg.item().item_count();

	UseItem(nPlayerID, strItemID, xTargetID);
}