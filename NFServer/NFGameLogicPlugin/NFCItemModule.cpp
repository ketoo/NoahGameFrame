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
    //λ����
    return true;
}

bool NFCItemModule::AfterInit()
{
    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCItemModule::OnClassObjectEvent );
	CheckConfig();
    return true;
}

int NFCItemModule::OnClassObjectEvent( const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
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


int NFCItemModule::OnRequireUseItemEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
    //EGameErrorCode errorCode = EGameErrorCode::EGEC_INVALID_ITEM;
    //int nItemRowID = var.IntVal(0);

    //NFIObject* pObejct = m_pKernelModule->GetObject(self);
    //if (pObejct)
    //{
    //  //����CD�� string float
    //  NFIRecord* pRecordPack = pObejct->GetRecordManager()->GetElement(mstrPackTableName);
    //  if (pRecordPack)
    //  {
    //      const std::string& strConfigIndex = m_pPackModule->GetGridConfigID(self, nItemRowID);
    //      NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigIndex);
    //      if (pPropertyManager)
    //      {
    //          NFIProperty* pItemTypeProperty = pPropertyManager->GetElement("ItemType");
    //          if (pItemTypeProperty)
    //          {
    //              //���ñ����������ô������
    //              EGameItemType nItemType = (EGameItemType)pItemTypeProperty->QueryInt();
    //              NFIItemConsumeProcessModule* pConsumeProcessModule = m_pItemConsumeManagerModule->GetConsumeModule(nItemType);
    //              if (pConsumeProcessModule)
    //              {
    //                  NFCDataList valueOther;
    //                  valueOther.Append(var, 1, var.GetCount() - 1);
    //                  if (pConsumeProcessModule->ConsumeLegal(self, nItemRowID, valueOther) > 0)
    //                  {
    //                      if(pConsumeProcessModule->ConsumeSelf(self, nItemRowID) > 0)
    //                      {
    //                          int nResult = pConsumeProcessModule->ConsumeProcess(self, strConfigIndex, valueOther);

    //                          NFIProperty* pGUIDProperty = pPropertyManager->GetElement("GUID"); // xxx
    //                          NFIDENTID nGUID = pGUIDProperty->QueryObject();

    //                          //����¼�--����ʧ�ܻ����ǳɹ������ᷢ��ȥ--��Ȼʹ�ý��ֻ��ʹ�����·�--�ɹ��Ľ�������öԱ�ʩ�ŵ��˷�
    //                          NFCDataList valueResult;
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
    //  //������Y�ĺ�
    //}

    //NFCDataList valueError;
    //valueError.AddInt((int)errorCode);
    //m_pEventProcessModule->DoEvent(self, NFED_ON_GENERAL_MESSAGE, valueError);

    return 0;
}

int NFCItemModule::OnRequireUseItemPosEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{


    return 0;
}

bool NFCItemModule::CheckConfig()
{
	NF_SHARE_PTR<NFILogicClass> pLogicCLass = m_pLogicClassModule->GetElement("Item");
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
		int nType = m_pElementInfoModule->GetPropertyInt(strConfigID, "ItemType");

		if (nType < 0)
		{
			assert(0);
		}
		int nSubType = m_pElementInfoModule->GetPropertyInt(strConfigID, "ItemSubType");
		if (nSubType < 0)
		{
			assert(0);
		}
		
		int nLevel = m_pElementInfoModule->GetPropertyInt(strConfigID, "Level");
		if (nLevel < 0)
		{
			assert(0);
		}

		int nQuality = m_pElementInfoModule->GetPropertyInt(strConfigID, "Quality");
		if (nQuality < 0)
		{
			assert(0);
		}

// 		int nCoolDown = m_pElementInfoModule->GetPropertyInt(strConfigID, "CoolDwnTime");
// 		if (nCoolDown <= 0)
// 		{
// 			assert(0);
// 		}

		int nOverlayCount = m_pElementInfoModule->GetPropertyInt(strConfigID, "OverlayCount");
		if (nOverlayCount <= 0)
		{
			assert(0);
		}


		int nBuyPrice = m_pElementInfoModule->GetPropertyInt(strConfigID, "BuyPrice");
		int nSalePrice = m_pElementInfoModule->GetPropertyInt(strConfigID, "SalePrice");
		
		if (nSalePrice < 0 || nBuyPrice < 0)
		{
			assert(0);
		}

		 bRet = configList.Next(strConfigID);
	}
	return true;
}
