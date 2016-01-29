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
#include "NFComm/NFPluginModule/NFINetModule.h"


bool NFCEquipModule::Init()
{
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

	assert( NULL != m_pGameServerNet_ServerModule );

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_INTENSIFYLEVEL_TO_EQUIP, this, &NFCEquipModule::OnIntensifylevelToEquipProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_HOLE_TO_EQUIP, this, &NFCEquipModule::OnHoleToEquipProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_INLAYSTONE_TO_EQUIP, this, &NFCEquipModule::OnInlaystoneToEquipProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_REQ_ELEMENTLEVEL_TO_EQUIP, this, &NFCEquipModule::OnElementlevelToEquipProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_WEAR_EQUIP, this, &NFCEquipModule::OnReqWearEquipProcess)){ return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGEC_TAKEOFF_EQUIP, this, &NFCEquipModule::OnTakeOffEquipProcess)){ return false; }
    return true;
}

bool NFCEquipModule::Shut()
{
    return true;
}

bool NFCEquipModule::Execute()
{
    //λ����
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
    const int nLevel = m_pPackModule->GetEquipIntensifyLevel(self, xEquipID);

    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "ConstItem");

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
        const int nIsDownLevel = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nLevel), "IsDownLevel");
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
    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("HoleToEquip", boost::lexical_cast<std::string>(nHoleCount), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("HoleToEquip", boost::lexical_cast<std::string>(nHoleCount), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("HoleToEquip", boost::lexical_cast<std::string>(nHoleCount), "ConstItem");

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
    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nHoleID), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nHoleID), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("ElementlevelToEquip", boost::lexical_cast<std::string>(nHoleID), "ConstItem");

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

    const int nCostMoney = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "CostMoney");
    const int nConstItemCount = m_pCommonConfigModule->GetAttributeInt("ElementlevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "ConstItemCount");
    const std::string& strCostItemID = m_pCommonConfigModule->GetAttributeString("ElementlevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "ConstItem");

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
        const int nIsDownLevel = m_pCommonConfigModule->GetAttributeInt("IntensifylevelToEquip", boost::lexical_cast<std::string>(nElementLevel), "IsDownLevel");
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

    // װ�����Լ���
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
    // ����װ�����Լ���
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
    // ����װ�����Լ���
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

bool NFCEquipModule::NeedEquipProperty( const NFGUID& self, const NFGUID& xWearID )
{
    return true;
}

bool NFCEquipModule::WearEquip( const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget )
{
    if (self == xTareget)
    {
        //����װ
    }
    else
    {
        //to do 
        //��û��װ��������
        return m_pPackModule->DressEquipForHero(self, xTareget, xEquipID);
    }

    return true;
}

bool NFCEquipModule::TakeOffEquip( const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget )
{
    if (self == xTareget)
    {
        //����װ
    }
    else
    {
        return m_pPackModule->TakeOffEquipForm(self, xTareget, xEquipID);
    }

    return true;
}

void NFCEquipModule::OnIntensifylevelToEquipProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
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

void NFCEquipModule::OnHoleToEquipProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
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

void NFCEquipModule::OnInlaystoneToEquipProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
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

void NFCEquipModule::OnElementlevelToEquipProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
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

void NFCEquipModule::OnReqWearEquipProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqWearEquip);

	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));
	const NFGUID xTarget = NFINetModule::PBToNF((xMsg.targetid()));

	WearEquip(self, xEquipID, xTarget);
}

void NFCEquipModule::OnTakeOffEquipProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::TakeOffEquip);
	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));
	const NFGUID xTarget = NFINetModule::PBToNF((xMsg.targetid()));

	TakeOffEquip(self, xEquipID, xTarget);
}
