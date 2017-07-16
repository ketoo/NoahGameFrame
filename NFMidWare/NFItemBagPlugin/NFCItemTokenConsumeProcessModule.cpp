// -------------------------------------------------------------------------
//    @FileName			:   NFCItemEquipConsumeProcessModule.cpp
//    @Author           :   Yu.Tang
//    @Date             :   2016-07-02
//    @Module           :   NFCItemEquipConsumeProcessModule

// -------------------------------------------------------------------------
#include "NFCItemTokenConsumeProcessModule.h"

bool NFCItemTokenConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_TOKEN, this);

	return true;
}

bool NFCItemTokenConsumeProcessModule::AfterInit()
{


	return true;
}

bool NFCItemTokenConsumeProcessModule::Shut()
{
	return true;
}

bool NFCItemTokenConsumeProcessModule::Execute()
{
	return true;
}


int NFCItemTokenConsumeProcessModule::ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pBuild = m_pKernelModule->FindRecord(self, NFrame::Player::R_BuildingList());
	if (nullptr == pBuild)
	{
		return  1;
	}

	const std::string& strBuildingCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());
	if (strBuildingCnfID.empty())
	{
		return 2;
	}

	NFDataList varList;
	if (pBuild->FindString(NFrame::Player::BuildingList::BuildingList_BuildingID, strBuildingCnfID, varList) <= 0)
	{
		return 0;
	}

	return 100;
}

int NFCItemTokenConsumeProcessModule::ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID)
{
	NF_SHARE_PTR<NFIRecord> pBuild = m_pKernelModule->FindRecord(self, NFrame::Player::R_BuildingList());
	if (nullptr == pBuild)
	{
		return  1;
	}

	const int nItemType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::ItemSubType());
	const std::string& strBuildingCnfID = m_pElementModule->GetPropertyString(strItemID, NFrame::Item::Extend());

	const NFVector3 vPos = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::Position());
	const NFGUID xID = m_pKernelModule->CreateGUID();


	NF_SHARE_PTR<NFDataList> xDataList = pBuild->GetInitData();
	xDataList->SetString(NFrame::Player::BuildingList::BuildingList_BuildingID(), strBuildingCnfID);
	xDataList->SetObject(NFrame::Player::BuildingList::BuildingList_BuildingGUID(), xID);
	xDataList->SetVector3(NFrame::Player::BuildingList::BuildingList_Pos(), vPos);

	pBuild->AddRow(-1, *xDataList);

	return 100;
}
