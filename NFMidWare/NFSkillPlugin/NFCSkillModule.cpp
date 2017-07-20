// -------------------------------------------------------------------------
//    @FileName         :    NFCSkillModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCSkillModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCSkillModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCSkillModule::Init()
{
    mstrSkillTableName = "SkillList";

	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	//m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();


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
   
	m_pEventModule->AddEventCallBack( NFGUID(), NFED_ON_CLIENT_REQUIRE_USE_SKILL, this, &NFCSkillModule::OnRequireUseSkillEvent );
	m_pEventModule->AddEventCallBack( NFGUID(), NFED_ON_CLIENT_REQUIRE_USE_SKILL_POS, this, &NFCSkillModule::OnRequireUseSkillPosEvent );

    m_pKernelModule->AddClassCallBack( NFrame::Player::ThisName(), this, &NFCSkillModule::OnClassObjectEvent );
    m_pKernelModule->AddClassCallBack( NFrame::NPC::ThisName(), this, &NFCSkillModule::OnClassObjectEvent );

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SKILL_OBJECTX, this, &NFCSkillModule::OnClienUseSkill)) { return false; }

    return true;
}

int NFCSkillModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
    {
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
    {

    }

    return 0;
}

int NFCSkillModule::OnUseSkill(const NFGUID& self, const NFDataList& var)
{
    if ( var.GetCount() != 2)
    {
        return 1;
    }

    const std::string& strSkillID = var.String( 0 );
    const NFGUID& nTargetID = var.Object(1);

    NF_SHARE_PTR<NFIObject> pObejct = m_pKernelModule->GetObject(self);
    if ( pObejct == NULL )
    {
        return 1;
    }

    //NF_SHARE_PTR<NFIRecord> pRecordSkill = pObejct->GetRecordManager()->GetElement( mstrSkillTableName );
    //if ( pRecordSkill == NULL )
    //{
    //    return 1;
    //}

    //NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager( var.String( 2 ) );
    //if ( pPropertyManager == NULL )
    //{
    //    return 1;
    //}

    //NF_SHARE_PTR<NFIProperty> pItemTypeProperty = pPropertyManager->GetElement( "SkillType" );
    //if ( pItemTypeProperty == NULL )
    //{
    //    return 1;
    //}

    
    //EGameSkillType eItemType = ( EGameSkillType )pItemTypeProperty->GetInt();

    if (!m_pElementModule->ExistElement(strSkillID))
    {
        return 1;
    }

    //NFISkillConsumeProcessModule* pConsumeProcessModule = m_pSkillConsumeManagerModule->GetConsumeModule( EGameSkillType::EGST_JOBSKILL_BRIEF );
    //if ( pConsumeProcessModule == NULL )
    //{
    //    return 1;
    //}

    //NFDataList damageValueList;
    //NFDataList damageResultList;
    //int nResult = pConsumeProcessModule->ConsumeProcess( var.Object( 0 ), var.String( 2 ), valueOther, damageValueList, damageResultList );
    //for (int i = 0; i < valueOther.GetCount(); i++)
    //{
    //    m_pKernelModule->SetPropertyInt(valueOther.Object(i), "HP", 0);
    //    damageValueList.AddInt(0);
    //    damageResultList.AddInt(0);
    //}

    int nCurHP = m_pKernelModule->GetPropertyInt(nTargetID, NFrame::NPC::HP());
    if (nCurHP <= 0)
    {
        return 1;
    }

    m_pKernelModule->SetPropertyObject(nTargetID, NFrame::NPC::LastAttacker(), self);
    m_pKernelModule->SetPropertyInt(nTargetID, NFrame::NPC::HP(), (nCurHP - 10) >= 0 ? (nCurHP - 10) : 0); 
    

    //
    //if ( damageValueList.GetCount() == damageResultList.GetCount()
    //    && damageValueList.GetCount() == valueOther.GetCount() )
    //{
    //    NFDataList valueResult;
    //    valueResult.AddString( var.String( 2 ).c_str() );
    //    valueResult.AddInt( valueOther.GetCount() );
    //    valueResult.Append( valueOther, 0, valueOther.GetCount() ); 
    //    valueResult.Append( damageValueList, 0, damageValueList.GetCount() ); 
    //    valueResult.Append( damageResultList, 0, damageResultList.GetCount() ); 

    //    
    //    //m_pEventProcessModule->DoEvent( pObejct->Self(), NFED_ON_CLIENT_USE_SKILL_RESULT, valueResult );
    //}

    return 0;
}

int NFCSkillModule::OnRequireUseSkillEvent( const NFGUID& self, const NFEventDefine nEventID, const NFDataList& var )
{
    //if ( var.GetCount() < 3)
    //{
    //    return 1;
    //}

    ////EGameErrorCode errorCode = EGameErrorCode::EGEC_INVALID_SKILL;
    //NF_SHARE_PTR<NFIObject> pObejct = m_pKernelModule->GetObject( var.Object( 0 ) );
    //if ( pObejct == NULL )
    //{
    //    return 1;
    //}

    //NF_SHARE_PTR<NFIRecord> pRecordSkill = pObejct->GetRecordManager()->GetElement( NFrame::Player::R_SkillTable() );
    //if ( pRecordSkill == NULL )
    //{
    //    return 1;
    //}

    //NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager( var.String( 2 ) );
    //if ( pPropertyManager == NULL )
    //{
    //    return 1;
    //}

    //NF_SHARE_PTR<NFIProperty> pItemTypeProperty = pPropertyManager->GetElement(NFrame::Skill::SkillType());
    //if ( pItemTypeProperty == NULL )
    //{
    //    return 1;
    //}

    //
    //EGameSkillType eItemType = ( EGameSkillType )pItemTypeProperty->GetInt();
    //NFISkillConsumeProcessModule* pConsumeProcessModule = m_pSkillConsumeManagerModule->GetConsumeModule( EGameSkillType::EGST_JOBSKILL_BRIEF );
    //if ( pConsumeProcessModule == NULL )
    //{
    //    return 1;
    //}

    //NFDataList valueOther;
    
    ////     if ( pConsumeProcessModule->ConsumeLegal( var.ObjectVal( 0 ), var.StringVal( 2 ), valueOther ) != 0 )
    ////     {
    ////         return 1;
    ////     }
    //// 
    ////     if ( pConsumeProcessModule->ConsumeSelf( var.ObjectVal( 0 ), var.StringVal( 2 ) ) != 0 )
    ////     {
    ////         return 1;
    ////     }
    //// 
    //NFDataList damageValueList;
    //NFDataList damageResultList;
    //int nResult = pConsumeProcessModule->ConsumeProcess( var.Object( 0 ), var.String( 2 ), valueOther, damageValueList, damageResultList );
    //for (int i = 0; i < valueOther.GetCount(); i++)
    //{
    //    m_pKernelModule->SetPropertyInt(valueOther.Object(i), NFrame::NPC::HP(), 0);
    //    damageValueList.AddInt(0);
    //    damageResultList.AddInt(0);
    //}

    //
    //if ( damageValueList.GetCount() == damageResultList.GetCount()
    //    && damageValueList.GetCount() == valueOther.GetCount() )
    //{
    //    NFDataList valueResult;
    //    valueResult.AddString( var.String( 2 ).c_str() );
    //    valueResult.AddInt( valueOther.GetCount() );
    //    valueResult.Append( valueOther, 0, valueOther.GetCount() ); 
    //    valueResult.Append( damageValueList, 0, damageValueList.GetCount() ); 
    //    valueResult.Append( damageResultList, 0, damageResultList.GetCount() ); 

    //    
    //    //m_pEventProcessModule->DoEvent( pObejct->Self(), NFED_ON_CLIENT_USE_SKILL_RESULT, valueResult );
    //}

    return 0;
}

int NFCSkillModule::OnRequireUseSkillPosEvent( const NFGUID& self, const NFEventDefine nEventID, const NFDataList& var )
{
    if ( var.GetCount() < 4 ||
        !var.TypeEx(TDATA_OBJECT, TDATA_FLOAT, TDATA_FLOAT, TDATA_FLOAT, TDATA_OBJECT))
    {
        return 1;
    }

    int nSkillID = var.Int( 0 );
    double fX = var.Float( 1 );
    double fY = var.Float( 2 );
    double fZ = var.Float( 3 );
    

    
    //  NFDataList valueResult;
    //  valueResult.AddInt(nSkillID);
    //  valueResult.AddInt(nResult);
    //  valueResult.Append(var, 4, var.GetCount());
    //
    //  m_pEventProcessModule->DoEvent(self, NFED_ON_CLIENT_USE_SKILL_POS_RESULT, valueResult);

    return 0;
}

int NFCSkillModule::AddSkill( const NFGUID& self, const std::string& strSkillName )
{
    if(m_pElementModule->ExistElement(strSkillName))
    {
        if ( ExistSkill( self, strSkillName ) < 0 )
        {
            NF_SHARE_PTR<NFIRecord> pRecord =  m_pKernelModule->FindRecord( self, mstrSkillTableName );
            if ( pRecord )
            {
                return pRecord->AddRow( -1,  NFDataList() << strSkillName.c_str());
                //if ( nRow >= 0 )
                //{
                //    return pRecord->SetString( nRow, EGameSkillStoreType::EGSST_TYPE_SKILL_CONFIGID, strSkillName.c_str() );
                //}
            }
        }
    }

    m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, NFGUID(), strSkillName, "There is no element", __FUNCTION__, __LINE__);

    return 0;
}

int NFCSkillModule::ExistSkill( const NFGUID& self, const std::string& strSkillName )
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

int NFCSkillModule::SetSkillLevel( const NFGUID& self, const std::string& strSkillName, const int nLevel )
{
    //     int nFindRow = ExistSkill( self, strSkillName );
    //     if ( nFindRow >= 0 )
    //     {
    //         m_pKernelModule->SetRecordInt( self, mstrSkillTableName, nFindRow, EGameSkillStoreType::EGSST_TYPE_SKILL_LEVEL, nLevel );
    //     }

    return 0;
}

int NFCSkillModule::SetSkillGem( const NFGUID& self, const std::string& strSkillName, const std::string& strGemName )
{
    int nFindRow = ExistSkill( self, strSkillName );
    if ( nFindRow >= 0 )
    {
        //m_pKernelModule->SetRecordString( self, mstrSkillTableName, nFindRow, EGameSkillStoreType::EGSST_TYPE_SKILL_GEM_CONFIGID, strGemName.c_str() );
    }

    return 0;
}

int NFCSkillModule::SetSkillGemLevel( const NFGUID& self, const std::string& strSkillName, const int nLevel )
{
    int nFindRow = ExistSkill( self, strSkillName );
    if ( nFindRow >= 0 )
    {
        //m_pKernelModule->SetRecordInt( self, mstrSkillTableName, nFindRow, EGameSkillStoreType::EGSST_TYPE_SKILL_GEM_LEVEL, nLevel );
    }

    return 0;
}

int NFCSkillModule::GetSkillLevel( const NFGUID& self, const std::string& strSkillName )
{
    //     int nFindRow = ExistSkill( self, strSkillName );
    //     if ( nFindRow >= 0 )
    //     {
    //         m_pKernelModule->QueryRecordInt( self, mstrSkillTableName, nFindRow, EGameSkillStoreType::EGSST_TYPE_SKILL_LEVEL );
    //     }

    return 0;
}

std::string NFCSkillModule::GetSkillGem( const NFGUID& self, const std::string& strSkillName )
{
    int nFindRow = ExistSkill( self, strSkillName );
    if ( nFindRow >= 0 )
    {
        //return m_pKernelModule->QueryRecordString( self, mstrSkillTableName, nFindRow, EGameSkillStoreType::EGSST_TYPE_SKILL_GEM_CONFIGID );
    }

    return NULL_STR;
}

int NFCSkillModule::GetSkillGemLevel( const NFGUID& self, const std::string& strSkillName )
{
    int nFindRow = ExistSkill( self, strSkillName );
    if ( nFindRow >= 0 )
    {
        //return m_pKernelModule->QueryRecordInt( self, mstrSkillTableName, nFindRow, EGameSkillStoreType::EGSST_TYPE_SKILL_GEM_LEVEL );
    }

    return 0;
}

int NFCSkillModule::AddNewerSkill( const NFGUID& self )
{
    
    
    

    return 0;
}

void NFCSkillModule::OnClienUseSkill(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckUseSkill)

		//bc
	const std::string& strSkillID =  xMsg.skill_id();
	int nContianerID = m_pKernelModule->GetPropertyInt(nPlayerID, "SceneID");
	int nGroupID = m_pKernelModule->GetPropertyInt(nPlayerID, "GroupID");

	NFMsg::ReqAckUseSkill xReqAckUseSkill;
	xReqAckUseSkill.set_skill_id(strSkillID);
	*xReqAckUseSkill.mutable_user() = NFINetModule::NFToPB(nPlayerID);
	*xReqAckUseSkill.mutable_now_pos() = xMsg.now_pos();
	*xReqAckUseSkill.mutable_tar_pos() = xMsg.tar_pos();
	xReqAckUseSkill.set_use_index( xMsg.use_index());

	for (int i = 0; i < xMsg.effect_data_size(); ++i)
	{
		const NFMsg::EffectData& xEffectData = xMsg.effect_data(i);
		const NFGUID nTarget = NFINetModule::PBToNF(xEffectData.effect_ident());
		
		OnUseSkill(nPlayerID, NFDataList() << strSkillID << nTarget);

		NFMsg::EffectData* pNewEffectData = xReqAckUseSkill.add_effect_data();

		*pNewEffectData->mutable_effect_ident() = NFINetModule::NFToPB(nTarget);
		pNewEffectData->set_effect_value(20);
		pNewEffectData->set_effect_rlt(xEffectData.effect_rlt());
	}

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_ACK_SKILL_OBJECTX, xReqAckUseSkill, nPlayerID);
}