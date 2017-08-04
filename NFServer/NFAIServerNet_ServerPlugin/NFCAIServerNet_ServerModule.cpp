// -------------------------------------------------------------------------
//    @FileName			:    NFCAIServerNet_ServerModule.cpp
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFCAIServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCAIServerNet_ServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCAIServerNet_ServerModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

	return true;
}

bool NFCAIServerNet_ServerModule::AfterInit()
{

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REFRESH, this, &NFCAIServerNet_ServerModule::OnRefreshProxyServerInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REGISTERED, this, &NFCAIServerNet_ServerModule::OnProxyServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_UNREGISTERED, this, &NFCAIServerNet_ServerModule::OnProxyServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME, this, &NFCAIServerNet_ServerModule::OnClienEnterGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_GAME, this, &NFCAIServerNet_ServerModule::OnClienLeaveGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST, this, &NFCAIServerNet_ServerModule::OnReqiureRoleListProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE, this, &NFCAIServerNet_ServerModule::OnCreateRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE, this, &NFCAIServerNet_ServerModule::OnDeleteRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SWAP_SCENE, this, &NFCAIServerNet_ServerModule::OnClienSwapSceneProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME_FINISH, this, &NFCAIServerNet_ServerModule::OnClienEnterGameFinishProcess);
	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_INT, this, &NFCAIServerNet_ServerModule::OnClientPropertyIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_FLOAT, this, &NFCAIServerNet_ServerModule::OnClientPropertyFloatProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_STRING, this, &NFCAIServerNet_ServerModule::OnClientPropertyStringProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_OBJECT, this, &NFCAIServerNet_ServerModule::OnClientPropertyObjectProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_VECTOR2, this, &NFCAIServerNet_ServerModule::OnClientPropertyVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_VECTOR3, this, &NFCAIServerNet_ServerModule::OnClientPropertyVector3Process);

	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_ADD_ROW, this, &NFCAIServerNet_ServerModule::OnClientAddRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_REMOVE_ROW, this, &NFCAIServerNet_ServerModule::OnClientRemoveRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_SWAP_ROW, this, &NFCAIServerNet_ServerModule::OnClientSwapRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_INT, this, &NFCAIServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_FLOAT, this, &NFCAIServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_STRING, this, &NFCAIServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_OBJECT, this, &NFCAIServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_VECTOR2, this, &NFCAIServerNet_ServerModule::OnClientRecordVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_VECTOR3, this, &NFCAIServerNet_ServerModule::OnClientRecordVector3Process);

	//EGMI_ACK_RECORD_CLEAR = 228,
	//EGMI_ACK_RECORD_SORT = 229,

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_GUILD, this, &NFCAIServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_CREATE_CHATGROUP, this, &NFCAIServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_JOIN_CHATGROUP, this, &NFCAIServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_LEAVE_CHATGROUP, this, &NFCAIServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SUBSCRIPTION_CHATGROUP, this, &NFCAIServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_MOVE, this, &NFCAIServerNet_ServerModule::OnClienReqMoveProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_MOVE_IMMUNE, this, &NFCAIServerNet_ServerModule::OnClienReqMoveImmuneProcess);

	m_pNetModule->AddEventCallBack(this, &NFCAIServerNet_ServerModule::OnSocketPSEvent);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCAIServerNet_ServerModule::OnObjectClassEvent);

	m_pSceneAOIModule->AddObjectEnterCallBack(this, &NFCAIServerNet_ServerModule::OnObjectListEnter);
	m_pSceneAOIModule->AddObjectDataFinishedCallBack(this, &NFCAIServerNet_ServerModule::OnObjectDataFinished);
	m_pSceneAOIModule->AddObjectLeaveCallBack(this, &NFCAIServerNet_ServerModule::OnObjectListLeave);
	m_pSceneAOIModule->AddPropertyEnterCallBack(this, &NFCAIServerNet_ServerModule::OnPropertyEnter);
	m_pSceneAOIModule->AddRecordEnterCallBack(this, &NFCAIServerNet_ServerModule::OnRecordEnter);
	m_pSceneAOIModule->AddPropertyEventCallBack(this, &NFCAIServerNet_ServerModule::OnPropertyEvent);
	m_pSceneAOIModule->AddRecordEventCallBack(this, &NFCAIServerNet_ServerModule::OnRecordEvent);

	m_pSceneAOIModule->AddSwapSceneEventCallBack(this, &NFCAIServerNet_ServerModule::OnSceneEvent);

	/////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (nServerType == NF_SERVER_TYPES::NF_ST_AI && pPluginManager->GetAppID() == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				int nRet = m_pNetModule->Initialization(nMaxConnect, nPort, nCpus);
				if (nRet < 0)
				{
					std::ostringstream strLog;
					strLog << "Cannot init server net, Port = " << nPort;
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
					NFASSERT(nRet, "Cannot init server net", __FILE__, __FUNCTION__);
					exit(0);
				}
			}
		}
	}

	return true;
}

bool NFCAIServerNet_ServerModule::Shut()
{

	return true;
}

bool NFCAIServerNet_ServerModule::Execute()
{
	return true;
}

void NFCAIServerNet_ServerModule::OnSocketPSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else  if (eEvent == NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
		OnClientConnected(nSockIndex);
	}
}

void NFCAIServerNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
	int nServerID = 0;
	NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (nAddress == pServerData->xServerData.nFD)
		{
			nServerID = pServerData->xServerData.pData->server_id();
			break;
		}

		pServerData = mProxyMap.Next();
	}

	mProxyMap.RemoveElement(nServerID);
}

void NFCAIServerNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{

}

void NFCAIServerNet_ServerModule::OnClienEnterGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqEnterGameServer xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());

	if (m_pKernelModule->GetObject(nRoleID))
	{
		//it should be rebind with proxy's netobject
		m_pKernelModule->DestroyObject(nRoleID);
	}

	//////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIAIServerNet_ServerModule::GateBaseInfo>  pGateInfo = GetPlayerGateInfo(nRoleID);
	if (nullptr != pGateInfo)
	{
		RemovePlayerGateInfo(nRoleID);
	}

	NF_SHARE_PTR<NFIAIServerNet_ServerModule::GateServerInfo> pGateServerinfo = GetGateServerInfoBySockIndex(nSockIndex);
	if (nullptr == pGateServerinfo)
	{
		return;
	}

	int nGateID = -1;
	if (pGateServerinfo->xServerData.pData)
	{
		nGateID = pGateServerinfo->xServerData.pData->server_id();
	}

	if (nGateID < 0)
	{
		return;
	}

	if (!AddPlayerGateInfo(nRoleID, nClientID, nGateID))
	{
		return;
	}

	int nSceneID = 1;
	NFDataList var;
	var.AddString(NFrame::Player::Name());
	var.AddString(xMsg.name());

	var.AddString(NFrame::Player::GateID());
	var.AddInt(nGateID);

	var.AddString(NFrame::Player::GameID());
	var.AddInt(pPluginManager->GetAppID());

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, nSceneID, 0, NFrame::Player::ThisName(), "", var);
	if (NULL == pObject)
	{
		//mRoleBaseData
		//mRoleFDData
		return;
	}

	m_pSceneAOIModule->RequestEnterScene(pObject->Self(), nSceneID, 1, 0, NFDataList());
}

void NFCAIServerNet_ServerModule::OnClienLeaveGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqLeaveGameServer xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if (nPlayerID.IsNull())
	{
		return;
	}

	if (m_pKernelModule->GetObject(nPlayerID))
	{
		m_pKernelModule->DestroyObject(nPlayerID);
	}

	RemovePlayerGateInfo(nPlayerID);
}

int NFCAIServerNet_ServerModule::OnPropertyEnter(const NFDataList& argVar, const NFGUID& self)
{
	if (argVar.GetCount() <= 0 || self.IsNull())
	{
		return 0;
	}

	NFMsg::MultiObjectPropertyList xPublicMsg;
	NFMsg::MultiObjectPropertyList xPrivateMsg;
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (pObject)
	{
		NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();
		NFMsg::ObjectPropertyList* pPrivateData = xPrivateMsg.add_multi_player_property();

		*(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(self);
		*(pPrivateData->mutable_player_id()) = NFINetModule::NFToPB(self);

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIProperty> pPropertyInfo = pPropertyManager->First();
		while (pPropertyInfo)
		{
			if (pPropertyInfo->Changed())
			{
				switch (pPropertyInfo->GetType())
				{
				case TDATA_INT:
				{
					if (pPropertyInfo->GetPublic())
					{
						NFMsg::PropertyInt* pDataInt = pPublicData->add_property_int_list();
						pDataInt->set_property_name(pPropertyInfo->GetKey());
						pDataInt->set_data(pPropertyInfo->GetInt());
					}

					if (pPropertyInfo->GetPrivate())
					{
						NFMsg::PropertyInt* pDataInt = pPrivateData->add_property_int_list();
						pDataInt->set_property_name(pPropertyInfo->GetKey());
						pDataInt->set_data(pPropertyInfo->GetInt());
					}
				}
				break;

				case TDATA_FLOAT:
				{
					if (pPropertyInfo->GetPublic())
					{
						NFMsg::PropertyFloat* pDataFloat = pPublicData->add_property_float_list();
						pDataFloat->set_property_name(pPropertyInfo->GetKey());
						pDataFloat->set_data(pPropertyInfo->GetFloat());
					}

					if (pPropertyInfo->GetPrivate())
					{
						NFMsg::PropertyFloat* pDataFloat = pPrivateData->add_property_float_list();
						pDataFloat->set_property_name(pPropertyInfo->GetKey());
						pDataFloat->set_data(pPropertyInfo->GetFloat());
					}
				}
				break;

				case TDATA_STRING:
				{
					if (pPropertyInfo->GetPublic())
					{
						NFMsg::PropertyString* pDataString = pPublicData->add_property_string_list();
						pDataString->set_property_name(pPropertyInfo->GetKey());
						pDataString->set_data(pPropertyInfo->GetString());
					}

					if (pPropertyInfo->GetPrivate())
					{
						NFMsg::PropertyString* pDataString = pPrivateData->add_property_string_list();
						pDataString->set_property_name(pPropertyInfo->GetKey());
						pDataString->set_data(pPropertyInfo->GetString());
					}
				}
				break;

				case TDATA_OBJECT:
				{
					if (pPropertyInfo->GetPublic())
					{
						NFMsg::PropertyObject* pDataObject = pPublicData->add_property_object_list();
						pDataObject->set_property_name(pPropertyInfo->GetKey());
						*(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetObject());
					}

					if (pPropertyInfo->GetPrivate())
					{
						NFMsg::PropertyObject* pDataObject = pPrivateData->add_property_object_list();
						pDataObject->set_property_name(pPropertyInfo->GetKey());
						*(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetObject());
					}
				}
				break;

				default:
					break;
				}
			}

			pPropertyInfo = pPropertyManager->Next();
		}

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOther = argVar.Object(i);
			if (self == identOther)
			{
				
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

	for (int i = 0; i < pRecord->GetRows(); i++)
	{
		if (pRecord->IsUsed(i))
		{
			
			NFMsg::RecordAddRowStruct* pAddRowStruct = pObjectRecordBase->add_row_struct();
			pAddRowStruct->set_row(i);

			for (int j = 0; j < pRecord->GetCols(); j++)
			{
				
				NFDataList valueList;
				NFDATA_TYPE eType = pRecord->GetColType(j);
				switch (eType)
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					int64_t nValue = pRecord->GetInt(i, j);
					//if ( 0 != nValue )
					{
						NFMsg::RecordInt* pAddData = pAddRowStruct->add_record_int_list();
						pAddData->set_row(i);
						pAddData->set_col(j);
						pAddData->set_data(nValue);
					}
				}
				break;
				case NFDATA_TYPE::TDATA_FLOAT:
				{
					double dwValue = pRecord->GetFloat(i, j);
					//if ( dwValue < -0.01f || dwValue > 0.01f )
					{
						NFMsg::RecordFloat* pAddData = pAddRowStruct->add_record_float_list();
						pAddData->set_row(i);
						pAddData->set_col(j);
						pAddData->set_data(dwValue);
					}
				}
				break;
				case NFDATA_TYPE::TDATA_STRING:
				{
					const std::string& strData = pRecord->GetString(i, j);
					//if ( !strData.empty() )
					{
						NFMsg::RecordString* pAddData = pAddRowStruct->add_record_string_list();
						pAddData->set_row(i);
						pAddData->set_col(j);
						pAddData->set_data(strData);
					}
				}
				break;
				case NFDATA_TYPE::TDATA_OBJECT:
				{
					NFGUID ident = pRecord->GetObject(i, j);
					//if ( !ident.IsNull() )
					{
						NFMsg::RecordObject* pAddData = pAddRowStruct->add_record_object_list();
						pAddData->set_row(i);
						pAddData->set_col(j);
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

int NFCAIServerNet_ServerModule::OnRecordEnter(const NFDataList& argVar, const NFGUID& self)
{
	if (argVar.GetCount() <= 0 || self.IsNull())
	{
		return 0;
	}

	NFMsg::MultiObjectRecordList xPublicMsg;
	NFMsg::MultiObjectRecordList xPrivateMsg;

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (pObject)
	{
		NFMsg::ObjectRecordList* pPublicData = NULL;
		NFMsg::ObjectRecordList* pPrivateData = NULL;

		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();
		NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
		while (pRecord)
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
					*(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(self);
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
					*(pPrivateData->mutable_player_id()) = NFINetModule::NFToPB(self);
				}
				pPrivateRecordBase = pPrivateData->add_record_list();
				pPrivateRecordBase->set_record_name(pRecord->GetName());

				OnRecordEnterPack(pRecord, pPrivateRecordBase);
			}

			pRecord = pRecordManager->Next();
		}


		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOther = argVar.Object(i);
			if (self == identOther)
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

int NFCAIServerNet_ServerModule::OnPropertyEvent(const NFGUID & self, const std::string & strProperty, const NFData & oldVar, const NFData & newVar, const NFDataList & argVar)
{
	if (NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString(self, NFrame::Player::ClassName()))
	{
		NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
		if (xObject->GetState() != CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
		{
			return 0;
		}
	}

	switch (oldVar.GetType())
	{
	case TDATA_INT:
	{
		NFMsg::ObjectPropertyInt xPropertyInt;
		NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
		pDataInt->set_property_name(strProperty);
		pDataInt->set_data(newVar.GetInt());

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_INT, xPropertyInt, identOld);
		}
	}
	break;

	case TDATA_FLOAT:
	{
		NFMsg::ObjectPropertyFloat xPropertyFloat;
		NFMsg::Ident* pIdent = xPropertyFloat.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
		pDataFloat->set_property_name(strProperty);
		pDataFloat->set_data(newVar.GetFloat());

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_FLOAT, xPropertyFloat, identOld);
		}
	}
	break;

	case TDATA_STRING:
	{
		NFMsg::ObjectPropertyString xPropertyString;
		NFMsg::Ident* pIdent = xPropertyString.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
		pDataString->set_property_name(strProperty);
		pDataString->set_data(newVar.GetString());

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_STRING, xPropertyString, identOld);
		}
	}
	break;

	case TDATA_OBJECT:
	{
		NFMsg::ObjectPropertyObject xPropertyObject;
		NFMsg::Ident* pIdent = xPropertyObject.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
		pDataObject->set_property_name(strProperty);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_OBJECT, xPropertyObject, identOld);
		}
	}
	break;

	default:
		break;
	}

	return 0;
}

int NFCAIServerNet_ServerModule::OnRecordEvent(const NFGUID & self, const std::string& strRecord, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar, const NFDataList & argVar)
{
	if (NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString(self, NFrame::Player::ClassName()))
	{
		NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
		if (xObject->GetState() != CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
		{
			return 0;
		}
	}

	switch (xEventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		NFMsg::ObjectRecordAddRow xAddRecordRow;
		NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		xAddRecordRow.set_record_name(strRecord);

		NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
		pAddRowData->set_row(xEventData.nRow);


		NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, strRecord);
		if (xRecord)
		{
			NFDataList xRowDataList;
			if (xRecord->QueryRow(xEventData.nRow, xRowDataList))
			{
				for (int i = 0; i < xRowDataList.GetCount(); i++)
				{
					switch (xRowDataList.Type(i))
					{
					case TDATA_INT:
					{

						int64_t nValue = xRowDataList.Int(i);

						NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						pAddData->set_data(nValue);
					}
					break;
					case TDATA_FLOAT:
					{
						double fValue = xRowDataList.Float(i);

						NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						pAddData->set_data(fValue);
					}
					break;
					case TDATA_STRING:
					{
						const std::string& str = xRowDataList.String(i);
						NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						pAddData->set_data(str);
					}
					break;
					case TDATA_OBJECT:
					{
						NFGUID identValue = xRowDataList.Object(i);
						NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);

						*pAddData->mutable_data() = NFINetModule::NFToPB(identValue);
					}
					break;
					default:
						break;
					}
				}

				for (int i = 0; i < argVar.GetCount(); i++)
				{
					NFGUID identOther = argVar.Object(i);

					SendMsgPBToGate(NFMsg::EGMI_ACK_ADD_ROW, xAddRecordRow, identOther);
				}
			}
		}
	}
	break;
	case RECORD_EVENT_DATA::Del:
	{
		NFMsg::ObjectRecordRemove xReoveRecordRow;

		NFMsg::Ident* pIdent = xReoveRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		xReoveRecordRow.set_record_name(strRecord);
		xReoveRecordRow.add_remove_row(xEventData.nRow);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOther = argVar.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_REMOVE_ROW, xReoveRecordRow, identOther);
		}
	}
	break;
	case RECORD_EVENT_DATA::Swap:
	{

		NFMsg::ObjectRecordSwap xSwapRecord;
		*xSwapRecord.mutable_player_id() = NFINetModule::NFToPB(self);

		xSwapRecord.set_origin_record_name(strRecord);
		xSwapRecord.set_target_record_name(strRecord);
		xSwapRecord.set_row_origin(xEventData.nRow);
		xSwapRecord.set_row_target(xEventData.nCol);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOther = argVar.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_SWAP_ROW, xSwapRecord, identOther);
		}
	}
	break;
	case RECORD_EVENT_DATA::Update:
	{
		switch (oldVar.GetType())
		{
		case TDATA_INT:
		{
			NFMsg::ObjectRecordInt xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			int64_t nData = newVar.GetInt();
			recordProperty->set_data(nData);

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_INT, xRecordChanged, identOther);
			}
		}
		break;

		case TDATA_FLOAT:
		{
			NFMsg::ObjectRecordFloat xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			recordProperty->set_data(newVar.GetFloat());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_FLOAT, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_STRING:
		{
			NFMsg::ObjectRecordString xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			recordProperty->set_data(newVar.GetString());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_STRING, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_OBJECT:
		{
			NFMsg::ObjectRecordObject xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecord);
			NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

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
	case RECORD_EVENT_DATA::Create:
		return 0;
		break;
	case RECORD_EVENT_DATA::Cleared:
	{
		//             NFMsg::ObjectRecordObject xRecordChanged;
		//             xRecordChanged.set_player_id( self.nData64 );
		//             xRecordChanged.set_record_name( strRecordName );
		//
		//             for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
		//             {
		//                 NFGUID identOther = valueBroadCaseList.Object( i );
		//                 SendMsgPB(NFMsg::EGMI_ACK_RECORD_CLEAR, xRecordChanged, 0);
		//             }
	}
	break;
	default:
		break;
	}
	return 0;
}

int NFCAIServerNet_ServerModule::OnObjectListEnter(const NFDataList& self, const NFDataList& argVar)
{
	if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
	{
		return 0;
	}

	NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
	for (int i = 0; i < argVar.GetCount(); i++)
	{
		NFGUID identOld = argVar.Object(i);
		
		if (identOld.IsNull())
		{
			continue;
		}

		NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
		*(pEntryInfo->mutable_object_guid()) = NFINetModule::NFToPB(identOld);

		NFVector3 vPos = m_pKernelModule->GetPropertyVector3(identOld, NFrame::IObject::Position());
		pEntryInfo->set_x(vPos.X());
		pEntryInfo->set_y(vPos.Y());
		pEntryInfo->set_z(vPos.Z());
		pEntryInfo->set_career_type(m_pKernelModule->GetPropertyInt32(identOld, NFrame::Player::Job()));
		pEntryInfo->set_player_state(0);
		pEntryInfo->set_config_id(m_pKernelModule->GetPropertyString(identOld, NFrame::Player::ConfigID()));
		pEntryInfo->set_scene_id(m_pKernelModule->GetPropertyInt32(identOld, NFrame::Player::SceneID()));
		pEntryInfo->set_class_id(m_pKernelModule->GetPropertyString(identOld, NFrame::Player::ClassName()));

	}

	if (xPlayerEntryInfoList.object_list_size() <= 0)
	{
		return 0;
	}

	for (int i = 0; i < self.GetCount(); i++)
	{
		NFGUID ident = self.Object(i);
		if (ident.IsNull())
		{
			continue;
		}

		
		SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerEntryInfoList, ident);
	}

	return 1;
}

int NFCAIServerNet_ServerModule::OnObjectDataFinished(const NFDataList & self, const NFDataList & argVar)
{
	if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
	{
		return 0;
	}

	NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
	for (int i = 0; i < argVar.GetCount(); i++)
	{
		NFGUID identOld = argVar.Object(i);

		if (identOld.IsNull())
		{
			continue;
		}

		NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
		*(pEntryInfo->mutable_object_guid()) = NFINetModule::NFToPB(identOld);

		NFVector3 vPos = m_pKernelModule->GetPropertyVector3(identOld, NFrame::IObject::Position());
		pEntryInfo->set_x(vPos.X());
		pEntryInfo->set_y(vPos.Y());
		pEntryInfo->set_z(vPos.Z());
		pEntryInfo->set_career_type(m_pKernelModule->GetPropertyInt32(identOld, NFrame::Player::Job()));
		pEntryInfo->set_player_state(0);
		pEntryInfo->set_config_id(m_pKernelModule->GetPropertyString(identOld, NFrame::Player::ConfigID()));
		pEntryInfo->set_scene_id(m_pKernelModule->GetPropertyInt32(identOld, NFrame::Player::SceneID()));
		pEntryInfo->set_class_id(m_pKernelModule->GetPropertyString(identOld, NFrame::Player::ClassName()));

	}

	if (xPlayerEntryInfoList.object_list_size() <= 0)
	{
		return 0;
	}

	for (int i = 0; i < self.GetCount(); i++)
	{
		NFGUID ident = self.Object(i);
		if (ident.IsNull())
		{
			continue;
		}


		SendMsgPBToGate(NFMsg::EGMI_ACK_DATA_FINISHED, xPlayerEntryInfoList, ident);
	}

	return 1;
}

int NFCAIServerNet_ServerModule::OnObjectListLeave(const NFDataList& self, const NFDataList& argVar)
{
	if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
	{
		return 0;
	}

	NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
	for (int i = 0; i < argVar.GetCount(); i++)
	{
		NFGUID identOld = argVar.Object(i);
		
		if (identOld.IsNull())
		{
			continue;
		}

		NFMsg::Ident* pIdent = xPlayerLeaveInfoList.add_object_list();
		*pIdent = NFINetModule::NFToPB(argVar.Object(i));
	}

	for (int i = 0; i < self.GetCount(); i++)
	{
		NFGUID ident = self.Object(i);
		if (ident.IsNull())
		{
			continue;
		}
		
		SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, ident);
	}

	return 1;
}

int NFCAIServerNet_ServerModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{
		
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		//m_pKernelModule->AddPropertyCallBack(self, NFrame::Scene::SceneID, this, &NFCAIServerNet_ServerModule::OnSceneEvent);
	}

	return 0;
}

int NFCAIServerNet_ServerModule::OnSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList)
{
	NFVector3 vRelivePos = m_pSceneAOIModule->GetRelivePosition(nSceneID, 0);

	NFMsg::ReqAckSwapScene xAckSwapScene;
	xAckSwapScene.set_scene_id(nSceneID);
	xAckSwapScene.set_transfer_type(NFMsg::ReqAckSwapScene::EGameSwapType::ReqAckSwapScene_EGameSwapType_EGST_NARMAL);
	xAckSwapScene.set_line_id(0);
	xAckSwapScene.set_x(vRelivePos.X());
	xAckSwapScene.set_y(vRelivePos.Y());
	xAckSwapScene.set_z(vRelivePos.Z());
	
	SendMsgPBToGate(NFMsg::EGMI_ACK_SWAP_SCENE, xAckSwapScene, self);

	return 0;
}

void NFCAIServerNet_ServerModule::OnReqiureRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFCAIServerNet_ServerModule::OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqCreateRole xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
	pData->mutable_id()->CopyFrom(NFINetModule::NFToPB(m_pKernelModule->CreateGUID()));
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

	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFCAIServerNet_ServerModule::OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqDeleteRole xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nPlayerID);
}

void NFCAIServerNet_ServerModule::OnClienEnterGameFinishProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckEnterGameSuccess);
	m_pKernelModule->DoEvent(nPlayerID, NFrame::Player::ThisName(), CLASS_OBJECT_EVENT::COE_CREATE_CLIENT_FINISH, NFDataList());
	
	NFMsg::ReqAckEnterGameSuccess xReqAckEnterGameSuccess;
	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ENTER_GAME_FINISH, xReqAckEnterGameSuccess, nSockIndex, nPlayerID);
}

void NFCAIServerNet_ServerModule::OnClienSwapSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckSwapScene)

	//m_pSceneProcessModule->RequestEnterScene(pObject->Self(), xMsg.scene_id(), 0, NFDataList());
}

void NFCAIServerNet_ServerModule::OnClienReqMoveProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckPlayerMove)

	const NFGUID  &self = NFINetModule::PBToNF(xMsg.mover());

	const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());

	this->SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE, xMsg, nSceneID, nGroupID);
}

void NFCAIServerNet_ServerModule::OnClienReqMoveImmuneProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckPlayerMove)

	const NFGUID  &self = NFINetModule::PBToNF(xMsg.mover());

	const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());

	this->SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE_IMMUNE, xMsg, nSceneID, nGroupID);
}

void NFCAIServerNet_ServerModule::OnClientPropertyIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyInt)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyInt &xPropertyInt = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyInt.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload())
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client int set", xPropertyInt.property_name(), __FUNCTION__, __LINE__);
				pProperty->SetInt(xPropertyInt.data());
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client int set Upload error", xPropertyInt.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client int set Property error", xPropertyInt.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFCAIServerNet_ServerModule::OnClientPropertyFloatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyFloat)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyFloat &xPropertyFloat = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyFloat.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload())
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client float set", xPropertyFloat.property_name(), __FUNCTION__, __LINE__);
				pProperty->SetFloat(xPropertyFloat.data());
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client float set Upload error", xPropertyFloat.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client float set Property error", xPropertyFloat.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFCAIServerNet_ServerModule::OnClientPropertyStringProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyString)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyString &xPropertyString = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyString.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload())
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client string set", xPropertyString.property_name(), __FUNCTION__, __LINE__);
				pProperty->SetString(xPropertyString.data());
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client string set Upload error", xPropertyString.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client string set Property error", xPropertyString.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFCAIServerNet_ServerModule::OnClientPropertyObjectProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyObject)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyObject &xPropertyObject = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyObject.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload())
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client object set", xPropertyObject.property_name(), __FUNCTION__, __LINE__);
				pProperty->SetObject(NFINetModule::PBToNF(xPropertyObject.data()));
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Upload error", xPropertyObject.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Property error", xPropertyObject.property_name(), __FUNCTION__, __LINE__);
		}
	}
}


void NFCAIServerNet_ServerModule::OnClientPropertyVector2Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyVector2)

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			NFMsg::PropertyVector2 xProperty = xMsg.property_list().Get(i);
			NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xProperty.property_name());
			if (pProperty)
			{
				//judge upload then set value
				if (pProperty->GetUpload())
				{
					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client object set", xProperty.property_name(), __FUNCTION__, __LINE__);
					pProperty->SetVector2(NFINetModule::PBToNF(xProperty.data()));
				}
				else
				{
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Upload error", xProperty.property_name(), __FUNCTION__, __LINE__);
				}
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Property error", xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
}

void NFCAIServerNet_ServerModule::OnClientPropertyVector3Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyVector3)

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			NFMsg::PropertyVector3 xProperty = xMsg.property_list().Get(i);
			NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xProperty.property_name());
			if (pProperty)
			{
				//judge upload then set value
				if (pProperty->GetUpload())
				{
					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client object set", xProperty.property_name(), __FUNCTION__, __LINE__);
					pProperty->SetVector3(NFINetModule::PBToNF(xProperty.data()));
				}
				else
				{
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Upload error", xProperty.property_name(), __FUNCTION__, __LINE__);
				}
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Property error", xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
}


void NFCAIServerNet_ServerModule::OnClientAddRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordAddRow)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client add row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client add row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.row_data_size(); i++)
	{
		const NFMsg::RecordAddRowStruct &xAddRowStruct = xMsg.row_data().Get(i);
		int row = xAddRowStruct.row();

		std::map<int, NFData> colDataMap;
		for (int j = 0; j < xAddRowStruct.record_int_list_size(); j++)
		{
			const NFMsg::RecordInt &xRecordInt = xAddRowStruct.record_int_list().Get(j);
			NFData data;
			data.SetInt(xRecordInt.data());
			colDataMap.insert(std::map<int, NFData>::value_type(xRecordInt.col(), data));
		}
		for (int j = 0; j < xAddRowStruct.record_float_list_size(); j++)
		{
			const NFMsg::RecordFloat &xRecordFloat = xAddRowStruct.record_float_list().Get(j);
			NFData data;
			data.SetFloat(xRecordFloat.data());
			colDataMap.insert(std::map<int, NFData>::value_type(xRecordFloat.col(), data));
		}
		for (int j = 0; j < xAddRowStruct.record_string_list_size(); j++)
		{
			const NFMsg::RecordString &xRecordString = xAddRowStruct.record_string_list().Get(j);
			NFData data;
			data.SetString(xRecordString.data());
			colDataMap.insert(std::map<int, NFData>::value_type(xRecordString.col(), data));
		}
		for (int j = 0; j < xAddRowStruct.record_object_list_size(); j++)
		{
			const NFMsg::RecordObject &xRecordObject = xAddRowStruct.record_object_list().Get(j);
			NFData data;
			data.SetObject(NFINetModule::PBToNF(xRecordObject.data()));
			colDataMap.insert(std::map<int, NFData>::value_type(xRecordObject.col(), data));
		}

		NFDataList xDataList;
		for (int j = 0; j < colDataMap.size(); j++)
		{
			if (colDataMap.find(j) != colDataMap.end())
			{
				xDataList.Append(colDataMap[j]);
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client add row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
				return;
			}
		}

		if (pRecord->AddRow(row, xDataList) >= 0)
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client add row record", xMsg.record_name(), __FUNCTION__, __LINE__);
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client add row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}

}

void NFCAIServerNet_ServerModule::OnClientRemoveRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordRemove)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client remove row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client remove row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.remove_row_size(); i++)
	{
		if (pRecord->Remove(xMsg.remove_row().Get(i)))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client remove row record", xMsg.record_name(), __FUNCTION__, __LINE__);
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client remove row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFCAIServerNet_ServerModule::OnClientSwapRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordSwap)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.origin_record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client swap row record error", xMsg.origin_record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client swap row record error", xMsg.origin_record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (pRecord->SwapRowInfo(xMsg.row_origin(),xMsg.row_target()))
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client swap row record", xMsg.origin_record_name(), __FUNCTION__, __LINE__);
	}
	else
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client swap row record error", xMsg.origin_record_name(), __FUNCTION__, __LINE__);
	}
}

void NFCAIServerNet_ServerModule::OnClientRecordIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordInt)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client int set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client int set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordInt &xRecordInt = xMsg.property_list().Get(i);
		pRecord->SetInt(xRecordInt.row(), xRecordInt.col(), xRecordInt.data());
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client int set record", xMsg.record_name(), __FUNCTION__, __LINE__);
	}

}


void NFCAIServerNet_ServerModule::OnClientRecordFloatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordFloat)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client float set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client float set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordFloat &xRecordFloat = xMsg.property_list().Get(i);
		pRecord->SetFloat(xRecordFloat.row(), xRecordFloat.col(), xRecordFloat.data());
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client float set record", xMsg.record_name(), __FUNCTION__, __LINE__);
	}
	
}

void NFCAIServerNet_ServerModule::OnClientRecordStringProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordString)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client String set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client String set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordString &xRecordString = xMsg.property_list().Get(i);
		pRecord->SetString(xRecordString.row(), xRecordString.col(), xRecordString.data());
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client String set record", xMsg.record_name(), __FUNCTION__, __LINE__);
	}
	
}

void NFCAIServerNet_ServerModule::OnClientRecordObjectProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordObject)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client Object set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client Object set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordObject &xRecordObject = xMsg.property_list().Get(i);
		pRecord->SetObject(xRecordObject.row(), xRecordObject.col(), NFINetModule::PBToNF(xRecordObject.data()));
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client Object set record", xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFCAIServerNet_ServerModule::OnClientRecordVector2Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordVector2)

		NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client vector2 set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client vector2 set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordVector2 &xRecordVector2 = xMsg.property_list().Get(i);
		pRecord->SetVector2(xRecordVector2.row(), xRecordVector2.col(), NFINetModule::PBToNF(xRecordVector2.data()));
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client vector2 set record", xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFCAIServerNet_ServerModule::OnClientRecordVector3Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordVector3)

		NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client vector3 set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client vector3 set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordVector3 &xRecordVector3 = xMsg.property_list().Get(i);
		pRecord->SetVector3(xRecordVector3.row(), xRecordVector3.col(), NFINetModule::PBToNF(xRecordVector3.data()));
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client vector3 set record", xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFCAIServerNet_ServerModule::OnProxyServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<GateServerInfo>(NF_NEW GateServerInfo());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->xServerData.nFD = nSockIndex;
		*(pServerData->xServerData.pData) = xData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
	}

	return;
}

void NFCAIServerNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());


		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
	}

	return;
}

void NFCAIServerNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<GateServerInfo>(NF_NEW GateServerInfo());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->xServerData.nFD = nSockIndex;
		*(pServerData->xServerData.pData) = xData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
	}

	return;
}

void NFCAIServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self)
{
	NF_SHARE_PTR<GateBaseInfo> pData = mRoleBaseData.GetElement(self);
	if (pData)
	{
		NF_SHARE_PTR<GateServerInfo> pProxyData = mProxyMap.GetElement(pData->nGateID);
		if (pProxyData)
		{
			m_pNetModule->SendMsgPB(nMsgID, xMsg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

void NFCAIServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self)
{
	NF_SHARE_PTR<GateBaseInfo> pData = mRoleBaseData.GetElement(self);
	if (pData)
	{
		NF_SHARE_PTR<GateServerInfo> pProxyData = mProxyMap.GetElement(pData->nGateID);
		if (pProxyData)
		{
			m_pNetModule->SendMsgPB(nMsgID, strMsg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

void NFCAIServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message & xMsg, const int nSceneID, const int nGroupID)
{
	//care: batch
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			this->SendMsgPBToGate(nMsgID, xMsg, xObject);
		}
	}
}

void NFCAIServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, const std::string & strMsg, const int nSceneID, const int nGroupID)
{
	//care: batch
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			this->SendMsgPBToGate(nMsgID, strMsg, xObject);
		}
	}
}

bool NFCAIServerNet_ServerModule::AddPlayerGateInfo(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID)
{
    if (nGateID <= 0)
    {
        
        return false;
    }

    if (nClientID.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<NFCAIServerNet_ServerModule::GateBaseInfo> pBaseData = mRoleBaseData.GetElement(nRoleID);
    if (nullptr != pBaseData)
    {
        
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nClientID, "player is exist, cannot enter game", "", __FUNCTION__, __LINE__);
        return false;
    }

    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(nGateID);
    if (nullptr == pServerData)
    {
        return false;
    }

    if (!pServerData->xRoleInfo.insert(std::make_pair(nRoleID, pServerData->xServerData.nFD)).second)
    {
        return false;
    }

    if (!mRoleBaseData.AddElement(nRoleID, NF_SHARE_PTR<GateBaseInfo>(NF_NEW GateBaseInfo(nGateID, nClientID))))
    {
        pServerData->xRoleInfo.erase(nRoleID) ;
        return false;
    }

    return true;
}

bool NFCAIServerNet_ServerModule::RemovePlayerGateInfo(const NFGUID& nRoleID)
{
    NF_SHARE_PTR<GateBaseInfo> pBaseData = mRoleBaseData.GetElement(nRoleID);
    if (nullptr == pBaseData)
    {
        return false;
    }

    mRoleBaseData.RemoveElement(nRoleID);

    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(pBaseData->nGateID);
    if (nullptr == pServerData)
    {
        return false;
    }

    pServerData->xRoleInfo.erase(nRoleID);
    return true;
}

NF_SHARE_PTR<NFIAIServerNet_ServerModule::GateBaseInfo> NFCAIServerNet_ServerModule::GetPlayerGateInfo(const NFGUID& nRoleID)
{
    return mRoleBaseData.GetElement(nRoleID);
}

NF_SHARE_PTR<NFIAIServerNet_ServerModule::GateServerInfo> NFCAIServerNet_ServerModule::GetGateServerInfo(const int nGateID)
{
    return mProxyMap.GetElement(nGateID);
}

NF_SHARE_PTR<NFIAIServerNet_ServerModule::GateServerInfo> NFCAIServerNet_ServerModule::GetGateServerInfoBySockIndex(const NFSOCK nSockIndex)
{
    int nGateID = -1;
    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.First();
    while (pServerData)
    {
        if (nSockIndex == pServerData->xServerData.nFD)
        {
            nGateID = pServerData->xServerData.pData->server_id();
            break;
        }

        pServerData = mProxyMap.Next();
    }

    if (nGateID == -1)
    {
        return nullptr;
    }

    return pServerData;
}

void NFCAIServerNet_ServerModule::OnTransWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	std::string strMsg;
	NFGUID nPlayer;
	int64_t nHasKey = 0;
	if (NFINetModule::ReceivePB( nMsgID, msg, nLen, strMsg, nPlayer))
	{
		nHasKey = nPlayer.nData64;
	}

	m_pNetClientModule->SendBySuit(NF_SERVER_TYPES::NF_ST_WORLD, nHasKey, nMsgID, msg, nLen);
}

void NFCAIServerNet_ServerModule::OnTransWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen, const int nWorldKey)
{
	m_pNetClientModule->SendBySuit(NF_SERVER_TYPES::NF_ST_WORLD, nWorldKey, nMsgID, msg, nLen);
}
