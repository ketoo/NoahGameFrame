// -------------------------------------------------------------------------
//    @FileName         :    NFCEquipModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCEquipModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCEquipModule.h"
#include "NFComm/NFCore/NFTimer.h"
#include "NFComm/NFPluginModule/NFINetModule.h"


bool NFCEquipModule::Init()
{
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

	assert( NULL != m_pGameServerNet_ServerModule );

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_INTENSIFYLEVEL_TO_EQUIP, this, &NFCEquipModule::OnIntensifylevelToEquipMsg)){ return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_HOLE_TO_EQUIP, this, &NFCEquipModule::OnHoleToEquipMsg)){ return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_INLAYSTONE_TO_EQUIP, this, &NFCEquipModule::OnInlaystoneToEquipMsg)){ return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_ELEMENTLEVEL_TO_EQUIP, this, &NFCEquipModule::OnElementlevelToEquipMsg)){ return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_WEAR_EQUIP, this, &NFCEquipModule::OnReqWearEquipMsg)){ return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_TAKEOFF_EQUIP, this, &NFCEquipModule::OnTakeOffEquipMsg)){ return false; }
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
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>("NFCCommonConfigModule");
    
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pSceneProcessModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );
    assert( NULL != m_pUUIDModule );
	assert( NULL != m_pPackModule );
	assert( NULL != m_pCommonConfigModule );

    std::string strEquipPath = pPluginManager->GetConfigPath();

    strEquipPath += "NFDataCfg/Ini/Common/EqupConfig.xml";
    m_pCommonConfigModule->LoadConfig(strEquipPath);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCEquipModule::OnClassObjectEvent );
    return true;
}

bool NFCEquipModule::IntensifylevelToEquip( const NFGUID& self, const NFGUID& xEquipID )
{
    const int nLevel = GetEquipIntensifyLevel(self, xEquipID);

    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", lexical_cast<std::string>(nLevel), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", lexical_cast<std::string>(nLevel), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("IntensifylevelToEquip", lexical_cast<std::string>(nLevel), "ConstItem");

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
        return SetEquipIntensifyLevel(self, xEquipID, nAfterLevel);
    }
    else
    {
        const int nIsDownLevel = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", lexical_cast<std::string>(nLevel), "IsDownLevel");
        if (nIsDownLevel > 0)
        {
            int nAfterLevel = nLevel - 1;
            if (nAfterLevel < 0 )
            {
                nAfterLevel = 0;
            }

            return SetEquipIntensifyLevel(self, xEquipID, nAfterLevel);
        }

        return false;
    }

    return false;
}

bool NFCEquipModule::HoleToEquip( const NFGUID& self, const NFGUID& xEquipID )
{
    const int nHoleCount = GetEquipHoleCount(self, xEquipID);
    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("HoleToEquip", lexical_cast<std::string>(nHoleCount), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("HoleToEquip", lexical_cast<std::string>(nHoleCount), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("HoleToEquip", lexical_cast<std::string>(nHoleCount), "ConstItem");

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
        return SetEquipHoleCount(self, xEquipID, nAfterHoleCount);
    }

    return true;
}

bool NFCEquipModule::InlaystoneToEquip( const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex )
{
    if (nHoleIndex < NFrame::Player::BagEquipList::BagEquipList_InlayStone1 || nHoleIndex > NFrame::Player::BagEquipList::BagEquipList_InlayStone10)
    {
        return false;
    }

    const int nHoleCount = GetEquipHoleCount(self, xEquipID);
    if ( nHoleIndex > (NFrame::Player::BagEquipList::BagEquipList_InlayStone1 + nHoleCount))
    {
        return false;
    }

    const std::string& strIDCount = GetEquipInlayStoneID(self, xEquipID, (NFrame::Player::BagEquipList)nHoleIndex);
    if (!strIDCount.empty())
    {
        return false;
    }

    const int nHoleID = nHoleIndex - NFrame::Player::BagEquipList::BagEquipList_InlayStone1 + 1;
    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", lexical_cast<std::string>(nHoleID), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", lexical_cast<std::string>(nHoleID), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("ElementlevelToEquip", lexical_cast<std::string>(nHoleID), "ConstItem");

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

    return SetEquipInlayStoneID(self, xEquipID, (NFrame::Player::BagEquipList)nHoleIndex, strStoneID);
}

bool NFCEquipModule::ElementlevelToEquip( const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType )
{

    const int nElementIndex = NFrame::Player::BagEquipList::BagEquipList_ElementLevel1 + eElemetType;
    const int nElementLevel = GetEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex);

    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", lexical_cast<std::string>(nElementLevel), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", lexical_cast<std::string>(nElementLevel), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("ElementlevelToEquip", lexical_cast<std::string>(nElementLevel), "ConstItem");

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
        return SetEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex, nAfterLevel);
    }
    else
    {
        const int nIsDownLevel = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", lexical_cast<std::string>(nElementLevel), "IsDownLevel");
        if (nIsDownLevel > 0)
        {
            int nAfterLevel = nElementLevel - 1;
            if (nAfterLevel < 0 )
            {
                nAfterLevel = 0;
            }

            return SetEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex, nAfterLevel);
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
         m_pKernelModule->AddRecordCallBack( self, m_pPackModule->GetPackName( PackTableType::BagEquipPack ), this, &NFCEquipModule::OnObjectBagEquipRecordEvent );
    }

    return 0;
}


int NFCEquipModule::OnObjectBagEquipRecordEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar )
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
            if (NeedEquipProperty(self, newVar.GetObject())) // FIXME:RECORD
            {
                AddEquipProperty(self, xEventData.nRow);
            }
        }
        break;
    case NFIRecord::RecordOptype::Del:
        {
            if (NeedEquipProperty(self, oldVar.GetObject())) // FIXME:RECORD
            {
                RemoveEquipProperty(self, xEventData.nRow);
            }
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
            if (xEventData.nCol != NFrame::Player::BagEquipList::BagEquipList_WearGUID)
            {
                if (!NeedEquipProperty(self, oldVar.GetObject())) // FIXME:RECORD
                {
                    break;
                }
            }

            switch (xEventData.nCol)
            {
            case NFrame::Player::BagEquipList::BagEquipList_IntensifyLevel:
                {
                    const std::string& strConfigID = xRecord->GetString( xEventData.nRow, NFrame::Player::BagEquipList::BagEquipList_ConfigID );
                    const int nOldLevel = oldVar.GetInt();
                    const int nNewLevel = newVar.GetInt();
                    if (strConfigID.empty())
                    {
                        return 1;
                    }

                    const float nLevelRate = GetEquipLevelRate(nOldLevel);
                    RemoveEffectDataProperty(self, strConfigID, nLevelRate);
                    const float nNewLevelRate = GetEquipLevelRate(nNewLevel);
                    AddEffectDataProperty(self, strConfigID, nNewLevelRate);
                }
                break;

            case NFrame::Player::BagEquipList::BagEquipList_InlayStone1:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone2:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone3:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone4:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone5:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone6:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone7:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone8:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone9:
            case NFrame::Player::BagEquipList::BagEquipList_InlayStone10:
                {
                    const std::string& strOldeStoneID= oldVar.GetString();
                    const std::string& strNewStoneID= newVar.GetString();

                    RemoveEffectDataProperty(self, strOldeStoneID, 0);
                    AddEffectDataProperty(self, strNewStoneID, 0);
                }
                break;
            case NFrame::Player::BagEquipList::BagEquipList_WearGUID:
                {
                    const NFGUID& xOldeID= oldVar.GetObject();
                    const NFGUID& xNewID= newVar.GetObject();

                    if (NeedEquipProperty(self, xOldeID))
                    {
                        RemoveEquipProperty(self, xEventData.nRow);
                    }

                    if (NeedEquipProperty(self, xNewID))
                    {
                        AddEquipProperty(self, xEventData.nRow);
                    }
                }
                break;
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
        const std::string strStoneID = GetEquipInlayStoneID(self, xEuipID, (NFrame::Player::BagEquipList)i);
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
        const std::string strStoneID = GetEquipInlayStoneID(self, xEuipID, (NFrame::Player::BagEquipList)i);
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

bool NFCEquipModule::NeedEquipProperty( const NFGUID& self, const NFGUID& xWearID )
{
    return true;
}

bool NFCEquipModule::WearEquip( const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget )
{
    if (self == xTareget)
    {
        //给人装
    }
    else
    {
        //to do 
        //有没有装备给别人
        return DressEquipForHero(self, xTareget, xEquipID);
    }

    return true;
}

bool NFCEquipModule::TakeOffEquip( const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget )
{
    if (self == xTareget)
    {
        //给人装,暂时不支持
        return false;
    }
    else
    {
        return TakeOffEquipForm(self, xTareget, xEquipID);
    }

    return true;
}

bool NFCEquipModule::DressEquipForHero(const NFGUID& self, const NFGUID& hero, const NFGUID& id)
{
    if (id.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pEquipRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
    if (!pEquipRecord)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_PlayerHero());
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

bool NFCEquipModule::TakeOffEquipForm(const NFGUID& self, const NFGUID& hero, const NFGUID& id)
{
    if (id.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pEquipRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
    if (!pEquipRecord)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_PlayerHero());
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

int NFCEquipModule::SetEquipRandPropertyID(const NFGUID& self, const NFGUID& id, const std::string& strPropertyID)
{
    if (id.IsNull())
    {
        return 0;
    }

    //删除3个地方，背包，英雄穿戴，玩家穿戴
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return 0;
    }

    if (!m_pElementInfoModule->ExistElement(strPropertyID))
    {
        return 0;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

const std::string& NFCEquipModule::GetEquipRandPropertyID(const NFGUID& self, const NFGUID& id)
{
    if (id.IsNull())
    {
        return NULL_STR;
    }

    //删除3个地方，背包，英雄穿戴，玩家穿戴
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return NULL_STR;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

bool NFCEquipModule::SetEquipHoleCount(const NFGUID& self, const NFGUID& id, const int nCount)
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
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

int NFCEquipModule::GetEquipHoleCount(const NFGUID& self, const NFGUID& id)
{
    if (id.IsNull())
    {
        return 0;
    }

    //删除3个地方，背包，英雄穿戴，玩家穿戴
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return 0;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

bool NFCEquipModule::SetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const std::string& strPropertyID)
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
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

const std::string& NFCEquipModule::GetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex)
{
    if (id.IsNull())
    {
        return NULL_STR;
    }

    //删除3个地方，背包，英雄穿戴，玩家穿戴
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return NULL_STR;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

bool NFCEquipModule::SetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id, const int nLevel)
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
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

int NFCEquipModule::GetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id)
{
    if (id.IsNull())
    {
        return 0;
    }

    //删除3个地方，背包，英雄穿戴，玩家穿戴
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return 0;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

bool NFCEquipModule::SetEquipElementLevel(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const int nLevel)
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
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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

int NFCEquipModule::GetEquipElementLevel(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex)
{
    if (id.IsNull())
    {
        return 0;
    }

    //删除3个地方，背包，英雄穿戴，玩家穿戴
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return 0;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(m_pPackModule->GetPackName(PackTableType::BagEquipPack));
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
//////////////////////////////////////////////////////////////////////////
//msg process
void NFCEquipModule::OnIntensifylevelToEquipMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqIntensifylevelToEquip);

	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));

	int nResult = IntensifylevelToEquip(self, xEquipID);

	NFMsg::AckIntensifylevelToEquip xAck;

	*xAck.mutable_selfid() = xMsg.selfid();
	*xAck.mutable_equipid() = xMsg.equipid();
	xAck.set_result(nResult);

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_INTENSIFYLEVEL_TO_EQUIP, xAck, self);
}

void NFCEquipModule::OnHoleToEquipMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqHoleToEquip);

	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));

	int nResult = HoleToEquip(self, xEquipID);

	NFMsg::AckHoleToEquip xAck;

	*xAck.mutable_selfid() = xMsg.selfid();
	*xAck.mutable_equipid() = xMsg.equipid();
	xAck.set_result(nResult);

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_HOLE_TO_EQUIP, xAck, self);
}

void NFCEquipModule::OnInlaystoneToEquipMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqInlaystoneToEquip);

	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));
	const std::string& strStoneID = xMsg.stoneid();
	const int nHoleIndex = xMsg.hole_index();

	int nResult = InlaystoneToEquip(self, xEquipID, strStoneID, nHoleIndex);

	NFMsg::AckInlaystoneToEquip xAck;

	*xAck.mutable_selfid() = xMsg.selfid();
	*xAck.mutable_equipid() = xMsg.equipid();
	xAck.set_result(nResult);

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_INLAYSTONE_TO_EQUIP, xAck, self);
}

void NFCEquipModule::OnElementlevelToEquipMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqElementlevelToEquip);

	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));

	int nResult = ElementlevelToEquip(self, xEquipID, xMsg.eelementtype());

	NFMsg::AckElementlevelToEquip xAck;

	*xAck.mutable_selfid() = xMsg.selfid();
	*xAck.mutable_equipid() = xMsg.equipid();
	xAck.set_result(nResult);

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGEC_ACK_ELEMENTLEVEL_TO_EQUIP, xAck, self);
}

void NFCEquipModule::OnReqWearEquipMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqWearEquip);

	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));
	const NFGUID xTarget = NFINetModule::PBToNF((xMsg.targetid()));

	WearEquip(self, xEquipID, xTarget);
}

void NFCEquipModule::OnTakeOffEquipMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::TakeOffEquip);
	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));
	const NFGUID xTarget = NFINetModule::PBToNF((xMsg.targetid()));

	TakeOffEquip(self, xEquipID, xTarget);
}
