// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFCore/NFCMemory.h"

bool NFCGameServerNet_ServerModule::Init()
{
	return true;
}

bool NFCGameServerNet_ServerModule::AfterInit()
{
	m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
	m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
	m_pSceneProcessModule = dynamic_cast<NFISceneProcessModule*>(pPluginManager->FindModule("NFCSceneProcessModule"));
	m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pSLGShopModule = dynamic_cast<NFISLGShopModule*>(pPluginManager->FindModule("NFCSLGShopModule"));
	m_pSLGBuildingModule = dynamic_cast<NFISLGBuildingModule*>(pPluginManager->FindModule("NFCSLGBuildingModule"));
	m_pUUIDModule = dynamic_cast<NFIUUIDModule*>(pPluginManager->FindModule("NFCUUIDModule"));
	m_pPVPModule = dynamic_cast<NFIPVPModule*>(pPluginManager->FindModule("NFCPVPModule"));
	m_pSkillModule = dynamic_cast<NFISkillModule*>(pPluginManager->FindModule("NFCSkillModule"));
	m_pDataProcessModule = dynamic_cast<NFIDataProcessModule*>(pPluginManager->FindModule("NFCDataProcessModule"));
	m_pEctypeModule = dynamic_cast<NFIEctypeModule*>(pPluginManager->FindModule("NFCEctypeModule"));
    m_pGameServerToWorldModule = dynamic_cast<NFIGameServerToWorldModule*>(pPluginManager->FindModule("NFCGameServerToWorldModule"));

	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pSceneProcessModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pSLGShopModule);
	assert(NULL != m_pSLGBuildingModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pPVPModule);
	assert(NULL != m_pSkillModule);
	assert(NULL != m_pDataProcessModule);
	assert(NULL != m_pEctypeModule);
    assert(NULL != m_pGameServerToWorldModule);

	m_pKernelModule->ResgisterCommonClassEvent( this, &NFCGameServerNet_ServerModule::OnClassCommonEvent );
	m_pKernelModule->ResgisterCommonPropertyEvent( this, &NFCGameServerNet_ServerModule::OnPropertyCommonEvent );
	m_pKernelModule->ResgisterCommonRecordEvent( this, &NFCGameServerNet_ServerModule::OnRecordCommonEvent );

	m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCGameServerNet_ServerModule::OnObjectClassEvent );
	m_pEventProcessModule->AddClassCallBack( "NPC", this, &NFCGameServerNet_ServerModule::OnObjectNPCClassEvent );

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName;
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_GAME && pPluginManager->AppID() == nServerID)
			{
				const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
				const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
				const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

				Initialization(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCGameServerNet_ServerModule::OnRecivePSPack, &NFCGameServerNet_ServerModule::OnSocketPSEvent, nMaxConnect, nPort, nCpus);
			}
		}
	}

	return true;
}

bool NFCGameServerNet_ServerModule::Shut()
{

	return true;
}

bool NFCGameServerNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	return NFINetModule::Execute(fLasFrametime, fStartedTime);
}

int NFCGameServerNet_ServerModule::OnRecivePSPack( const NFIPacket& msg )
{
	m_pLogModule->LogNormal(NFILogModule::NLL_WARING_NORMAL, NFIDENTID(), "BeginMsg:", msg.GetMsgHead()->GetMsgID(), __FUNCTION__, __LINE__ );
	// TOCHECK
	//NFIActorMessage xActorMsg;

	//xActorMsg.eType = NFIActorMessage::EACTOR_NET_MSG;
	//xActorMsg.nSubMsgID = msg.GetMsgHead()->GetMsgID();
	//xActorMsg.data.assign(msg.GetData(), msg.GetDataLen());

	////�����û�������Ϣ�õ�Ŀ��actor��ַ,���Լ�����ô������Ż�
	//pPluginManager->GetFramework().Send(xActorMsg, pPluginManager->GetAddress(), pPluginManager->GetAddress());
	//����actor�����
	switch (msg.GetMsgHead()->GetMsgID())
	{
	case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REFRESH:
		OnRefreshProxyServerInfoProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED:
		OnProxyServerRegisteredProcess(msg);
		break;

	case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_UNREGISTERED:
		OnProxyServerUnRegisteredProcess(msg);
		break;
		//////////////////////////////////////////////////////////////////////////
	case NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME:
		OnClienEnterGameProcess(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GAME:
		OnClienLeaveGameProcess(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST:
		OnReqiureRoleListProcess(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE:
		OnCreateRoleGameProcess(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_DELETE_ROLE:
		OnDeleteRoleGameProcess(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_RECOVER_ROLE:
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_SWAP_SCENE:
		OnClienSwapSceneProcess(msg);
		break;
		//case NFMsg::EGameMsgID::EGMI_REQ_COMMAND:
		//    OnClienGMProcess(msg);
		//    break;

	case NFMsg::EGameMsgID::EGMI_REQ_SKILL_OBJECTX:
		OnClienUseSkill(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_MOVE:
		OnClienMove(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_MOVE_IMMUNE:
		OnClienMoveImmune(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_CHAT:
		OnClienChatProcess(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_JOIN_PVP:
		OnClientJoinPVP(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_EXIT_PVP:
		OnClientExitPVP(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_END_BATTLE:
		OnClientEndBattle(msg);
		break;


		//SLG////////////////////////////////////////////////////////////////////////
	case NFMsg::EGameMsgID::EGMI_REQ_BUY_FORM_SHOP:
		OnSLGClienBuyItem(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_MOVE_BUILD_OBJECT:
		OnSLGClienMoveObject(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_UP_BUILD_LVL:
		OnSLGClienUpgradeBuilding(msg);
		break;
	case NFMsg::EGameMsgID::EGMI_REQ_CREATE_ITEM:
		OnSLGClienCreateItem(msg);
		break;
		//////////////////////////////////////////////////////////////////////////
	case NFMsg::EGameMsgID::EGMI_REQ_CREATE_GUILD:
        {
            NFIDENTID xGuildID = GetGuildID<NFMsg::ReqAckCreateGuild>(msg);
            OnTransWorld(msg, xGuildID.nData64);
        }
        break;
	case NFMsg::EGameMsgID::EGMI_REQ_JOIN_GUILD:
        {
            NFIDENTID xGuildID = GetGuildID<NFMsg::ReqAckJoinGuild>(msg);
            OnTransWorld(msg, xGuildID.nData64);
        }
        break;
	case NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GUILD:
        {
            NFIDENTID xGuildID = GetGuildID<NFMsg::ReqAckLeaveGuild>(msg);
            OnTransWorld(msg, xGuildID.nData64);
        }
        break;
	case NFMsg::EGameMsgID::EGMI_REQ_OPR_GUILD:
        {
            NFIDENTID xGuildID = GetGuildID<NFMsg::ReqAckOprGuildMember>(msg);
            OnTransWorld(msg, xGuildID.nData64);
        }
        break;
    case NFMsg::EGameMsgID::EGMI_REQ_SEARCH_GUILD:
		OnTransWorld(msg);
		break;

    case NFMsg::EGameMsgID::EGEC_REQ_CREATE_CHATGROUP:
    case NFMsg::EGameMsgID::EGEC_REQ_JOIN_CHATGROUP:
    case NFMsg::EGameMsgID::EGEC_REQ_LEAVE_CHATGROUP:
    case NFMsg::EGameMsgID::EGEC_REQ_SUBSCRIPTION_CHATGROUP:
        {
            OnTransWorld(msg);
        }
        break;
		//////////////////////////////////////////////////////////////////////////
	default:
		break;
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_WARING_NORMAL, NFIDENTID(), "EndMsg:", msg.GetMsgHead()->GetMsgID(), __FUNCTION__, __LINE__ );

	return true;
}

int NFCGameServerNet_ServerModule::OnSocketPSEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else  if (eEvent == NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
		OnClientConnected(nSockIndex);
	}

	return true;
}

void NFCGameServerNet_ServerModule::OnClientDisconnect( const int nAddress )
{
	//ֻ���������ض���
	int nServerID = 0;
	NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.First();
	while (pServerData.get())
	{
		if (nAddress == pServerData->nFD)
		{
			nServerID = pServerData->pData->server_id();
			break;
		}

		pServerData = mProxyMap.Next();
	}

	mProxyMap.RemoveElement(nServerID);
}

void NFCGameServerNet_ServerModule::OnClientConnected( const int nAddress )
{

}

void NFCGameServerNet_ServerModule::OnClienEnterGameProcess( const NFIPacket& msg )
{
	//�ڽ�����Ϸ֮ǰnPlayerIDΪ�������ص�FD
	NFIDENTID nClientID;
	NFMsg::ReqEnterGameServer xMsg;
	if (!RecivePB(msg, xMsg, nClientID))
	{
		return;
	}

	NFIDENTID nRoleID = PBToNF(xMsg.id());

	PlayerLeaveGameServer(nRoleID);

	//////////////////////////////////////////////////////////////////////////
	//��ȡ����
	if(!m_pDataProcessModule->LoadDataFormNoSql(nRoleID))
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFCGameServerNet_ServerModule::BaseData> pBaseData = mRoleBaseData.GetElement(nRoleID);
	if (nullptr != pBaseData)
	{// �Ѿ�����
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nClientID, "player is exist, cannot enter game", "", __FUNCTION__, __LINE__);
		return;
	}

	int nGateID = -1;
	NF_SHARE_PTR<ServerData> pServerData = mProxyMap.First();
	while (pServerData.get())
	{
		if (msg.GetFd() == pServerData->nFD)
		{
			nGateID = pServerData->pData->server_id();
			break;
		}

		pServerData = mProxyMap.Next();
	}

	if (nGateID <= 0)
	{
		//�Ƿ�gate
		return;
	}

	pServerData = mProxyMap.GetElement(nGateID);
	if (nullptr == pServerData)
	{
		return;
	}

	// proxy��playerID��gateFD
	if (!pServerData->xRoleInfo.insert(std::make_pair(nRoleID, msg.GetFd())).second)
	{
		return;
	}

	//id��fd,gateid��
	mRoleBaseData.AddElement(nRoleID, NF_SHARE_PTR<BaseData>(NF_NEW BaseData(nGateID, nClientID)));

	//Ĭ��1�ų���
	int nSceneID = 1;
	NFCDataList var;
	var.AddString("Name");
	var.AddString(xMsg.name());

	var.AddString("GateID");
	var.AddInt(nGateID);

	var.AddString("ClientID");
	var.AddObject(nClientID);

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, nSceneID, 0, "Player", "",  var);
	if ( NULL == pObject.get() )
	{
		//�ڴ�й©
		//mRoleBaseData
		//mRoleFDData
		return;
	}

	pObject->SetPropertyInt("LoadPropertyFinish", 1);
    pObject->SetPropertyInt("GateID", nGateID);
	pObject->SetPropertyInt("GameID", pPluginManager->AppID());

 	m_pEventProcessModule->DoEvent(pObject->Self(), "Player", CLASS_OBJECT_EVENT::COE_CREATE_FINISH, NFCDataList() );

	NFCDataList varEntry;
	varEntry << pObject->Self();
	varEntry << NFINT64(0);
	varEntry << nSceneID;
	varEntry << -1;
	m_pEventProcessModule->DoEvent( pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry );
}

void NFCGameServerNet_ServerModule::OnClienLeaveGameProcess( const NFIPacket& msg )
{
	NFIDENTID nPlayerID;
	NFMsg::ReqLeaveGameServer xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return;
	}

	if (nPlayerID.IsNull())
	{
		return;
	}

	PlayerLeaveGameServer(nPlayerID);

}

int NFCGameServerNet_ServerModule::OnPropertyEnter( const NFIDataList& argVar, const NFIDENTID& self )
{
	if ( argVar.GetCount() <= 0 || self.IsNull())
	{
		return 0;
	}

	NFMsg::MultiObjectPropertyList xPublicMsg;
	NFMsg::MultiObjectPropertyList xPrivateMsg;

	//��Ϊ�Լ�������
	//1.public���͸�������
	//2.����Լ����б��У��ٴη���private����
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject.get() )
	{
		NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();
		NFMsg::ObjectPropertyList* pPrivateData = xPrivateMsg.add_multi_player_property();

		*(pPublicData->mutable_player_id()) = NFToPB(self);
		*(pPrivateData->mutable_player_id()) = NFToPB(self);

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIProperty> pPropertyInfo = pPropertyManager->First();
		while ( pPropertyInfo.get() )
		{
			if ( pPropertyInfo->Changed() )
			{
				switch ( pPropertyInfo->GetType())
				{
				case TDATA_INT:
					{
						if ( pPropertyInfo->GetPublic() )
						{
							NFMsg::PropertyInt* pDataInt = pPublicData->add_property_int_list();
							pDataInt->set_property_name( pPropertyInfo->GetKey() );
							pDataInt->set_data( pPropertyInfo->GetInt() );
						}

						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyInt* pDataInt = pPrivateData->add_property_int_list();
							pDataInt->set_property_name( pPropertyInfo->GetKey() );
							pDataInt->set_data( pPropertyInfo->GetInt() );
						}
					}
					break;

				case TDATA_FLOAT:
					{
						if ( pPropertyInfo->GetPublic() )
						{
							NFMsg::PropertyFloat* pDataFloat = pPublicData->add_property_float_list();
							pDataFloat->set_property_name( pPropertyInfo->GetKey() );
							pDataFloat->set_data( pPropertyInfo->GetInt() );
						}

						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyFloat* pDataFloat = pPrivateData->add_property_float_list();
							pDataFloat->set_property_name( pPropertyInfo->GetKey() );
							pDataFloat->set_data( pPropertyInfo->GetFloat() );
						}
					}
					break;

				case TDATA_DOUBLE:
					{
						if ( pPropertyInfo->GetPublic() )
						{
							NFMsg::PropertyFloat* pDataFloat = pPublicData->add_property_float_list();
							pDataFloat->set_property_name( pPropertyInfo->GetKey() );
							pDataFloat->set_data( pPropertyInfo->GetDouble() );
						}

						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyFloat* pDataFloat = pPrivateData->add_property_float_list();
							pDataFloat->set_property_name( pPropertyInfo->GetKey() );
							pDataFloat->set_data( pPropertyInfo->GetDouble() );
						}
					}
					break;

				case TDATA_STRING:
					{
						if ( pPropertyInfo->GetPublic() )
						{
							NFMsg::PropertyString* pDataString = pPublicData->add_property_string_list();
							pDataString->set_property_name( pPropertyInfo->GetKey() );
							pDataString->set_data( pPropertyInfo->GetString() );
						}

						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyString* pDataString = pPrivateData->add_property_string_list();
							pDataString->set_property_name( pPropertyInfo->GetKey() );
							pDataString->set_data( pPropertyInfo->GetString() );
						}
					}
					break;

				case TDATA_OBJECT:
					{
						if ( pPropertyInfo->GetPublic() )
						{
							NFMsg::PropertyObject* pDataObject = pPublicData->add_property_object_list();
							pDataObject->set_property_name( pPropertyInfo->GetKey() );
							*(pDataObject->mutable_data()) = NFToPB(pPropertyInfo->GetObject());
						}

						if ( pPropertyInfo->GetPrivate() )
						{
							NFMsg::PropertyObject* pDataObject = pPrivateData->add_property_object_list();
							pDataObject->set_property_name( pPropertyInfo->GetKey() );
							*(pDataObject->mutable_data()) = NFToPB(pPropertyInfo->GetObject());
						}
					}
					break;

				default:
					break;
				}
			}

			pPropertyInfo = pPropertyManager->Next();
		}

		for ( int i = 0; i < argVar.GetCount(); i++ )
		{
			NFIDENTID identOther = argVar.Object( i );
			if ( self == identOther )
			{
				//�ҵ����������ص�FD
				SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, xPrivateMsg, identOther);
			}
			else
			{
				SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, xPublicMsg, identOther);
			}
		}
	}

	return 0;
}

bool OnRecordEnterPack(NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase)
{
	if (!pRecord || !pObjectRecordBase)
	{
		return false;
	}

	for ( int i = 0; i < pRecord->GetRows(); i ++ )
	{
		if ( pRecord->IsUsed( i ) )
		{
			//����public����private��Ҫ���ϣ���Ȼpublic�㲥���ǲ���private�͹㲥������
			NFMsg::RecordAddRowStruct* pAddRowStruct = pObjectRecordBase->add_row_struct();
			pAddRowStruct->set_row( i );

			for ( int j = 0; j < pRecord->GetCols(); j++ )
			{
				//�����0�Ͳ������ˣ���Ϊ�ͻ���Ĭ����0
				NFCDataList valueList;
				TDATA_TYPE eType = pRecord->GetColType( j );
				switch ( eType )
				{
				case TDATA_TYPE::TDATA_INT:
					{
						int nValue = pRecord->GetInt( i, j );
						//if ( 0 != nValue )
						{
							NFMsg::RecordInt* pAddData = pAddRowStruct->add_record_int_list();
							pAddData->set_row( i );
							pAddData->set_col( j );
							pAddData->set_data( nValue );
						}
					}
					break;
				case TDATA_TYPE::TDATA_DOUBLE:
					{
						double dwValue = pRecord->GetDouble( i, j );
						//if ( dwValue < -0.01f || dwValue > 0.01f )
						{
							NFMsg::RecordFloat* pAddData = pAddRowStruct->add_record_float_list();
							pAddData->set_row( i );
							pAddData->set_col( j );
							pAddData->set_data( dwValue );
						}
					}
					break;
				case TDATA_TYPE::TDATA_FLOAT:
					{
						float fValue = pRecord->GetFloat( i, j );
						//if ( fValue < -0.01f || fValue > 0.01f )
						{
							NFMsg::RecordFloat* pAddData = pAddRowStruct->add_record_float_list();
							pAddData->set_row( i );
							pAddData->set_col( j );
							pAddData->set_data( fValue );
						}
					}
					break;
				case TDATA_TYPE::TDATA_STRING:
					{
						const std::string& strData = pRecord->GetString( i, j );
						//if ( !strData.empty() )
						{
							NFMsg::RecordString* pAddData = pAddRowStruct->add_record_string_list();
							pAddData->set_row( i );
							pAddData->set_col( j );
							pAddData->set_data( strData );
						}
					}
					break;
				case TDATA_TYPE::TDATA_OBJECT:
					{
						NFIDENTID ident = pRecord->GetObject( i, j );
						//if ( !ident.IsNull() )
						{
							NFMsg::RecordObject* pAddData = pAddRowStruct->add_record_object_list();
							pAddData->set_row( i );
							pAddData->set_col( j );
							*(pAddData->mutable_data()) = NFINetModule::NFToPB(ident);
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

	return true;
}

int NFCGameServerNet_ServerModule::OnRecordEnter( const NFIDataList& argVar, const NFIDENTID& self )
{
	if ( argVar.GetCount() <= 0 || self.IsNull() )
	{
		return 0;
	}

	NFMsg::MultiObjectRecordList xPublicMsg;
	NFMsg::MultiObjectRecordList xPrivateMsg;

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject.get() )
	{
		NFMsg::ObjectRecordList* pPublicData = NULL;
		NFMsg::ObjectRecordList* pPrivateData = NULL;

		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();
		NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
		while ( pRecord.get() )
		{
			if (!pRecord->GetPublic() && !pRecord->GetPrivate())
			{
				pRecord = pRecordManager->Next();
				continue;
			}

			NFMsg::ObjectRecordBase* pPrivateRecordBase = NULL;
			NFMsg::ObjectRecordBase* pPublicRecordBase = NULL;
			if (pRecord->GetPublic())
			{
				if (!pPublicData)
				{
					pPublicData = xPublicMsg.add_multi_player_record();
					*(pPublicData->mutable_player_id()) = NFToPB(self);
				}
				pPublicRecordBase = pPublicData->add_record_list();
				pPublicRecordBase->set_record_name(pRecord->GetName());

				OnRecordEnterPack(pRecord, pPublicRecordBase);
			}

			if (pRecord->GetPrivate())
			{
				if (!pPrivateData)
				{
					pPrivateData = xPrivateMsg.add_multi_player_record();
					*(pPrivateData->mutable_player_id()) = NFToPB(self);
				}
				pPrivateRecordBase = pPrivateData->add_record_list();
				pPrivateRecordBase->set_record_name(pRecord->GetName());

				OnRecordEnterPack(pRecord, pPrivateRecordBase);
			}

			pRecord = pRecordManager->Next();
		}


		for ( int i = 0; i < argVar.GetCount(); i++ )
		{
			NFIDENTID identOther = argVar.Object( i );
			if ( self == identOther )
			{
				if (xPrivateMsg.multi_player_record_size() > 0)
				{
					SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, xPrivateMsg, identOther);
				}
			}
			else
			{
				if (xPublicMsg.multi_player_record_size() > 0)
				{
					SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_RECORD_ENTRY, xPublicMsg, identOther);
				}
			}
		}
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar )
{
	if ( self.GetCount() <= 0 || argVar.GetCount() <= 0)
	{
		return 0;
	}

	NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
	for ( int i = 0; i < argVar.GetCount(); i++ )
	{
		NFIDENTID identOld = argVar.Object( i );
		//�ų��ն���
		if (identOld.IsNull())
		{
			continue;
		}

		NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
		*(pEntryInfo->mutable_object_guid()) = NFToPB(identOld);

		pEntryInfo->set_x( m_pKernelModule->GetPropertyFloat( identOld, "X" ) );
		pEntryInfo->set_y( m_pKernelModule->GetPropertyFloat( identOld, "Y" ) );
		pEntryInfo->set_z( m_pKernelModule->GetPropertyFloat( identOld, "Z" ) );
		pEntryInfo->set_career_type( m_pKernelModule->GetPropertyInt( identOld, "Job" ) );
		pEntryInfo->set_player_state( m_pKernelModule->GetPropertyInt( identOld, "State" ) );
		pEntryInfo->set_config_id( m_pKernelModule->GetPropertyString( identOld, "ConfigID" ) );
		pEntryInfo->set_scene_id( m_pKernelModule->GetPropertyInt( identOld, "SceneID" ) );
		pEntryInfo->set_class_id( m_pKernelModule->GetPropertyString( identOld, "ClassName" ) );

	}

	if (xPlayerEntryInfoList.object_list_size() <= 0)
	{
		return 0;
	}

	for (int i = 0; i < self.GetCount(); i++)
	{
		NFIDENTID ident = self.Object(i);
		if (ident.IsNull())
		{
			continue;
		}

		//�����ڲ�ͬ��������,�õ��������ڵ�����FD
		SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerEntryInfoList, ident);
	}

	return 1;
}

int NFCGameServerNet_ServerModule::OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar )
{
	if ( self.GetCount() <= 0 || argVar.GetCount() <= 0)
	{
		return 0;
	}

	NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
	for ( int i = 0; i < argVar.GetCount(); i++ )
	{
		NFIDENTID identOld = argVar.Object( i );
		//�ų��ն���
		if (identOld.IsNull())
		{
			continue;
		}

		NFMsg::Ident* pIdent = xPlayerLeaveInfoList.add_object_list();
		*pIdent = NFToPB(argVar.Object(i));
	}

	for (int i = 0; i < self.GetCount(); i++)
	{
		NFIDENTID ident = self.Object(i);
		if (ident.IsNull())
		{
			continue;
		}
		//�����ڲ�ͬ��������,�õ��������ڵ�����FD
		SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, ident);
	}

	return 1;
}

int NFCGameServerNet_ServerModule::OnPropertyCommonEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar)
{
	if ( oldVar.GetCount() <= 0 )
	{
		return 0;
	}

	//if ( "Player" == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
	{
		if ( "GroupID" == strPropertyName )
		{
			//�Լ�����Ҫ֪���Լ���������Ա仯��,���Ǳ��˾Ͳ���Ҫ֪����
			OnGroupEvent( self, strPropertyName, oldVar, newVar);
		}

		if ( "SceneID" == strPropertyName )
		{
			//�Լ�����Ҫ֪���Լ���������Ա仯��,���Ǳ��˾Ͳ���Ҫ֪����
			OnContainerEvent( self, strPropertyName, oldVar, newVar);
		}

		if ( "Player" == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
		{
			if ( m_pKernelModule->GetPropertyInt( self, "LoadPropertyFinish" ) <= 0 )
			{
				return 0;
			}
		}
	}

	NFCDataList valueBroadCaseList;
	int nCount = 0;//argVar.GetCount() ;
	if ( nCount <= 0 )
	{
		nCount = GetBroadCastObject( self, strPropertyName, false, valueBroadCaseList );
	}
	else
	{
		//����Ĳ�����Ҫ�㲥�Ķ����б�
		//valueBroadCaseList = argVar;
	}

	if ( valueBroadCaseList.GetCount() <= 0 )
	{
		return 0;
	}

	switch ( oldVar.Type( 0 ) )
	{
	case TDATA_INT:
		{
			NFMsg::ObjectPropertyInt xPropertyInt;
			NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
			*pIdent = NFToPB(self);

			NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
			pDataInt->set_property_name( strPropertyName );
			pDataInt->set_data( newVar.Int( 0 ) );

			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOld = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_INT, xPropertyInt, identOld);
			}
		}
		break;

	case TDATA_FLOAT:
		{
			NFMsg::ObjectPropertyFloat xPropertyFloat;
			NFMsg::Ident* pIdent = xPropertyFloat.mutable_player_id();
			*pIdent = NFToPB(self);

			NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
			pDataFloat->set_property_name( strPropertyName );
			pDataFloat->set_data( newVar.Float( 0 ) );

			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOld = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_FLOAT, xPropertyFloat, identOld);
			}
		}
		break;

	case TDATA_DOUBLE:
		{
			NFMsg::ObjectPropertyFloat xPropertyDouble;
			NFMsg::Ident* pIdent = xPropertyDouble.mutable_player_id();
			*pIdent = NFToPB(self);

			NFMsg::PropertyFloat* pDataFloat = xPropertyDouble.add_property_list();
			pDataFloat->set_property_name( strPropertyName );
			pDataFloat->set_data( newVar.Double( 0 ) );

			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOld = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_DOUBLE, xPropertyDouble, identOld);
			}
		}
		break;

	case TDATA_STRING:
		{
			NFMsg::ObjectPropertyString xPropertyString;
			NFMsg::Ident* pIdent = xPropertyString.mutable_player_id();
			*pIdent = NFToPB(self);

			NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
			pDataString->set_property_name( strPropertyName );
			pDataString->set_data( newVar.String( 0 ) );

			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOld = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_STRING, xPropertyString, identOld);
			}
		}
		break;

	case TDATA_OBJECT:
		{
			NFMsg::ObjectPropertyObject xPropertyObject;
			NFMsg::Ident* pIdent = xPropertyObject.mutable_player_id();
			*pIdent = NFToPB(self);

			NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
			pDataObject->set_property_name( strPropertyName );
			*pDataObject->mutable_data() = NFToPB(newVar.Object( 0 ));

			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOld = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_OBJECT, xPropertyObject, identOld);
			}
		}
		break;

	default:
		break;
	}


	return 0;
}

int NFCGameServerNet_ServerModule::OnRecordCommonEvent( const NFIDENTID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar)
{
    const std::string& strRecordName = xEventData.strRecordName;
    const int nOpType = xEventData.nOpType;
    const int nRow = xEventData.nRow;
    const int nCol = xEventData.nCol;

	int nObjectContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
	int nObjectGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

	if ( nObjectGroupID < 0 )
	{
		//����
		return 0;
	}

	if ( "Player" == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
	{
		if (m_pKernelModule->GetPropertyInt(self, "LoadPropertyFinish") <= 0)
		{
			return 0;
		}
	}

	NFCDataList valueBroadCaseList;
	GetBroadCastObject( self, strRecordName, true, valueBroadCaseList );

	switch ( nOpType )
	{
	case NFIRecord::RecordOptype::Add:
		{
			NFMsg::ObjectRecordAddRow xAddRecordRow;
			NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
			*pIdent = NFToPB(self);

			xAddRecordRow.set_record_name( strRecordName );

			NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
			pAddRowData->set_row(nRow);

			//add row ��Ҫ������row
			for ( int i = 0; i < newVar.GetCount(); i++ )
			{
				switch ( newVar.Type( i ) )
				{
				case TDATA_INT:
					{
						//��ӵ�ʱ������Ҫȫs
						int nValue = newVar.Int( i );
						//if ( 0 != nValue )
						{
							NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
							pAddData->set_col( i );
							pAddData->set_row( nRow );
							pAddData->set_data( nValue );
						}
					}
					break;
				case TDATA_FLOAT:
					{
						float fValue = newVar.Float( i );
						//if ( fValue > 0.001f  || fValue < -0.001f )
						{
							NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
							pAddData->set_col( i );
							pAddData->set_row( nRow );
							pAddData->set_data( fValue );
						}
					}
					break;
				case TDATA_DOUBLE:
					{
						float fValue = newVar.Double( i );
						//if ( fValue > 0.001f  || fValue < -0.001f )
						{
							NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
							pAddData->set_col( i );
							pAddData->set_row( nRow );
							pAddData->set_data( fValue );
						}
					}
					break;
				case TDATA_STRING:
					{
						const std::string& str = newVar.String( i );
						//if (!str.empty())
						{
							NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
							pAddData->set_col( i );
							pAddData->set_row( nRow );
							pAddData->set_data( str );
						}
					}
					break;
				case TDATA_OBJECT:
					{
						NFIDENTID identValue = newVar.Object( i );
						//if (!identValue.IsNull())
						{
							NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
							pAddData->set_col( i );
							pAddData->set_row( nRow );

							*pAddData->mutable_data() = NFToPB(identValue);
						}
					}
					break;
				default:
					break;
				}
			}


			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOther = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_ADD_ROW, xAddRecordRow, identOther);
			}
		}
		break;
	case NFIRecord::RecordOptype::Del:
		{
			NFMsg::ObjectRecordRemove xReoveRecordRow;

			NFMsg::Ident* pIdent = xReoveRecordRow.mutable_player_id();
			*pIdent = NFToPB(self);

			xReoveRecordRow.set_record_name( strRecordName );
			xReoveRecordRow.add_remove_row( nRow );

			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOther = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_REMOVE_ROW, xReoveRecordRow, identOther);
			}
		}
		break;
	case NFIRecord::RecordOptype::Swap:
		{
			//��ʵ��2��row����
			NFMsg::ObjectRecordSwap xSwapRecord;
			*xSwapRecord.mutable_player_id() = NFToPB(self);

			xSwapRecord.set_origin_record_name( strRecordName );
			xSwapRecord.set_target_record_name( strRecordName ); // ��ʱû��
			xSwapRecord.set_row_origin( nRow );
			xSwapRecord.set_row_target( nCol );

			for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			{
				NFIDENTID identOther = valueBroadCaseList.Object( i );

				SendMsgPBToGate(NFMsg::EGMI_ACK_SWAP_ROW, xSwapRecord, identOther);
			}
		}
		break;
	case NFIRecord::RecordOptype::UpData:
		{
			switch ( oldVar.Type( 0 ) )
			{
			case TDATA_INT:
				{
					NFMsg::ObjectRecordInt xRecordChanged;
					*xRecordChanged.mutable_player_id() = NFToPB(self);

					xRecordChanged.set_record_name( strRecordName );
					NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
					recordProperty->set_row( nRow );
					recordProperty->set_col( nCol );
					int nData = newVar.Int( 0 );
					recordProperty->set_data( nData );

					for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
					{
						NFIDENTID identOther = valueBroadCaseList.Object( i );

						SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_INT, xRecordChanged, identOther);
					}
				}
				break;

			case TDATA_FLOAT:
				{
					NFMsg::ObjectRecordFloat xRecordChanged;
					*xRecordChanged.mutable_player_id() = NFToPB(self);

					xRecordChanged.set_record_name( strRecordName );
					NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
					recordProperty->set_row( nRow );
					recordProperty->set_col( nCol );
					recordProperty->set_data( newVar.Float( 0 ) );

					for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
					{
						NFIDENTID identOther = valueBroadCaseList.Object( i );

						SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_FLOAT, xRecordChanged, identOther);
					}
				}
			case TDATA_DOUBLE:
				{
					NFMsg::ObjectRecordFloat xRecordChanged;
					*xRecordChanged.mutable_player_id() = NFToPB(self);

					xRecordChanged.set_record_name( strRecordName );
					NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
					recordProperty->set_row( nRow );
					recordProperty->set_col( nCol );
					recordProperty->set_data( newVar.Double( 0 ) );

					for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
					{
						NFIDENTID identOther = valueBroadCaseList.Object( i );

						SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_DOUBLE, xRecordChanged, identOther);
					}
				}
				break;
			case TDATA_STRING:
				{
					NFMsg::ObjectRecordString xRecordChanged;
					*xRecordChanged.mutable_player_id() = NFToPB(self);

					xRecordChanged.set_record_name( strRecordName );
					NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
					recordProperty->set_row( nRow );
					recordProperty->set_col( nCol );
					recordProperty->set_data( newVar.String( 0 ) );

					for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
					{
						NFIDENTID identOther = valueBroadCaseList.Object( i );

						SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_STRING, xRecordChanged, identOther);
					}
				}
				break;
			case TDATA_OBJECT:
				{
					NFMsg::ObjectRecordObject xRecordChanged;
					*xRecordChanged.mutable_player_id() = NFToPB(self);

					xRecordChanged.set_record_name( strRecordName );
					NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
					recordProperty->set_row( nRow );
					recordProperty->set_col( nCol );
					*recordProperty->mutable_data() = NFToPB(newVar.Object( 0 ));

					for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
					{
						NFIDENTID identOther = valueBroadCaseList.Object( i );

						SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_OBJECT, xRecordChanged, identOther);
					}
				}
				break;

			default:
				return 0;
				break;
			}
		}
		break;
	case NFIRecord::RecordOptype::Create:
		return 0;
		break;
	case NFIRecord::RecordOptype::Cleared:
		{
			//             NFMsg::ObjectRecordObject xRecordChanged;
			//             xRecordChanged.set_player_id( self.nData64 );
			//             xRecordChanged.set_record_name( strRecordName );
			//
			//             for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
			//             {
			//                 NFIDENTID identOther = valueBroadCaseList.Object( i );
			//                 SendMsgPB(NFMsg::EGMI_ACK_RECORD_CLEAR, xRecordChanged, 0);
			//             }
		}
		break;
	default:
		break;
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
	////////////1:�㲥���Ѿ����ڵ���//////////////////////////////////////////////////////////////
	if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
	{
		//ɾ�����߱�־

		//////////////////////////////////////////////////////////////////////////

		int nObjectContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
		int nObjectGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

		if ( nObjectGroupID < 0 )
		{
			//����
			return 0;
		}

		NFCDataList valueAllObjectList;
		NFCDataList valueBroadCaseList;
		NFCDataList valueBroadListNoSelf;
		m_pKernelModule->GetGroupObjectList( nObjectContainerID, nObjectGroupID, valueAllObjectList );

		for ( int i = 0; i < valueAllObjectList.GetCount(); i++ )
		{
			NFIDENTID identBC = valueAllObjectList.Object( i );
			const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
			if ( "Player" == strClassName )
			{
				valueBroadCaseList.Add( identBC );
				if (identBC != self)
				{
					valueBroadListNoSelf.Add( identBC );
				}
			}
		}

		//����Ǹ����Ĺ֣�����Ҫ���ͣ���Ϊ�����뿪������ʱ��һ����һ����Ϣ����
		OnObjectListLeave( valueBroadListNoSelf, NFCDataList() << self );
	}

	else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
	{
		//id��fd,gateid��
		NF_SHARE_PTR<BaseData> pDataBase = mRoleBaseData.GetElement(self);
		if (pDataBase.get())
		{
			//�ظ��ͻ��˽�ɫ������Ϸ����ɹ���
			NFMsg::AckEventResult xMsg;
			xMsg.set_event_code(NFMsg::EGEC_ENTER_GAME_SUCCESS);

			*xMsg.mutable_event_client() = NFToPB(pDataBase->xClientID);
			*xMsg.mutable_event_object() = NFToPB(self);

			SendMsgPBToGate(NFMsg::EGameMsgID::EGMI_ACK_ENTER_GAME, xMsg, self);
		}
	}
	else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
	{
	}
	else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
	{
		//�Լ��㲥���Լ��͹���
		if ( strClassName == "Player" )
		{
			OnObjectListEnter( NFCDataList() << self, NFCDataList() << self );

			OnPropertyEnter(NFCDataList() << self, self);
			OnRecordEnter(NFCDataList() << self, self);
		}
	}
	else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
	{

	}
	return 0;
}

int NFCGameServerNet_ServerModule::OnGroupEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar)
{
	//���������仯��ֻ���ܴ�A������0���л���B������0��
	//��Ҫע�����------------�κβ�ı��ʱ�򣬴������ʵ��δ����㣬��ˣ���ı��ʱ���ȡ������б�Ŀ����ǲ������Լ���
	int nSceneID = m_pKernelModule->GetPropertyInt( self, "SceneID" );

	//�㲥�������Լ���ȥ(�㽵����Ծ��)
	int nOldGroupID = oldVar.Int( 0 );
	if ( nOldGroupID > 0 )
	{
		NFCDataList valueAllOldObjectList;
		NFCDataList valueAllOldPlayerList;
		m_pKernelModule->GetGroupObjectList( nSceneID, nOldGroupID, valueAllOldObjectList );
		if ( valueAllOldObjectList.GetCount() > 0 )
		{
			//�Լ�ֻ��Ҫ�㲥�������
			for ( int i = 0; i < valueAllOldObjectList.GetCount(); i++ )
			{
				NFIDENTID identBC = valueAllOldObjectList.Object( i );

				if(valueAllOldObjectList.Object(i) == self)
				{
					valueAllOldObjectList.Set(i, NFIDENTID());
				}

				const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
				if ( "Player" == strClassName )
				{
					valueAllOldPlayerList.Add(identBC );
				}
			}

			OnObjectListLeave( valueAllOldPlayerList, NFCDataList() << self );

			//�ϵ�ȫ��Ҫ�㲥ɾ��
			OnObjectListLeave( NFCDataList() << self, valueAllOldObjectList );
		}

		m_pEventProcessModule->DoEvent(self, NFED_ON_CLIENT_LEAVE_SCENE, NFCDataList() << nOldGroupID);
	}

	//�ٹ㲥�������Լ�����(��������Ծ��)
	int nNewGroupID = newVar.Int( 0 );
	if ( nNewGroupID > 0 )
	{
		//������Ҫ���Լ��ӹ㲥���ų�
		//////////////////////////////////////////////////////////////////////////
		NFCDataList valueAllObjectList;
		NFCDataList valueAllObjectListNoSelf;
		NFCDataList valuePlayerList;
		NFCDataList valuePlayerListNoSelf;
		m_pKernelModule->GetGroupObjectList( nSceneID, nNewGroupID, valueAllObjectList );
		for ( int i = 0; i < valueAllObjectList.GetCount(); i++ )
		{
			NFIDENTID identBC = valueAllObjectList.Object( i );
			const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
			if ( "Player" == strClassName )
			{
				valuePlayerList.Add( identBC );
				if (identBC != self)
				{
					valuePlayerListNoSelf.Add( identBC );
				}
			}

			if (identBC != self)
			{
				valueAllObjectListNoSelf.Add( identBC );
			}
		}

		//�㲥������,�Լ�����(���ﱾ��Ӧ�ù㲥���Լ�)
		if ( valuePlayerListNoSelf.GetCount() > 0 )
		{
			OnObjectListEnter( valuePlayerListNoSelf, NFCDataList() << self );
		}

		const std::string& strSelfClassName = m_pKernelModule->GetPropertyString( self, "ClassName" );

		//�㲥���Լ�,���еı��˳���
		if ( valueAllObjectListNoSelf.GetCount() > 0 )
		{
			if (strSelfClassName == "Player")
			{
				OnObjectListEnter( NFCDataList() << self, valueAllObjectListNoSelf );
			}
		}

		if (strSelfClassName == "Player")
		{
			for ( int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++ )
			{
				//��ʱ�����ٹ㲥�Լ������Ը��Լ�
				//���Ѿ����ڵ��˵����Թ㲥����������
				NFIDENTID identOld = valueAllObjectListNoSelf.Object( i );

				OnPropertyEnter(NFCDataList() << self, identOld);
				//���Ѿ����ڵ��˵ı�㲥����������
				OnRecordEnter(NFCDataList() << self, identOld);
			}
		}

		//���������˵����Թ㲥���ܱߵ���
		if ( valuePlayerListNoSelf.GetCount() > 0 )
		{
			OnPropertyEnter(valuePlayerListNoSelf, self);
			OnRecordEnter(valuePlayerListNoSelf, self);
		}
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnContainerEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar)
{
	//���������仯��ֻ���ܴ�A������0���л���B������0��
	//��Ҫע�����------------�κ������ı��ʱ����ұ�����0��
	int nOldSceneID = oldVar.Int( 0 );
	int nNowSceneID = newVar.Int( 0 );

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Enter Scene:", nNowSceneID);

	//�Լ���ʧ,��Ҳ��ù㲥����Ϊ����ʧ֮ǰ����ص�0�㣬���ѹ㲥�����
	NFCDataList valueOldAllObjectList;
	NFCDataList valueNewAllObjectList;
	NFCDataList valueAllObjectListNoSelf;
	NFCDataList valuePlayerList;
	NFCDataList valuePlayerNoSelf;

	m_pKernelModule->GetGroupObjectList( nOldSceneID, 0, valueOldAllObjectList );
	m_pKernelModule->GetGroupObjectList( nNowSceneID, 0, valueNewAllObjectList );

	for ( int i = 0; i < valueOldAllObjectList.GetCount(); i++ )
	{
		NFIDENTID identBC = valueOldAllObjectList.Object( i );
		if (identBC == self)
		{
			valueOldAllObjectList.Set(i, NFIDENTID());
		}
	}

	for ( int i = 0; i < valueNewAllObjectList.GetCount(); i++ )
	{
		NFIDENTID identBC = valueNewAllObjectList.Object( i );
		const std::string& strClassName = m_pKernelModule->GetPropertyString( identBC, "ClassName" );
		if ( "Player" == strClassName )
		{
			valuePlayerList.Add( identBC );
			if (identBC != self)
			{
				valuePlayerNoSelf.Add( identBC );
			}
		}

		if (identBC != self)
		{
			valueAllObjectListNoSelf.Add( identBC );
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//���Ǿɳ���0���NPC��Ҫ�㲥
	OnObjectListLeave( NFCDataList() << self, valueOldAllObjectList );

	//�㲥�������˳��ֶ���(�������ң�������㲥���Լ�)
	//����㲥�Ķ���0���
	if ( valuePlayerList.GetCount() > 0 )
	{
		//��self�㲥��argVar��Щ��
		OnObjectListEnter( valuePlayerNoSelf, NFCDataList() << self);
	}

	//�²��Ȼ��0����0��NPC�㲥���Լ�------------�Լ��㲥���Լ���������㲥����Ϊ����ID�ڿ糡��ʱ�ᾭ���仯

	//��valueAllObjectList�㲥��self
	OnObjectListEnter( NFCDataList() << self, valueAllObjectListNoSelf );

	////////////////////���Ѿ����ڵ��˵����Թ㲥����������//////////////////////////////////////////////////////
	for ( int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++ )
	{
		NFIDENTID identOld = valueAllObjectListNoSelf.Object( i );
		OnPropertyEnter(NFCDataList() << self , identOld);
		////////////////////���Ѿ����ڵ��˵ı�㲥����������//////////////////////////////////////////////////////
		OnRecordEnter(NFCDataList() << self , identOld);
	}

	//���������˵����Թ㲥���ܱߵ���()
	if ( valuePlayerNoSelf.GetCount() > 0 )
	{
		OnPropertyEnter(valuePlayerNoSelf, self);
		OnRecordEnter(valuePlayerNoSelf, self);
	}

	return 0;
}

int NFCGameServerNet_ServerModule::GetBroadCastObject( const NFIDENTID& self, const std::string& strPropertyName, const bool bTable, NFIDataList& valueObject )
{
	int nObjectContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
	int nObjectGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

	//��ͨ�����������жϹ㲥����
	std::string strClassName = m_pKernelModule->GetPropertyString( self, "ClassName" );
	NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = m_pLogicClassModule->GetClassRecordManager( strClassName );
	NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager( strClassName );

	NF_SHARE_PTR<NFIRecord> pRecord( NULL );
	NF_SHARE_PTR<NFIProperty> pProperty( NULL );
	if ( bTable )
	{
		if ( NULL == pClassRecordManager )
		{
			return -1;
		}

		pRecord = pClassRecordManager->GetElement( strPropertyName );
		if ( NULL == pRecord.get() )
		{
			return -1;
		}
	}
	else
	{
		if ( NULL == pClassPropertyManager )
		{
			return -1;
		}
		pProperty = pClassPropertyManager->GetElement( strPropertyName );
		if ( NULL == pProperty.get() )
		{
			return -1;
		}
	}

	if ( "Player" == strClassName )
	{
		if ( bTable )
		{
			if ( pRecord->GetPublic() )
			{
				GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
			}
			else if ( pRecord->GetPrivate() )
			{
				valueObject.Add( self );
			}
		}
		else
		{
			if ( pProperty->GetPublic() )
			{
				GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
			}
			else if ( pProperty->GetPrivate() )
			{
				valueObject.Add( self );
			}
		}
		//һ����Ҷ����㲥
		return valueObject.GetCount();
	}

	//�������,NPC�͹������
	if ( bTable )
	{
		if ( pRecord->GetPublic() )
		{
			//�㲥���ͻ����Լ����ܱ���
			GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
		}
	}
	else
	{
		if ( pProperty->GetPublic() )
		{
			//�㲥���ͻ����Լ����ܱ���
			GetBroadCastObject( nObjectContainerID, nObjectGroupID, valueObject );
		}
	}

	return valueObject.GetCount();
}

int NFCGameServerNet_ServerModule::GetBroadCastObject( const int nObjectContainerID, const int nGroupID, NFIDataList& valueObject )
{
	NFCDataList valContainerObjectList;
	m_pKernelModule->GetGroupObjectList( nObjectContainerID, nGroupID, valContainerObjectList );
	for ( int i  = 0; i < valContainerObjectList.GetCount(); i++ )
	{
		const std::string& strObjClassName = m_pKernelModule->GetPropertyString( valContainerObjectList.Object( i ), "ClassName" );
		if ( "Player" == strObjClassName )
		{
			valueObject.Add( valContainerObjectList.Object( i ) );
		}
	}

	return valueObject.GetCount();
}

int NFCGameServerNet_ServerModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
	if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
	{
		//SaveDataToNoSql( self, true );
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Player Offline", "");
	}
	else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
	{
		//LoadDataFormNoSql( self );
	}
	else if ( CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
	{
		m_pEventProcessModule->AddEventCallBack(self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, this, &NFCGameServerNet_ServerModule::OnSwapSceneResultEvent);
		m_pEventProcessModule->AddEventCallBack(self, NFED_ON_NOTICE_ECTYPE_AWARD, this, &NFCGameServerNet_ServerModule::OnNoticeEctypeAward);
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnObjectNPCClassEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
	{
		m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_REQUIRE_MOVE, this, &NFCGameServerNet_ServerModule::OnMoveEvent );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int NFCGameServerNet_ServerModule::OnReturnEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
	return 0;
}

int NFCGameServerNet_ServerModule::OnMoveEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
	if (var.GetCount() != 3 || !var.TypeEx(TDATA_FLOAT, TDATA_FLOAT, TDATA_FLOAT, TDATA_UNKNOWN))
	{
		return 1;
	}

	NFMsg::ReqAckPlayerMove xMsg;
	NFMsg::Position* pPos = xMsg.add_target_pos();

	*xMsg.mutable_mover() = NFToPB(self);
	xMsg.set_movetype(1);   // �ƶ�������ʱ����һ��
	pPos->set_x(var.Float(0));
	pPos->set_y(var.Float(1));
	pPos->set_z(var.Float(2));

	//bc
	int nContianerID = m_pKernelModule->GetPropertyInt(PBToNF(xMsg.mover()), "SceneID");
	int nGroupID = m_pKernelModule->GetPropertyInt(PBToNF(xMsg.mover()), "GroupID");

	NFCDataList xDataList;
	m_pKernelModule->GetGroupObjectList(nContianerID, nGroupID, xDataList);
	for (int i = 0; i < xDataList.GetCount(); ++i)
	{
		const NFIDENTID& xIdent = xDataList.Object(i);

		SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE, xMsg, xIdent);
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnUseSkillResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
	return 0;
}

int NFCGameServerNet_ServerModule::OnSwapSceneResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
	if ( var.GetCount() != 7 ||
		!var.TypeEx(TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT,
		TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_FLOAT,
		TDATA_TYPE::TDATA_FLOAT, TDATA_TYPE::TDATA_FLOAT, TDATA_TYPE::TDATA_UNKNOWN)
		)
	{
		return 1;
	}

	NFIDENTID ident = var.Object( 0 );
	int nType = var.Int( 1 );
	int nTargetScene = var.Int( 2 );
	int nTargetGroupID = var.Int( 3 );
	float fX = var.Float( 4 );
	float fY = var.Float( 5 );
	float fZ = var.Float( 6 );

	NFMsg::ReqAckSwapScene xSwapScene;
	xSwapScene.set_transfer_type( NFMsg::ReqAckSwapScene::EGameSwapType::ReqAckSwapScene_EGameSwapType_EGST_NARMAL );
	xSwapScene.set_scene_id( nTargetScene );
	xSwapScene.set_line_id( nTargetGroupID );
	xSwapScene.set_x(fX);
	xSwapScene.set_y(fY);
	xSwapScene.set_z(fZ);

	SendMsgPBToGate(NFMsg::EGMI_ACK_SWAP_SCENE, xSwapScene, self);

	return 0;
}

int NFCGameServerNet_ServerModule::OnNoticeEctypeAward(const NFIDENTID& self, const int nEventID, const NFIDataList& var)
{
	if (var.GetCount() < 3)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Args error", "", __FUNCTION__, __LINE__);
		return 1;
	}

	if ((var.GetCount() - 3) % 2 != 0)
	{
		return 1;
	}

	int nStartIndex = 3;//3��

	int nAddMoney = var.Int(0);
	int nAddExp = var.Int(1);
	int nAddDiamond = var.Int(2);

	NFMsg::ReqAckEndBattle xMsg;

	xMsg.set_money(nAddMoney);
	xMsg.set_exp(nAddExp);
	xMsg.set_diamond(nAddDiamond);

	for (int i = nStartIndex; i < var.GetCount(); i += 2)
	{
		NFMsg::ItemStruct* pItemStruct = xMsg.add_item_list();
		if (pItemStruct)
		{
			pItemStruct->set_item_id(var.String(i));
			pItemStruct->set_item_count(var.Int(i + 1));
		}
	}

	SendMsgPBToGate(NFMsg::EGMI_ACK_END_BATTLE, xMsg, self);

	return 0;
}

int NFCGameServerNet_ServerModule::OnChatResultEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
	return 0;
}

void NFCGameServerNet_ServerModule::OnReqiureRoleListProcess( const NFIPacket& msg )
{
	//fd
	NFIDENTID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!RecivePB(msg, xMsg, nClientID))
	{
		return;
	}

	std::vector<std::string> vFieldVector;
	std::vector<std::string> vValueVector;

    vFieldVector.push_back("ID");
    vFieldVector.push_back("Level");
    vFieldVector.push_back("Sex");
	vFieldVector.push_back("Job");
    vFieldVector.push_back("Name");  
	vFieldVector.push_back("Race");

    const NFIDENTID ident = m_pDataProcessModule->GetChar(xMsg.account(), vFieldVector, vValueVector);
	if (ident.IsNull())
	{
		NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
		SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, msg.GetFd(), nClientID);
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
	pData->mutable_id()->CopyFrom(NFToPB(ident));
	pData->set_career(0);
	pData->set_sex(0);
	pData->set_race(0);
	pData->set_noob_name("");
	pData->set_game_id(xMsg.game_id());
	pData->set_role_level(0);
	pData->set_delete_time(0);
	pData->set_reg_time(0);
	pData->set_last_offline_time(0);
	pData->set_last_offline_ip(0);
	pData->set_view_record("");

    NFIDENTID xID;
    int nLevel = 0;
    int nSex = 0;
	int nJob = 0;
	int nRace = 0;
    std::string strName;

    if (!xID.FromString(vValueVector[0]))
    {
        return;
    }
    
    if (!NF_StrTo(vValueVector[1], nLevel))
    {
        return ;
    }

    if (!NF_StrTo(vValueVector[2], nSex))
    {
        return ;
    }

    if (!NF_StrTo(vValueVector[3], nJob))
    {
        return ;
    }

    strName = vValueVector[4];

	if (!NF_StrTo(vValueVector[5], nRace))
	{
		return ;
	}
    pData->set_role_level(nLevel);
    pData->set_sex(nSex);
    pData->set_career(nJob);
	pData->set_noob_name(strName);
	pData->set_race(nRace);

	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, msg.GetFd(), nClientID);
}

void NFCGameServerNet_ServerModule::OnCreateRoleGameProcess( const NFIPacket& msg )
{
	NFIDENTID nClientID;
	NFMsg::ReqCreateRole xMsg;
	if (!RecivePB(msg, xMsg, nClientID))
	{
		return;
	}

	NFIDENTID xRoleID = m_pDataProcessModule->CreateRole(xMsg.account(), xMsg.noob_name(), xMsg.race(), xMsg.career(), xMsg.sex());
	if (xRoleID.IsNull())
	{
		return;
	}

	//m_pDataProcessModule->GetChar()

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
	pData->mutable_id()->CopyFrom(NFToPB(xRoleID));
	pData->set_career(xMsg.career());
	pData->set_sex(xMsg.sex());
	pData->set_race(xMsg.race());
	pData->set_noob_name(xMsg.noob_name());
	pData->set_game_id(xMsg.game_id());
	pData->set_role_level(1);
	pData->set_delete_time(0);
	pData->set_reg_time(0);
	pData->set_last_offline_time(0);
	pData->set_last_offline_ip(0);
	pData->set_view_record("");

	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, msg.GetFd(), nClientID);
}

void NFCGameServerNet_ServerModule::OnDeleteRoleGameProcess( const NFIPacket& msg )
{
	NFIDENTID nPlayerID;
	NFMsg::ReqDeleteRole xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return;
	}

	m_pDataProcessModule->DeleteRole(xMsg.account(), nPlayerID);

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, msg.GetFd(), nPlayerID);
}

void NFCGameServerNet_ServerModule::OnClienSwapSceneProcess( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckSwapScene)

	NFCDataList varEntry;
	varEntry << pObject->Self();
	varEntry << 0;
	varEntry << xMsg.scene_id();
	varEntry << -1;
	m_pEventProcessModule->DoEvent( pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry );
}

void NFCGameServerNet_ServerModule::OnClienUseSkill( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckUseSkill)

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
		const NFIDENTID nTarget = PBToNF(xEffectData.effect_ident());
		// �����˺�
		m_pSkillModule->OnUseSkill(nPlayerID, NFCDataList() << strSkillID << nTarget);

		NFMsg::EffectData* pNewEffectData = xReqAckUseSkill.add_effect_data();

		*pNewEffectData->mutable_effect_ident() = NFToPB(nTarget);
		pNewEffectData->set_effect_value(20);// ��ʱ����
		pNewEffectData->set_effect_rlt(xEffectData.effect_rlt());
	}

	SendMsgPBToGate(NFMsg::EGMI_ACK_SKILL_OBJECTX, xReqAckUseSkill, nPlayerID);
}

void NFCGameServerNet_ServerModule::OnClienMove( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckPlayerMove)

	m_pKernelModule->SetPropertyFloat(PBToNF(xMsg.mover()), "X", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(PBToNF(xMsg.mover()), "Y", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(PBToNF(xMsg.mover()), "Z", xMsg.target_pos(0).z());

	//bc
	int nContianerID = m_pKernelModule->GetPropertyInt(PBToNF(xMsg.mover()), "SceneID");
	int nGroupID = m_pKernelModule->GetPropertyInt(PBToNF(xMsg.mover()), "GroupID");

	NFCDataList xDataList;
	m_pKernelModule->GetGroupObjectList(nContianerID, nGroupID, xDataList);
	for (int i = 0; i < xDataList.GetCount(); ++i)
	{
		SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE, xMsg, xDataList.Object(i));
	}
}

void NFCGameServerNet_ServerModule::OnClienMoveImmune( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckPlayerMove)

	m_pKernelModule->SetPropertyFloat(PBToNF(xMsg.mover()), "X", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(PBToNF(xMsg.mover()), "Y", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(PBToNF(xMsg.mover()), "Z", xMsg.target_pos(0).z());

	//bc
	int nContianerID = m_pKernelModule->GetPropertyInt(PBToNF(xMsg.mover()), "SceneID");
	int nGroupID = m_pKernelModule->GetPropertyInt(PBToNF(xMsg.mover()), "GroupID");

	NFCDataList xDataList;
	m_pKernelModule->GetGroupObjectList(nContianerID, nGroupID, xDataList);
	for (int i = 0; i < xDataList.GetCount(); ++i)
	{
		SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE_IMMUNE, xMsg, xDataList.Object(i));
	}
}

void NFCGameServerNet_ServerModule::OnClienCommand( const NFIPacket& msg )
{

}

void NFCGameServerNet_ServerModule::OnClienAcceptTask( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAcceptTask)
}

void NFCGameServerNet_ServerModule::OnClienPushTask( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqCompeleteTask)
}

void NFCGameServerNet_ServerModule::OnClienPushCustom( const NFIPacket& msg )
{
	//CLIENT_MSG_PROCESS(msg, NFMsg::cu)
}

void NFCGameServerNet_ServerModule::OnClienPickItem( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqPickDropItem)
}

void NFCGameServerNet_ServerModule::OnClienChatProcess( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckPlayerChat)

	switch (xMsg.chat_type())
	{
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_WORLD:
		{
			SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
		}
		break;
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_GUILD:
		{
			NFIDENTID xGuildID = m_pKernelModule->GetPropertyObject(nPlayerID, "GuildID");
			if (!xGuildID.IsNull())
			{
				OnTransWorld(msg, xGuildID.nData64);
			}
		}
		break;
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_PRIVATE:
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_TEAM:
		{
			if (xMsg.has_target_id())
			{
				NFIDENTID xTargetID = PBToNF(xMsg.target_id());
				if (!xTargetID.IsNull())
				{
					OnTransWorld(msg, nPlayerID.nData64);
				}
			}
		}
		break;
	default:
		break;;
	}
	

}

void NFCGameServerNet_ServerModule::OnClientJoinPVP(const NFIPacket& msg)
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckJoinActivity)

	switch (xMsg.activity_type())
	{
		case NFMsg::ReqAckJoinActivity_EGameActivityType_EGAT_PVP:
			m_pPVPModule->StartPVPWar(nPlayerID);
			break;
		default:
			break;
	}
}

void NFCGameServerNet_ServerModule::OnClientExitPVP(const NFIPacket& msg)
{
	//CLIENT_MSG_PROCESS(msg, NFMsg::reqex)

}

void NFCGameServerNet_ServerModule::OnClienUseItem( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckUseItem)
}

void NFCGameServerNet_ServerModule::OnProxyServerRegisteredProcess( const NFIPacket& msg )
{
	NFIDENTID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
		NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.GetElement(pData->server_id());
		if (!pServerData.get())
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mProxyMap.AddElement(pData->server_id(), pServerData);
		}

		pServerData->nFD = msg.GetFd();
		*(pServerData->pData) = *pData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Proxy Registered");
	}

	return;
}

void NFCGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess( const NFIPacket& msg )
{
	NFIDENTID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
		mProxyMap.RemoveElement(pData->server_id());


		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Proxy UnRegistered");
	}

	return;
}

void NFCGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess( const NFIPacket& msg )
{
	NFIDENTID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
		NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.GetElement(pData->server_id());
		if (!pServerData.get())
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mProxyMap.AddElement(pData->server_id(), pServerData);
		}

		pServerData->nFD = msg.GetFd();
		*(pServerData->pData) = *pData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Proxy Registered");
	}

	return;
}

void NFCGameServerNet_ServerModule::OnClienGMProcess( const NFIPacket& msg )
{
	NFIDENTID nPlayerID;
	NFMsg::ReqCommand xMsg;
	if (!RecivePB(msg, xMsg, nPlayerID))
	{
		return;
	}

	if (nPlayerID != PBToNF(xMsg.control_id()))
	{
		return;
	}

	switch (xMsg.command_id())
	{
	case NFMsg::ReqCommand_EGameCommandType_EGCT_MODIY_PROPERTY:
		{
			const std::string& strPropertyName = xMsg.command_str_value();
			//const int nValue = xMsg.command_value();
			//m_pKernelModule->SetPropertyInt(nPlayerID, strPropertyName, nValue);
		}
		break;
	case NFMsg::ReqCommand_EGameCommandType_EGCT_CREATE_OBJECT:
		{
			const int nContianerID = m_pKernelModule->GetPropertyInt(nPlayerID, "SceneID");
			const int nGroupID = m_pKernelModule->GetPropertyInt(nPlayerID, "GroupID");

			float fX = m_pKernelModule->GetPropertyFloat(nPlayerID, "X");
			float fY = m_pKernelModule->GetPropertyFloat(nPlayerID, "Y");
			float fZ = m_pKernelModule->GetPropertyFloat(nPlayerID, "Z");

			const std::string& strObjectIndex = xMsg.command_str_value();
			//const int nValue = xMsg.command_value();
			if (m_pElementInfoModule->ExistElement(strObjectIndex))
			{
				NFCDataList xDataList;
				xDataList << "X" << fX;
				xDataList << "Y" << fY;
				xDataList << "Z" << fZ;
				const std::string& strObjectClass = m_pElementInfoModule->GetPropertyString(strObjectIndex, "ClassName");
				m_pKernelModule->CreateObject(NFIDENTID(), nContianerID, nGroupID, strObjectClass, strObjectIndex, xDataList);
			}
		}
		break;
	default:
		break;
	}
}

void NFCGameServerNet_ServerModule::OnClientEndBattle(const NFIPacket& msg)
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckEndBattle);
	m_pEctypeModule->OnEctypeSettleEvent(nPlayerID, 1, 3); // ��ʱ�ȸ�1ͨ����3��
}

//SLG////////////////////////////////////////////////////////////////////////
void NFCGameServerNet_ServerModule::OnSLGClienBuyItem(const NFIPacket& msg)
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckBuyObjectFormShop);

	const std::string strItemID = xMsg.config_id();
	const float fX = xMsg.x();
	const float fY = xMsg.y();
	const float fZ = xMsg.z();
	m_pSLGShopModule->ReqBuyItem(nPlayerID, strItemID, fX, fY, fZ);
}

void NFCGameServerNet_ServerModule::OnSLGClienMoveObject(const NFIPacket& msg)
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqAckMoveBuildObject);

	const NFIDENTID xBuildID = PBToNF(xMsg.object_guid());
	const float fX = xMsg.x();
	const float fY = xMsg.y();
	const float fZ = xMsg.z();

	m_pSLGBuildingModule->Move(nPlayerID, xBuildID, fX, fY, fZ);
}

void NFCGameServerNet_ServerModule::OnSLGClienUpgradeBuilding(const NFIPacket& msg)
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqUpBuildLv);

	const NFIDENTID xBuilID = PBToNF(xMsg.object_guid());
	m_pSLGBuildingModule->Upgrade(nPlayerID, xBuilID);
}

void NFCGameServerNet_ServerModule::OnSLGClienCreateItem(const NFIPacket& msg)
{
	CLIENT_MSG_PROCESS(msg, NFMsg::ReqCreateItem);

	const NFIDENTID& xBuilID = PBToNF(xMsg.object_guid());
	const std::string& strItemID = xMsg.config_id();
	const int nCount = xMsg.count();

	m_pSLGBuildingModule->Produce(nPlayerID, xBuilID, strItemID, nCount);
}

void NFCGameServerNet_ServerModule::SendMsgPBToGate( const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFIDENTID self )
{
	NF_SHARE_PTR<BaseData> pData = mRoleBaseData.GetElement(self);
	if (pData.get())
	{
		NF_SHARE_PTR<ServerData> pProxyData = mProxyMap.GetElement(pData->nGateID);
		if (pProxyData.get())
		{
			SendMsgPB(nMsgID, xMsg, pProxyData->nFD, pData->xClientID);
		}
	}
}

void NFCGameServerNet_ServerModule::SendMsgPBToGate( const uint16_t nMsgID, const std::string& strMsg, const NFIDENTID self )
{
    NF_SHARE_PTR<BaseData> pData = mRoleBaseData.GetElement(self);
    if (pData.get())
    {
        NF_SHARE_PTR<ServerData> pProxyData = mProxyMap.GetElement(pData->nGateID);
        if (pProxyData.get())
        {
            SendMsgPB(nMsgID, strMsg, pProxyData->nFD, pData->xClientID);
        }
    }
}

void NFCGameServerNet_ServerModule::PlayerLeaveGameServer( const NFIDENTID self )
{
	if(!m_pKernelModule->GetObject(self))
	{
		return;
	}

	m_pKernelModule->DestroyObject(self);


	NF_SHARE_PTR<BaseData> pBaseData = mRoleBaseData.GetElement(self);
	if (nullptr == pBaseData)
	{
		return;
	}

	mRoleBaseData.RemoveElement(self);


	NF_SHARE_PTR<ServerData> pServerData = mProxyMap.GetElement(pBaseData->nGateID);
	if (nullptr == pServerData)
	{
		return;
	}

	pServerData->xRoleInfo.erase(self);
}

void NFCGameServerNet_ServerModule::OnTransWorld( const NFIPacket& msg )
{
    std::string strMsg;
    NFIDENTID nPlayer;
    int nHasKey = 0;
    if (NFINetModule::RecivePB(msg, strMsg, nPlayer))
    {
        nHasKey = nPlayer.nData64;
    }
    
    m_pGameServerToWorldModule->SendBySuit(nHasKey, msg);
}

void NFCGameServerNet_ServerModule::OnTransWorld( const NFIPacket& msg, const int nWorldKey )
{
    m_pGameServerToWorldModule->SendBySuit(nWorldKey, msg);
}