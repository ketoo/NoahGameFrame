// -------------------------------------------------------------------------
//    @FileName         :    NFCEquipModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCEquipModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCEquipModule.h"
#include "NFComm/Define/NFStringInfo.h"
#include "NFComm/NFCore/NFTimer.h"
#include "NFComm/NFCore/NFCCommonConfig.h"



bool NFCEquipModule::Init()
{
    return true;
}

bool NFCEquipModule::Shut()
{
    return true;
}

bool NFCEquipModule::Execute()
{
    //位置呢
    return true;
}

bool NFCEquipModule::AfterInit()
{
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>( "NFCEventProcessModule" );
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
    
    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );
    assert( NULL != m_pUUIDModule );
    assert( NULL != m_pPackModule );

    std::string strEquipPath = pPluginManager->GetConfigPath();

    strEquipPath += "NFDataCfg\Ini\Common\EqupConfig.xml";
    NFCCommonConfig::GetSingletonPtr()->LoadConfig(strEquipPath);

    std::string strPlayerPath = pPluginManager->GetConfigPath();
    strPlayerPath += "NFDataCfg\Ini\Common\PlayerLevelConfig.xml";
    NFCCommonConfig::GetSingletonPtr()->LoadConfig(strPlayerPath);

    m_pEventProcessModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCEquipModule::OnClassObjectEvent );
    return true;
}

bool NFCEquipModule::IntensifylevelToEquip( const NFGUID& self, const NFGUID& xEquipID )
{
    const int nLevel = m_pPackModule->GetEquipIntensifyLevel(self, xEquipID);

    const int nCostMoney = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "CostMoney");
    const int nConstItemCount = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "ConstItemCount");
    const std::string& strCostItemID = NFCCommonConfig::GetSingletonPtr()->GetAttributeString("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughMoney(self, nCostMoney))
    {
        return false;
    }
    
    if (!m_pPropertyModule->ConsumeMoney(self, nCostMoney))
    {
        return false;
    }
    
    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }
 
    if (RamdomSucess(nLevel, 100))
    {
        const int nAfterLevel = nLevel +1;
        m_pPackModule->SetEquipIntensifyLevel(self, xEquipID, nAfterLevel);

        return true;
    }
    else
    {
        const int nIsDownLevel = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "IsDownLevel");
        if (nIsDownLevel)
        {
            int nAfterLevel = nLevel - 1;
            if (nAfterLevel < 0 )
            {
                nAfterLevel = 0;
            }

            m_pPackModule->SetEquipIntensifyLevel(self, xEquipID, nAfterLevel);
        }
        return false;
    }

    return false;
}

bool NFCEquipModule::HoleToEquip( const NFGUID& self, const NFGUID& xEquipID )
{
    const int nHoleCount = m_pPackModule->GetEquipHoleCount(self, xEquipID);
    const int nCostMoney = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("HoleToEquip", boost::lexical_cast<std::string>(nHoleCount), "CostMoney");
    const int nConstItemCount = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("HoleToEquip", boost::lexical_cast<std::string>(nHoleCount), "ConstItemCount");
    const std::string& strCostItemID = NFCCommonConfig::GetSingletonPtr()->GetAttributeString("HoleToEquip", boost::lexical_cast<std::string>(nHoleCount), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->ConsumeMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }

    if (RamdomSucess(nHoleCount, 10))
    {
        const int nAfterHoleCount = nHoleCount +1;
        return m_pPackModule->SetEquipHoleCount(self, xEquipID, nAfterHoleCount);
    }
    return true;
}

bool NFCEquipModule::InlaystoneToEquip( const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex )
{
    if (nHoleIndex < NFrame::Player::BagEquipList::BagEquipList_InlayStone1 || nHoleIndex > NFrame::Player::BagEquipList::BagEquipList_InlayStone10)
    {
        return false;
    }

    const int nHoleCount = m_pPackModule->GetEquipHoleCount(self, xEquipID);
    if ( nHoleIndex > (NFrame::Player::BagEquipList::BagEquipList_InlayStone1 + nHoleCount))
    {
        return false;
    }

    const std::string& strIDCount = m_pPackModule->GetEquipInlayStoneID(self, xEquipID, (NFrame::Player::BagEquipList)nHoleIndex);
    if (!strIDCount.empty())
    {
        return false;
    }

    const int nHoleID = nHoleIndex - NFrame::Player::BagEquipList::BagEquipList_InlayStone1 + 1;
    const int nCostMoney = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nHoleID), "CostMoney");
    const int nConstItemCount = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nHoleID), "ConstItemCount");
    const std::string& strCostItemID = NFCCommonConfig::GetSingletonPtr()->GetAttributeString("ElementlevelToEquip", boost::lexical_cast<std::string>(nHoleID), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->ConsumeMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }


    if (!m_pPackModule->DeleteItem(self, strStoneID, 1))
    {
        return false;
    }

    return m_pPackModule->SetEquipInlayStoneID(self, xEquipID, (NFrame::Player::BagEquipList)nHoleIndex, strStoneID);
}

bool NFCEquipModule::ElementlevelToEquip( const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType )
{

    const int nElementIndex = NFrame::Player::BagEquipList::BagEquipList_ElementLevel1 + eElemetType;
    const int nElementLevel = m_pPackModule->GetEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex);

    const int nCostMoney = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "CostMoney");
    const int nConstItemCount = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "ConstItemCount");
    const std::string& strCostItemID = NFCCommonConfig::GetSingletonPtr()->GetAttributeString("ElementlevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "ConstItem");

    if (!strCostItemID.empty())
    {
        if (!m_pPackModule->EnoughItem(self, strCostItemID, nConstItemCount))
        {
            return false;
        }
    }

    if (!m_pPropertyModule->EnoughMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->ConsumeMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPackModule->DeleteItem(self, strCostItemID, nConstItemCount))
    {
        return false;
    }

    if (RamdomSucess(nElementLevel, 100))
    {
        const int nAfterLevel = nElementLevel + 1;
        return m_pPackModule->SetEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex, nAfterLevel);
    }
    else
    {
        const int nIsDownLevel = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "IsDownLevel");
        if (nIsDownLevel)
        {
            int nAfterLevel = nElementLevel - 1;
            if (nAfterLevel < 0 )
            {
                nAfterLevel = 0;
            }

            m_pPackModule->SetEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex, nAfterLevel);
        }
        return false;
    }

    return false;
}

bool NFCEquipModule::RamdomSucess( const int nNowLevel , const int nMaxLevel)
{
    if (nNowLevel >= nMaxLevel)
    {
        return false;
    }

    if (nNowLevel < 0)
    {
        return false;
    }

    if (nMaxLevel <= 0)
    {
        return false;
    }

    NFCDataList varList;
    m_pKernelModule->Random(0, nMaxLevel, 1, varList);

    const NFINT64 nRandomNum = varList.Int(0);

    if (nRandomNum > nNowLevel && nRandomNum <= nMaxLevel)
    {
        return true;
    }

    return false;
}


int NFCEquipModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
    {
        m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFCEquipModule::OnObjectLevelEvent);
         m_pKernelModule->AddRecordCallBack( self, m_pPackModule->GetPackName( PackTableType::BagEquipPack ), this, &NFCEquipModule::OnObjectBagEquipRecordEvent );
    }

    return 0;
}


int NFCEquipModule::OnObjectBagEquipRecordEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar )
{
    std::ostringstream stream;
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
    if (nullptr == xRecord)
    {
        return 0;
    }

    switch (xEventData.nOpType)
    {
    case NFIRecord::RecordOptype::Add:
        {
            AddEquipProperty(self, xEventData.nRow);
        }
        break;
    case NFIRecord::RecordOptype::Del:
        {
            RemoveEquipProperty(self, xEventData.nRow);
        }
        break;
    case NFIRecord::RecordOptype::Swap:
        {
            stream << " Trail Swap Row[" << xEventData.nRow << "] Row[" << xEventData.nCol << "]";
            m_pLogModule->LogRecord(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, xRecord->GetName(), stream.str(),  __FUNCTION__, __LINE__);
        }
        break;
    case NFIRecord::RecordOptype::Create:
        break;
    case NFIRecord::RecordOptype::UpData:
        {
            switch (xEventData.nCol)
            {
            case NFrame::Player::BagEquipList::BagEquipList_IntensifyLevel:
            default:
                break;
            }
        }
        break;
    case NFIRecord::RecordOptype::Cleared:
        {
        }
        break;
    case NFIRecord::RecordOptype::Sort:
        break;
    default:
        break;
    }

    return 0;
}


int NFCEquipModule::OnObjectLevelEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar )
{
    NFINT64 nLevel = newVar.GetInt();

    AddLevelUpAward(self, nLevel);
    return 0;
}

bool NFCEquipModule::AddLevelUpAward( const NFGUID& self, const int nLevel )
{
    const std::string& strAwardID = NFCCommonConfig::GetSingletonPtr()->GetAttributeString("PlayerLevel", boost::lexical_cast<std::string>(nLevel), "AwardPack");
    if (strAwardID.empty())
    {
        return true;
    }

    std::vector<std::string> xList;
    NFCCommonConfig::GetSingletonPtr()->GetStructItemList(strAwardID, xList);

    for (int i = 0; i < xList.size(); ++i)
    {
        const std::string& strItemID = xList[i];
        const int nCout = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("strAwardID", strItemID, "Count");
        if (m_pElementInfoModule->ExistElement(strItemID))
        {
            const int nItemType = m_pElementInfoModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
            switch (nItemType)
            {
            case NFMsg::EIT_EQUIP:
                {
                    m_pPackModule->CreateEquip(self, strItemID);
                }
                break;
            default:
                m_pPackModule->CreateItem(self, strItemID, nCout);
                break;
            }
        }
    }

    return true;
}


int NFCEquipModule::AddEquipProperty( const NFGUID& self )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( m_pPackModule->GetPackName( PackTableType::BagEquipPack ) );
    if ( NULL == pRecord )
    {
        return 1;
    }

    // 装备属性计算
    for (int i = 0; i < pRecord->GetRows(); ++i)
    {
        if (pRecord->IsUsed(i))
        {
            AddEquipProperty(self, i);
        }
    }

    return 0;
}

int NFCEquipModule::AddEquipProperty( const NFGUID& self, const int nRow )
{
    // 单件装备属性计算
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( m_pPackModule->GetPackName( PackTableType::BagEquipPack ) );
    if ( NULL == pRecord )
    {
        return 1;
    }

    if(!pRecord->IsUsed(nRow))
    {
        return 1;
    }

    const NFGUID& xEuipID = pRecord->GetObject( nRow, NFrame::Player::BagEquipList::BagEquipList_GUID );
    const std::string& strConfigID = pRecord->GetString( nRow, NFrame::Player::BagEquipList::BagEquipList_ConfigID );
    const int& nLevel = pRecord->GetInt( nRow, NFrame::Player::BagEquipList::BagEquipList_IntensifyLevel );
    if (strConfigID.empty())
    {
        return 1;
    }

    const float nLevelRate = GetEquipLevelRate(nLevel);
    AddEffectDataProperty(self, strConfigID, nLevelRate);

    //Stone
    for (int i = NFrame::Player::BagEquipList::BagEquipList_InlayStone1; i <= NFrame::Player::BagEquipList::BagEquipList_InlayStone10; ++i)
    {
        const std::string strStoneID = m_pPackModule->GetEquipInlayStoneID(self, xEuipID, (NFrame::Player::BagEquipList)i);
        AddEffectDataProperty(self, strStoneID, 0);
    }

    return 0;
}


int NFCEquipModule::RemoveEquipProperty( const NFGUID& self, const int nRow )
{
    // 单件装备属性计算
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( NULL == pObject )
    {
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement( m_pPackModule->GetPackName( PackTableType::BagEquipPack ) );
    if ( NULL == pRecord )
    {
        return 1;
    }

    if(!pRecord->IsUsed(nRow))
    {
        return 1;
    }

    const NFGUID& xEuipID = pRecord->GetObject( nRow, NFrame::Player::BagEquipList::BagEquipList_GUID );
    const std::string& strConfigID = pRecord->GetString( nRow, NFrame::Player::BagEquipList::BagEquipList_ConfigID );
    const int& nLevel = pRecord->GetInt( nRow, NFrame::Player::BagEquipList::BagEquipList_IntensifyLevel );
    if (strConfigID.empty())
    {
        return 1;
    }

    const float nLevelRate = GetEquipLevelRate(nLevel);
    RemoveEffectDataProperty(self, strConfigID, nLevelRate);

    //Stone
    for (int i = NFrame::Player::BagEquipList::BagEquipList_InlayStone1; i <= NFrame::Player::BagEquipList::BagEquipList_InlayStone10; ++i)
    {
        const std::string strStoneID = m_pPackModule->GetEquipInlayStoneID(self, xEuipID, (NFrame::Player::BagEquipList)i);
        RemoveEffectDataProperty(self, strStoneID, 0);
    }

    return 0;
}

int NFCEquipModule::AddEffectDataProperty( const NFGUID& self, const std::string& strConfigID, const float fAddRate)
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
            const NFINT64 nOldValue = pProperty->GetInt();
            const NFINT64 nNewVaue = (float)nOldValue * fAddRate;
            m_pPropertyModule->AddPropertyValue( self, pProperty->GetKey(), NFIPropertyModule::NPG_EQUIP, nNewVaue);
        }

        pProperty = pEffectDataPropertyManager->Next();
    }

    return 0;
}

int NFCEquipModule::RemoveEffectDataProperty( const NFGUID& self, const std::string& strConfigID, const float fAddRate)
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
            const NFINT64 nOldValue = pProperty->GetInt();
            const NFINT64 nNewVaue = (float)nOldValue * fAddRate;
            m_pPropertyModule->SubPropertyValue( self, pProperty->GetKey(), NFIPropertyModule::NPG_EQUIP, nNewVaue);
        }

        pProperty = pEffectDataPropertyManager->Next();
    }

    return 0;
}

float NFCEquipModule::GetEquipLevelRate( const int nNowLevel )
{
    float fRate = 0.5f * nNowLevel;
    return fRate ;
}
