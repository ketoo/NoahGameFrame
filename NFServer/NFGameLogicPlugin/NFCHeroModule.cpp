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
    
    assert( NULL != m_pKernelModule );
	assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pGameServerNet_ServerModule);
    assert( NULL != m_pUUIDModule);
    assert( NULL != m_pElementInfoModule);

	
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroProcess)){ return false;}



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

    NFCDataList varFind;
    if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, varFind) <= 0)
    {
        return false;
    }

    const int nRow = varFind.Int(0);
    NFCDataList varRowData;
    if (!pHeroRecord->QueryRow(nRow, varRowData))
    {
        return false;
    }

    if (nIndex > (NFrame::Player::PlayerHero::PlayerHero_Skill5 - NFrame::Player::PlayerHero::PlayerHero_Skill1))
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
        case 0:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL1());
            break;
        case 1:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL2());
            break;
        case 2:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL3());
            break;
        case 3:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL4());
            break;
        case 4:
            strSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL5());
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

bool NFCHeroModule::SetFightHero(const NFGUID& self, const NFGUID& xID)
{
    return true;
}

void NFCHeroModule::OnSetFightHeroProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSetFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());
	SetFightHero(nPlayerID, xHero);
}