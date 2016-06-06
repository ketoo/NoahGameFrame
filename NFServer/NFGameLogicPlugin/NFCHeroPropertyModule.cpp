#include "NFCHeroPropertyModule.h"

bool NFCHeroPropertyModule::Init()
{
	return true;
}

bool NFCHeroPropertyModule::Shut()
{
	return true;
}

bool NFCHeroPropertyModule::Execute()
{
	return true;
}

bool NFCHeroPropertyModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
	m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pGameServerNet_ServerModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pElementInfoModule);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCHeroPropertyModule::OnObjectClassEvent);




	return true;

}

bool NFCHeroPropertyModule::BeforeShut()
{
	return true;
}

int NFCHeroPropertyModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	switch (eClassEvent)
	{
	case CLASS_OBJECT_EVENT::COE_CREATE_FINISH:
	{
		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::R_PlayerHero(), this, &NFCHeroPropertyModule::OnObjectHeroRecordEvent);
	}
	break;
	default:
		break;
	}

	return 0;
}


int NFCHeroPropertyModule::OnObjectHeroRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	std::ostringstream stream;
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
	if (nullptr == pHeroRecord)
	{
		return 0;
	}

	switch (xEventData.nOpType)
	{
	case NFIRecord::RecordOptype::Add:
	{
		NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_HeroPropertyValue());
		if (nullptr == pHeroPropertyRecord.get())
		{
			return false;
		}

		const NFGUID& xHeroGUID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::PlayerHero_GUID);
		const std::string& strConfigID = pHeroRecord->GetString(xEventData.nRow, NFrame::Player::PlayerHero::PlayerHero_ConfigID);
		const std::string& strEffectData = m_pElementInfoModule->GetPropertyString(strConfigID, NFrame::NPC::EffectData());

		NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pElementInfoModule->GetPropertyManager(strEffectData);

		NFCDataList xDataList = pHeroPropertyRecord->GetInitData();
		xDataList.SetObject(NFrame::Player::HeroPropertyValue_HeroGUID, xHeroGUID);

		pHeroPropertyRecord->AddRow(-1, xDataList);
	}
	break;
	case NFIRecord::RecordOptype::Del:
	{
		NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_HeroPropertyValue());
		if (nullptr == pHeroPropertyRecord.get())
		{
			return false;
		}

		const NFGUID& xHeroGUID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::PlayerHero_GUID);
		//pHeroPropertyRecord->Remove();
	}
	break;
	case NFIRecord::RecordOptype::UpData:
	{
		switch (xEventData.nCol)
		{
		case NFrame::Player::PlayerHero::PlayerHero_Level:
		case NFrame::Player::PlayerHero::PlayerHero_Star:
		case NFrame::Player::PlayerHero::PlayerHero_Equip1:
		case NFrame::Player::PlayerHero::PlayerHero_Equip2:
		case NFrame::Player::PlayerHero::PlayerHero_Equip3:
		case NFrame::Player::PlayerHero::PlayerHero_Equip4:
		case NFrame::Player::PlayerHero::PlayerHero_Equip5:
		case NFrame::Player::PlayerHero::PlayerHero_Equip6:
		case NFrame::Player::PlayerHero::PlayerHero_Equip7:
		case NFrame::Player::PlayerHero::PlayerHero_Equip8:
		case NFrame::Player::PlayerHero::PlayerHero_Talent1:
		case NFrame::Player::PlayerHero::PlayerHero_Talent2:
		case NFrame::Player::PlayerHero::PlayerHero_Talent3:
		case NFrame::Player::PlayerHero::PlayerHero_Talent4:
		case NFrame::Player::PlayerHero::PlayerHero_Talent5:
		case NFrame::Player::PlayerHero::PlayerHero_Talent6:
		case NFrame::Player::PlayerHero::PlayerHero_Talent7:
		case NFrame::Player::PlayerHero::PlayerHero_Talent8:
		{
			const NFGUID& xHeroGUID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::PlayerHero_GUID);

			OnHeroPropertyUpdate(self, xHeroGUID);
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}

	return 0;
}

int NFCHeroPropertyModule::CalHeroPropertySelf(const NFGUID & self, const NFGUID & xHeroID, NFCDataList& xPpropertyManager)
{
	return 0;
}
int NFCHeroPropertyModule::CalHeroPropertyTalent(const NFGUID & self, const NFGUID & xHeroID, NFCDataList& xPpropertyManager)
{
	return 0;
}
int NFCHeroPropertyModule::CalHeroPropertyEquip(const NFGUID & self, const NFGUID & xHeroID, NFCDataList& xPpropertyManager)
{
	return 0;
}
int NFCHeroPropertyModule::OnHeroPropertyUpdate(const NFGUID & self, const NFGUID & xHeroID)
{
	//EQUIP
	//talent
	//hero property
	//buff
	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_HeroPropertyValue());
	if (nullptr == pHeroPropertyRecord.get())
	{
		return false;
	}

	NFCDataList xDataList = pHeroPropertyRecord->GetInitData();

	NF_SHARE_PTR<NFIPropertyManager> xPropertypProperty;

	CalHeroPropertySelf(self, xHeroID, xDataList);
	CalHeroPropertyTalent(self, xHeroID, xDataList);
	CalHeroPropertyEquip(self, xHeroID, xDataList);

	xDataList.SetObject(NFrame::Player::HeroPropertyValue_HeroGUID, xHeroID);

	pHeroPropertyRecord->AddRow(-1, xDataList);

	return 0;
}
