// -------------------------------------------------------------------------
//    @FileName			:   NFCBriefSkillConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBriefSkillConsumeProcessModule
// -------------------------------------------------------------------------

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFCBriefSkillConsumeProcessModule.h"

bool NFCBriefSkillConsumeProcessModule::Init()
{
 
    return true;
}

bool NFCBriefSkillConsumeProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSkillModule = pPluginManager->FindModule<NFISkillModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();

    return true;
}

bool NFCBriefSkillConsumeProcessModule::Shut()
{
    return true;
}

bool NFCBriefSkillConsumeProcessModule::Execute()
{
    return true;
}


int NFCBriefSkillConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFDataList& other )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject == NULL )
	{
	    return 1;
	}

    return 0;
}

int NFCBriefSkillConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strSkillName, const NFDataList& other, NFDataList& damageListValue, NFDataList& damageResultList )
{
    if (!m_pElementModule->ExistElement(strSkillName))
    {
        return 1;
    }

	const std::string& strConsumeProperty = m_pElementModule->GetPropertyString(strSkillName, NFrame::Skill::ConsumeProperty());
	const NFINT64 nConsumeValue = m_pElementModule->GetPropertyInt(strSkillName, NFrame::Skill::ConsumeValue());
	const NFINT64 nConsumeTYpe = m_pElementModule->GetPropertyInt(strSkillName, NFrame::Skill::ConsumeType());

	const std::string& strDamageProperty = m_pElementModule->GetPropertyString(strSkillName, NFrame::Skill::DamageProperty());
	const NFINT64 nDamageCnfValue = m_pElementModule->GetPropertyInt(strSkillName, NFrame::Skill::DamageValue());
	const NFINT64 nDamageTYpe = m_pElementModule->GetPropertyInt(strSkillName, NFrame::Skill::DamageType());

	const std::string& strGetBuffList = m_pElementModule->GetPropertyString(strSkillName, NFrame::Skill::GetBuffList());
	const std::string& strSendBuffList = m_pElementModule->GetPropertyString(strSkillName, NFrame::Skill::SendBuffList());

	const double fRequireDistance = m_pElementModule->GetPropertyFloat(strSkillName, NFrame::Skill::RequireDistance());
	const double fDamageDistance = m_pElementModule->GetPropertyFloat(strSkillName, NFrame::Skill::DamageDistance());
	const NFINT64 nTargetType = m_pElementModule->GetPropertyInt(strSkillName, NFrame::Skill::TargetType());

	int64_t nOldConsumeVaue = m_pKernelModule->GetPropertyInt(self, strConsumeProperty);
	nOldConsumeVaue -= nConsumeValue;
	if (nOldConsumeVaue < 0)
	{
		return 2;
	}

	m_pKernelModule->SetPropertyInt(self, strConsumeProperty, nOldConsumeVaue);

	NFINT64 nDamageValue = m_pKernelModule->Random(nDamageCnfValue, nDamageCnfValue * 1.2f);

    for ( int j = 0; j < other.GetCount(); j++ )
    {
        NFGUID identOther = other.Object( j );
        if ( identOther.IsNull() )
        {
			damageListValue.AddInt(0);
			damageResultList.AddInt(0);
            continue;
        }

        NF_SHARE_PTR<NFIObject> pOtherObject = m_pKernelModule->GetObject( identOther );
        if ( pOtherObject == NULL )
        {
			damageListValue.AddInt(0);
			damageResultList.AddInt(0);
            continue;
        }

		pOtherObject->SetPropertyObject(NFrame::NPC::LastAttacker(), self);

		m_pPropertyModule->CalculatePropertyValue(identOther, strDamageProperty, NFIPropertyModule::NPG_ALL, -nDamageValue, true);

		//NFMsg::EffectData_EResultType
		damageListValue.AddInt(nDamageValue);
		damageResultList.AddInt(NFMsg::EffectData_EResultType::EffectData_EResultType_EET_SUCCESS);

    }

    return 0;
}
