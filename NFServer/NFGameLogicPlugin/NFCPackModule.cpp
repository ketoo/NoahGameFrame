// -------------------------------------------------------------------------
//    @FileName         :    NFCSkillModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCSkillModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCPackModule.h"
#include "NFComm/Define/NFStringInfo.h"
#include "NFComm/NFCore/NFTimer.h"

const std::string mstrPackTableName[] =
{
    NFrame::Player::R_BagItemList(),
    NFrame::Player::R_BagEquipList(),
    NFrame::Player::R_PlayerHero(),
	NFrame::Player::R_DropItemList(),
};

bool NFCPackModule::Init()
{
    return true;
}

bool NFCPackModule::Shut()
{
    return true;
}

bool NFCPackModule::Execute()
{
    //λ����
    return true;
}

bool NFCPackModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );
    assert( NULL != m_pUUIDModule );

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCPackModule::OnClassObjectEvent );
    m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCPackModule::OnClassObjectEvent );

    return true;
}

const std::string& NFCPackModule::GetPackName( const PackTableType name )
{
    if ( name > PackTableType::MaxPack || name < PackTableType::NONE_PACK )
    {
        return NULL_STR;
    }

    return mstrPackTableName[name];
}

// const NFGUID& NFCPackModule::CreateEquip( const NFGUID& self, const std::string& strConfigName )
// {
//     NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
//     if ( NULL == pObject )
//     {
//         return NULL_OBJECT;
//     }
// 
//     //����ȷ�������װ��
//     NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager( strConfigName );
//     if ( NULL == pPropertyManager )
//     {
//         return NULL_OBJECT;
//     }
// 
//     NF_SHARE_PTR<NFIProperty> pPropertyType = pPropertyManager->GetElement( "ItemType" );
//     if (NULL == pPropertyType)
//     {
//         return NULL_OBJECT;
//     }
// 
//     // �ж���Ʒ�Ƿ�Ϊװ��
//     int nItemType = pPropertyType->GetInt();
//     if ( NFMsg::EIT_EQUIP != nItemType )
//     {
//         return NULL_OBJECT;
//     }
// 
//     NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//     const int nPackCount = pObject->GetPropertyInt("PackCount"); // ������������
// 
//     int nHasUsedCount = 0;
//     for ( int i = 0; i < pRecord->GetRows(); i++ )
//     {
//         nHasUsedCount++;
//         if ( pRecord->IsUsed( i ) )
//         {
//             continue;
//         }
// 
//         if (nHasUsedCount > nPackCount)
//         {
//             m_pLogModule->LogElement(NFILogModule::NLL_WARING_NORMAL, self, strConfigName, "NormalPack is full, cannot add equip", __FUNCTION__, __LINE__);
//             continue;
//         }
// 
//         NFGUID ident = m_pUUIDModule->CreateGUID();
//         NFCDataList var;
//         var << ident;
//         var << strConfigName.c_str() ;
//         var << 1;
// 
//         // ������
//         NF_SHARE_PTR<NFIProperty> pBoundProperty = pPropertyManager->GetElement( "Bound" );
//         var << (NULL != pBoundProperty ? pBoundProperty->GetInt() : 0);
//         var << 0;
//         var << 0;
// 
//         for ( int j = 0; j < 6; j++ )
//         {
//             var <<  NULL_STR;
//         }
// 
//         var << 0;
//         var << 0;
//         var << NULL_STR;
// 
//         std::string strRndProperty;
//         var << strRndProperty.c_str();
// 
//         var << NFTimeEx::GetNowTime();
// 
//         std::string strBaseProperty;
//         var << strBaseProperty.c_str(); // �̶�
// 
//         pRecord->AddRow( i, var );
//         return ident;
//     }
// 
//     return NULL_OBJECT;
// }

int NFCPackModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if (strClassNames == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
        {
            //m_pKernelModule->AddRecordCallBack( self, GetPackName( PackTableType::BagItemPack ), this, &NFCPackModule::OnObjectPackViewRecordEvent );
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
        {
            //RefreshInitViewItem(self);
            //��һ�β�ˢ�£���Ϊ��һ��װ���󣬻��Զ�ˢ��
            //RefreshEquipProperty(self);
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
        {
            //m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_SWAP_TABLE, this, &NFCPackModule::OnSwapTableRowEvent );

            // TOADD ����������Ҫ���ټӻص�
        }
    }

    if ( strClassNames == NFrame::NPC::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
        {
            m_pKernelModule->AddEventCallBack(self, NFED_ON_OBJECT_BE_KILLED, this, &NFCPackModule::OnObjectBeKilled);
        }
    }

    return 0;
}

// int NFCPackModule::QueryCount( const NFGUID& self, const std::string& strItemConfigID ) const
// {
//     NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
//     if ( NULL == pObject )
//     {
//         m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
//         return 0;
//     }
// 
//     NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//     if (NULL == pRecord)
//     {
//         return 0;
//     }
// 
//     NFINT64 nCount = 0;
//     for (int i = 0; i < pRecord->GetRows(); ++i)
//     {
//         if (pRecord->IsUsed(i) && pRecord->GetString(i, NFMsg::BagItemList_ItemCount) == strItemConfigID)
//         {
//             nCount += pRecord->GetInt(i, NFMsg::BagItemList_ItemCount);
//         }
//     }
// 
//     return nCount;
// }

// bool NFCPackModule::DeleteItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
// {
//      if (QueryCount(self, strItemConfigID) < nCount)
//      {
//          m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "There is not enough item count", strItemConfigID, __FUNCTION__, __LINE__);
//  
//          return false;
//      }
//  
//      NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
//      if ( NULL == pObject )
//      {
//          m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
//          return false;
//      }
//  
//      NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//      if (NULL == pRecord)
//      {
//          return false;
//      }
//  
//      int nRemainDelCount = nCount;
//      for (int i = 0; i < pRecord->GetRows(); ++i)
//      {
//          if (nRemainDelCount <= 0)
//          {
//              break;
//          }
//  
//          if (pRecord->IsUsed(i) && pRecord->GetString(i, NFMsg::BagItemList_ConfigID) == strItemConfigID)
//          {
//              const int nGridCount = pRecord->GetInt(i, NFMsg::BagItemList_ItemCount);
//              if (nGridCount < nRemainDelCount)
//              {
//                  //�˸�����������
//                  nRemainDelCount -= nGridCount;
//                  pRecord->Remove(i);
//              }
//              else if (nGridCount == nRemainDelCount)
//              {
//                  //�˸������պú���
//                  pRecord->Remove(i);
//                  return true;
//              }
//              else if (nGridCount > nRemainDelCount)
//              {
//                  //������ɾ������ʣ��
//                  pRecord->SetInt(i, NFMsg::BagItemList_ItemCount, nGridCount - nRemainDelCount);
//                  return true;
//              }
//          }
//      }
// 
//     return false;
// }

void NFCPackModule::AddDropItem(const NFGUID& self, const NFIDataList& var)
{
    if (var.GetCount() != 4 || !var.TypeEx(TDATA_OBJECT, TDATA_STRING, TDATA_INT, TDATA_INT, TDATA_UNKNOWN))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "args error", "", __FUNCTION__, __LINE__);
        return;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_DropItemList());
    if (NULL == pRecord.get())
    {
        return;
    }

    pRecord->AddRow(-1, var);
}

int NFCPackModule::OnObjectBeKilled(const NFGUID& self, const int nEventID, const NFIDataList& var)
{
    if ( var.GetCount() != 1 || !var.TypeEx(TDATA_OBJECT, TDATA_UNKNOWN))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "args error", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFGUID identKiller = var.Object( 0 );
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(identKiller);
    if (nullptr == pObject)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(identKiller, NFrame::Player::R_DropItemList());
    if (nullptr == pRecord)
    {
        return 1;
    }

    NFCDataList varResult;
    int nRowCount = pRecord->FindObject(NFrame::Player::DropItemList_MonsterID, self, varResult);
    for (int i = 0; i < nRowCount; ++i)
    {
        pRecord->SetInt(varResult.Int(i), NFrame::Player::DropItemList_DrawState, NFMsg::E_DRAW_STATE_GAIN);
    }

    return 0;
}

const NFGUID& NFCPackModule::CreateEquip( const NFGUID& self, const std::string& strConfigName )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return NULL_OBJECT;
	}

	//����ȷ�������װ��
	bool bExist = m_pElementInfoModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		return NULL_OBJECT;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_EQUIP != nItemType )
	{
		return NULL_OBJECT;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return NULL_OBJECT;
	}

	NFGUID ident = m_pUUIDModule->CreateGUID();

	NFCDataList var;
	var << ident;
	var << NULL_OBJECT;
	var << strConfigName;
	var << 0;
	var << 0;
	var << NULL_STR;
	var << NFTimeEx::GetNowTime();
	var << 0;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << NULL_STR;
	var << 0;
	var << 0;
	var << 0;
	var << 0;
	var << 0;
	var << 0;
	var << 0;
	var << 0;
	var << 0;
	var << 0;
	var << 0;

	int nAddRow = pRecord->AddRow(-1, var);
	if (nAddRow > 0)
	{
		return pRecord->GetObject(nAddRow, NFrame::Player::BagEquipList_GUID);
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

	//����ȷ�������װ��
	bool bExist = m_pElementInfoModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		return 0;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strConfigName, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_NORMAL != nItemType )
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return 0;
	}

	int nRow = FindItemRowByConfig(self, strConfigName);
	if (nRow < 0)
	{
		NFGUID ident = m_pUUIDModule->CreateGUID();

		NFCDataList var;
		var << ident;
		var << nCount;
		var << strConfigName;
		var << 0;
		var << NFTimeEx::GetNowTime();

		bool bAddRet = pRecord->AddRow(-1, var);
		if (bAddRet)
		{
			return nCount;
		}
	}
	else
	{
		int nOldCount = pRecord->GetInt(nRow, NFrame::Player::BagItemList_ItemCount);
		int nNewCount = nOldCount + nCount;
		pRecord->SetInt(nRow, NFrame::Player::BagItemList_ItemCount, nNewCount);
	}

	return 0;
}

int NFCPackModule::FindItemRowByConfig( const NFGUID& self, const std::string& strItemConfigID )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}

	//����ȷ�������װ��
	bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
	if ( !bExist )
	{
		return 0;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_NORMAL != nItemType )
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return 0;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const std::string& strCfgID = pRecord->GetString(i, NFrame::Player::BagItemList_ConfigID);
			if (strCfgID == strItemConfigID)
			{
				return i;
			}
		}
	}

	return -1;
}

bool NFCPackModule::DeleteEquip( const NFGUID& self, const NFGUID& id )
{
	if (id.IsNull())
	{
		return false;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}


	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return 0;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				const NFGUID& xWearID = pRecord->GetObject(i, NFrame::Player::BagEquipList_WearGUID);
				if (!xWearID.IsNull())
				{
					//�������װ��������Ӣ��װ��
					const std::string& strCfgID = pRecord->GetString(i, NFrame::Player::BagEquipList_ConfigID);

				}

				pRecord->Remove(i);
			}
		}
	}

	return false;
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

	//����ȷ�������װ��
	bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
	if ( !bExist )
	{
		return false;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
	if ( NFMsg::EItemType::EIT_NORMAL != nItemType )
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return false;
	}

	int nRow = FindItemRowByConfig(self, strItemConfigID);
	if (nRow >= 0)
	{
		int nOldCount = pRecord->GetInt(nRow, NFrame::Player::BagItemList_ItemCount);
		int nNewCount = nOldCount - nCount;
		if (nNewCount >= 0)
		{
			if (nNewCount > 0)
			{
				pRecord->SetInt(nRow, NFrame::Player::BagItemList_ItemCount, nNewCount);
			}
			else
			{
				pRecord->Remove(nRow);
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool NFCPackModule::DressEquipForHero( const NFGUID& self, const NFGUID& hero, const NFGUID& id )
{
	if(id.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pEquipRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pEquipRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagHeroPack ) );
	if (!pHeroRecord)
	{
		return false;
	}

	int nEquipRow = -1;
	int nHeroRow = -1;
	int nHeroEquipCol = -1;

	for (int i = 0; i < pEquipRecord->GetRows(); ++i)
	{
		if (pEquipRecord->IsUsed(i))
		{
			const NFGUID& xID = pEquipRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				nEquipRow = i;
				break;
			}
		}
	}

	for (int i = 0; i < pHeroRecord->GetRows(); ++i)
	{
		if (pHeroRecord->IsUsed(i))
		{
			const NFGUID& xHeroID = pHeroRecord->GetObject(i, NFrame::Player::PlayerHero_GUID);
			if (xHeroID == hero)
			{
				for (int j = NFrame::Player::PlayerHero_Equip1; i <= NFrame::Player::PlayerHero_Equip6; ++j)
				{
					const NFGUID& xHeroEquip = pHeroRecord->GetObject(i, j);
					if (xHeroEquip.IsNull())
					{
						nHeroRow = i;
						nHeroEquipCol = j;

						break;
					}
				}
			}
		}
	}

	if (nEquipRow >= 0 && nHeroRow >= 0)
	{
		//�ҵ���װ��,Ҳ�ҵ���Ӣ��--��Ҫ�鿴װ����ʲô���ͣ�Ȼ���ʲôλ��
		pHeroRecord->SetObject(nHeroRow, nHeroEquipCol, id);
		pEquipRecord->SetObject(nEquipRow, NFrame::Player::BagEquipList_WearGUID, hero);
	}

	return false;
}

bool NFCPackModule::TakeOffEquipForm( const NFGUID& self, const NFGUID& hero, const NFGUID& id )
{
	if(id.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pEquipRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pEquipRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagHeroPack ) );
	if (!pHeroRecord)
	{
		return false;
	}

	int nEquipRow = -1;
	int nHeroRow = -1;
	int nHeroEquipCol = -1;

	for (int i = 0; i < pEquipRecord->GetRows(); ++i)
	{
		if (pEquipRecord->IsUsed(i))
		{
			const NFGUID& xID = pEquipRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				nEquipRow = i;
				break;
			}
		}
	}

	for (int i = 0; i < pHeroRecord->GetRows(); ++i)
	{
		if (pHeroRecord->IsUsed(i))
		{
			const NFGUID& xHeroID = pHeroRecord->GetObject(i, NFrame::Player::PlayerHero_GUID);
			if (xHeroID == hero)
			{
				for (int j = NFrame::Player::PlayerHero_Equip1; i <= NFrame::Player::PlayerHero_Equip6; ++j)
				{
					const NFGUID& xHeroEquip = pHeroRecord->GetObject(i, j);
					if (xHeroEquip == id)
					{
						nHeroRow = i;
						nHeroEquipCol = j;

						break;
					}
				}
			}
		}
	}

	if (nEquipRow >= 0 && nHeroRow >= 0)
	{
		//�ҵ���װ��,Ҳ�ҵ���Ӣ��
        //TODO:��Ҫ�鿴װ����ʲô���ͣ�Ȼ���ʲôλ�õ������ж�
        //Hero��PackItem GUID�����
		pHeroRecord->SetObject(nHeroRow, nHeroEquipCol, id);
		pEquipRecord->SetObject(nEquipRow, NFrame::Player::BagEquipList_WearGUID, hero);
	}

	return false;
}

int NFCPackModule::SetEquipRandPropertyID( const NFGUID& self, const NFGUID& id, const std::string& strPropertyID )
{
	if (id.IsNull())
	{
		return 0;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}

	if (!m_pElementInfoModule->ExistElement(strPropertyID))
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return 0;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				pRecord->SetString(i, NFrame::Player::BagEquipList_RandPropertyID, strPropertyID.c_str());
			}
		}
	}

	return 0;
}

const std::string& NFCPackModule::GetEquipRandPropertyID( const NFGUID& self, const NFGUID& id )
{
	if (id.IsNull())
	{
		return NULL_STR;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return NULL_STR;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return NULL_STR;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->GetString(i, NFrame::Player::BagEquipList_RandPropertyID);
			}
		}
	}

	return NULL_STR;
}

bool NFCPackModule::SetEquipHoleCount( const NFGUID& self, const NFGUID& id, const int nCount )
{
	if (nCount <= 0
		|| nCount > (NFrame::Player::BagEquipList_InlayStone10 - NFrame::Player::BagEquipList_InlayStone1))
	{
		return false;
	}

	if (id.IsNull())
	{
		return false;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return false;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				pRecord->SetInt(i, NFrame::Player::BagEquipList_SlotCount, nCount);
				return true;
			}
		}
	}

	return false;
}

int NFCPackModule::GetEquipHoleCount( const NFGUID& self, const NFGUID& id )
{
	if (id.IsNull())
	{
		return 0;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return 0;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->GetInt(i, NFrame::Player::BagEquipList_SlotCount);
			}
		}
	}

	return 0;
}

bool NFCPackModule::SetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const std::string& strPropertyID )
{
	if (id.IsNull())
	{
		return false;
	}

	if (!m_pElementInfoModule->ExistElement(strPropertyID))
	{
		return false;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return false;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->SetString(i, eIndex, strPropertyID.c_str());
			}
		}
	}

	return false;
}

const std::string& NFCPackModule::GetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex )
{
	if (id.IsNull())
	{
		return NULL_STR;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return NULL_STR;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return NULL_STR;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->GetString(i, eIndex);
			}
		}
	}

	return NULL_STR;
}

bool NFCPackModule::SetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id, const int nLevel )
{
	if (id.IsNull())
	{
		return false;
	}

	if (nLevel <= 0)
	{
		return false;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return false;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->SetInt(i, NFrame::Player::BagEquipList_IntensifyLevel, nLevel);
			}
		}
	}

	return false;
}

int NFCPackModule::GetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id )
{
	if (id.IsNull())
	{
		return 0;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return 0;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->GetInt(i, NFrame::Player::BagEquipList_IntensifyLevel);
			}
		}
	}

	return 0;
}

bool NFCPackModule::SetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const int nLevel )
{
	if (id.IsNull())
	{
		return false;
	}

	if (nLevel <= 0)
	{
		return false;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return false;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->SetInt(i, eIndex, nLevel);
			}
		}
	}

	return false;
}

int NFCPackModule::GetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex )
{
	if (id.IsNull())
	{
		return 0;
	}

	//ɾ��3���ط���������Ӣ�۴�������Ҵ���
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagEquipPack ) );
	if (!pRecord)
	{
		return 0;
	}

	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
		if (pRecord->IsUsed(i))
		{
			const NFGUID& xID = pRecord->GetObject(i, NFrame::Player::BagEquipList_GUID);
			if (xID == id)
			{
				//�������װ��������Ӣ��װ��
				return pRecord->GetInt(i, eIndex);
			}
		}
	}

	return 0;
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

    //����ȷ�������װ��
    bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
    if ( !bExist )
    {
        return false;
    }

    int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
    if ( NFMsg::EItemType::EIT_NORMAL != nItemType )
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
    if (!pRecord)
    {
        return false;
    }

    int nRow = FindItemRowByConfig(self, strItemConfigID);
    if (nRow >= 0)
    {
        int nOldCount = pRecord->GetInt(nRow, NFrame::Player::BagItemList_ItemCount);
        if (nCount >= nOldCount)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
