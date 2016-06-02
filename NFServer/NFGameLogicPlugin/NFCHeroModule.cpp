// -------------------------------------------------------------------------
//    @FileName      :    NFCHeroModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCHeroModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCHeroModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCHeroModule::Init()
{
    return true;
}

bool NFCHeroModule::Shut()
{
    return true;
}

bool NFCHeroModule::Execute()
{
    return true;
}

bool NFCHeroModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>("NFCCommonConfigModule");
    
    assert( NULL != m_pKernelModule );
	assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pGameServerNet_ServerModule);
    assert( NULL != m_pUUIDModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pCommonConfigModule);

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroMsg)){ return false;}

    std::string strEquipPath = pPluginManager->GetConfigPath();

    strEquipPath += "NFDataCfg/Ini/Common/FightHeroPos.xml";
    m_pCommonConfigModule->LoadConfig(strEquipPath);
    return true;
}

bool NFCHeroModule::BeforeShut()
{
    return true;
}

bool NFCHeroModule::AddHero( const NFGUID& self, const std::string& strID )
{
    NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
    if (nullptr == pHeroRecord.get())
    {
        return false;
    }

    NFCDataList xRowData = pHeroRecord->GetInitData();

    NFGUID xHeroID = m_pUUIDModule->CreateGUID();
    xRowData.SetObject(NFrame::Player::PlayerHero::PlayerHero_GUID, xHeroID);
    xRowData.SetString(NFrame::Player::PlayerHero::PlayerHero_ConfigID, strID.data());


    if (pHeroRecord->AddRow(-1, xRowData) < 0)
    {
        return false;
    }

    return true;
}

bool NFCHeroModule::AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int nExp)
{
    NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
    if (nullptr == pHeroRecord.get())
    {
        return false;
    }

    if (xHeroID.IsNull())
    {
        return false;
    }

    NFCDataList varFind;
    if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, varFind) <=0)
    {
        return false;
    }
    
    const int nRow = varFind.Int(0);
    NFCDataList varRowData;
    if (!pHeroRecord->QueryRow(nRow, varRowData))
    {
        return false;
    }

    const int nCurExp = varRowData.Int(NFrame::Player::PlayerHero::PlayerHero_Exp);
    const int nBeforeLevel = varRowData.Int(NFrame::Player::PlayerHero::PlayerHero_Level);

    int nLeftExp = nCurExp + nExp;
    int nAfterLevel = nBeforeLevel;
    for (int i = nBeforeLevel; i < ECONSTDEFINE_HERO_MAXLEVEL; i++)
    {
        const int nNeedExp = (i + 1) * ECONSTDEFINE_HERO_ONCELEVEEXP;

        if (nLeftExp >= nNeedExp)
        {
            nAfterLevel + 1;
            nLeftExp -= nNeedExp;
        }
        else
        {
            break;
        }
    }

    pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Exp, nLeftExp);
    pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Level, nAfterLevel);

    return true;
}

bool NFCHeroModule::HeroStarUp(const NFGUID& self, const NFGUID& xHeroID)
{
    NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
    if (nullptr == pHeroRecord.get())
    {
        return false;
    }

    if (xHeroID.IsNull())
    {
        return false;
    }

    NFCDataList varFind;
    if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, varFind) <= 0)
    {
        return false;
    }

    const int nRow = varFind.Int(0);
    const int nBeforeStar = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Star);

    int nAfterStar = nBeforeStar + 1;
    if (nAfterStar > ECONSTDEFINE_HERO_MAXSTAR)
    {
        nAfterStar = ECONSTDEFINE_HERO_MAXSTAR;
    }

    pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Star, nAfterStar);
    return true;
}

bool NFCHeroModule::HeroSkillUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex)
{
    NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
    if (nullptr == pHeroRecord.get())
    {
        return false;
    }

    if (xHeroID.IsNull())
    {
        return false;
    }

    NFCDataList xMatchList;
	int nMatchCout = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xMatchList);
	if (nMatchCout <= 0)
    {
        return false;
    }

    const int nRow = xMatchList.Int(0);
    NFCDataList varRowData;
    if (!pHeroRecord->QueryRow(nRow, varRowData))
    {
        return false;
    }

    if (nIndex > (NFrame::Player::PlayerHero::PlayerHero_Skill5 - NFrame::Player::PlayerHero::PlayerHero_Skill1)
		|| nIndex < 0)
    {
        return false;
    }

    int nSkillIDCol = NFrame::Player::PlayerHero::PlayerHero_Skill1 + nIndex;
    int nSkillLevleCol = NFrame::Player::PlayerHero::PlayerHero_SkillLevel1 + nIndex;

    const std::string strHeroID= varRowData.String(NFrame::Player::PlayerHero::PlayerHero_ConfigID);
    std::string strSkillID = varRowData.String(nSkillIDCol);
    const int nLevel = varRowData.Int(nSkillLevleCol);

    int nAfterLevel = nLevel + 1;
    if (strSkillID.empty())
    {
        const std::string& strSkillRef = m_pElementInfoModule->GetPropertyString(strHeroID, NFrame::NPC::SkillIDRef());

        switch (nSkillIDCol)
        {
        case NFrame::Player::PlayerHero::PlayerHero_Skill1:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL1());
            break;
        case NFrame::Player::PlayerHero::PlayerHero_Skill2:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL2());
            break;
        case NFrame::Player::PlayerHero::PlayerHero_Skill3:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL3());
            break;
        case NFrame::Player::PlayerHero::PlayerHero_Skill4:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL4());
            break;
        case NFrame::Player::PlayerHero::PlayerHero_Skill5:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL5());
            break;
		default:
			return 1;
			break;
        }

        pHeroRecord->SetString(nRow, nSkillIDCol, strSkillID.data());
        nAfterLevel = 1;
    }

    if (strSkillID.empty())
    {
        return false;
    }

    pHeroRecord->SetInt(nRow, nSkillLevleCol, nAfterLevel);
    return true;
}

bool NFCHeroModule::HeroTalentUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex)
{
    return true;
}

bool NFCHeroModule::SetFightHero(const NFGUID& self, const int nIndex, const NFGUID& xHeroID)
{
	return false;
}

bool NFCHeroModule::HeroWearDress(const NFGUID & self, const NFGUID & xHeroID, const NFGUID & xEquipID)
{
	return false;
}

bool NFCHeroModule::HeroUnDress(const NFGUID & self, const NFGUID & xHeroID, const NFGUID & xEquipID)
{
	return false;
}

bool NFCHeroModule::HeroWearSkill(const NFGUID & self, const NFGUID & xHeroID, const std::string & xEquipID)
{
	return false;
}

bool NFCHeroModule::HeroUnSkill(const NFGUID & self, const NFGUID & xHeroID, const std::string & xEquipID)
{
	return false;
}

void NFCHeroModule::OnSetFightHeroMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSetFightHero);

    const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());
    const int nFightPos = xMsg.fightpos();

    NF_SHARE_PTR<NFIRecord> pHeroList = m_pKernelModule->FindRecord(nPlayerID, NFrame::Player::R_PlayerHero());
    if (!pHeroList.get())
    {
        return;
    }


	/*
    NFCDataList varHeroList;
    if (pHeroList->FindObject(NFrame::Player::FightHeroList::FightHeroList_HeroGUID, xHero, varHeroList) <=0 )
    {
        return;
    }
    
    NF_SHARE_PTR<NFIRecord> pFightHeroList = m_pKernelModule->FindRecord(nPlayerID, NFrame::Player::R_FightHeroList());
    if (pFightHeroList.get())
    {
        NFCDataList varRowList;
        if (pFightHeroList->FindInt(NFrame::Player::FightHeroList::FightHeroList_FightPos, nFightPos, varRowList) > 0)
        {
            int nRow = varRowList.Int(0);
            if (pFightHeroList->SetObject(nRow, NFrame::Player::FightHeroList::FightHeroList_HeroGUID, xHero))
            {
                //SwichFightHero(nPlayerID, xHero, nFightPos);
            }
        }
        else
        {
            NFCDataList varList = pFightHeroList->GetInitData();
            
            varList.SetObject(NFrame::Player::FightHeroList::FightHeroList_HeroGUID, xHero);
            varList.SetInt(NFrame::Player::FightHeroList::FightHeroList_FightPos, nFightPos);

            if (pFightHeroList->AddRow(-1, varList) < 0)
            {
                //SwichFightHero(nPlayerID, xHero, nFightPos);
            }
        }
    }
	*/
}