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

//最大洞数量
const int mnMaxSlotCount = 6;
//格子最大容纳数量
const int mnMaxOverlayCount = 99;

const std::string mstrPackTableName[] =
{
    std::string( "BagItemList" ),
    std::string( "PlayerViewItem" ),
    std::string( "DropItemList" ),
};

bool NFCPackModule::Init()
{
    

    return true;
}


bool NFCPackModule::Shut()
{
    return true;
}

bool NFCPackModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //位置呢
    return true;
}

bool NFCPackModule::AfterInit()
{
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>( "NFCEventProcessModule" );
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );
    assert( NULL != m_pUUIDModule );

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCPackModule::OnClassObjectEvent );
    m_pEventProcessModule->AddClassCallBack( "NPC", this, &NFCPackModule::OnClassObjectEvent );

    return true;
}

const std::string& GetPackName( const PackTableType name )
{
    if ( name > PackTableType::MaxPack || name < PackTableType::NormalPack )
    {
        return NULL_STR;
    }

    return mstrPackTableName[name];
}

const NFGUID& NFCPackModule::CreateEquip( const NFGUID& self, const std::string& strConfigName )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return NULL_OBJECT;
    }

    //还得确定有这个装备
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager( strConfigName );
    if ( NULL == pPropertyManager )
    {
        return NULL_OBJECT;
    }

    NF_SHARE_PTR<NFIProperty> pPropertyType = pPropertyManager->GetElement( "ItemType" );
    if (NULL == pPropertyType)
    {
        return NULL_OBJECT;
    }

    // 判断物品是否为装备
    int nItemType = pPropertyType->GetInt();
    if ( NFMsg::EIT_EQUIP != nItemType )
    {
        return NULL_OBJECT;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
    const int nPackCount = pObject->GetPropertyInt("PackCount"); // 背包格子数量

    int nHasUsedCount = 0;
    for ( int i = 0; i < pRecord->GetRows(); i++ )
    {
        nHasUsedCount++;
        if ( pRecord->IsUsed( i ) )
        {
            continue;
        }

        if (nHasUsedCount > nPackCount)
        {
            m_pLogModule->LogElement(NFILogModule::NLL_WARING_NORMAL, self, strConfigName, "NormalPack is full, cannot add equip", __FUNCTION__, __LINE__);
            continue;
        }

        NFGUID ident = m_pUUIDModule->CreateGUID();
        NFCDataList var;
        var << ident;
        var << strConfigName.c_str() ;
        var << 1;

        // 绑定属性
        NF_SHARE_PTR<NFIProperty> pBoundProperty = pPropertyManager->GetElement( "Bound" );
        var << (NULL != pBoundProperty ? pBoundProperty->GetInt() : 0);
        var << 0;
        var << 0;

        for ( int j = 0; j < 6; j++ )
        {
            var <<  NULL_STR;
        }

        var << 0;
        var << 0;
        var << NULL_STR;

        std::string strRndProperty;
        var << strRndProperty.c_str();

        var << NFTimeEx::GetNowTime();

        std::string strBaseProperty;
        var << strBaseProperty.c_str(); // 固定

        pRecord->AddRow( i, var );
        return ident;
    }

    return NULL_OBJECT;
}

bool NFCPackModule::DeleteGrid( const NFGUID& self, const int nOrigin)
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            pRecord->Remove( nOrigin );

            return true;
        }
    }

    return false;
}

bool NFCPackModule::SetGridCount( const NFGUID& self, const int nOrigin, const int nCount)
{
    //判断nOrigin合法性，以及nCount合法性(物品堆叠数量)
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            pRecord->SetInt( nOrigin, NFMsg::BagItemList_RecordColType::BagItemList_ItemCount, nCount );

            return true;
        }
    }

    return false;
}

NFINT64 NFCPackModule::GetGridCount( const NFGUID& self, const int nOrigin)
{
    //判断nOrigin合法性
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetInt( nOrigin, NFMsg::BagItemList_RecordColType::BagItemList_ItemCount );
        }
    }

    return 0;
}

bool NFCPackModule::SetEquipCreatTime( const NFGUID& self, const int nOrigin, const NFINT64 nTime )
{
    //判断nOrigin合法性(越界以及是否是装备)

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->SetInt( nOrigin,  NFMsg::BagItemList_Date, nTime );
        }
    }

    return false;
}

const NFINT64 NFCPackModule::GetEquipCreatTime( const NFGUID& self, const int nOrigin )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetInt( nOrigin, NFMsg::BagItemList_Date );
        }
    }

    return 0;
}

bool NFCPackModule::CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount )
{
    if (strConfigName.empty())
    {
        return false;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return false;
    }

    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager( strConfigName );
    if ( NULL == pPropertyManager )
    {
        return false;
    }

    NF_SHARE_PTR<NFIProperty> pPropertyType =  pPropertyManager->GetElement("ItemType");
    if (NULL == pPropertyType)
    {
        return false;
    }

//     int nItemType = pPropertyType->GetInt();
//     switch (nItemType)
//     {
//     case EGameItemType::EGIT_Item:
//     case EGameItemType::EGIT_Material:
//     case EGameItemType::EGIT_Task:
//         {
//             //NF_SHARE_PTR<NFIProperty> pPropertySubType =  pPropertyManager->GetElement("ItemSubType");
// 
// 
//             int nOverlayCount = pPropertyManager->GetElement("OverlayCount")->GetInt();
//             int nNewCount = nCount;
//             if (nCount > nOverlayCount)
//             {
//                 nNewCount = nOverlayCount; // 重新设置值
//             }
// 
//             NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
//             if (NULL == pRecord)
//             {
//                 return false;
//             }
// 
//             const NFINT64 nPackCount = pObject->GetPropertyInt("PackCount");
//             NFCDataList matchItemList;
//             int nMatchCount = pRecord->FindString(EGIT_TYPE_CONFIGID, strConfigName.c_str(), matchItemList);
// 
//             // 给已有的增加至堆叠上限
//             for (int i = 0; i < nMatchCount; ++i)
//             {
//                 int nRow = matchItemList.Int(i);
//                 if (nRow < mnPackStart)
//                 {// 前11个是已装备物品
//                     continue;
//                 }
// 
//                 int nCurrRowItemCount = pRecord->GetInt(nRow, EGIT_TYPE_ITEMCOUNT);
//                 if (nCurrRowItemCount == nOverlayCount)
//                 {// 本行数量已满
//                     continue;
//                 }
// 
//                 int nMissingValue = nOverlayCount - nCurrRowItemCount; // 本行数量还差的值
//                 if (nMissingValue >= nNewCount)
//                 {
//                     pRecord->SetInt(nRow, EGIT_TYPE_ITEMCOUNT, nCurrRowItemCount + nNewCount);
//                     return true;
//                 }
//                 else
//                 {
//                     nNewCount -= nMissingValue;
//                 }
//             }
// 
//             // 如果还有剩余
//             if (nNewCount > 0)
//             {
//                 for (int i = mnPackStart; i < pRecord->GetRows(); ++i)
//                 {
//                     if (!pRecord->IsUsed(i))
//                     {
//                         NFCDataList var;
//                         var << m_pUUIDModule->CreateGUID();
//                         var << strConfigName.c_str() ;
//                         var << nNewCount;
// 
//                         NF_SHARE_PTR<NFIProperty> pBoundProperty = pPropertyManager->GetElement( "Bound" );
//                         var << (pBoundProperty != NULL ? pBoundProperty->GetInt() : 0);
//                         var << (NFINT32)eExpiredType;
//                         var << 0;
// 
//                         for ( int j = 0; j < 6; j++ )
//                         {
//                             var << "";
//                         }
// 
//                         var << 0;
//                         var << 0;
//                         var << "";
//                         var << "";  // 随机属性
//                         var << NFTimeEx::GetNowTime();;   // TODO:物品时间,后面补上
// 						var << "";
//                         int rowss = pRecord->AddRow( i, var );
//                         return true;
//                     }
//                 }
//             }
// 
//             return true;
//         }
//         break;
//     default:
//         return false;
//     }

    return false;
}

bool NFCPackModule::SetGridBan( const NFGUID& self, const int nOrigin, const bool bBan )
{
    return false;
}

bool NFCPackModule::GetGridBan( const NFGUID& self, const int nOrigin )
{
    return false;
}

int NFCPackModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if (strClassNames == "Player")
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
        {
            m_pKernelModule->AddRecordCallBack( self, GetPackName( PackTableType::NormalPack ), this, &NFCPackModule::OnObjectPackViewRecordEvent );
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
        {
            //RefreshInitViewItem(self);
            //第一次不刷新，因为第一次装备后，会自动刷新
            RefreshEquipProperty(self);
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
        {
            m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_SWAP_TABLE, this, &NFCPackModule::OnSwapTableRowEvent );

            // TOADD 其他背包需要的再加回调
        }
    }

    if ( strClassNames == "NPC")
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
        {
            m_pEventProcessModule->AddEventCallBack(self, NFED_ON_OBJECT_BE_KILLED, this, &NFCPackModule::OnObjectBeKilled);
        }
    }

    return 0;
}

int NFCPackModule::OnSwapTableRowEvent( const NFGUID& object, const int nEventID, const NFIDataList& var )
{
    if ( 4 == var.GetCount() )
    {
        const std::string& strOriginTableName = var.String( 0 );
        const std::string& strTargetTableName = var.String( 1 );
        int nOrigin = var.Int( 2 );
        int nTarget = var.Int( 3 );

        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(object);
        if (pObject)
        {
            NF_SHARE_PTR<NFIRecord> pOriginRecord = pObject->GetRecordManager()->GetElement(strOriginTableName);
            NF_SHARE_PTR<NFIRecord> pTargetRecord = pObject->GetRecordManager()->GetElement(strTargetTableName);
            if (pOriginRecord && pTargetRecord)
            {
                if ( nOrigin >= 0 && nOrigin < pOriginRecord->GetRows()
                    && nTarget >= 0 && nTarget < pTargetRecord->GetRows() )
                {
                    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( object );
                    if ( pObject )
                    {
                        //SwapGrid( object, nOrigin, nTarget, GetPackType( strOriginTableName ), GetPackType( strTargetTableName ) );
                    }
                }
            }
        }


    }

    return 0;
}

int NFCPackModule::OnObjectPackViewRecordEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar )
{
	const std::string& strRecordName = xEventData.strRecordName;
	const int nOpType = xEventData.nOpType;
	const int nRow = xEventData.nRow;
	const int nCol = xEventData.nCol; 

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pPackRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pPackRecord )
        {
            switch ( nOpType )
            {
            case NFIRecord::RecordOptype::Add:
                {
                    const std::string& strConfigID = pPackRecord->GetString( nRow, ( int )NFMsg::BagItemList_ConfigID );
                    AddEquipProperty(self, strConfigID, nRow);
                }
                break;

            case NFIRecord::RecordOptype::Del:
                {
					const std::string& strOldConfigID = pPackRecord->GetString( nRow, ( int )NFMsg::BagItemList_ConfigID );
					RemoveEquipProperty(self, strOldConfigID, nRow);
                }
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

const std::string& NFCPackModule::GetGridConfigID( const NFGUID& self, const int nRow)
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetString( nRow, NFMsg::BagItemList_ConfigID );
        }
    }

    return NULL_STR;
}

bool NFCPackModule::SetGridData( const NFGUID& self, const int nRow, const int nCol, const NFIDataList& var)
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
    if ( pRecord == NULL )
    {
        return false;
    }

    if ( !pRecord->SetInt( nRow, nCol, var.Int( 0 ) ) )
    {
        // log
    }

    return false;
}

int NFCPackModule::RefreshEquipProperty( const NFGUID& self )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( NULL == pObject )
	{
		return 1;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::ViewPack ) );
	if ( NULL == pRecord )
	{
		return 1;
	}

    // 装备属性计算
	for (int i = 0; i < pRecord->GetRows(); ++i)
	{
        RefreshEquipProperty(self, i);
    }

    return 0;
}

int NFCPackModule::RefreshEquipProperty( const NFGUID& self, const int nRow )
{
    // 单件装备属性计算
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::ViewPack ) );
    if ( NULL == pRecord )
    {
        return 1;
    }

    if(!pRecord->IsUsed(nRow))
    {
        return 1;
    }

    const std::string& strConfigID = pRecord->GetString( nRow, ( int )NFMsg::PlayerViewItem_ConfigID );
    if (strConfigID.empty())
    {
        return 1;
    }

	AddEquipProperty(self, strConfigID, nRow);

    return 0;
}

int NFCPackModule::AddEquipProperty( const NFGUID& self, const std::string& strConfigID, const int nRow )
{
    if (strConfigID.empty())
    {
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
    if ( NULL == pRecord || !pRecord->IsUsed(nRow) )
    {
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////

    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigID);
    if (!pPropertyManager)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIProperty> pEffectDataProperty = pPropertyManager->GetElement("EffectData");
    if (!pEffectDataProperty)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIPropertyManager> pEffectDataPropertyManager = m_pElementInfoModule->GetPropertyManager(pEffectDataProperty->GetString());
    if (!pEffectDataPropertyManager)
    {
        return 1;
    }

   NF_SHARE_PTR<NFIProperty> pProperty = pEffectDataPropertyManager->First();
   while (pProperty)
   {
       if (pProperty->GetInt() != 0)
       {
           m_pPropertyModule->AddPropertyValue( self, pProperty->GetKey(), NFIPropertyModule::NPG_EQUIP, pProperty->GetInt() );
       }

       pProperty = pEffectDataPropertyManager->Next();
   }

   return 0;
}

int NFCPackModule::RemoveEquipProperty( const NFGUID& self, const std::string& strConfigID, const int nRow )
{
    if (strConfigID.empty())
    {
        return 1;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
    if ( NULL == pRecord || !pRecord->IsUsed(nRow) )
    {
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigID);
    if (!pPropertyManager)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIProperty> pEffectDataProperty = pPropertyManager->GetElement("EffectData");
    if (!pEffectDataProperty)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIPropertyManager> pEffectDataPropertyManager = m_pElementInfoModule->GetPropertyManager(pEffectDataProperty->GetString());
    if (!pEffectDataPropertyManager)
    {
        return 1;
    }

    NF_SHARE_PTR<NFIProperty> pProperty = pEffectDataPropertyManager->First();
    while (pProperty)
    {
        if (pProperty->GetInt() != 0)
        {
            m_pPropertyModule->SubPropertyValue( self, pProperty->GetKey(), NFIPropertyModule::NPG_EQUIP, pProperty->GetInt() );
        }

        pProperty = pEffectDataPropertyManager->Next();
    }

    
    return 0;
}

int NFCPackModule::QueryCount( const NFGUID& self, const std::string& strItemConfigID ) const
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
        return 0;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
    if (NULL == pRecord)
    {
        return 0;
    }

    NFINT64 nCount = 0;
    for (int i = 0; i < pRecord->GetRows(); ++i)
    {
        if (pRecord->IsUsed(i) && pRecord->GetString(i, NFMsg::BagItemList_ItemCount) == strItemConfigID)
        {
            nCount += pRecord->GetInt(i, NFMsg::BagItemList_ItemCount);
        }
    }

    return nCount;
}

bool NFCPackModule::DeleteItem( const NFGUID& self, const std::string& strItemConfigID, const int nCount )
{
     if (QueryCount(self, strItemConfigID) < nCount)
     {
         m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "There is not enough item count", strItemConfigID, __FUNCTION__, __LINE__);
 
         return false;
     }
 
     NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
     if ( NULL == pObject )
     {
         m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
         return false;
     }
 
     NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
     if (NULL == pRecord)
     {
         return false;
     }
 
     int nRemainDelCount = nCount;
     for (int i = 0; i < pRecord->GetRows(); ++i)
     {
         if (nRemainDelCount <= 0)
         {
             break;
         }
 
         if (pRecord->IsUsed(i) && pRecord->GetString(i, NFMsg::BagItemList_ConfigID) == strItemConfigID)
         {
             const int nGridCount = pRecord->GetInt(i, NFMsg::BagItemList_ItemCount);
             if (nGridCount < nRemainDelCount)
             {
                 //此格子数量不够
                 nRemainDelCount -= nGridCount;
                 pRecord->Remove(i);
             }
             else if (nGridCount == nRemainDelCount)
             {
                 //此格子数刚好合适
                 pRecord->Remove(i);
                 return true;
             }
             else if (nGridCount > nRemainDelCount)
             {
                 //格子数删掉后还有剩余
                 pRecord->SetInt(i, NFMsg::BagItemList_ItemCount, nGridCount - nRemainDelCount);
                 return true;
             }
         }
     }

    return false;
}

void NFCPackModule::AddDropItem(const NFGUID& self, const NFIDataList& var)
{
    if (var.GetCount() != 4 || !var.TypeEx(TDATA_OBJECT, TDATA_STRING, TDATA_INT, TDATA_INT, TDATA_UNKNOWN))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "args error", "", __FUNCTION__, __LINE__);
        return;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "DropItemList");
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

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(identKiller, "DropItemList");
    if (nullptr == pRecord)
    {
        return 1;
    }

    NFCDataList varResult;
    int nRowCount = pRecord->FindObject(NFMsg::DropItemList_MonsterID, self, varResult);
    for (int i = 0; i < nRowCount; ++i)
    {
        pRecord->SetInt(varResult.Int(i), NFMsg::DropItemList_DrawState, NFMsg::E_DRAW_STATE_GAIN);
    }

    return 0;
}