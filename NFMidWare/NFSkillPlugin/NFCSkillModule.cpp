// -------------------------------------------------------------------------
//    @FileName				:    NFCSkillModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCSkillModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCSkillModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCSkillModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pSkillCooldownModule = pPluginManager->FindModule<NFISkillCooldownModule>();

	return true;
}

bool NFCSkillModule::Shut()
{
	return true;
}

bool NFCSkillModule::Execute()
{
	return true;
}

bool NFCSkillModule::AfterInit()
{

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCSkillModule::OnClassObjectEvent);
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCSkillModule::OnClassObjectEvent);

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SKILL_OBJECTX, this, &NFCSkillModule::OnClienUseSkill)) { return false; }

	return true;
}

int NFCSkillModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{

	}

	return 0;
}

int NFCSkillModule::ExistSkill(const NFGUID& self, const std::string& strSkillName)
{
	//NF_SHARE_PTR<NFIRecord> pRecord =  m_pKernelModule->FindRecord( self, mstrSkillTableName );
	//if ( pRecord )
	//{
	//    NFDataList varResult;
	//    pRecord->FindString( EGameSkillStoreType::EGSST_TYPE_SKILL_CONFIGID, strSkillName.c_str(), varResult );
	//    if ( varResult.GetCount() == 1 )
	//    {
	//        return varResult.Int( 0 );
	//    }
	//}

	return -1;
}

void NFCSkillModule::OnClienUseSkill(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckUseSkill)

		//bc
	NFGUID xUser = NFINetModule::PBToNF(xMsg.user());
	const std::string& strSkillID = xMsg.skill_id();
	if (xUser != nPlayerID)
	{
		NFGUID xAIOnwer = m_pKernelModule->GetPropertyObject(xUser, NFrame::NPC::AIOwnerID());
		if (xAIOnwer != nPlayerID)
		{
			m_pLogModule->LogError(xUser, "AIOwnerID " + xAIOnwer.ToString() + " not " + nPlayerID.ToString());
			return;
		}
	}

	if (m_pSkillCooldownModule->ExistSkillCD(xUser, strSkillID))
	{
		m_pLogModule->LogError(xUser, "ExistSkillCD " + strSkillID);
		return;
	}

	const int nSkillType = m_pElementModule->GetPropertyInt32(strSkillID, NFrame::Skill::SkillType());
	NF_SHARE_PTR<NFISkillConsumeProcessModule> xProcessModule = m_pSkillConsumeManagerModule->GetConsumeModule((SKILL_CONSUME_TYPE)nSkillType);
	if (xProcessModule)
	{
		NFDataList xList;
		for (int i = 0; i < xMsg.effect_data_size(); ++i)
		{
			const NFMsg::EffectData& xEffectData = xMsg.effect_data(i);
			const NFGUID nTarget = NFINetModule::PBToNF(xEffectData.effect_ident());

			xList.Add(nTarget);
		}

		if (xProcessModule->ConsumeLegal(xUser, strSkillID, xList) == 0)
		{
			//once the skill consume than add CD time
			m_pSkillCooldownModule->AddSkillCD(xUser, strSkillID);

			NFDataList xDamageList;
			NFDataList xResultList;
			if (xProcessModule->ConsumeProcess(xUser, strSkillID, xList, xDamageList, xResultList) == 0)
			{
				NFMsg::ReqAckUseSkill xReqAckUseSkill;
				xReqAckUseSkill.set_skill_id(strSkillID);
				*xReqAckUseSkill.mutable_user() = NFINetModule::NFToPB(xUser);
				xReqAckUseSkill.set_use_index(xMsg.use_index());

				for (int i = 0; i < xList.GetCount(); ++i)
				{
					NFMsg::EffectData* pNewEffectData = xReqAckUseSkill.add_effect_data();

					*pNewEffectData->mutable_effect_ident() = NFINetModule::NFToPB(xList.Object(i));

					pNewEffectData->set_effect_value(xDamageList.Int32(i));
					pNewEffectData->set_effect_rlt((NFMsg::EffectData_EResultType)xResultList.Int(i));
				}

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SKILL_OBJECTX, xReqAckUseSkill, nPlayerID);
			}
		}
	}
}