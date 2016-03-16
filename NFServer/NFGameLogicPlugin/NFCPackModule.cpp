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
    //位置呢
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
//     //还得确定有这个装备
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
//     // 判断物品是否为装备
//     int nItemType = pPropertyType->GetInt();
//     if ( NFMsg::EIT_EQUIP != nItemType )
//     {
//         return NULL_OBJECT;
//     }
// 
//     NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//     const int nPackCount = pObject->GetPropertyInt("PackCount"); // 背包格子数量
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
//         // 绑定属性
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
//         var << strBaseProperty.c_str(); // 固定
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
            //第一次不刷新，因为第一次装备后，会自动刷新
            //RefreshEquipProperty(self);
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
        {
            //m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_SWAP_TABLE, this, &NFCPackModule::OnSwapTableRowEvent );

            // TOADD 其他背包需要的再加回调
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
//                  //此格子数量不够
//                  nRemainDelCount -= nGridCount;
//                  pRecord->Remove(i);
//              }
//              else if (nGridCount == nRemainDelCount)
//              {
//                  //此格子数刚好合适
//                  pRecord->Remove(i);
//                  return true;
//              }
//              else if (nGridCount > nRemainDelCount)
//              {
//                  //格子数删掉后还有剩余
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

	//还得确定有这个装备
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

	//还得确定有这个装备
	bool bExist = m_pElementInfoModule->ExistElement( strConfigName );
	if ( !bExist )
	{
		return 0;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strConfigName, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
    {
        return -1;
    }

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return 0;
	}

	int nRow = FindItemRowByConfig(self, strConfigName);
	if (nRow < 0)
	{
        NFCDataList varRowData;
//         if (!pRecord->IniRowData(varRowData))
//         {
//             return 0;
//         }

//         varRowData.SetString(NFrame::Player::BagItemList::BagItemList_ConfigID, strConfigName.c_str());
//         varRowData.SetInt(NFrame::Player::BagItemList::BagItemList_ItemCount, nCount);
//         varRowData.SetInt(NFrame::Player::BagItemList::BagItemList_Bound, 0);
//         varRowData.SetInt(NFrame::Player::BagItemList::BagItemList_ExpiredType, 0);
//         varRowData.SetObject(NFrame::Player::BagItemList::BagItemList_Date, NFGUID(0, NFTimeEx::GetNowTime()));

        varRowData << strConfigName;
        varRowData << nCount;
        varRowData << 0;
        varRowData << 0;
        varRowData << NFGUID(0, NFTimeEx::GetNowTime());

		int nRow= pRecord->AddRow(-1, varRowData);
		if (nRow >= 0)
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

PackTableType NFCPackModule::GetPackBagType(int nItemType)
{
    switch (nItemType)
    {
    case NFMsg::EItemType::EIT_NORMAL:
        break;
    case NFMsg::EItemType::EIT_EQUIP:
    {
        return PackTableType::BagEquipPack;
    }
    break;
    case NFMsg::EItemType::EIT_BOUNTY:
    case NFMsg::EItemType::EIT_HERO_CARD:
    case NFMsg::EItemType::EIT_HERO_STONE:
    case NFMsg::EItemType::EIT_STRENGTHEN_STONE:
    case NFMsg::EItemType::EIT_DEMONIZATION_STONE:
    case NFMsg::EItemType::EIT_GEM_STONE:
    case NFMsg::EItemType::EIT_CURRENCY:
    case NFMsg::EItemType::EIT_ITEM_REBORN:
    case NFMsg::EItemType::EIT_ITEM_POSITION:
    case NFMsg::EItemType::EIT_WOOD:
    case NFMsg::EItemType::EIT_STONE:
        break;
    default:
        break;
    }

    return PackTableType::BagItemPack;
}

int NFCPackModule::FindItemRowByConfig( const NFGUID& self, const std::string& strItemConfigID )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return -1;
	}

	//还得确定有这个装备
	bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
	if ( !bExist )
	{
		return -1;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
    {
        return -1;
    }

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::BagItemPack ) );
	if (!pRecord)
	{
		return -1;
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
					//看是玩家装备，还是英雄装备
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

	//还得确定有这个装备
	bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
	if ( !bExist )
	{
		return false;
	}

	int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
    {
        return -1;
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
		//找到了装备,也找到了英雄--还要查看装备是什么类型，然后放什么位置
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
		//找到了装备,也找到了英雄
        //TODO:还要查看装备是什么类型，然后放什么位置等条件判断
        //Hero和PackItem GUID互相绑定
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

	//删除3个地方，背包，英雄穿戴，玩家穿戴
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
				//看是玩家装备，还是英雄装备
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

    //还得确定有这个装备
    bool bExist = m_pElementInfoModule->ExistElement( strItemConfigID );
    if ( !bExist )
    {
        return false;
    }

    int nItemType = m_pElementInfoModule->GetPropertyInt(strItemConfigID, NFrame::Item::ItemType());
    PackTableType eBagType = GetPackBagType(nItemType);
    if (eBagType != PackTableType::BagItemPack)
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
