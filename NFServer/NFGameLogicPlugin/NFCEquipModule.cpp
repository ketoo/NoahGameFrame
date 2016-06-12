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
	return true;
}

bool NFCEquipModule::Shut()
{
    return true;
}

bool NFCEquipModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCEquipModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    std::string strEquipPath = pPluginManager->GetConfigPath();

    strEquipPath += "NFDataCfg/Ini/Common/EqupConfig.xml";
    m_pCommonConfigModule->LoadConfig(strEquipPath);

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_INTENSIFYLEVEL_TO_EQUIP, this, &NFCEquipModule::OnIntensifylevelToEquipMsg)) { return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_HOLE_TO_EQUIP, this, &NFCEquipModule::OnHoleToEquipMsg)) { return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_INLAYSTONE_TO_EQUIP, this, &NFCEquipModule::OnInlaystoneToEquipMsg)) { return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_REQ_ELEMENTLEVEL_TO_EQUIP, this, &NFCEquipModule::OnElementlevelToEquipMsg)) { return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_WEAR_EQUIP, this, &NFCEquipModule::OnReqWearEquipMsg)) { return false; }
	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGEC_TAKEOFF_EQUIP, this, &NFCEquipModule::OnTakeOffEquipMsg)) { return false; }

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
 
    if (Ramdom(nLevel, 100))
    {
        return AddEquipIntensifyLevel(self, xEquipID);
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

    if (Ramdom(nHoleCount, 10))
    {
        return AddEquipHoleCount(self, xEquipID);
    }

    return true;
}

bool NFCEquipModule::InlaystoneToEquip( const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex )
{
	if (xEquipID.IsNull() || self.IsNull() || strStoneID.empty())
	{
		return false;
	}

	if (!m_pElementModule->ExistElement(strStoneID))
	{
		return false;
	}

    if (nHoleIndex < NFrame::Player::BagEquipList::BagEquipList_InlayStone1
		|| nHoleIndex > NFrame::Player::BagEquipList::BagEquipList_InlayStone10)
    {
        return false;
    }

    const int nHoleCount = GetEquipHoleCount(self, xEquipID);
    if ((nHoleIndex - NFrame::Player::BagEquipList::BagEquipList_InlayStone1) <= nHoleCount)
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

    const int nElementIndex = NFrame::Player::BagEquipList::BagEquipList_ElementLevel1_FIRE + eElemetType;
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

    if (Ramdom(nElementLevel, 100))
    {
        const int nAfterLevel = nElementLevel + 1;
        return AddEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex);
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

            return AddEquipElementLevel(self, xEquipID, (NFrame::Player::BagEquipList)nElementIndex);
        }

        return false;
    }

    return false;
}

bool NFCEquipModule::Ramdom( const int nNowLevel , const int nMaxLevel)
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

bool NFCEquipModule::DressEquipForHero(const NFGUID& self, const NFGUID& hero, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull() || hero.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pBagRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_PlayerHero());
	if (!pHeroRecord)
	{
		return false;
	}

	NFCDataList xEquipDataList;
	pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xEquipDataList);
	if (xEquipDataList.GetCount() != 1)
	{
		return false;
	}

	NFCDataList xHeroDataList;
	pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, id, xHeroDataList);
	if (xHeroDataList.GetCount() != 1)
	{
		return false;
	}

	const int nEquipRow = xEquipDataList.Int(0);
	const int nHeroRow = xHeroDataList.Int(0);
	const std::string& strEquipID = pBagRecord->GetString(nEquipRow, NFrame::Player::BagEquipList_ConfigID);
	const int nEquipPos = m_pElementModule->GetPropertyInt(strEquipID, NFrame::Equip::ItemSubType());

    if (nEquipRow < 0
		|| nHeroRow < 0
		|| strEquipID.empty()
		|| nEquipPos < 0
		|| nEquipPos > (NFrame::Player::PlayerHero_Equip6 - NFrame::Player::PlayerHero_Equip1))
    {
		return false;
    }

	//so there have any bind?
	//hero, position
	pHeroRecord->SetObject(nHeroRow, nEquipPos + NFrame::Player::PlayerHero_Equip1, id);
	pBagRecord->SetObject(nEquipRow, NFrame::Player::BagEquipList_WearGUID, hero);

    return false;
}

bool NFCEquipModule::TakeOffEquipForm(const NFGUID& self, const NFGUID& hero, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull() || hero.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pBagRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_PlayerHero());
	if (!pHeroRecord)
	{
		return false;
	}

	NFCDataList xEquipDataList;
	pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xEquipDataList);
	if (xEquipDataList.GetCount() != 1)
	{
		return false;
	}

	NFCDataList xHeroDataList;
	pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, id, xHeroDataList);
	if (xHeroDataList.GetCount() != 1)
	{
		return false;
	}

	const int nEquipRow = xEquipDataList.Int(0);
	const int nHeroRow = xHeroDataList.Int(0);
	const std::string& strEquipID = pBagRecord->GetString(nEquipRow, NFrame::Player::BagEquipList_ConfigID);
	const int nEquipPos = m_pElementModule->GetPropertyInt(strEquipID, NFrame::Equip::ItemSubType());

	if (nEquipRow < 0
		|| nHeroRow < 0
		|| strEquipID.empty()
		|| nEquipPos < 0
		|| nEquipPos >(NFrame::Player::PlayerHero_Equip6 - NFrame::Player::PlayerHero_Equip1))
	{
		return false;
	}

	//so there have any bind?
	//hero, position
	pHeroRecord->SetObject(nHeroRow, nEquipPos + NFrame::Player::PlayerHero_Equip1, NFGUID());
	pBagRecord->SetObject(nEquipRow, NFrame::Player::BagEquipList_WearGUID, NFGUID());
}

bool NFCEquipModule::SetEquipRandPropertyID(const NFGUID& self, const NFGUID& id, const std::string& strPropertyID)
{
	if (id.IsNull() || self.IsNull() || strPropertyID.empty())
	{
		return false;
	}

	if (!m_pElementModule->ExistElement(strPropertyID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pRecord)
	{
		return false;
	}

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	pRecord->SetString(nRow, NFrame::Player::BagEquipList_RandPropertyID, strPropertyID);

	return true;
}

bool NFCEquipModule::AddEquipHoleCount(const NFGUID& self, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pRecord)
	{
		return false;
	}

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	const int nSoltCount = pRecord->GetInt(nRow, NFrame::Player::BagEquipList_SlotCount);
	pRecord->SetInt(nRow, NFrame::Player::BagEquipList_SlotCount, nSoltCount + 1);

	return true;
}

int NFCEquipModule::GetEquipHoleCount(const NFGUID & self, const NFGUID & id)
{
	if (id.IsNull() || self.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pRecord)
	{
		return false;
	}

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	return pRecord->GetInt(nRow, NFrame::Player::BagEquipList_SlotCount);
}

bool NFCEquipModule::SetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const std::string& strStoneID)
{
	if (id.IsNull() || self.IsNull() || strStoneID.empty())
	{
		return false;
	}

	if (eIndex > NFrame::Player::BagEquipList_InlayStone10
		|| eIndex < NFrame::Player::BagEquipList_InlayStone1)
	{
		return false;
	}

	if (!m_pElementModule->ExistElement(strStoneID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pRecord)
	{
		return false;
	}

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	const int nSoltCount = pRecord->GetInt(nRow, NFrame::Player::BagEquipList_SlotCount);
	if ((eIndex - NFrame::Player::BagEquipList_InlayStone1) <= nSoltCount)
	{
		return false;
	}

	pRecord->SetString(nRow, eIndex, strStoneID);

	return true;
}

bool NFCEquipModule::AddEquipIntensifyLevel(const NFGUID& self, const NFGUID& id)
{
	if (id.IsNull() || self.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pRecord)
	{
		return false;
	}

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	const int nLevel = pRecord->GetInt(nRow, NFrame::Player::BagEquipList_IntensifyLevel);
	pRecord->SetInt(nRow, NFrame::Player::BagEquipList_IntensifyLevel, nLevel + 1);

	return true;
}

int NFCEquipModule::GetEquipIntensifyLevel(const NFGUID & self, const NFGUID & id)
{
	if (id.IsNull() || self.IsNull())
	{
		return -1;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return -1;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pRecord)
	{
		return -1;
	}

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return -1;
	}

	const int nRow = xDataList.Int(0);
	return pRecord->GetInt(nRow, NFrame::Player::BagEquipList_IntensifyLevel);
}

bool NFCEquipModule::AddEquipElementLevel(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex)
{
    if (id.IsNull() || self.IsNull())
    {
        return false;
    }

	if (eIndex > NFrame::Player::BagEquipList_ElementLevel5_POISON
		|| eIndex < NFrame::Player::BagEquipList_ElementLevel1_FIRE)
	{
		return false;
	}

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
    if (!pRecord)
    {
        return false;
    }

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	const int nLevel = GetEquipElementLevel(self, id, eIndex);
	pRecord->SetInt(nRow, eIndex, nLevel + 1);

    return true;
}

int NFCEquipModule::GetEquipElementLevel(const NFGUID & self, const NFGUID & id, NFrame::Player::BagEquipList eIndex)
{
	if (id.IsNull() || self.IsNull())
	{
		return -1;
	}

	if (eIndex > NFrame::Player::BagEquipList_ElementLevel5_POISON
		|| eIndex < NFrame::Player::BagEquipList_ElementLevel1_FIRE)
	{
		return -1;
	}

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (NULL == pObject)
	{
		return -1;
	}

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_BagEquipList());
	if (!pRecord)
	{
		return -1;
	}

	NFCDataList xDataList;
	pRecord->FindObject(NFrame::Player::BagEquipList_GUID, id, xDataList);
	if (xDataList.GetCount() != 1)
	{
		return -1;
	}

	const int nRow = xDataList.Int(0);
	return pRecord->GetInt(nRow, eIndex);
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

	DressEquipForHero(self, xTarget, xEquipID);
}

void NFCEquipModule::OnTakeOffEquipMsg( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::TakeOffEquip);
	const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
	const NFGUID xEquipID = NFINetModule::PBToNF((xMsg.equipid()));
	const NFGUID xTarget = NFINetModule::PBToNF((xMsg.targetid()));

	TakeOffEquipForm(self, xTarget, xEquipID);
}
