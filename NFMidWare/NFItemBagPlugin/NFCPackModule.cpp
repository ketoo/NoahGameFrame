// -------------------------------------------------------------------------
//    @FileName				:    NFCPackModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCPackModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCPackModule.h"

bool NFCPackModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}

bool NFCPackModule::Shut()
{
    return true;
}

bool NFCPackModule::Execute()
{
    
    return true;
}

bool NFCPackModule::AfterInit()
{

    return true;
}


const NFGUID& NFCPackModule::CreateEquip( const NFGUID& self, const std::string& strConfigName )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return NULL_OBJECT;
	}

	
	bool bExist = m_pElementModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		return NULL_OBJECT;
	}

	int nItemType = m_pElementModule->GetPropertyInt32(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_EQUIP != nItemType )
	{
		return NULL_OBJECT;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagEquipList::ThisName() );
	if (!pRecord)
	{
		return NULL_OBJECT;
	}

	NFGUID ident = m_pKernelModule->CreateGUID();

	NF_SHARE_PTR<NFDataList> var = pRecord->GetInitData();

	var->SetObject(NFrame::Player::BagEquipList::GUID, ident);
	var->SetString(NFrame::Player::BagEquipList::ConfigID, strConfigName.c_str());
	var->SetInt(NFrame::Player::BagEquipList::Date, pPluginManager->GetNowTime());


	int nAddRow = pRecord->AddRow(-1, *var);
	if (nAddRow > 0)
	{
		return pRecord->GetObject(nAddRow, NFrame::Player::BagEquipList::GUID);
	}

	return NULL_OBJECT;
}

bool NFCPackModule::CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount )
{
	if (nCount <= 0)
	{
		return 0;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}

	
	bool bExist = m_pElementModule->ExistElement(NFrame::Item::ThisName(), strConfigName );
	if ( !bExist )
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagItemList::ThisName() );
	if (!pRecord)
	{
		return 0;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindString(NFrame::Player::BagItemList::ConfigID, strConfigName, varFindResult);
	if (nFindRowCount <= 0)
	{
		NF_SHARE_PTR<NFDataList> xRowData = pRecord->GetInitData();

		xRowData->SetString(NFrame::Player::BagItemList::ConfigID, strConfigName);
		xRowData->SetInt(NFrame::Player::BagItemList::ItemCount, nCount);
		xRowData->SetInt(NFrame::Player::BagItemList::Date, pPluginManager->GetNowTime());

		pRecord->AddRow(-1, *xRowData);
	}
	else
	{
		int nFindRow = varFindResult.Int32(0);
		int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::BagItemList::ItemCount);
		int nNewCount = nOldCount + nCount;
		pRecord->SetInt(nFindRow, NFrame::Player::BagItemList::ItemCount, nNewCount);
	}

	return 0;
}

bool NFCPackModule::DeleteEquip( const NFGUID& self, const NFGUID& id )
{
	if (id.IsNull())
	{
		return false;
	}

	
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if (nullptr == pObject)
	{
		return false;
	}


	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagEquipList::ThisName() );
	if (nullptr == pRecord)
	{
		return false;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindObject(NFrame::Player::BagEquipList::GUID, id, varFindResult);
	if (nFindRowCount > 0)
	{
		int nTotalCount = 0;
		for (int i = 0; i < varFindResult.GetCount(); ++i)
		{
			int nFindRow = varFindResult.Int32(i);
			pRecord->Remove(nFindRow);
		}
	}

	return true;
}

bool NFCPackModule::DeleteItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
{
	if(nCount <= 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	
	if (!m_pElementModule->ExistElement(NFrame::Item::ThisName(), strItemConfigID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagItemList::ThisName() );
	if (!pRecord)
	{
		return false;
	}

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindString(NFrame::Player::BagItemList::ConfigID, strItemConfigID, varFindResult);
	if (nFindRowCount > 0)
	{
		int nNeedDelCount = nCount;
		for (int i = 0; i < varFindResult.GetCount(); ++i)
		{
			int nFindRow = varFindResult.Int32(i);
			int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::BagItemList::ItemCount);
			if (nOldCount > nNeedDelCount)
			{
				int nNewCount = nOldCount - nNeedDelCount;
				pRecord->SetInt(nFindRow, NFrame::Player::BagItemList::ItemCount, nNewCount);
				nNeedDelCount = 0;
			}
			else if (nOldCount == nNeedDelCount)
			{
				pRecord->Remove(nFindRow);
				nNeedDelCount = 0;
			}
			else if (nOldCount < nNeedDelCount)
			{
				pRecord->Remove(nFindRow);
				nNeedDelCount -= nOldCount;
			}
		}

		if (nNeedDelCount <= 0)
		{
			return true;
		}
	}

	return false;
}



bool NFCPackModule::EnoughItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
{
    if(nCount <= 0)
    {
        return false;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return false;
    }

    
    bool bExist = m_pElementModule->ExistElement(NFrame::Item::ThisName(), strItemConfigID );
    if ( !bExist )
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( NFrame::Player::BagItemList::ThisName() );
    if (!pRecord)
    {
        return false;
    }

	NFDataList varFindResult;
	int nFindRowCount = pRecord->FindString(NFrame::Player::BagItemList::ConfigID, strItemConfigID, varFindResult);
	if (nFindRowCount > 0)
	{
		int nTotalCount = 0;
		for (int i = 0; i < varFindResult.GetCount(); ++i)
		{
			int nFindRow = varFindResult.Int32(i);
			int nOldCount = pRecord->GetInt32(nFindRow, NFrame::Player::BagItemList::ItemCount);
			nTotalCount += nOldCount;
		}

		if (nTotalCount > 0)
		{
			return true;
		}
	}

    return false;
}
