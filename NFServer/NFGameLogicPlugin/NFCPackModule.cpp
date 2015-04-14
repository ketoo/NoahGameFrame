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

//�Զ�ID��������
char* mstrAutoIDName;
//�������
const int mnMaxSlotCount = 6;
//������ʼ����
const int mnPackStart = 11;
//��ʼ��������
const int mnPackSartCapacity = 100;
//���������������
const int mnMaxOverlayCount = 99;

const std::string mstrPackTableName[] =
{
    std::string( "BagItemList" ),
    std::string( "PlayerViewItem" ),
    std::string( "BuyBackItemList" ),
    std::string( "DropItemList" ),
    std::string( "FriendViewList" ),
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
    //λ����
    return true;
}

bool NFCPackModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pSceneProcessModule = dynamic_cast<NFISceneProcessModule*>( pPluginManager->FindModule( "NFCSceneProcessModule" ) );
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>( pPluginManager->FindModule( "NFCPropertyModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
    m_pUUIDModule = dynamic_cast<NFIUUIDModule*>(pPluginManager->FindModule("NFCUUIDModule"));

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );
    assert( NULL != m_pUUIDModule );

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCPackModule::OnClassObjectEvent );

    CheckEquip();

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

PackTableType NFCPackModule::GetPackType( const std::string& name )
{
    for ( int i = 0; i < PackTableType::MaxPack; ++i )
    {
        if ( mstrPackTableName[i] == name )
        {
            return PackTableType( i );
        }
    }

    return PackTableType::None_Pack;
}

NFIDENTID NFCPackModule::CreateEquip( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount, const NFIDataList& inlayCardList, const int nIntensiveLevel, const int nEnchantLevel, const std::string& strEnchantCard )
{
    if (nSoltCount > mnMaxSlotCount || inlayCardList.GetCount() > mnMaxSlotCount)
    {
        return NFIDENTID();
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return NFIDENTID();
    }

    //����ȷ�������װ��
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementInfoModule->GetPropertyManager( strConfigName );
    if ( NULL == pPropertyManager )
    {
        return NFIDENTID();
    }

    NF_SHARE_PTR<NFIProperty> pPropertyType = pPropertyManager->GetElement( "ItemType" );
    if (NULL == pPropertyType)
    {
        return NFIDENTID();
    }

    // �ж���Ʒ�Ƿ�Ϊװ��
    int nItemType = pPropertyType->GetInt();
    if ( EGameItemType::EGIT_Equip != nItemType )
    {
        return NFIDENTID();
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
    const int nPackCount = pObject->GetPropertyInt("PackCount"); // ������������

    //ǰ��10����װ����
    int nHasUsedCount = 0;
    for ( int i = mnPackStart; i < pRecord->GetRows(); i++ )
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

        NFIDENTID ident = m_pUUIDModule->CreateGUID();
        NFCDataList var;
        var << ident;
        var << strConfigName.c_str() ;
        var << 1;

        // ������
        NF_SHARE_PTR<NFIProperty> pBoundProperty = pPropertyManager->GetElement( "Bound" );
        var << (NULL != pBoundProperty ? pBoundProperty->GetInt() : 0);
        var << (NFINT32)eExpiredType;
        var << nSoltCount;

        for ( int j = 0; j < 6; j++ )
        {
            int nInlayCardCount = inlayCardList.GetCount();
            var << (j < nInlayCardCount ? inlayCardList.String( j ).c_str() : NULL_STR);
        }

        var << nIntensiveLevel;
        var << nEnchantLevel;
        var << strEnchantCard.c_str();

        std::string strRndProperty;
        var << strRndProperty.c_str();

        var << NFTimeEx::GetNowTime();
        //boost::date_time::date temDate;
        //boost::posix_time::posix_time_system timea;
        //boost::posix_time::time_duration td;
        //td.minutes()

        std::string strBaseProperty;
        var << strBaseProperty.c_str(); // �̶�

        pRecord->AddRow( i, var );
        return ident;
    }

    return NFIDENTID();
}

NFIDENTID NFCPackModule::CreateEquip( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount )
{
    if ( nSoltCount <= mnMaxSlotCount )
    {
        return CreateEquip( self, strConfigName, eExpiredType, nSoltCount, NFCDataList(), 0, 0, "" );
    }

    return NFIDENTID();
}

bool NFCPackModule::can_normal_pack_item_swap( const NFIDENTID& self, NF_SHARE_PTR<NFIRecord> pOriginRecord, NF_SHARE_PTR<NFIRecord> pTargetRecord, const int origin, const int target )
{
    if ( pOriginRecord == NULL || pTargetRecord == NULL )
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "pRecord is NULL", "can_normal_pack_item_swap", __FUNCTION__, __LINE__);
        return false;
    }

    const std::string& strOriginID = pOriginRecord->GetString( origin, EGIT_TYPE_CONFIGID );
    NF_SHARE_PTR<NFIPropertyManager> pOriginPropertyManager = m_pElementInfoModule->GetPropertyManager( strOriginID );

    if ( NULL == pOriginPropertyManager )
    {
        m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strOriginID, "There is no element", __FUNCTION__, __LINE__);

        return false;
    }

    int nOriginItemType = pOriginPropertyManager->GetElement( "ItemType" )->GetInt();

    // �ж���Ʒ�Ƿ�Ϊװ��
    if ( nOriginItemType != EGameItemType::EGIT_Equip )
    {
        m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strOriginID, "ItemType  is a error value", __FUNCTION__, __LINE__);

        return false;
    }

    if (m_pKernelModule->GetPropertyInt(self, "Level") < pOriginPropertyManager->GetElement("Level")->GetInt() )
    {
        return false;
    }

    // С���Ͳ�һ��
    if ( pOriginPropertyManager->GetElement( "ItemSubType" )->GetInt() != target )
    {
        return false;
    }

    // ְҵ����
    if ( pOriginPropertyManager->GetElement( "Job" )->GetInt() != m_pKernelModule->GetPropertyInt(self, "Job") )
    {
        return false;
    }

    // �Ա𲻷�
//     if ( pOriginPropertyManager->GetElement( "CharType" )->GetInt() != m_pKernelModule->GetPropertyInt(self, "CharType") )
//     {
//         return false;
//     }

    return true;
}

bool NFCPackModule::SwapGrid( const NFIDENTID& self, const int nOrigin, const int nTarget, const PackTableType origin_name/* = PackTableType::NormalPack*/, const PackTableType target_name/* = PackTableType::NormalPack*/ )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pOriginRecord = pObject->GetRecordManager()->GetElement( GetPackName( origin_name ) );
    NF_SHARE_PTR<NFIRecord> pTargetRecord = pObject->GetRecordManager()->GetElement( GetPackName( target_name ) );
    if ( NULL == pOriginRecord || NULL == pTargetRecord )
    {
        return false;
    }

    NFCDataList originRow;
    NFCDataList targetRow;
    
    bool bOrigin = pOriginRecord->QueryRow( nOrigin, originRow);
    bool bTarget = pTargetRecord->QueryRow( nTarget, targetRow );

    if (!bOrigin)
    {
        return false;
    }

    if ( pOriginRecord->GetInitData() == pTargetRecord->GetInitData() )
    {
        switch ( origin_name )
        {
        case NormalPack:
            {
                switch ( target_name )
                {
                case NormalPack:
                    {
                        if ( !can_normal_pack_item_swap( self, pOriginRecord, pTargetRecord, nOrigin, nTarget ) )
                        {
                            return false;
                        }

                        return pOriginRecord->SwapRowInfo(nOrigin, nTarget);
                    }
                    break;
                case BuyBackPack:
                    {
                        NFCDataList var( targetRow );

                        if ( !pTargetRecord->IsUsed( nTarget ) )
                        {
                            pTargetRecord->AddRow( nTarget, originRow );
                            pOriginRecord->Remove( nOrigin );
                        }
                        else
                        {
                            pTargetRecord->AddRow( nTarget, originRow );
                            pOriginRecord->AddRow( nOrigin, var );
                        }
                    }
                    return true;
                    break;
                default:
                    return false;
                }
            }
            break;
        case BuyBackPack:
            {
                switch ( target_name )
                {
                case NormalPack:
                    {
                        NFCDataList var( targetRow );

                        if ( !pTargetRecord->IsUsed( nTarget ) )
                        {
                            pTargetRecord->AddRow( nTarget, originRow );
                            pOriginRecord->Remove( nOrigin );
                        }
                        else
                        {
                            pTargetRecord->AddRow( nTarget, originRow );
                            pOriginRecord->AddRow( nOrigin, var );
                        }

                        return true;
                    }
                    break;
                default:
                    return false;
                }
            }
            break;
        default:
            return false;
        }
    }

    return false;
}

bool NFCPackModule::DeleteGrid( const NFIDENTID& self, const int nOrigin, const PackTableType name/* = PackTableType::NormalPack*/ )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( name ) );
        if ( pRecord )
        {
            pRecord->Remove( nOrigin );

            return true;
        }
    }

    return false;
}

bool NFCPackModule::SetGridCount( const NFIDENTID& self, const int nOrigin, const int nCount, const PackTableType name/* = PackTableType::NormalPack*/ )
{
    //�ж�nOrigin�Ϸ��ԣ��Լ�nCount�Ϸ���(��Ʒ�ѵ�����)
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( name ) );
        if ( pRecord )
        {
            pRecord->SetInt( nOrigin, EGameItemStoreType::EGIT_TYPE_ITEMCOUNT, nCount );

            return true;
        }
    }

    return false;
}

int NFCPackModule::GetGridCount( const NFIDENTID& self, const int nOrigin, const PackTableType name/* = PackTableType::NormalPack*/ )
{
    //�ж�nOrigin�Ϸ���
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( name ) );
        if ( pRecord )
        {
            return pRecord->GetInt( nOrigin, EGameItemStoreType::EGIT_TYPE_ITEMCOUNT );
        }
    }

    return 0;
}

bool NFCPackModule::SetEquipIntensiveLevel( const NFIDENTID& self, const int nOrigin, const int nLevel )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��),�������ǿ���ȼ�
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->SetInt( nOrigin, EGameItemStoreType::EGIT_TYPE_INTENSIVE_LEVEL, nLevel );
        }
    }

    return false;
}

int NFCPackModule::GetEquipIntensiveLevel( const NFIDENTID& self, const int nOrigin )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��)
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetInt( nOrigin, EGameItemStoreType::EGIT_TYPE_INTENSIVE_LEVEL );
        }
    }

    return 0;
}

bool NFCPackModule::SetEquipSlotCount( const NFIDENTID& self, const int nOrigin, const int nCount )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��),������������
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->SetInt( nOrigin, EGameItemStoreType::EGIT_TYPE_SOLTCOUNT, nCount );
        }
    }

    return false;
}

int NFCPackModule::GetEquipSlotCount( const NFIDENTID& self, const int nOrigin )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��)
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetInt( nOrigin, EGameItemStoreType::EGIT_TYPE_SOLTCOUNT );
        }
    }

    return 0;
}

bool NFCPackModule::SetEquipInlayCard( const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex, const std::string& strCardIndex )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��),eSlotIndex�Ƿ�Ϸ���Խ�磬�Ѿ��Ѿ��б�ʯ��strCardIndex�Ƿ���Ĵ��ڣ��Լ���ʯ�Ƿ������Ƕ
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->SetString( nOrigin, eSlotIndex, strCardIndex.c_str() );
        }
    }

    return false;
}

const std::string& NFCPackModule::GetEquipInlayCard( const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��),eSlotIndex�Ƿ�Ϸ���Խ��

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetString( nOrigin, eSlotIndex );
        }
    }

    return NULL_STR;
}

bool NFCPackModule::SetEquipEnchantmentLevel( const NFIDENTID& self, const int nOrigin, const int nLevel )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��),nLevel�Ƿ�Ϸ���Խ��

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->SetInt( nOrigin,  EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_LEVEL, nLevel );
        }
    }

    return false;
}

int NFCPackModule::GetEquipEnchantmentLevel( const NFIDENTID& self, const int nOrigin )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��)

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetInt( nOrigin,  EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_LEVEL );
        }
    }

    return 0;
}

bool NFCPackModule::SetEquipEnchantmentCard( const NFIDENTID& self, const int nOrigin, const std::string& strCardIndex )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��),strCardIndex�Ƿ���Ч

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->SetString( nOrigin,  EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_CARD, strCardIndex.c_str() );
        }
    }

    return false;
}

const std::string& NFCPackModule::GetEquipEnchantmentCard( const NFIDENTID& self, const int nOrigin )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��)

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetString( nOrigin,  EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_CARD );
        }
    }

    return NULL_STR;
}

bool NFCPackModule::SetEquipCreatTime( const NFIDENTID& self, const int nOrigin, const std::string& strTime )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��)

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->SetString( nOrigin,  EGameItemStoreType::EGIT_TYPE_CREATE_TIME, strTime.c_str() );
        }
    }

    return false;
}

const std::string& NFCPackModule::GetEquipCreatTime( const NFIDENTID& self, const int nOrigin )
{
    //�ж�nOrigin�Ϸ���(Խ���Լ��Ƿ���װ��)

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        if ( pRecord )
        {
            return pRecord->GetString( nOrigin,  EGameItemStoreType::EGIT_TYPE_CREATE_TIME );
        }
    }

    return NULL_STR;
}

bool NFCPackModule::CreateItem( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nCount )
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

    int nItemType = pPropertyType->GetInt();
    switch (nItemType)
    {
    case EGameItemType::EGIT_Item:
    case EGameItemType::EGIT_Material:
    case EGameItemType::EGIT_Task:
        {
            NF_SHARE_PTR<NFIProperty> pPropertySubType =  pPropertyManager->GetElement("ItemSubType");
// 			if (NULL != pPropertySubType)
// 			{
// 				int nItemSubType = pPropertySubType->GetInt();
// 				switch (nItemSubType)
// 				{
// 				case EGameItemSubType::EGIT_ITEM_GOLD:
// 					return m_pPropertyModule->AddMoney(self, nCount);
// 					break;
// 				case EGameItemSubType::EGIT_ITEM_SOUL_STONE:
// 					return m_pPropertyModule->AddSoulStone(self, nCount);
// 					break;
// 				case EGameItemSubType::EGIT_ITEM_ESSENCE_VALUE:
// 					break;
// 					return m_pPropertyModule->AddEssence(self, nCount);
// 				default:
// 					break;
// 				}
// 			}

            int nOverlayCount = pPropertyManager->GetElement("OverlayCount")->GetInt();
            int nNewCount = nCount;
            if (nCount > nOverlayCount)
            {
                nNewCount = nOverlayCount; // ��������ֵ
            }

            NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
            if (NULL == pRecord)
            {
                return false;
            }

            const int nPackCount = pObject->GetPropertyInt("PackCount");
            NFCDataList matchItemList;
            int nMatchCount = pRecord->FindString(EGIT_TYPE_CONFIGID, strConfigName.c_str(), matchItemList);

            // �����е��������ѵ�����
            for (int i = 0; i < nMatchCount; ++i)
            {
                int nRow = matchItemList.Int(i);
                if (nRow < mnPackStart)
                {// ǰ11������װ����Ʒ
                    continue;
                }

                int nCurrRowItemCount = pRecord->GetInt(nRow, EGIT_TYPE_ITEMCOUNT);
                if (nCurrRowItemCount == nOverlayCount)
                {// ������������
                    continue;
                }

                int nMissingValue = nOverlayCount - nCurrRowItemCount; // �������������ֵ
                if (nMissingValue >= nNewCount)
                {
                    pRecord->SetInt(nRow, EGIT_TYPE_ITEMCOUNT, nCurrRowItemCount + nNewCount);
                    return true;
                }
                else
                {
                    nNewCount -= nMissingValue;
                }
            }

            // �������ʣ��
            if (nNewCount > 0)
            {
                for (int i = mnPackStart; i < pRecord->GetRows(); ++i)
                {
                    if (!pRecord->IsUsed(i))
                    {
                        NFCDataList var;
                        var << m_pUUIDModule->CreateGUID();
                        var << strConfigName.c_str() ;
                        var << nNewCount;

                        NF_SHARE_PTR<NFIProperty> pBoundProperty = pPropertyManager->GetElement( "Bound" );
                        var << (pBoundProperty != NULL ? pBoundProperty->GetInt() : 0);
                        var << (NFINT32)eExpiredType;
                        var << 0;

                        for ( int j = 0; j < 6; j++ )
                        {
                            var << "";
                        }

                        var << 0;
                        var << 0;
                        var << "";
                        var << "";  // �������
                        var << NFTimeEx::GetNowTime();;   // TODO:��Ʒʱ��,���油��
						var << "";
                        int rowss = pRecord->AddRow( i, var );
                        return true;
                    }
                }
            }

            return true;
        }
        break;
    default:
        return false;
    }

    return false;
}

bool NFCPackModule::SetGridBan( const NFIDENTID& self, const int nOrigin, const bool bBan )
{
    return false;
}

bool NFCPackModule::GetGridBan( const NFIDENTID& self, const int nOrigin )
{
    return false;
}

int NFCPackModule::OnClassObjectEvent( const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
        m_pKernelModule->AddRecordCallBack( self, GetPackName( PackTableType::NormalPack ), this, &NFCPackModule::OnObjectPackViewRecordEvent );
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
    {
        //RefreshInitViewItem(self);
        //��һ�β�ˢ�£���Ϊ��һ��װ���󣬻��Զ�ˢ��
        RefreshEquipProperty(self);

        m_pKernelModule->AddRecordCallBack( self, GetPackName( PackTableType::NormalPack ), this, &NFCPackModule::OnObjectPackRecordEvent );
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
    {
        m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_SWAP_TABLE, this, &NFCPackModule::OnSwapTableRowEvent );
        m_pEventProcessModule->AddEventCallBack(self, NFED_ON_OBJECT_ENTER_SCENE_RESULT, this, &NFCPackModule::OnAddDropListEvent);

        // TOADD ����������Ҫ���ټӻص�
    }

    return 0;
}

int NFCPackModule::OnSwapTableRowEvent( const NFIDENTID& object, const int nEventID, const NFIDataList& var )
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
                        SwapGrid( object, nOrigin, nTarget, GetPackType( strOriginTableName ), GetPackType( strTargetTableName ) );
                    }
                }
            }
        }


    }

    return 0;
}

int NFCPackModule::OnObjectPackRecordEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{
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
                    if ( nRow >= 0 && nRow < mnPackStart)
                    {
                        const std::string& strConfigID = pPackRecord->GetString( nRow, ( int )EGameItemStoreType::EGIT_TYPE_CONFIGID );
                        AddEquipProperty(self, strConfigID, nRow);
                    }
                }
                break;

            case NFIRecord::RecordOptype::Del:
                {
                    if ( nRow >= 0 && nRow < mnPackStart)
                    {
                        const std::string& strOldConfigID = pPackRecord->GetString( nRow, ( int )EGameItemStoreType::EGIT_TYPE_CONFIGID );
                        RemoveEquipProperty(self, strOldConfigID, nRow);
                    }
                }
                break;

            case NFIRecord::RecordOptype::Swap:
                {
                    //col��Ŀ���У���װ��ʱ��col��0-10����11������
                    //��װ��ʱ��col�Ǵ�11��ʼ�ҵ�ַ,���col>=11��row��<11 pViewRecord->GetRows()��ʵ����11
                    if ( nCol >= 0 && nCol < mnPackStart)
                    {
                        //VIEW��ʾ����
                        const std::string& strConfigID = pPackRecord->GetString( nCol, ( int )EGameItemStoreType::EGIT_TYPE_CONFIGID );
                        if ( !strConfigID.empty() )
                        {
                            //////////////////////////////////////////////////////////////////////////
                            // ��װ������
                            const std::string& strOldConfigID = pPackRecord->GetString( nRow, ( int )EGameItemStoreType::EGIT_TYPE_CONFIGID );
                            if (nRow >= 0 && nRow < mnPackStart)
                            {
								RemoveEquipProperty(self, strOldConfigID, nRow);
                            }

                            // ��װ������
                            AddEquipProperty(self, strConfigID, nCol);
                        }
                    }
                }
                break;
            case NFIRecord::RecordOptype::UpData:
                {
                    //װ���Ĺ������ǲ��ܸ��µ�
                }
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

int NFCPackModule::OnObjectPackViewRecordEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nTargetRow, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pPackRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
        NF_SHARE_PTR<NFIRecord> pViewRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::ViewPack ) );
        if ( pPackRecord && pViewRecord )
        {
            switch ( nOpType )
            {
            case NFIRecord::RecordOptype::Add:
                {
                    if ( nRow >= 0 && nRow < pViewRecord->GetRows() )
                    {
                        const std::string& strConfigID = pPackRecord->GetString( nRow, ( int )EGameItemStoreType::EGIT_TYPE_CONFIGID );
                        int nIntensiveLevel = pPackRecord->GetInt( nRow, ( int )EGameItemStoreType::EGIT_TYPE_INTENSIVE_LEVEL );
                        const std::string& strConfigGemID = pPackRecord->GetString( nRow, ( int )EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_CARD );
                        int nEnchentmentLevel = pPackRecord->GetInt( nRow, ( int )EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_LEVEL );

                        NFCDataList val;
                        val << strConfigID.c_str();
                        val << nIntensiveLevel;
                        val << strConfigGemID.c_str();
                        val << nEnchentmentLevel;

                        pViewRecord->AddRow( nRow, val );
                    }
                }
                break;

            case NFIRecord::RecordOptype::Del:
                {
                    if ( nRow >= 0 && nRow < pViewRecord->GetRows() )
                    {
                        pViewRecord->Remove( nRow );
                    }
                }
                break;

            case NFIRecord::RecordOptype::Swap:
                {
                    //col��Ŀ���У���װ��ʱ��col��0-10����11������
                    //��װ��ʱ��col�Ǵ�11��ʼ�ҵ�ַ,���col>=11��row��<11 pViewRecord->GetRows()��ʵ����11
                    if ( nTargetRow >= 0 && nTargetRow < pViewRecord->GetRows() )
                    {
                        //VIEW��ʾ����
                        const std::string& strConfigID = pPackRecord->GetString( nTargetRow, ( int )EGameItemStoreType::EGIT_TYPE_CONFIGID );
                        if ( !strConfigID.empty() )
                        {
                            int nIntensiveLevel = pPackRecord->GetInt( nTargetRow, ( int )EGameItemStoreType::EGIT_TYPE_INTENSIVE_LEVEL );
                            const std::string& strConfigGemID = pPackRecord->GetString( nTargetRow, ( int )EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_CARD );
                            int nEnchentmentLevel = pPackRecord->GetInt( nTargetRow, ( int )EGameItemStoreType::EGIT_TYPE_ENCHANTMENT_LEVEL );

                            if (pViewRecord->IsUsed( nTargetRow ) )
                            {
                                pViewRecord->Remove(nTargetRow);
                            }

                            NFCDataList val;
                            val << strConfigID.c_str();
                            val << nIntensiveLevel;
                            val << strConfigGemID.c_str();
                            val << nEnchentmentLevel;

                            pViewRecord->AddRow( nTargetRow, val );
                        }
                    }
                    else if ( nRow >= 0 && nRow < pViewRecord->GetRows() )
                    {
                        pViewRecord->Remove( nRow );
                    }
                }
                break;
            case NFIRecord::RecordOptype::UpData:
                {
                    //װ���Ĺ������ǲ��ܸ��µ�
                }
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

const std::string& NFCPackModule::GetGridConfigID( const NFIDENTID& self, const int nRow, const PackTableType name/* = PackTableType::NormalPack*/ )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( name ) );
        if ( pRecord )
        {
            return pRecord->GetString( nRow, EGameItemStoreType::EGIT_TYPE_CONFIGID );
        }
    }

    return NULL_STR;
}

bool NFCPackModule::pack_item_type_greater( NFIDataList* elem1, NFIDataList* elem2 )
{
    std::string config_id1 = elem1->String( EGameItemStoreType::EGIT_TYPE_CONFIGID );
    int item_type1 = m_pElementInfoModule->GetPropertyInt( config_id1, "ItemType" );
    int item_subtype1 = m_pElementInfoModule->GetPropertyInt( config_id1, "ItemSubType" );

    std::string config_id2 = elem2->String( EGameItemStoreType::EGIT_TYPE_CONFIGID );
    int item_type2 = m_pElementInfoModule->GetPropertyInt( config_id2, "ItemType" );
    int item_subtype2 = m_pElementInfoModule->GetPropertyInt( config_id2, "ItemSubType" );

    if ( item_type1 != item_type2 )
    {
        return item_type1 < item_type2;
    }

    return item_subtype1 < item_subtype2;
}

int NFCPackModule::GetCanUsedCount(const NFIDENTID& self, const PackTableType name/* = PackTableType::NormalPack*/) const
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return 0;
    }

    NF_SHARE_PTR<NFIRecord> pNormalRecord = pObject->GetRecordManager()->GetElement( GetPackName(name) );
    if (NULL == pNormalRecord)
    {
        return 0;
    }

    int nCount = 0;
    for (int i = mnPackStart; i < pNormalRecord->GetRows(); ++i)
    {
        if (!pNormalRecord->IsUsed(i))
        {
            nCount++;
        }
    }

    return nCount;
}

bool NFCPackModule::SetGridData( const NFIDENTID& self, const int nRow, const int nCol, const NFIDataList& var, const PackTableType name /*= PackTableType::NormalPack*/ )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( name ) );
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

int NFCPackModule::RefreshEquipProperty( const NFIDENTID& self )
{
    // װ�����Լ���
    for (int i = 0; i < mnPackStart; ++i)
    {
        RefreshEquipProperty(self, i);
    }

    return 0;
}

int NFCPackModule::RefreshEquipProperty( const NFIDENTID& self, const int nRow )
{
    // ����װ�����Լ���
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( GetPackName( PackTableType::NormalPack ) );
    if ( NULL == pRecord )
    {
        return 1;
    }

    if(!pRecord->IsUsed(nRow))
    {
        return 1;
    }

    const std::string& strConfigID = pRecord->GetString( nRow, ( int )EGameItemStoreType::EGIT_TYPE_CONFIGID );
    if (strConfigID.empty())
    {
        return 1;
    }
	if(nRow > 0 && nRow <= mnPackStart)
	{
		AddEquipProperty(self, strConfigID, nRow);
	}
    return 0;
}

int NFCPackModule::AddEquipProperty( const NFIDENTID& self, const std::string& strConfigID, const int nRow )
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

int NFCPackModule::RemoveEquipProperty( const NFIDENTID& self, const std::string& strConfigID, const int nRow )
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

int NFCPackModule::CheckEquip()
{

    return 0;
}

int NFCPackModule::QueryCount( const NFIDENTID& self, const std::string& strItemConfigID ) const
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

    int nCount = 0;
    for (int i = mnPackStart; i < pRecord->GetRows(); ++i)
    {
        if (pRecord->IsUsed(i) && pRecord->GetString(i, EGameItemStoreType::EGIT_TYPE_CONFIGID) == strItemConfigID)
        {
            nCount += pRecord->GetInt(i, EGameItemStoreType::EGIT_TYPE_ITEMCOUNT);
        }
    }

    return nCount;
}

bool NFCPackModule::DeleteItem( const NFIDENTID& self, const std::string& strItemConfigID, const int nCount )
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
    for (int i = mnPackStart; i < pRecord->GetRows(); ++i)
    {
        if (nRemainDelCount <= 0)
        {
            break;
        }

        if (pRecord->IsUsed(i) && pRecord->GetString(i, EGameItemStoreType::EGIT_TYPE_CONFIGID) == strItemConfigID)
        {
            const int nGridCount = pRecord->GetInt(i, EGameItemStoreType::EGIT_TYPE_ITEMCOUNT);
            if (nGridCount < nRemainDelCount)
            {
                //�˸�����������
                nRemainDelCount -= nGridCount;
                pRecord->Remove(i);
            }
            else if (nGridCount == nRemainDelCount)
            {
                //�˸������պú���
                pRecord->Remove(i);
                return true;
            }
            else if (nGridCount > nRemainDelCount)
            {
                //������ɾ������ʣ��
                pRecord->SetInt(i, EGameItemStoreType::EGIT_TYPE_ITEMCOUNT, nGridCount - nRemainDelCount);
                return true;
            }
        }
    }

    return false;
}

int NFCPackModule::OnAddDropListEvent(const NFIDENTID& self, const int nEventID, const NFIDataList& var)
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
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
}