// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCGameServerNet_ServerModule::Init()
{
	m_pNetModule = NF_NEW NFINetModule(pPluginManager);
	return true;
}

bool NFCGameServerNet_ServerModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>("NFCSceneProcessModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
	m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>("NFCGameServerToWorldModule");


	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pSceneProcessModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pGameServerToWorldModule);

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
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_PICK_ITEM, this, &NFCGameServerNet_ServerModule::OnClienPickItem);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_MOVE, this, &NFCGameServerNet_ServerModule::OnClienMove);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_MOVE_IMMUNE, this, &NFCGameServerNet_ServerModule::OnClienMoveImmune);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACCEPT_TASK, this, &NFCGameServerNet_ServerModule::OnClienAcceptTask);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_COMPELETE_TASK, this, &NFCGameServerNet_ServerModule::OnClienPushTask);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFCGameServerNet_ServerModule::OnClienChatProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_END_BATTLE, this, &NFCGameServerNet_ServerModule::OnClientEndBattle);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_GUILD, this, &NFCGameServerNet_ServerModule::OnCreateGuild);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_GUILD, this, &NFCGameServerNet_ServerModule::OnJoinGuild);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_GUILD, this, &NFCGameServerNet_ServerModule::OnLeaveGuild);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_OPR_GUILD, this, &NFCGameServerNet_ServerModule::OnOprGuild);
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
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCGameServerNet_ServerModule::OnObjectNPCClassEvent);

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
	//只可能是网关丢了
	int nServerID = 0;
	NF_SHARE_PTR<GateData> pServerData = mProxyMap.First();
	while (pServerData.get())
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
	//在进入游戏之前nPlayerID为其在网关的FD
	NFGUID nClientID;
	NFMsg::ReqEnterGameServer xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());

	PlayerLeaveGameServer(nRoleID);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFCGameServerNet_ServerModule::BaseData> pBaseData = mRoleBaseData.GetElement(nRoleID);
	if (nullptr != pBaseData)
	{
		// 已经存在
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nClientID, "player is exist, cannot enter game", "", __FUNCTION__, __LINE__);
		return;
	}

	int nGateID = -1;
	NF_SHARE_PTR<GateData> pServerData = mProxyMap.First();
	while (pServerData.get())
	{
		if (nSockIndex == pServerData->xServerData.nFD)
		{
			nGateID = pServerData->xServerData.pData->server_id();
			break;
		}

		pServerData = mProxyMap.Next();
	}

	if (nGateID <= 0)
	{
		//非法gate
		return;
	}

	pServerData = mProxyMap.GetElement(nGateID);
	if (nullptr == pServerData)
	{
		return;
	}

	// proxy绑定playerID和gateFD
	if (!pServerData->xRoleInfo.insert(std::make_pair(nRoleID, nSockIndex)).second)
	{
		return;
	}

	//id和fd,gateid绑定
	mRoleBaseData.AddElement(nRoleID, NF_SHARE_PTR<BaseData>(NF_NEW BaseData(nGateID, nClientID)));

	//默认1号场景
	int nSceneID = 1;
	NFCDataList var;
	var.AddString("Name");
	var.AddString(xMsg.name());

	var.AddString("GateID");
	var.AddInt(nGateID);

	var.AddString("ClientID");
	var.AddObject(nClientID);

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, nSceneID, 0, "Player", "", var);
	if (NULL == pObject.get())
	{
		//内存泄漏
		//mRoleBaseData
		//mRoleFDData
		return;
	}

	pObject->SetPropertyInt("LoadPropertyFinish", 1);
	pObject->SetPropertyInt("GateID", nGateID);
	pObject->SetPropertyInt("GameID", pPluginManager->AppID());

	m_pKernelModule->DoEvent(pObject->Self(), NFrame::Player::ThisName(), CLASS_OBJECT_EVENT::COE_CREATE_FINISH, NFCDataList());

	NFCDataList varEntry;
	varEntry << pObject->Self();
	varEntry << NFINT64(0);
	varEntry << nSceneID;
	varEntry << -1;
	m_pKernelModule->DoEvent(pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry);
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

	PlayerLeaveGameServer(nPlayerID);

}

int NFCGameServerNet_ServerModule::OnPropertyEnter(const NFIDataList& argVar, const NFGUID& self)
{
	if (argVar.GetCount() <= 0 || self.IsNull())
	{
		return 0;
	}

	NFMsg::MultiObjectPropertyList xPublicMsg;
	NFMsg::MultiObjectPropertyList xPrivateMsg;

	//分为自己和外人
	//1.public发送给所有人
	//2.如果自己在列表中，再次发送private数据
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (pObject.get())
	{
		NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();
		NFMsg::ObjectPropertyList* pPrivateData = xPrivateMsg.add_multi_player_property();

		*(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(self);
		*(pPrivateData->mutable_player_id()) = NFINetModule::NFToPB(self);

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIProperty> pPropertyInfo = pPropertyManager->First();
		while (pPropertyInfo.get())
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
				//找到他所在网关的FD
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
			//不管public还是private都要加上，不然public广播了那不是private就广播不了了
			NFMsg::RecordAddRowStruct* pAddRowStruct = pObjectRecordBase->add_row_struct();
			pAddRowStruct->set_row(i);

			for (int j = 0; j < pRecord->GetCols(); j++)
			{
				//如果是0就不发送了，因为客户端默认是0
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
	if (pObject.get())
	{
		NFMsg::ObjectRecordList* pPublicData = NULL;
		NFMsg::ObjectRecordList* pPrivateData = NULL;

		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();
		NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
		while (pRecord.get())
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
		//排除空对象
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

		//可能在不同的网关呢,得到后者所在的网关FD
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
		//排除空对象
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
		//可能在不同的网关呢,得到后者所在的网关FD
		SendMsgPBToGate(NFMsg::EGMI_ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, ident);
	}

	return 1;
}

int NFCGameServerNet_ServerModule::OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	//if ( "Player" == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
	{
		if ("GroupID" == strPropertyName)
		{
			//自己还是要知道自己的这个属性变化的,但是别人就不需要知道了
			OnGroupEvent(self, strPropertyName, oldVar, newVar);
		}

		if ("SceneID" == strPropertyName)
		{
			//自己还是要知道自己的这个属性变化的,但是别人就不需要知道了
			OnContainerEvent(self, strPropertyName, oldVar, newVar);
		}

		if ("Player" == m_pKernelModule->GetPropertyString(self, "ClassName"))
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
		//传入的参数是要广播的对象列表
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

			SendMsgPBToGate(NFMsg::EGMI_ACK_PROPERTY_DOUBLE, xPropertyFloat, identOld);
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
		//容器
		return 0;
	}

	if ("Player" == m_pKernelModule->GetPropertyString(self, "ClassName"))
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
	case NFIRecord::RecordOptype::Add:
	{
		NFMsg::ObjectRecordAddRow xAddRecordRow;
		NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		xAddRecordRow.set_record_name(strRecordName);

		NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
		pAddRowData->set_row(nRow);

		//add row 需要完整的row
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
						//添加的时候数据要全s
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
	case NFIRecord::RecordOptype::Del:
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
	case NFIRecord::RecordOptype::Swap:
	{
		//其实是2个row交换
		NFMsg::ObjectRecordSwap xSwapRecord;
		*xSwapRecord.mutable_player_id() = NFINetModule::NFToPB(self);

		xSwapRecord.set_origin_record_name(strRecordName);
		xSwapRecord.set_target_record_name(strRecordName);   // 暂时没用
		xSwapRecord.set_row_origin(nRow);
		xSwapRecord.set_row_target(nCol);

		for (int i = 0; i < valueBroadCaseList.GetCount(); i++)
		{
			NFGUID identOther = valueBroadCaseList.Object(i);

			SendMsgPBToGate(NFMsg::EGMI_ACK_SWAP_ROW, xSwapRecord, identOther);
		}
	}
	break;
	case NFIRecord::RecordOptype::UpData:
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

				SendMsgPBToGate(NFMsg::EGMI_ACK_RECORD_DOUBLE, xRecordChanged, identOther);
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
	////////////1:广播给已经存在的人//////////////////////////////////////////////////////////////
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		//删除在线标志

		//////////////////////////////////////////////////////////////////////////

		int nObjectContainerID = m_pKernelModule->GetPropertyInt(self, "SceneID");
		int nObjectGroupID = m_pKernelModule->GetPropertyInt(self, "GroupID");

		if (nObjectGroupID < 0)
		{
			//容器
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
			if ("Player" == strClassName)
			{
				valueBroadCaseList.Add(identBC);
				if (identBC != self)
				{
					valueBroadListNoSelf.Add(identBC);
				}
			}
		}

		//如果是副本的怪，则不需要发送，因为会在离开副本的时候一次性一条消息发送
		OnObjectListLeave(valueBroadListNoSelf, NFCDataList() << self);
	}

	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{
		//id和fd,gateid绑定
		NF_SHARE_PTR<BaseData> pDataBase = mRoleBaseData.GetElement(self);
		if (pDataBase.get())
		{
			//回复客户端角色进入游戏世界成功了
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
		//自己广播给自己就够了
		if (strClassName == "Player")
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
	//容器发生变化，只可能从A容器的0层切换到B容器的0层
	//需要注意的是------------任何层改变的时候，此玩家其实还未进入层，因此，层改变的时候获取的玩家列表，目标层是不包含自己的
	int nSceneID = m_pKernelModule->GetPropertyInt(self, "SceneID");

	//广播给别人自己离去(层降或者跃层)
	int nOldGroupID = oldVar.GetInt();
	if (nOldGroupID > 0)
	{
		NFCDataList valueAllOldObjectList;
		NFCDataList valueAllOldPlayerList;
		m_pKernelModule->GetGroupObjectList(nSceneID, nOldGroupID, valueAllOldObjectList);
		if (valueAllOldObjectList.GetCount() > 0)
		{
			//自己只需要广播其他玩家
			for (int i = 0; i < valueAllOldObjectList.GetCount(); i++)
			{
				NFGUID identBC = valueAllOldObjectList.Object(i);

				if (valueAllOldObjectList.Object(i) == self)
				{
					valueAllOldObjectList.Set(i, NFGUID());
				}

				const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, "ClassName");
				if ("Player" == strClassName)
				{
					valueAllOldPlayerList.Add(identBC);
				}
			}

			OnObjectListLeave(valueAllOldPlayerList, NFCDataList() << self);

			//老的全部要广播删除
			OnObjectListLeave(NFCDataList() << self, valueAllOldObjectList);
		}

		m_pKernelModule->DoEvent(self, NFED_ON_CLIENT_LEAVE_SCENE, NFCDataList() << nOldGroupID);
	}

	//再广播给别人自己出现(层升或者跃层)
	int nNewGroupID = newVar.GetInt();
	if (nNewGroupID > 0)
	{
		//这里需要把自己从广播中排除
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
			if ("Player" == strClassName)
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

		//广播给别人,自己出现(这里本不应该广播给自己)
		if (valuePlayerListNoSelf.GetCount() > 0)
		{
			OnObjectListEnter(valuePlayerListNoSelf, NFCDataList() << self);
		}

		const std::string& strSelfClassName = m_pKernelModule->GetPropertyString(self, "ClassName");

		//广播给自己,所有的别人出现
		if (valueAllObjectListNoSelf.GetCount() > 0)
		{
			if (strSelfClassName == "Player")
			{
				OnObjectListEnter(NFCDataList() << self, valueAllObjectListNoSelf);
			}
		}

		if (strSelfClassName == "Player")
		{
			for (int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++)
			{
				//此时不用再广播自己的属性给自己
				//把已经存在的人的属性广播给新来的人
				NFGUID identOld = valueAllObjectListNoSelf.Object(i);

				OnPropertyEnter(NFCDataList() << self, identOld);
				//把已经存在的人的表广播给新来的人
				OnRecordEnter(NFCDataList() << self, identOld);
			}
		}

		//把新来的人的属性广播给周边的人
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
	//容器发生变化，只可能从A容器的0层切换到B容器的0层
	//需要注意的是------------任何容器改变的时候，玩家必须是0层
	int nOldSceneID = oldVar.GetInt();
	int nNowSceneID = newVar.GetInt();

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Enter Scene:", nNowSceneID);

	//自己消失,玩家不用广播，因为在消失之前，会回到0层，早已广播了玩家
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
		if ("Player" == strClassName)
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

	//但是旧场景0层的NPC需要广播
	OnObjectListLeave(NFCDataList() << self, valueOldAllObjectList);

	//广播给所有人出现对象(如果是玩家，则包括广播给自己)
	//这里广播的都是0层的
	if (valuePlayerList.GetCount() > 0)
	{
		//把self广播给argVar这些人
		OnObjectListEnter(valuePlayerNoSelf, NFCDataList() << self);
	}

	//新层必然是0，把0层NPC广播给自己------------自己广播给自己不在这里广播，因为场景ID在跨场景时会经常变化

	//把valueAllObjectList广播给self
	OnObjectListEnter(NFCDataList() << self, valueAllObjectListNoSelf);

	////////////////////把已经存在的人的属性广播给新来的人//////////////////////////////////////////////////////
	for (int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++)
	{
		NFGUID identOld = valueAllObjectListNoSelf.Object(i);
		OnPropertyEnter(NFCDataList() << self, identOld);
		////////////////////把已经存在的人的表广播给新来的人//////////////////////////////////////////////////////
		OnRecordEnter(NFCDataList() << self, identOld);
	}

	//把新来的人的属性广播给周边的人()
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

	//普通场景容器，判断广播属性
	std::string strClassName = m_pKernelModule->GetPropertyString(self, "ClassName");
	NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);
	NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);

	NF_SHARE_PTR<NFIRecord> pRecord(NULL);
	NF_SHARE_PTR<NFIProperty> pProperty(NULL);
	if (bTable)
	{
		if (NULL == pClassRecordManager)
		{
			return -1;
		}

		pRecord = pClassRecordManager->GetElement(strPropertyName);
		if (NULL == pRecord.get())
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
		if (NULL == pProperty.get())
		{
			return -1;
		}
	}

	if ("Player" == strClassName)
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
		//一个玩家都不广播
		return valueObject.GetCount();
	}

	//不是玩家,NPC和怪物类等
	if (bTable)
	{
		if (pRecord->GetPublic())
		{
			//广播给客户端自己和周边人
			GetBroadCastObject(nObjectContainerID, nObjectGroupID, valueObject);
		}
	}
	else
	{
		if (pProperty->GetPublic())
		{
			//广播给客户端自己和周边人
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
		if ("Player" == strObjClassName)
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
		m_pKernelModule->AddEventCallBack(self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, this, &NFCGameServerNet_ServerModule::OnSwapSceneResultEvent);
		m_pKernelModule->AddEventCallBack(self, NFED_ON_NOTICE_ECTYPE_AWARD, this, &NFCGameServerNet_ServerModule::OnNoticeEctypeAward);
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnObjectNPCClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		m_pKernelModule->AddEventCallBack(self, NFED_ON_CLIENT_REQUIRE_MOVE, this, &NFCGameServerNet_ServerModule::OnMoveEvent);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int NFCGameServerNet_ServerModule::OnReturnEvent(const NFGUID& self, const int nEventID, const NFIDataList& var)
{
	return 0;
}

int NFCGameServerNet_ServerModule::OnMoveEvent(const NFGUID& self, const int nEventID, const NFIDataList& var)
{
	if (var.GetCount() != 3 || !var.TypeEx(TDATA_FLOAT, TDATA_FLOAT, TDATA_FLOAT, TDATA_UNKNOWN))
	{
		return 1;
	}

	NFMsg::ReqAckPlayerMove xMsg;
	NFMsg::Position* pPos = xMsg.add_target_pos();

	*xMsg.mutable_mover() = NFINetModule::NFToPB(self);
	xMsg.set_movetype(1);   // 移动类型暂时随便给一个
	pPos->set_x(var.Float(0));
	pPos->set_y(var.Float(1));
	pPos->set_z(var.Float(2));

	//bc
	int nContianerID = m_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "SceneID");
	int nGroupID = m_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "GroupID");

	NFCDataList xDataList;
	m_pKernelModule->GetGroupObjectList(nContianerID, nGroupID, xDataList);
	for (int i = 0; i < xDataList.GetCount(); ++i)
	{
		const NFGUID& xIdent = xDataList.Object(i);

		SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE, xMsg, xIdent);
	}

	return 0;
}

int NFCGameServerNet_ServerModule::OnUseSkillResultEvent(const NFGUID& self, const int nEventID, const NFIDataList& var)
{
	return 0;
}

int NFCGameServerNet_ServerModule::OnSwapSceneResultEvent(const NFGUID& self, const int nEventID, const NFIDataList& var)
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

int NFCGameServerNet_ServerModule::OnNoticeEctypeAward(const NFGUID& self, const int nEventID, const NFIDataList& var)
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

	int nStartIndex = 3;//3个

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

int NFCGameServerNet_ServerModule::OnChatResultEvent(const NFGUID& self, const int nEventID, const NFIDataList& var)
{
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
	pData->mutable_id()->CopyFrom(NFINetModule::NFToPB(m_pUUIDModule->CreateGUID()));
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
	m_pKernelModule->DoEvent(pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry);
}


void NFCGameServerNet_ServerModule::OnClienMove(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckPlayerMove)

		m_pKernelModule->SetPropertyFloat(NFINetModule::PBToNF(xMsg.mover()), "X", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(NFINetModule::PBToNF(xMsg.mover()), "Y", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(NFINetModule::PBToNF(xMsg.mover()), "Z", xMsg.target_pos(0).z());

	//bc
	int nContianerID = m_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "SceneID");
	int nGroupID = m_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "GroupID");

	NFCDataList xDataList;
	m_pKernelModule->GetGroupObjectList(nContianerID, nGroupID, xDataList);
	for (int i = 0; i < xDataList.GetCount(); ++i)
	{
		SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE, xMsg, xDataList.Object(i));
	}
}

void NFCGameServerNet_ServerModule::OnClienMoveImmune(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckPlayerMove)

		m_pKernelModule->SetPropertyFloat(NFINetModule::PBToNF(xMsg.mover()), "X", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(NFINetModule::PBToNF(xMsg.mover()), "Y", xMsg.target_pos(0).x());
	m_pKernelModule->SetPropertyFloat(NFINetModule::PBToNF(xMsg.mover()), "Z", xMsg.target_pos(0).z());

	//bc
	int nContianerID = m_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "SceneID");
	int nGroupID = m_pKernelModule->GetPropertyInt(NFINetModule::PBToNF(xMsg.mover()), "GroupID");

	NFCDataList xDataList;
	m_pKernelModule->GetGroupObjectList(nContianerID, nGroupID, xDataList);
	for (int i = 0; i < xDataList.GetCount(); ++i)
	{
		SendMsgPBToGate(NFMsg::EGMI_ACK_MOVE_IMMUNE, xMsg, xDataList.Object(i));
	}
}

void NFCGameServerNet_ServerModule::OnClienCommand(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

void NFCGameServerNet_ServerModule::OnClienAcceptTask(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAcceptTask)
}

void NFCGameServerNet_ServerModule::OnClienPushTask(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqCompeleteTask)
}

void NFCGameServerNet_ServerModule::OnClienPushCustom(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::cu)
}

void NFCGameServerNet_ServerModule::OnClienPickItem(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqPickDropItem)
}

void NFCGameServerNet_ServerModule::OnClienChatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat)

		switch (xMsg.chat_type())
		{
		case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_WORLD:
		{
			m_pNetModule->SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
		}
		break;
		case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_GUILD:
		{
			NFGUID xGuildID = m_pKernelModule->GetPropertyObject(nPlayerID, "GuildID");
			if (!xGuildID.IsNull())
			{
				OnTransWorld(nSockIndex, nMsgID, msg, nLen, xGuildID.nData64);
			}
		}
		break;
		case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_PRIVATE:
		case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_TEAM:
		{
			if (xMsg.has_target_id())
			{
				NFGUID xTargetID = NFINetModule::PBToNF(xMsg.target_id());
				if (!xTargetID.IsNull())
				{
					OnTransWorld(nSockIndex, nMsgID, msg, nLen, nPlayerID.nData64);
				}
			}
		}
		break;
		default:
			break;;
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
		NF_SHARE_PTR<GateData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData.get())
		{
			pServerData = NF_SHARE_PTR<GateData>(NF_NEW GateData());
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
		NF_SHARE_PTR<GateData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData.get())
		{
			pServerData = NF_SHARE_PTR<GateData>(NF_NEW GateData());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->xServerData.nFD = nSockIndex;
		*(pServerData->xServerData.pData) = xData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
	}

	return;
}

void NFCGameServerNet_ServerModule::OnClienGMProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqCommand xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if (nPlayerID != NFINetModule::PBToNF(xMsg.control_id()))
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

		double fX = m_pKernelModule->GetPropertyFloat(nPlayerID, "X");
		double fY = m_pKernelModule->GetPropertyFloat(nPlayerID, "Y");
		double fZ = m_pKernelModule->GetPropertyFloat(nPlayerID, "Z");

		const std::string& strObjectIndex = xMsg.command_str_value();
		//const int nValue = xMsg.command_value();
		if (m_pElementInfoModule->ExistElement(strObjectIndex))
		{
			NFCDataList xDataList;
			xDataList << "X" << fX;
			xDataList << "Y" << fY;
			xDataList << "Z" << fZ;
			const std::string& strObjectClass = m_pElementInfoModule->GetPropertyString(strObjectIndex, "ClassName");
			m_pKernelModule->CreateObject(NFGUID(), nContianerID, nGroupID, strObjectClass, strObjectIndex, xDataList);
		}
	}
	break;
	default:
		break;
	}
}

void NFCGameServerNet_ServerModule::OnClientEndBattle(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
}

void NFCGameServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self)
{
	NF_SHARE_PTR<BaseData> pData = mRoleBaseData.GetElement(self);
	if (pData.get())
	{
		NF_SHARE_PTR<GateData> pProxyData = mProxyMap.GetElement(pData->nGateID);
		if (pProxyData.get())
		{
			m_pNetModule->SendMsgPB(nMsgID, xMsg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

void NFCGameServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self)
{
	NF_SHARE_PTR<BaseData> pData = mRoleBaseData.GetElement(self);
	if (pData.get())
	{
		NF_SHARE_PTR<GateData> pProxyData = mProxyMap.GetElement(pData->nGateID);
		if (pProxyData.get())
		{
			m_pNetModule->SendMsgPB(nMsgID, strMsg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

NFINetModule* NFCGameServerNet_ServerModule::GetNetModule()
{
	return m_pNetModule;
}

void NFCGameServerNet_ServerModule::ProcessSwitchToGame(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID, const int nSceneID)
{
	//PlayerLeaveGameServer(nRoleID);

	////////////////////////////////////////////////////////////////////////////
	////拉取数据
	//if (!m_pPlayerMysqlModule->LoadDataFormSql(nRoleID, "Player"))
	//{
	//	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nClientID, "Cannot load data from mysql", "", __FUNCTION__, __LINE__);
	//	return;
	//}
	////////////////////////////////////////////////////////////////////////////

	//NF_SHARE_PTR<NFCGameServerNet_ServerModule::BaseData> pBaseData = mRoleBaseData.GetElement(nRoleID);
	//if (nullptr != pBaseData)
	//{
	//	// 已经存在
	//	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nClientID, "player is exist, cannot enter game", "", __FUNCTION__, __LINE__);
	//	return;
	//}

	//if (nGateID <= 0)
	//{
	//	//非法gate
	//	return;
	//}

	//NF_SHARE_PTR<GateData> pServerData = mProxyMap.GetElement(nGateID);
	//if (nullptr == pServerData)
	//{
	//	return;
	//}

	//// proxy绑定playerID和gateFD
	//if (!pServerData->xRoleInfo.insert(std::make_pair(nRoleID, pServerData->xServerData.nFD)).second)
	//{
	//	return;
	//}

	////默认1号场景
	//NFCDataList var;
	//var.AddString("GateID");
	//var.AddInt(nGateID);

	//var.AddString("ClientID");
	//var.AddObject(nClientID);

	//NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, nSceneID, 0, "Player", "", var);
	//if (NULL == pObject.get())
	//{
	//	//内存泄漏
	//	//mRoleBaseData
	//	//mRoleFDData
	//	return;
	//}

	//pObject->SetPropertyInt("LoadPropertyFinish", 1);
	//pObject->SetPropertyInt("GateID", nGateID);
	//pObject->SetPropertyInt("GameID", pPluginManager->AppID());

	//m_pKernelModule->DoEvent(pObject->Self(), NFrame::Player::ThisName(), CLASS_OBJECT_EVENT::COE_CREATE_FINISH, NFCDataList());

	//NFCDataList varEntry;
	//varEntry << pObject->Self();
	//varEntry << NFINT64(0);
	//varEntry << nSceneID;
	//varEntry << -1;
	//m_pKernelModule->DoEvent(pObject->Self(), NFED_ON_CLIENT_ENTER_SCENE, varEntry);

	////id和fd,gateid绑定
	//mRoleBaseData.AddElement(nRoleID, NF_SHARE_PTR<BaseData>(NF_NEW BaseData(nGateID, nClientID)));
}

void NFCGameServerNet_ServerModule::ProcessSwitchOffGame(const NFGUID& self)
{
	NF_SHARE_PTR<BaseData> pBaseData = mRoleBaseData.GetElement(self);
	if (nullptr == pBaseData)
	{
		return;
	}

	mRoleBaseData.RemoveElement(self);

	NF_SHARE_PTR<GateData> pServerData = mProxyMap.GetElement(pBaseData->nGateID);
	if (nullptr == pServerData)
	{
		return;
	}

	pServerData->xRoleInfo.erase(self);

	if (!m_pKernelModule->GetObject(self))
    {
        return;
    }

    m_pKernelModule->DestroyObject(self);
}

bool NFCGameServerNet_ServerModule::GetGateInfo(const NFGUID& self, int & nGateID, NFGUID& xClientID)
{
	NF_SHARE_PTR<BaseData> pData = mRoleBaseData.GetElement(self);
	if (pData.get())
	{
		nGateID = pData->nGateID;
		xClientID = pData->xClientID;

		return true;
	}

	return false;
}

void NFCGameServerNet_ServerModule::PlayerLeaveGameServer(const NFGUID& self)
{
	if (!m_pKernelModule->GetObject(self))
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


	NF_SHARE_PTR<GateData> pServerData = mProxyMap.GetElement(pBaseData->nGateID);
	if (nullptr == pServerData)
	{
		return;
	}

	pServerData->xRoleInfo.erase(self);
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

void NFCGameServerNet_ServerModule::OnCreateGuild(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID xGuildID = GetGuildID<NFMsg::ReqAckCreateGuild>(nSockIndex, nMsgID, msg, nLen);
	OnTransWorld(nSockIndex, nMsgID, msg, nLen, xGuildID.nData64);
}

void NFCGameServerNet_ServerModule::OnJoinGuild(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID xGuildID = GetGuildID<NFMsg::ReqAckJoinGuild>(nSockIndex, nMsgID, msg, nLen);
	OnTransWorld(nSockIndex, nMsgID, msg, nLen, xGuildID.nData64);
}

void NFCGameServerNet_ServerModule::OnLeaveGuild(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID xGuildID = GetGuildID<NFMsg::ReqAckLeaveGuild>(nSockIndex, nMsgID, msg, nLen);
	OnTransWorld(nSockIndex, nMsgID, msg, nLen, xGuildID.nData64);
}

void NFCGameServerNet_ServerModule::OnOprGuild(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID xGuildID = GetGuildID<NFMsg::ReqAckOprGuildMember>(nSockIndex, nMsgID, msg, nLen);
	OnTransWorld(nSockIndex, nMsgID, msg, nLen, xGuildID.nData64);
}