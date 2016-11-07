// -------------------------------------------------------------------------
//    @FileName			:    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCGameServerNet_ServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCGameServerNet_ServerModule::Init()
{
	m_pNetModule = NF_NEW NFINetModule(pPluginManager);
	return true;
}

bool NFCGameServerNet_ServerModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>();

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REFRESH, this, &NFCGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REGISTERED, this, &NFCGameServerNet_ServerModule::OnProxyServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_UNREGISTERED, this, &NFCGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME, this, &NFCGameServerNet_ServerModule::OnClienEnterGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_GAME, this, &NFCGameServerNet_ServerModule::OnClienLeaveGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST, this, &NFCGameServerNet_ServerModule::OnReqiureRoleListProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE, this, &NFCGameServerNet_ServerModule::OnCreateRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE, this, &NFCGameServerNet_ServerModule::OnDeleteRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_RECOVER_ROLE, this, &NFCGameServerNet_ServerModule::OnClienSwapSceneProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SWAP_SCENE, this, &NFCGameServerNet_ServerModule::OnClienSwapSceneProcess);
	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_INT, this, &NFCGameServerNet_ServerModule::OnClientPropertyIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_FLOAT, this, &NFCGameServerNet_ServerModule::OnClientPropertyFloatProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_STRING, this, &NFCGameServerNet_ServerModule::OnClientPropertyStringProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_OBJECT, this, &NFCGameServerNet_ServerModule::OnClientPropertyObjectProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_VECTOR2, this, &NFCGameServerNet_ServerModule::OnClientPropertyVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_VECTOR3, this, &NFCGameServerNet_ServerModule::OnClientPropertyVector3Process);

	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_ADD_ROW, this, &NFCGameServerNet_ServerModule::OnClientAddRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_REMOVE_ROW, this, &NFCGameServerNet_ServerModule::OnClientRemoveRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_SWAP_ROW, this, &NFCGameServerNet_ServerModule::OnClientSwapRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_INT, this, &NFCGameServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_FLOAT, this, &NFCGameServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_STRING, this, &NFCGameServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_OBJECT, this, &NFCGameServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_VECTOR2, this, &NFCGameServerNet_ServerModule::OnClientRecordVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_VECTOR3, this, &NFCGameServerNet_ServerModule::OnClientRecordVector3Process);

	//EGMI_ACK_RECORD_CLEAR = 228,
	//EGMI_ACK_RECORD_SORT = 229,

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_GUILD, this, &NFCGameServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_CREATE_CHATGROUP, this, &NFCGameServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_JOIN_CHATGROUP, this, &NFCGameServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_LEAVE_CHATGROUP, this, &NFCGameServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SUBSCRIPTION_CHATGROUP, this, &NFCGameServerNet_ServerModule::OnTransWorld);

	m_pNetModule->AddEventCallBack(this, &NFCGameServerNet_ServerModule::OnSocketPSEvent);

	m_pKernelModule->RegisterCommonClassEvent(this, &NFCGameServerNet_ServerModule::OnClassCommonEvent);
	m_pKernelModule->RegisterCommonPropertyEvent(this, &NFCGameServerNet_ServerModule::OnPropertyCommonEvent);
	m_pKernelModule->RegisterCommonRecordEvent(this, &NFCGameServerNet_ServerModule::OnRecordCommonEvent);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCGameServerNet_ServerModule::OnObjectClassEvent);

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		NFList<std::string>& strIdList = xLogicClass->GetIdList();
		std::string strId;
		for (bool bRet = strIdList.First(strId); bRet; bRet = strIdList.Next(strId))
		{
			const int nServerType = m_pElementModule->GetPropertyInt(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt(strId, NFrame::Server::ServerID());
			if (nServerType == NF_SERVER_TYPES::NF_ST_GAME && pPluginManager->GetAppID() == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt(strId, NFrame::Server::Port());
				const int nMaxConnect = m_pElementModule->GetPropertyInt(strId, NFrame::Server::MaxOnline());
				const int nCpus = m_pElementModule->GetPropertyInt(strId, NFrame::Server::CpuCount());
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

bool NFCGameServerNet_ServerModule::Shut()
{

	return true;
}

bool NFCGameServerNet_ServerModule::Execute()
{
	return m_pNetModule->Execute();
}

void NFCGameServerNet_ServerModule::OnSocketPSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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

void NFCGameServerNet_ServerModule::OnClientDisconnect(const int nAddress)
{
	//ֻ���������ض���
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

void NFCGameServerNet_ServerModule::OnClientConnected(const int nAddress)
{

}

void NFCGameServerNet_ServerModule::OnClienEnterGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//�ڽ�����Ϸ֮ǰnPlayerIDΪ�������ص�FD
	NFGUID nClientID;
	NFMsg::ReqEnterGameServer xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());

    if (m_pKernelModule->GetObject(nRoleID))
    {
        m_pKernelModule->DestroyObject(nRoleID);
    }
    
	//////////////////////////////////////////////////////////////////////////

    NF_SHARE_PTR<NFCGameServerNet_ServerModule::GateBaseInfo>  pGateInfo = GetPlayerGateInfo(nRoleID);
    if (nullptr != pGateInfo)
    {
        RemovePlayerGateInfo(nRoleID);
    }

    NF_SHARE_PTR<NFCGameServerNet_ServerModule::GateServerInfo> pGateServereinfo = GetGateServerInfoBySockIndex(nSockIndex);
    if (nullptr == pGateServereinfo)
    {
        return;
    }

    int nGateID = -1;
    if (pGateServereinfo->xServerData.pData)
    {
        nGateID = pGateServereinfo->xServerData.pData->server_id();
    }

    if (nGateID < 0)
    {
        return;
    }
 
    if (!AddPlayerGateInfo(nRoleID, nClientID, nGateID))
    {
        return;
    }

	//Ĭ��1�ų���
	int nSceneID = 1;
	NFCDataList var;
	var.AddString("Name");
	var.AddString(xMsg.name());

	var.AddString("GateID");
	var.AddInt(nGateID);

	var.AddString("ClientID");
	var.AddObject(nClientID);

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, nSceneID, 0, NFrame::Player::ThisName(), "", var);
	if (NULL == pObject)
	{
		//�ڴ�й©
		//mRoleBaseData
		//mRoleFDData
		return;
	}

	pObject->SetPropertyInt("LoadPropertyFinish", 1);
	pObject->SetPropertyInt("GateID", nGateID);
	pObject->SetPropertyInt("GameID", pPluginManager->GetAppID());

	m_pKernelModule->DoEvent(pObject->Self(), NFrame::Player::ThisName(), CLASS_OBJECT_EVENT::COE_CREATE_FINISH, NFCDataList());

	NFCDataList varEntry;
	varEntry << pObject->Self();
	varEntry << NFINT64(0);
	varEntry << nSceneID;
	varEntry << -1;
	m_pEventModule->DoEvent(pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry);
}

void NFCGameServerNet_ServerModule::OnClienLeaveGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqLeaveGameServer xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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

int NFCGameServerNet_ServerModule::OnPropertyEnter(const NFIDataList& argVar, const NFGUID& self)
{
	if (argVar.GetCount() <= 0 || self.IsNull())
	{
		return 0;
	}

	NFMsg::MultiObjectPropertyList xPublicMsg;
	NFMsg::MultiObjectPropertyList xPrivateMsg;

	//��Ϊ�Լ�������
	//1.public���͸�������
	//2.�����Լ����б��У��ٴη���private����
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

	for (int i = 0; i < pRecord->GetRows(); i++)
	{
		if (pRecord->IsUsed(i))
		{
			//����public����private��Ҫ���ϣ���Ȼpublic�㲥���ǲ���private�͹㲥������
			NFMsg::RecordAddRowStruct* pAddRowStruct = pObjectRecordBase->add_row_struct();
			pAddRowStruct->set_row(i);

			for (int j = 0; j < pRecord->GetCols(); j++)
			{
				//������0�Ͳ������ˣ���Ϊ�ͻ���Ĭ����0
				NFCDataList valueList;
				TDATA_TYPE eType = pRecord->GetColType(j);
				switch (eType)
				{
				case TDATA_TYPE::TDATA_INT:
				{
					int nValue = pRecord->GetInt(i, j);
					//if ( 0 != nValue )
					{
						NFMsg::RecordInt* pAddData = pAddRowStruct->add_record_int_list();
						pAddData->set_row(i);
						pAddData->set_col(j);
						pAddData->set_data(nValue);
					}
				}
				break;
				case TDATA_TYPE::TDATA_FLOAT:
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
				case TDATA_TYPE::TDATA_STRING:
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
				case TDATA_TYPE::TDATA_OBJECT:
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

int NFCGameServerNet_ServerModule::OnRecordEnter(const NFIDataList& argVar, const NFGUID& self)
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

int NFCGameServerNet_ServerModule::OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar)
{
	if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
	{
		return 0;
	}

	NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
	for (int i = 0; i < argVar.GetCount(); i++)
	{
		NFGUID identOld = argVar.Object(i);
		//�ų��ն���
		if (identOld.IsNull())
		{
			continue;
		}

		NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
		*(pEntryInfo->mutable_object_guid()) = NFINetModule::NFToPB(identOld);

		pEntryInfo->set_x(m_pKernelModule->GetPropertyFloat(identOld, "X"));
		pEntryInfo->set_y(m_pKernelModule->GetPropertyFloat(identOld, "Y"));
		pEntryInfo->set_z(m_pKernelModule->GetPropertyFloat(identOld, "Z"));
		pEntryInfo->set_career_type(m_pKernelModule->GetPropertyInt(identOld, "Job"));
		pEntryInfo->set_player_state(m_pKernelModule->GetPropertyInt(identOld, "State"));
		pEntryInfo->set_config_id(m_pKernelModule->GetPropertyString(identOld, "ConfigID"));
		pEntryInfo->set_scene_id(m_pKernelModule->GetPropertyInt(identOld, "SceneID"));
		pEntryInfo->set_class_id(m_pKernelModule->GetPropertyString(identOld, "ClassName"));

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

		//�����ڲ�ͬ��������,�õ��������ڵ�����FD
		SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerEntryInfoList, ident);
	}

	return 1;
}

int NFCGameServerNet_ServerModule::OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar)
{
	if (self.GetCount() <= 0 || argVar.GetCount() <= 0)
	{
		return 0;
	}

	NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
	for (int i = 0; i < argVar.GetCount(); i++)
	{
		NFGUID identOld = argVar.Object(i);
		//�ų��ն���
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
		//�����ڲ�ͬ��������,�õ��������ڵ�����FD
		SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, ident);
	}

	return 1;
}

int NFCGameServerNet_ServerModule::OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	//if ( NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
	{
		if ("GroupID" == strPropertyName)
		{
			//�Լ�����Ҫ֪���Լ����������Ա仯��,���Ǳ��˾Ͳ���Ҫ֪����
			OnGroupEvent(self, strPropertyName, oldVar, newVar);
		}

		if ("SceneID" == strPropertyName)
		{
			//�Լ�����Ҫ֪���Լ����������Ա仯��,���Ǳ��˾Ͳ���Ҫ֪����
			OnContainerEvent(self, strPropertyName, oldVar, newVar);
		}

		if (NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString(self, "ClassName"))
		{
			if (m_pKernelModule->GetPropertyInt(self, "LoadPropertyFinish") <= 0)
			{
				return 0;
			}
		}
	}

	NFCDataList valueBroadCaseList;
	int nCount = 0;//argVar.GetCount() ;
	if (nCount <= 0)
	{
		nCount = GetBroadCastObject(self, strPropertyName, false, valueBroadCaseList);
	}
	else
	{
		//�����Ĳ�����Ҫ�㲥�Ķ����б�
		//valueBroadCaseList = argVar;
	}

	if (valueBroadCaseList.GetCount() <= 0)
	{
		return 0;
	}

	switch (oldVar.GetType())
	{
	case TDATA_INT:
	{
		NFMsg::ObjectPropertyInt xPropertyInt;
		NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
		pDataInt->set_property_name(strPropertyName);
		pDataInt->set_data(newVar.GetInt());

		for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
		{
			NFGUID identOld = valueBroadCaseList.Object(i);

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
		pDataFloat->set_property_name(strPropertyName);
		pDataFloat->set_data(newVar.GetFloat());

		for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
		{
			NFGUID identOld = valueBroadCaseList.Object(i);

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
		pDataString->set_property_name(strPropertyName);
		pDataString->set_data(newVar.GetString());

		for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
		{
			NFGUID identOld = valueBroadCaseList.Object(i);

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
		pDataObject->set_property_name(strPropertyName);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

		for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
		{
			NFGUID identOld = valueBroadCaseList.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_OBJECT, xPropertyObject, identOld);
		}
	}
	break;

	default:
		break;
	}


	return 0;
}

int NFCGameServerNet_ServerModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	const std::string& strRecordName = xEventData.strRecordName;
	const int nOpType = xEventData.nOpType;
	const int nRow = xEventData.nRow;
	const int nCol = xEventData.nCol;

	int nObjectContainerID = m_pKernelModule->GetPropertyInt(self, "SceneID");
	int nObjectGroupID = m_pKernelModule->GetPropertyInt(self, "GroupID");

	if (nObjectGroupID < 0)
	{
		//����
		return 0;
	}

	if (NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString(self, "ClassName"))
	{
		if (m_pKernelModule->GetPropertyInt(self, "LoadPropertyFinish") <= 0)
		{
			return 0;
		}
	}

	NFCDataList valueBroadCaseList;
	GetBroadCastObject(self, strRecordName, true, valueBroadCaseList);

	switch (nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		NFMsg::ObjectRecordAddRow xAddRecordRow;
		NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		xAddRecordRow.set_record_name(strRecordName);

		NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
		pAddRowData->set_row(nRow);

		//add row ��Ҫ������row
		NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, strRecordName);
		if (xRecord)
		{
			NFCDataList xRowDataList;
			if (xRecord->QueryRow(nRow, xRowDataList))
			{
				for (int i = 0; i < xRowDataList.GetCount(); i++)
				{
					switch (xRowDataList.Type(i))
					{
					case TDATA_INT:
					{
						//���ӵ�ʱ������Ҫȫs
						int nValue = xRowDataList.Int(i);
						//if ( 0 != nValue )
						{
							NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
							pAddData->set_col(i);
							pAddData->set_row(nRow);
							pAddData->set_data(nValue);
						}
					}
					break;
					case TDATA_FLOAT:
					{
						float fValue = xRowDataList.Float(i);
						//if ( fValue > 0.001f  || fValue < -0.001f )
						{
							NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
							pAddData->set_col(i);
							pAddData->set_row(nRow);
							pAddData->set_data(fValue);
						}
					}
					break;
					case TDATA_STRING:
					{
						const std::string& str = xRowDataList.String(i);
						//if (!str.empty())
						{
							NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
							pAddData->set_col(i);
							pAddData->set_row(nRow);
							pAddData->set_data(str);
						}
					}
					break;
					case TDATA_OBJECT:
					{
						NFGUID identValue = xRowDataList.Object(i);
						//if (!identValue.IsNull())
						{
							NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
							pAddData->set_col(i);
							pAddData->set_row(nRow);

							*pAddData->mutable_data() = NFINetModule::NFToPB(identValue);
						}
					}
					break;
					default:
						break;
					}
				}

				for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
				{
					NFGUID identOther = valueBroadCaseList.Object(i);

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

		xReoveRecordRow.set_record_name(strRecordName);
		xReoveRecordRow.add_remove_row(nRow);

		for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
		{
			NFGUID identOther = valueBroadCaseList.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_REMOVE_ROW, xReoveRecordRow, identOther);
		}
	}
	break;
	case RECORD_EVENT_DATA::Swap:
	{
		//��ʵ��2��row����
		NFMsg::ObjectRecordSwap xSwapRecord;
		*xSwapRecord.mutable_player_id() = NFINetModule::NFToPB(self);

		xSwapRecord.set_origin_record_name(strRecordName);
		xSwapRecord.set_target_record_name(strRecordName);   // ��ʱû��
		xSwapRecord.set_row_origin(nRow);
		xSwapRecord.set_row_target(nCol);

		for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
		{
			NFGUID identOther = valueBroadCaseList.Object(i);

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

			xRecordChanged.set_record_name(strRecordName);
			NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(nRow);
			recordProperty->set_col(nCol);
			int nData = newVar.GetInt();
			recordProperty->set_data(nData);

			for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
			{
				NFGUID identOther = valueBroadCaseList.Object(i);

				SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_INT, xRecordChanged, identOther);
			}
		}
		break;

		case TDATA_FLOAT:
		{
			NFMsg::ObjectRecordFloat xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecordName);
			NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(nRow);
			recordProperty->set_col(nCol);
			recordProperty->set_data(newVar.GetFloat());

			for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
			{
				NFGUID identOther = valueBroadCaseList.Object(i);

				SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_FLOAT, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_STRING:
		{
			NFMsg::ObjectRecordString xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecordName);
			NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(nRow);
			recordProperty->set_col(nCol);
			recordProperty->set_data(newVar.GetString());

			for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
			{
				NFGUID identOther = valueBroadCaseList.Object(i);

				SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_STRING, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_OBJECT:
		{
			NFMsg::ObjectRecordObject xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(strRecordName);
			NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(nRow);
			recordProperty->set_col(nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

			for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
			{
				NFGUID identOther = valueBroadCaseList.Object(i);

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

int NFCGameServerNet_ServerModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	////////////1:�㲥���Ѿ����ڵ���//////////////////////////////////////////////////////////////
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		//ɾ�����߱�־

		//////////////////////////////////////////////////////////////////////////

		int nObjectContainerID = m_pKernelModule->GetPropertyInt(self, "SceneID");
		int nObjectGroupID = m_pKernelModule->GetPropertyInt(self, "GroupID");

		if (nObjectGroupID < 0)
		{
			//����
			return 0;
		}

		NFCDataList valueAllObjectList;
		NFCDataList valueBroadCaseList;
		NFCDataList valueBroadListNoSelf;
		m_pKernelModule->GetGroupObjectList(nObjectContainerID, nObjectGroupID, valueAllObjectList);

		for (int i = 0; i < valueAllObjectList.GetCount(); i++)
		{
			NFGUID identBC = valueAllObjectList.Object(i);
			const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, "ClassName");
			if (NFrame::Player::ThisName() == strClassName)
			{
				valueBroadCaseList.Add(identBC);
				if (identBC != self)
				{
					valueBroadListNoSelf.Add(identBC);
				}
			}
		}

		//�����Ǹ����Ĺ֣�������Ҫ���ͣ���Ϊ�����뿪������ʱ��һ����һ����Ϣ����
		OnObjectListLeave(valueBroadListNoSelf, NFCDataList() << self);
	}

	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{
		//id��fd,gateid����
		NF_SHARE_PTR<GateBaseInfo> pDataBase = mRoleBaseData.GetElement(self);
		if (pDataBase)
		{
			//�ظ��ͻ��˽�ɫ������Ϸ�����ɹ���
			NFMsg::AckEventResult xMsg;
			xMsg.set_event_code(NFMsg::EGEC_ENTER_GAME_SUCCESS);

			*xMsg.mutable_event_client() = NFINetModule::NFToPB(pDataBase->xClientID);
			*xMsg.mutable_event_object() = NFINetModule::NFToPB(self);

			SendMsgPBToGate(NFMsg::EGMI_ACK_ENTER_GAME, xMsg, self);
		}
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		//�Լ��㲥���Լ��͹���
		if (strClassName == NFrame::Player::ThisName())
		{
			OnObjectListEnter(NFCDataList() << self, NFCDataList() << self);

			OnPropertyEnter(NFCDataList() << self, self);
			OnRecordEnter(NFCDataList() << self, self);
		}
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{

	}
	return 0;
}

int NFCGameServerNet_ServerModule::OnGroupEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	//���������仯��ֻ���ܴ�A������0���л���B������0��
	//��Ҫע������------------�κβ��ı���ʱ�򣬴�������ʵ��δ�����㣬���ˣ����ı���ʱ����ȡ�������б���Ŀ�����ǲ������Լ���
	int nSceneID = m_pKernelModule->GetPropertyInt(self, "SceneID");

	//�㲥�������Լ���ȥ(�㽵����Ծ��)
	int nOldGroupID = oldVar.GetInt();
	if (nOldGroupID > 0)
	{
		NFCDataList valueAllOldObjectList;
		NFCDataList valueAllOldPlayerList;
		m_pKernelModule->GetGroupObjectList(nSceneID, nOldGroupID, valueAllOldObjectList);
		if (valueAllOldObjectList.GetCount() > 0)
		{
			//�Լ�ֻ��Ҫ�㲥��������
			for (int i = 0; i < valueAllOldObjectList.GetCount(); i++)
			{
				NFGUID identBC = valueAllOldObjectList.Object(i);

				if (valueAllOldObjectList.Object(i) == self)
				{
					valueAllOldObjectList.Set(i, NFGUID());
				}

				const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, "ClassName");
				if (NFrame::Player::ThisName() == strClassName)
				{
					valueAllOldPlayerList.Add(identBC);
				}
			}

			OnObjectListLeave(valueAllOldPlayerList, NFCDataList() << self);

			//�ϵ�ȫ��Ҫ�㲥ɾ��
			OnObjectListLeave(NFCDataList() << self, valueAllOldObjectList);
		}

		m_pEventModule->DoEvent(self, NFED_ON_CLIENT_LEAVE_SCENE, NFCDataList() << nOldGroupID);
	}

	//�ٹ㲥�������Լ�����(��������Ծ��)
	int nNewGroupID = newVar.GetInt();
	if (nNewGroupID > 0)
	{
		//������Ҫ���Լ��ӹ㲥���ų�
		//////////////////////////////////////////////////////////////////////////
		NFCDataList valueAllObjectList;
		NFCDataList valueAllObjectListNoSelf;
		NFCDataList valuePlayerList;
		NFCDataList valuePlayerListNoSelf;
		m_pKernelModule->GetGroupObjectList(nSceneID, nNewGroupID, valueAllObjectList);
		for (int i = 0; i < valueAllObjectList.GetCount(); i++)
		{
			NFGUID identBC = valueAllObjectList.Object(i);
			const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, "ClassName");
			if (NFrame::Player::ThisName() == strClassName)
			{
				valuePlayerList.Add(identBC);
				if (identBC != self)
				{
					valuePlayerListNoSelf.Add(identBC);
				}
			}

			if (identBC != self)
			{
				valueAllObjectListNoSelf.Add(identBC);
			}
		}

		//�㲥������,�Լ�����(���ﱾ��Ӧ�ù㲥���Լ�)
		if (valuePlayerListNoSelf.GetCount() > 0)
		{
			OnObjectListEnter(valuePlayerListNoSelf, NFCDataList() << self);
		}

		const std::string& strSelfClassName = m_pKernelModule->GetPropertyString(self, "ClassName");

		//�㲥���Լ�,���еı��˳���
		if (valueAllObjectListNoSelf.GetCount() > 0)
		{
			if (strSelfClassName == NFrame::Player::ThisName())
			{
				OnObjectListEnter(NFCDataList() << self, valueAllObjectListNoSelf);
			}
		}

		if (strSelfClassName == NFrame::Player::ThisName())
		{
			for (int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++)
			{
				//��ʱ�����ٹ㲥�Լ������Ը��Լ�
				//���Ѿ����ڵ��˵����Թ㲥����������
				NFGUID identOld = valueAllObjectListNoSelf.Object(i);

				OnPropertyEnter(NFCDataList() << self, identOld);
				//���Ѿ����ڵ��˵ı��㲥����������
				OnRecordEnter(NFCDataList() << self, identOld);
			}
		}

		//���������˵����Թ㲥���ܱߵ���
		if (valuePlayerListNoSelf.GetCount() > 0)
		{
			OnPropertyEnter(valuePlayerListNoSelf, self);
			OnRecordEnter(valuePlayerListNoSelf, self);
		}
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnContainerEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	//���������仯��ֻ���ܴ�A������0���л���B������0��
	//��Ҫע������------------�κ������ı���ʱ�������ұ�����0��
	int nOldSceneID = oldVar.GetInt();
	int nNowSceneID = newVar.GetInt();

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Enter Scene:", nNowSceneID);

	//�Լ���ʧ,���Ҳ��ù㲥����Ϊ����ʧ֮ǰ�����ص�0�㣬���ѹ㲥������
	NFCDataList valueOldAllObjectList;
	NFCDataList valueNewAllObjectList;
	NFCDataList valueAllObjectListNoSelf;
	NFCDataList valuePlayerList;
	NFCDataList valuePlayerNoSelf;

	m_pKernelModule->GetGroupObjectList(nOldSceneID, 0, valueOldAllObjectList);
	m_pKernelModule->GetGroupObjectList(nNowSceneID, 0, valueNewAllObjectList);

	for (int i = 0; i < valueOldAllObjectList.GetCount(); i++)
	{
		NFGUID identBC = valueOldAllObjectList.Object(i);
		if (identBC == self)
		{
			valueOldAllObjectList.Set(i, NFGUID());
		}
	}

	for (int i = 0; i < valueNewAllObjectList.GetCount(); i++)
	{
		NFGUID identBC = valueNewAllObjectList.Object(i);
		const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, "ClassName");
		if (NFrame::Player::ThisName() == strClassName)
		{
			valuePlayerList.Add(identBC);
			if (identBC != self)
			{
				valuePlayerNoSelf.Add(identBC);
			}
		}

		if (identBC != self)
		{
			valueAllObjectListNoSelf.Add(identBC);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//���Ǿɳ���0����NPC��Ҫ�㲥
	OnObjectListLeave(NFCDataList() << self, valueOldAllObjectList);

	//�㲥�������˳��ֶ���(���������ң��������㲥���Լ�)
	//�����㲥�Ķ���0����
	if (valuePlayerList.GetCount() > 0)
	{
		//��self�㲥��argVar��Щ��
		OnObjectListEnter(valuePlayerNoSelf, NFCDataList() << self);
	}

	//�²���Ȼ��0����0��NPC�㲥���Լ�------------�Լ��㲥���Լ����������㲥����Ϊ����ID�ڿ糡��ʱ�ᾭ���仯

	//��valueAllObjectList�㲥��self
	OnObjectListEnter(NFCDataList() << self, valueAllObjectListNoSelf);

	////////////////////���Ѿ����ڵ��˵����Թ㲥����������//////////////////////////////////////////////////////
	for (int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++)
	{
		NFGUID identOld = valueAllObjectListNoSelf.Object(i);
		OnPropertyEnter(NFCDataList() << self, identOld);
		////////////////////���Ѿ����ڵ��˵ı��㲥����������//////////////////////////////////////////////////////
		OnRecordEnter(NFCDataList() << self, identOld);
	}

	//���������˵����Թ㲥���ܱߵ���()
	if (valuePlayerNoSelf.GetCount() > 0)
	{
		OnPropertyEnter(valuePlayerNoSelf, self);
		OnRecordEnter(valuePlayerNoSelf, self);
	}

	return 0;
}

int NFCGameServerNet_ServerModule::GetBroadCastObject(const NFGUID& self, const std::string& strPropertyName, const bool bTable, NFIDataList& valueObject)
{
	int nObjectContainerID = m_pKernelModule->GetPropertyInt(self, "SceneID");
	int nObjectGroupID = m_pKernelModule->GetPropertyInt(self, "GroupID");

	//��ͨ�����������жϹ㲥����
	std::string strClassName = m_pKernelModule->GetPropertyString(self, "ClassName");
	NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = m_pClassModule->GetClassRecordManager(strClassName);
	NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = m_pClassModule->GetClassPropertyManager(strClassName);

	NF_SHARE_PTR<NFIRecord> pRecord(NULL);
	NF_SHARE_PTR<NFIProperty> pProperty(NULL);
	if (bTable)
	{
		if (NULL == pClassRecordManager)
		{
			return -1;
		}

		pRecord = pClassRecordManager->GetElement(strPropertyName);
		if (NULL == pRecord)
		{
			return -1;
		}
	}
	else
	{
		if (NULL == pClassPropertyManager)
		{
			return -1;
		}
		pProperty = pClassPropertyManager->GetElement(strPropertyName);
		if (NULL == pProperty)
		{
			return -1;
		}
	}

	if (NFrame::Player::ThisName() == strClassName)
	{
		if (bTable)
		{
			if (pRecord->GetPublic())
			{
				GetBroadCastObject(nObjectContainerID, nObjectGroupID, valueObject);
			}
			else if (pRecord->GetPrivate())
			{
				valueObject.Add(self);
			}
		}
		else
		{
			if (pProperty->GetPublic())
			{
				GetBroadCastObject(nObjectContainerID, nObjectGroupID, valueObject);
			}
			else if (pProperty->GetPrivate())
			{
				valueObject.Add(self);
			}
		}
		//һ�����Ҷ����㲥
		return valueObject.GetCount();
	}

	//��������,NPC�͹�������
	if (bTable)
	{
		if (pRecord->GetPublic())
		{
			//�㲥���ͻ����Լ����ܱ���
			GetBroadCastObject(nObjectContainerID, nObjectGroupID, valueObject);
		}
	}
	else
	{
		if (pProperty->GetPublic())
		{
			//�㲥���ͻ����Լ����ܱ���
			GetBroadCastObject(nObjectContainerID, nObjectGroupID, valueObject);
		}
	}

	return valueObject.GetCount();
}

int NFCGameServerNet_ServerModule::GetBroadCastObject(const int nObjectContainerID, const int nGroupID, NFIDataList& valueObject)
{
	NFCDataList valContainerObjectList;
	m_pKernelModule->GetGroupObjectList(nObjectContainerID, nGroupID, valContainerObjectList);
	for (int i = 0; i < valContainerObjectList.GetCount(); i++)
	{
		const std::string& strObjClassName = m_pKernelModule->GetPropertyString(valContainerObjectList.Object(i), "ClassName");
		if (NFrame::Player::ThisName() == strObjClassName)
		{
			valueObject.Add(valContainerObjectList.Object(i));
		}
	}

	return valueObject.GetCount();
}

int NFCGameServerNet_ServerModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		//SaveDataToNoSql( self, true );
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Player Offline", "");
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
		//LoadDataFormNoSql( self );
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pEventModule->AddEventCallBack(self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, this, &NFCGameServerNet_ServerModule::OnSwapSceneResultEvent);
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnSwapSceneResultEvent(const NFGUID& self, const NFEventDefine nEventID, const NFIDataList& var)
{
	if (var.GetCount() != 7 ||
		!var.TypeEx(TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT,
			TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_FLOAT,
			TDATA_TYPE::TDATA_FLOAT, TDATA_TYPE::TDATA_FLOAT, TDATA_TYPE::TDATA_UNKNOWN)
		)
	{
		return 1;
	}

	NFGUID ident = var.Object(0);
	int nType = var.Int(1);
	int nTargetScene = var.Int(2);
	int nTargetGroupID = var.Int(3);
	float fX = var.Float(4);
	float fY = var.Float(5);
	float fZ = var.Float(6);

	NFMsg::ReqAckSwapScene xSwapScene;
	xSwapScene.set_transfer_type(NFMsg::ReqAckSwapScene::EGameSwapType::ReqAckSwapScene_EGameSwapType_EGST_NARMAL);
	xSwapScene.set_scene_id(nTargetScene);
	xSwapScene.set_line_id(nTargetGroupID);
	xSwapScene.set_x(fX);
	xSwapScene.set_y(fY);
	xSwapScene.set_z(fZ);

	SendMsgPBToGate(NFMsg::EGMI_ACK_SWAP_SCENE, xSwapScene, self);

	return 0;
}

void NFCGameServerNet_ServerModule::OnReqiureRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//fd
	NFGUID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFCGameServerNet_ServerModule::OnCreateRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqCreateRole xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nClientID))
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

void NFCGameServerNet_ServerModule::OnDeleteRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqDeleteRole xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}


	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nPlayerID);
}

void NFCGameServerNet_ServerModule::OnClienSwapSceneProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckSwapScene)

		NFCDataList varEntry;
	varEntry << pObject->Self();
	varEntry << 0;
	varEntry << xMsg.scene_id();
	varEntry << -1;
	m_pEventModule->DoEvent(pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry);
}

void NFCGameServerNet_ServerModule::OnClientPropertyIntProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectPropertyInt)

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

void NFCGameServerNet_ServerModule::OnClientPropertyFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectPropertyFloat)

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

void NFCGameServerNet_ServerModule::OnClientPropertyStringProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectPropertyString)

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

void NFCGameServerNet_ServerModule::OnClientPropertyObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectPropertyObject)

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


void NFCGameServerNet_ServerModule::OnClientPropertyVector2Process(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectPropertyVector2)

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

void NFCGameServerNet_ServerModule::OnClientPropertyVector3Process(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectPropertyVector3)

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


void NFCGameServerNet_ServerModule::OnClientAddRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordAddRow)

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

		std::map<int, NFIDataList::TData> colDataMap;
		for (int j = 0; j < xAddRowStruct.record_int_list_size(); j++)
		{
			const NFMsg::RecordInt &xRecordInt = xAddRowStruct.record_int_list().Get(j);
			NFIDataList::TData data;
			data.SetInt(xRecordInt.data());
			colDataMap.insert(std::map<int, NFIDataList::TData>::value_type(xRecordInt.col(), data));
		}
		for (int j = 0; j < xAddRowStruct.record_float_list_size(); j++)
		{
			const NFMsg::RecordFloat &xRecordFloat = xAddRowStruct.record_float_list().Get(j);
			NFIDataList::TData data;
			data.SetFloat(xRecordFloat.data());
			colDataMap.insert(std::map<int, NFIDataList::TData>::value_type(xRecordFloat.col(), data));
		}
		for (int j = 0; j < xAddRowStruct.record_string_list_size(); j++)
		{
			const NFMsg::RecordString &xRecordString = xAddRowStruct.record_string_list().Get(j);
			NFIDataList::TData data;
			data.SetString(xRecordString.data());
			colDataMap.insert(std::map<int, NFIDataList::TData>::value_type(xRecordString.col(), data));
		}
		for (int j = 0; j < xAddRowStruct.record_object_list_size(); j++)
		{
			const NFMsg::RecordObject &xRecordObject = xAddRowStruct.record_object_list().Get(j);
			NFIDataList::TData data;
			data.SetObject(NFINetModule::PBToNF(xRecordObject.data()));
			colDataMap.insert(std::map<int, NFIDataList::TData>::value_type(xRecordObject.col(), data));
		}

		NFCDataList xDataList;
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

void NFCGameServerNet_ServerModule::OnClientRemoveRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordRemove)

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

void NFCGameServerNet_ServerModule::OnClientSwapRowProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordSwap)

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

void NFCGameServerNet_ServerModule::OnClientRecordIntProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordInt)

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


void NFCGameServerNet_ServerModule::OnClientRecordFloatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordFloat)

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

void NFCGameServerNet_ServerModule::OnClientRecordStringProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordString)

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

void NFCGameServerNet_ServerModule::OnClientRecordObjectProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordObject)

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

void NFCGameServerNet_ServerModule::OnClientRecordVector2Process(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordVector2)

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

void NFCGameServerNet_ServerModule::OnClientRecordVector3Process(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ObjectRecordVector3)

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

void NFCGameServerNet_ServerModule::OnProxyServerRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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

void NFCGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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

void NFCGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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

void NFCGameServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self)
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

void NFCGameServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self)
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

NFINetModule* NFCGameServerNet_ServerModule::GetNetModule()
{
	return m_pNetModule;
}

bool NFCGameServerNet_ServerModule::AddPlayerGateInfo(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID)
{
    if (nGateID <= 0)
    {
        //�Ƿ�gate
        return false;
    }

    if (nClientID.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<NFCGameServerNet_ServerModule::GateBaseInfo> pBaseData = mRoleBaseData.GetElement(nRoleID);
    if (nullptr != pBaseData)
    {
        // �Ѿ�����
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

bool NFCGameServerNet_ServerModule::RemovePlayerGateInfo(const NFGUID& nRoleID)
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

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo> NFCGameServerNet_ServerModule::GetPlayerGateInfo(const NFGUID& nRoleID)
{
    return mRoleBaseData.GetElement(nRoleID);
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> NFCGameServerNet_ServerModule::GetGateServerInfo(const int nGateID)
{
    return mProxyMap.GetElement(nGateID);
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> NFCGameServerNet_ServerModule::GetGateServerInfoBySockIndex(const int nSockIndex)
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

void NFCGameServerNet_ServerModule::OnTransWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	std::string strMsg;
	NFGUID nPlayer;
	int nHasKey = 0;
	if (NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, strMsg, nPlayer))
	{
		nHasKey = nPlayer.nData64;
	}

	m_pGameServerToWorldModule->SendBySuit(nHasKey, nMsgID, msg, nLen);
}

void NFCGameServerNet_ServerModule::OnTransWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen, const int nWorldKey)
{
	m_pGameServerToWorldModule->SendBySuit(nWorldKey, nMsgID, msg, nLen);
}
