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
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>( "NFCEventProcessModule" );
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>("NFCItemConsumeManagerModule");
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pItemConsumeManagerModule );
    assert( NULL != m_pPackModule );
    assert( NULL != m_pElementInfoModule );
	assert(	NULL != m_pLogicClassModule);

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
    m_pEventProcessModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCItemModule::OnClassObjectEvent );
	CheckConfig();
    return true;
}

int NFCItemModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
        m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_REQUIRE_USE_ITEM, this, &NFCItemModule::OnRequireUseItemEvent );
        m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS, this, &NFCItemModule::OnRequireUseItemEvent );

    }

    return 0;
}


int NFCItemModule::OnRequireUseItemEvent( const NFGUID& self, const int nEventID, const NFIDataList& var )
{
	if (var.GetCount() != 2
		|| !var.TypeEx(TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
	{
		return 0;
	}
    //EGameErrorCode errorCode = EGameErrorCode::EGEC_INVALID_ITEM;
	const std::string& strItemID = var.String(0);
	const NFGUID xTargetID = var.Object(0);

	if(!m_pElementInfoModule->ExistElement(strItemID)
		|| !m_pKernelModule->GetObject(xTargetID))
	{
		return 0;
	}

    NF_SHARE_PTR<NFIRecord> pRecordPack = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagItemList());
    if (nullptr != pRecordPack)
    {
// 		const bool bRet = m_pPackModule->DeleteItem(self, strItemID, 1);
// 		if (bRet)
// 		{
// 			int nItemType = m_pElementInfoModule->GetPropertyInt(strItemID, "ItemType");
// 			NFIItemConsumeProcessModule* pConsumeProcessModule = m_pItemConsumeManagerModule->GetConsumeModule(nItemType);
// 			if (pConsumeProcessModule)
// 			{
// 				if (pConsumeProcessModule->ConsumeLegal(self, strItemID, xTargetID) > 0)
// 				{
// 					pConsumeProcessModule->ConsumeProcess(self, strItemID, xTargetID);
// 
// 					return 0;
// 				}
// 			}
// 		}
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
