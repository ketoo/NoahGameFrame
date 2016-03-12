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

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCHeroModule::OnObjectClassEvent);

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


int NFCHeroModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OnlineCount());
            m_pKernelModule->SetPropertyInt(self, NFrame::Player::OnlineCount(), (nOnlineCount + 1));

            m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::FightHero(), this, &NFCHeroModule::OnObjectFightHeroEvent);
        }
    }

    return 0;
}

int NFCHeroModule::OnObjectFightHeroEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    const NFGUID& xOldHero = oldVar.GetObject();
    const NFGUID& xNewHero = newVar.GetObject();

    if (!xOldHero.IsNull())
    {
        //Clear Property

        //Clear Skill 
        NF_SHARE_PTR<NFIRecord> pSkillRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_SkillTable());
        NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
        if (pSkillRecord.get() && pHeroRecord.get())
        {
            NFCDataList varHeroList;
            if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::PlayerHero_GUID, xOldHero, varHeroList) > 0)
            {
                int nHeroRow = varHeroList.Int(0);
                NFCDataList varHeroData;
                if (pHeroRecord->QueryRow(nHeroRow, varHeroData))
                {
                    for (int iSkillIndex = 0; iSkillIndex < (NFrame::Player::PlayerHero::PlayerHero_Skill5 - NFrame::Player::PlayerHero::PlayerHero_Skill1); iSkillIndex++)
                    {
                        const string& strSkillID = varHeroData.String(iSkillIndex + NFrame::Player::PlayerHero::PlayerHero_Skill1);
                        if (!strSkillID.empty())
                        {
                            NFCDataList varFindSkillList;
                            if (pSkillRecord->FindString(NFrame::Player::SkillTable_SkillID, strSkillID.data(), varFindSkillList) > 0)
                            {
                                pSkillRecord->Remove(varFindSkillList);
                            }
                        }
                    }
                }
            }
        }
    }

    if (!xNewHero.IsNull())
    {
        //Add Property
        //Add Skill
        NF_SHARE_PTR<NFIRecord> pSkillRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_SkillTable());
        NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
        if (pSkillRecord.get() && pHeroRecord.get())
        {
            NFCDataList varHeroList;
            if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::PlayerHero_GUID, xNewHero, varHeroList) > 0)
            {
                int nHeroRow = varHeroList.Int(0);
                NFCDataList varHeroData;
                if (pHeroRecord->QueryRow(nHeroRow, varHeroData))
                {
                    for (int iSkillIndex = 0; iSkillIndex < (NFrame::Player::PlayerHero::PlayerHero_Skill5 - NFrame::Player::PlayerHero::PlayerHero_Skill1); iSkillIndex++)
                    {
                        const string& strSkillID = varHeroData.String(iSkillIndex + NFrame::Player::PlayerHero::PlayerHero_Skill1);
                        const int nSkillLevel= varHeroData.Int(iSkillIndex + NFrame::Player::PlayerHero::PlayerHero_SkillLevel1);
                        if (!strSkillID.empty())
                        {
                            NFCDataList varSkillData = pSkillRecord->GetInitData();
                            varSkillData.SetInt(NFrame::Player::SkillTable_SkillLevel, nSkillLevel);
                            varSkillData.SetString(NFrame::Player::SkillTable_SkillID, strSkillID.data());

                            pSkillRecord->AddRow(-1, varSkillData);
                        }
                    }
                }
            }
        }
    }

    return 0;
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

bool NFCHeroModule::SetFightHero(const NFGUID& self, const NFGUID& xID)
{
    return m_pKernelModule->SetPropertyObject(self, "FightHero", xID);
}

void NFCHeroModule::OnSetFightHeroProcess( const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen )
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSetFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());
	SetFightHero(nPlayerID, xHero);
}