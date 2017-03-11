// -------------------------------------------------------------------------
//    @FileName			:    NFCUserGiftModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCUserGiftModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCUserGiftModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCUserGiftModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	
	return true;
}

bool NFCUserGiftModule::AfterInit()
{
	std::vector<std::string> xGiftItemList = m_pElementModule->GetListByProperty(NFrame::Item::ThisName(), NFrame::Item::ItemType(), NFMsg::EItemType::EIT_GIFT_PACK);
	for (int i = 0; i < xGiftItemList.size(); ++i)
	{
		const std::string& strItemID = xGiftItemList[i];
		int nLevel = m_pElementModule->GetPropertyInt(strItemID, NFrame::Item::Level());

		NF_SHARE_PTR<std::vector<std::string>> xItemList = mxGiftMap.GetElement(nLevel);
		if (!xItemList)
		{
			xItemList = NF_SHARE_PTR<std::vector<std::string>>(NF_NEW std::vector<std::string>());
			mxGiftMap.AddElement(nLevel, xItemList);
		}

		xItemList->push_back(strItemID);
	}

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCUserGiftModule::OnObjectClassEvent);


	return true;
}

bool NFCUserGiftModule::CheckConfig()
{

	return true;
}

int NFCUserGiftModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (eClassEvent == CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFCUserGiftModule::OnLevelPropertyEvent);
	}

	return 0;
}
int NFCUserGiftModule::OnLevelPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
	int nNewLevel = newVar.GetInt();
	NF_SHARE_PTR<std::vector<std::string>> xItemList = mxGiftMap.GetElement(nNewLevel);
	if (xItemList)
	{
		for (int i = 0; i < xItemList->size(); ++i)
		{
			const std::string& strGiftItemID = xItemList->at(i);
			//
			NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pElementModule->GetPropertyManager(strGiftItemID);
			NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->GetElement(NFrame::Item::AwardData());
			const NF_SHARE_PTR<NFMapEx<std::string, std::string>> xEmbeddedMap =  xProperty->GetEmbeddedMap();
			if (xEmbeddedMap)
			{
				std::string strItemID;
				NF_SHARE_PTR<std::string> strItemCount = xEmbeddedMap->First(strItemID);
				for (strItemCount; strItemCount; strItemCount = xEmbeddedMap->Next(strItemID))
				{
					int nCount = lexical_cast<int>(*strItemCount);

					m_pPackModule->CreateItem(self, strItemID, nCount);
				}
			}
		}
	}

	return 0;
}

bool NFCUserGiftModule::Shut()
{

	return true;
}

bool NFCUserGiftModule::Execute()
{
	return true;
}
