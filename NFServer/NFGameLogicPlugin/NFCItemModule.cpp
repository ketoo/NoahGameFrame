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
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pItemConsumeManagerModule = dynamic_cast<NFIItemConsumeManagerModule*>( pPluginManager->FindModule( "NFCItemConsumeManagerModule" ) );
    m_pPackModule = dynamic_cast<NFIPackModule*>( pPluginManager->FindModule( "NFCPackModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pItemConsumeManagerModule );
    assert( NULL != m_pPackModule );
    assert( NULL != m_pElementInfoModule );
	assert(	NULL != m_pLogicClassModule);


    mstrPackTableName = "BagItemList";

    return true;
}


bool NFCItemModule::Shut()
{
    return true;
}

bool NFCItemModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //位置呢
    return true;
}

bool NFCItemModule::AfterInit()
{
    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCItemModule::OnClassObjectEvent );
	CheckConfig();
    return true;
}

int NFCItemModule::OnClassObjectEvent( const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
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


int NFCItemModule::OnRequireUseItemEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    //EGameErrorCode errorCode = EGameErrorCode::EGEC_INVALID_ITEM;
    //int nItemRowID = var.IntVal(0);

    //NFIObject* pObejct = m_pKernelModule->GetObject(self);
    //if (pObejct)
    //{
    //  //还有CD表 string float
    //  NFIRecord* pRecordPack = pObejct->GetRecordManager()->GetElement(mstrPackTableName);
    //  if (pRecordPack)
    //  {
    //      const std::string& strConfigIndex = m_pPackModule->GetGridConfigID(self, nItemRowID);
    //      NFIPropertyManager* pPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigIndex);
    //      if (pPropertyManager)
    //      {
    //          NFIProperty* pItemTypeProperty = pPropertyManager->GetElement("ItemType");
    //          if (pItemTypeProperty)
    //          {
    //              //配置表中真的有这么个道具
    //              EGameItemType nItemType = (EGameItemType)pItemTypeProperty->QueryInt();
    //              NFIItemConsumeProcessModule* pConsumeProcessModule = m_pItemConsumeManagerModule->GetConsumeModule(nItemType);
    //              if (pConsumeProcessModule)
    //              {
    //                  NFCValueList valueOther;
    //                  valueOther.Append(var, 1, var.GetCount() - 1);
    //                  if (pConsumeProcessModule->ConsumeLegal(self, nItemRowID, valueOther) > 0)
    //                  {
    //                      if(pConsumeProcessModule->ConsumeSelf(self, nItemRowID) > 0)
    //                      {
    //                          int nResult = pConsumeProcessModule->ConsumeProcess(self, strConfigIndex, valueOther);

    //                          NFIProperty* pGUIDProperty = pPropertyManager->GetElement("GUID"); // xxx
    //                          NFIDENTID nGUID = pGUIDProperty->QueryObject();

    //                          //结果事件--无论失败或者是成功，都会发下去--当然使用结果只对使用者下发--成果的结果，还得对被施放的人发
    //                          NFCValueList valueResult;
    //                          valueResult.AddObject(nGUID);
    //                          valueResult.AddInt(nResult);
    //                          valueResult.Append(valueOther, 0, valueOther.GetCount());

    //                          m_pEventProcessModule->DoEvent(self, NFED_ON_CLIENT_USE_ITEM_RESULT, valueResult);

    //                          return 0;
    //                      }
    //                  }
    //              }
    //          }
    //
    //      }
    //  }
    //}
    //else
    //{
    //  //封死你Y的号
    //}

    //NFCValueList valueError;
    //valueError.AddInt((int)errorCode);
    //m_pEventProcessModule->DoEvent(self, NFED_ON_GENERAL_MESSAGE, valueError);

    return 0;
}

int NFCItemModule::OnRequireUseItemPosEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{


    return 0;
}

bool NFCItemModule::CheckConfig()
{
	NFILogicClass* pLogicCLass = m_pLogicClassModule->GetElement("Item");
	if (!pLogicCLass)
	{
		assert(0);
	}

	NFList<std::string>& configList = pLogicCLass->GetConfigNameList();
	std::string strConfigID;
	bool bRet = configList.First(strConfigID);

	while (bRet)
	{
		NFIPropertyManager* pPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigID);
		if (!pPropertyManager)
		{
			assert(0);
		}
		int nType = m_pElementInfoModule->QueryPropertyInt(strConfigID, "ItemType");

		if (nType < 0)
		{
			assert(0);
		}
		int nSubType = m_pElementInfoModule->QueryPropertyInt(strConfigID, "ItemSubType");
		if (nSubType < 0)
		{
			assert(0);
		}
		
		int nLevel = m_pElementInfoModule->QueryPropertyInt(strConfigID, "Level");
		if (nLevel < 0)
		{
			assert(0);
		}

		int nQuality = m_pElementInfoModule->QueryPropertyInt(strConfigID, "Quality");
		if (nQuality < 0)
		{
			assert(0);
		}

// 		int nCoolDown = m_pElementInfoModule->QueryPropertyInt(strConfigID, "CoolDwnTime");
// 		if (nCoolDown <= 0)
// 		{
// 			assert(0);
// 		}

		int nOverlayCount = m_pElementInfoModule->QueryPropertyInt(strConfigID, "OverlayCount");
		if (nOverlayCount <= 0)
		{
			assert(0);
		}


		int nBuyPrice = m_pElementInfoModule->QueryPropertyInt(strConfigID, "BuyPrice");
		int nSalePrice = m_pElementInfoModule->QueryPropertyInt(strConfigID, "SalePrice");
		
		if (nSalePrice < 0 || nBuyPrice < 0)
		{
			assert(0);
		}

		 bRet = configList.Next(strConfigID);
	}
	return true;
}
