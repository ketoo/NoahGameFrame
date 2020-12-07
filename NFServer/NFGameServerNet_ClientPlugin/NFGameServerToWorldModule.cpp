/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "NFGameServerToWorldModule.h"
#include "NFGameServerNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFServer/NFDBLogicPlugin/NFCommonRedisModule.h"

bool NFGameServerToWorldModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	return true;
}

bool NFGameServerToWorldModule::Shut()
{

	return true;
}


bool NFGameServerToWorldModule::Execute()
{
	ServerReport();
	return true;
}

void NFGameServerToWorldModule::Register(NFINet* pNet)
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (serverType == NF_SERVER_TYPES::NF_ST_GAME && pPluginManager->GetAppID() == serverID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				NFMsg::ServerInfoReportList xMsg;
				NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

				pData->set_server_id(serverID);
				pData->set_server_name(strId);
				pData->set_server_cur_count(0);
				pData->set_server_ip(ip);
				pData->set_server_port(nPort);
				pData->set_server_max_online(maxConnect);
				pData->set_server_state(NFMsg::EST_NARMAL);
				pData->set_server_type(serverType);

				NF_SHARE_PTR<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(pNet);
				if (pServerData)
				{
					int nTargetID = pServerData->nGameID;
					m_pNetClientModule->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::GTW_GAME_REGISTERED, xMsg);

					m_pLogModule->LogInfo(NFGUID(0, pData->server_id()), pData->server_name(), "Register");
				}
			}
		}
	}
}

void NFGameServerToWorldModule::ServerReport()
{
	if (mLastReportTime + 10 > pPluginManager->GetNowTime())
	{
		return;
	}
	mLastReportTime = pPluginManager->GetNowTime();

	std::shared_ptr<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (pPluginManager->GetAppID() == serverID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(serverID);
				reqMsg.set_server_name(strId);
				reqMsg.set_server_cur_count(m_pKernelModule->GetOnLineCount());
				reqMsg.set_server_ip(ip);
				reqMsg.set_server_port(nPort);
				reqMsg.set_server_max_online(maxConnect);
				reqMsg.set_server_state(NFMsg::EST_NARMAL);
				reqMsg.set_server_type(serverType);

				m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::STS_SERVER_REPORT, reqMsg, NFGUID());
			}
		}
	}
}

bool NFGameServerToWorldModule::AfterInit()
{
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_PROPERTY_INT, this, &NFGameServerToWorldModule::OnWorldPropertyIntProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_PROPERTY_FLOAT, this, &NFGameServerToWorldModule::OnWorldPropertyFloatProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_PROPERTY_STRING, this, &NFGameServerToWorldModule::OnWorldPropertyStringProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_PROPERTY_OBJECT, this, &NFGameServerToWorldModule::OnWorldPropertyObjectProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_PROPERTY_VECTOR2, this, &NFGameServerToWorldModule::OnWorldPropertyVector2Process);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_PROPERTY_VECTOR3, this, &NFGameServerToWorldModule::OnWorldPropertyVector3Process);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_OBJECT_RECORD_ENTRY, this, &NFGameServerToWorldModule::OnWorldRecordEnterProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_ADD_ROW, this, &NFGameServerToWorldModule::OnWorldAddRowProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_REMOVE_ROW, this, &NFGameServerToWorldModule::OnWorldRemoveRowProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_SWAP_ROW, this, &NFGameServerToWorldModule::OnWorldSwapRowProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_RECORD_INT, this, &NFGameServerToWorldModule::OnWorldRecordIntProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_RECORD_FLOAT, this, &NFGameServerToWorldModule::OnWorldRecordFloatProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_RECORD_STRING, this, &NFGameServerToWorldModule::OnWorldRecordStringProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_RECORD_OBJECT, this, &NFGameServerToWorldModule::OnWorldRecordObjectProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_RECORD_VECTOR2, this, &NFGameServerToWorldModule::OnWorldRecordVector2Process);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_RECORD_VECTOR3, this, &NFGameServerToWorldModule::OnWorldRecordVector3Process);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::STS_NET_INFO, this, &NFGameServerToWorldModule::OnServerInfoProcess);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, this, &NFGameServerToWorldModule::TransPBToProxy);
	
	m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_WORLD, this, &NFGameServerToWorldModule::OnSocketWSEvent);
	
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFGameServerToWorldModule::OnObjectClassEvent);

	m_pNetClientModule->ExpandBufferSize();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		const int nCurAppID = pPluginManager->GetAppID();
		std::vector<std::string>::const_iterator itr =
			std::find_if(strIdList.begin(), strIdList.end(), [&](const std::string& strConfigId)
		{
			return nCurAppID == m_pElementModule->GetPropertyInt32(strConfigId, NFrame::Server::ServerID());
		});

		if (strIdList.end() == itr)
		{
			std::ostringstream strLog;
			strLog << "Cannot find current server, AppID = " << nCurAppID;
			m_pLogModule->LogError(NULL_OBJECT, strLog, __FILE__, __LINE__);
			NFASSERT(-1, "Cannot find current server", __FILE__, __FUNCTION__);
			exit(0);
		}

		const int nCurArea = m_pElementModule->GetPropertyInt32(*itr, NFrame::Server::Area());

		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			const int nServerArea = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Area());
			if (serverType == NF_SERVER_TYPES::NF_ST_WORLD && nCurArea == nServerArea)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				//const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				ConnectData xServerData;

				xServerData.nGameID = serverID;
				xServerData.eServerType = (NF_SERVER_TYPES)serverType;
				xServerData.ip = ip;
				xServerData.nPort = nPort;
				xServerData.name = strId;

				m_pNetClientModule->AddServer(xServerData);
			}
		}
	}

	return true;
}

void NFGameServerToWorldModule::OnServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!NFINetModule::ReceivePB(msgID, msg, len, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

		//type
		ConnectData xServerData;

		xServerData.nGameID = xData.server_id();
		xServerData.ip = xData.server_ip();
		xServerData.nPort = xData.server_port();
		xServerData.name = xData.server_name();
		xServerData.nWorkLoad = xData.server_cur_count();
		xServerData.eServerType = (NF_SERVER_TYPES)xData.server_type();

		if (NF_SERVER_TYPES::NF_ST_WORLD == xServerData.eServerType)
		{
			m_pNetClientModule->AddServer(xServerData);
		}
		else if (NF_SERVER_TYPES::NF_ST_DB == xServerData.eServerType)
		{
			m_pNetClientModule->AddServer(xServerData);
		}
	}
}

void NFGameServerToWorldModule::OnSocketWSEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
		Register(pNet);

	}
}

int NFGameServerToWorldModule::OnObjectClassEvent(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var)
{
	if (className == NFrame::Player::ThisName())
	{
		if (CLASS_OBJECT_EVENT::COE_DESTROY == classEvent)
		{
			SendOffline(self);
		}
		else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == classEvent)
		{
			SendOnline(self);
		}
	}

	return 0;
}

void NFGameServerToWorldModule::SendOnline(const NFGUID& self)
{
	if (m_pKernelModule->ExistObject(self))
	{
		NFMsg::RoleOnlineNotify xMsg;
		const int& gateID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GateID());
		const std::string& playerName = m_pKernelModule->GetPropertyString(self, NFrame::Player::Name());
		const int bp = m_pKernelModule->GetPropertyInt(self, NFrame::Player::BattlePoint());

		*xMsg.mutable_self() = NFINetModule::NFToPB(self);
		xMsg.set_game(pPluginManager->GetAppID());
		xMsg.set_proxy(gateID);
		xMsg.set_name(playerName);
		xMsg.set_bp(bp);

		m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_ONLINE_NOTIFY, xMsg, self);
	}
}

void NFGameServerToWorldModule::SendOffline(const NFGUID& self)
{
	if (m_pKernelModule->ExistObject(self))
	{
		NFMsg::RoleOfflineNotify xMsg;

		//const NFGUID& xClan = m_pKernelModule->GetPropertyObject(self, NFrame::Player::Clan_ID());

		*xMsg.mutable_self() = NFINetModule::NFToPB(self);
		*xMsg.mutable_clan() = NFINetModule::NFToPB(NFGUID());
		xMsg.set_game(pPluginManager->GetAppID());
		xMsg.set_proxy(0);

		m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_OFFLINE_NOTIFY, xMsg, self);
	}
}

void NFGameServerToWorldModule::TransPBToProxy(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	NFGUID nPlayerID;
	std::string strData;
	if (!NFINetModule::ReceivePB( msgID, msg, len, strData, nPlayerID))
	{
		return;
	}

	m_pGameServerNet_ServerModule->SendMsgToGate(msgID, strData, nPlayerID);

	return;
}

void NFGameServerToWorldModule::TransmitToWorld(const int nHashKey, const int msgID, const google::protobuf::Message& xData)
{
	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nHashKey, msgID, xData);
}

void NFGameServerToWorldModule::OnWorldPropertyIntProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyInt)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyInt &xProperty = xMsg.property_list().Get(i);
		m_pKernelModule->SetPropertyInt(nPlayerID, xProperty.property_name(), xProperty.data());
	}
}

void NFGameServerToWorldModule::OnWorldPropertyFloatProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyFloat)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyFloat &xProperty = xMsg.property_list().Get(i);
		m_pKernelModule->SetPropertyFloat(nPlayerID, xProperty.property_name(), xProperty.data());
	}
}

void NFGameServerToWorldModule::OnWorldPropertyStringProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyString)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyString &xProperty = xMsg.property_list().Get(i);
		m_pKernelModule->SetPropertyString(nPlayerID, xProperty.property_name(), xProperty.data());
	}
}

void NFGameServerToWorldModule::OnWorldPropertyObjectProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyObject)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyObject &xProperty = xMsg.property_list().Get(i);
		m_pKernelModule->SetPropertyObject(nPlayerID, xProperty.property_name(), NFINetModule::PBToNF(xProperty.data()));
	}
}

void NFGameServerToWorldModule::OnWorldPropertyVector2Process(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyVector2)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyVector2 &xProperty = xMsg.property_list().Get(i);
		m_pKernelModule->SetPropertyVector2(nPlayerID, xProperty.property_name(),  NFINetModule::PBToNF(xProperty.data()));
	}
}

void NFGameServerToWorldModule::OnWorldPropertyVector3Process(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyVector3)

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyVector3 &xProperty = xMsg.property_list().Get(i);
		m_pKernelModule->SetPropertyVector3(nPlayerID, xProperty.property_name(),  NFINetModule::PBToNF(xProperty.data()));
	}
}

void NFGameServerToWorldModule::OnWorldRecordEnterProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::MultiObjectRecordList)

	for (int playerIndex = 0; playerIndex < xMsg.multi_player_record_size(); playerIndex++)
	{
		const NFMsg::ObjectRecordList& objectRecordList = xMsg.multi_player_record(playerIndex);
		for (int j = 0; j < objectRecordList.record_list_size(); ++j)
		{
			const NFMsg::ObjectRecordBase& recordBase = objectRecordList.record_list(j);
			auto record = m_pKernelModule->FindRecord(nPlayerID, recordBase.record_name());
			if (record)
			{
				NFCommonRedisModule::ConvertPBToRecord(recordBase, record);
			}
		}
	}
}

void NFGameServerToWorldModule::OnWorldAddRowProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordAddRow)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (pRecord)
	{
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
				colDataMap[xRecordInt.col()] = data;
			}

			for (int j = 0; j < xAddRowStruct.record_float_list_size(); j++)
			{
				const NFMsg::RecordFloat &xRecordFloat = xAddRowStruct.record_float_list().Get(j);
				NFData data;
				data.SetFloat(xRecordFloat.data());
				colDataMap[xRecordFloat.col()] = data;
			}
			for (int j = 0; j < xAddRowStruct.record_string_list_size(); j++)
			{
				const NFMsg::RecordString &xRecordString = xAddRowStruct.record_string_list().Get(j);
				NFData data;
				data.SetString(xRecordString.data());
				colDataMap[xRecordString.col()] = data;
			}
			for (int j = 0; j < xAddRowStruct.record_object_list_size(); j++)
			{
				const NFMsg::RecordObject &xRecordObject = xAddRowStruct.record_object_list().Get(j);
				NFData data;
				data.SetObject(NFINetModule::PBToNF(xRecordObject.data()));
				colDataMap[xRecordObject.col()] = data;
			}

			for (int j = 0; j < xAddRowStruct.record_vector2_list_size(); j++)
			{
				const NFMsg::RecordVector2 &xRecordObject = xAddRowStruct.record_vector2_list().Get(j);
				NFData data;
				data.SetVector2(NFINetModule::PBToNF(xRecordObject.data()));
				colDataMap[xRecordObject.col()] = data;
			}

			for (int j = 0; j < xAddRowStruct.record_vector3_list_size(); j++)
			{
				const NFMsg::RecordVector3 &xRecordObject = xAddRowStruct.record_vector3_list().Get(j);
				NFData data;
				data.SetVector3(NFINetModule::PBToNF(xRecordObject.data()));
				colDataMap[xRecordObject.col()] = data;
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
					m_pLogModule->LogInfo(nPlayerID, "Upload From Client add row record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
					return;
				}
			}

			if (pRecord->AddRow(row, xDataList) >= 0)
			{
				m_pLogModule->LogInfo(nPlayerID, "Upload From Client add row record " + xMsg.record_name(), __FUNCTION__, __LINE__);
			}
			else
			{
				m_pLogModule->LogInfo(nPlayerID, "Upload From Client add row record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
			}
		}
	}
}

void NFGameServerToWorldModule::OnWorldRemoveRowProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordRemove)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (pRecord)
	{
		for (int i = 0; i < xMsg.remove_row_size(); i++)
		{
			if (pRecord->Remove(xMsg.remove_row().Get(i)))
			{
				m_pLogModule->LogInfo(nPlayerID, "Upload From Client remove row record " + xMsg.record_name(), __FUNCTION__, __LINE__);
			}
			else
			{
				m_pLogModule->LogInfo(nPlayerID, "Upload From Client remove row record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
			}
		}
	}
}

void NFGameServerToWorldModule::OnWorldSwapRowProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{

}

void NFGameServerToWorldModule::OnWorldRecordIntProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordInt)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client int set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordInt &xRecordInt = xMsg.property_list().Get(i);
		pRecord->SetInt(xRecordInt.row(), xRecordInt.col(), xRecordInt.data());
		m_pLogModule->LogInfo(nPlayerID, "Upload From Client int set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFGameServerToWorldModule::OnWorldRecordFloatProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordFloat)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client float set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordFloat &xRecordFloat = xMsg.property_list().Get(i);
		pRecord->SetFloat(xRecordFloat.row(), xRecordFloat.col(), xRecordFloat.data());
		m_pLogModule->LogInfo(nPlayerID, "Upload From Client float set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFGameServerToWorldModule::OnWorldRecordStringProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordString)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "String set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordString &xRecordString = xMsg.property_list().Get(i);
		pRecord->SetString(xRecordString.row(), xRecordString.col(), xRecordString.data());
		m_pLogModule->LogInfo(nPlayerID, "String set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFGameServerToWorldModule::OnWorldRecordObjectProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordObject)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client Object set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordObject &xRecordObject = xMsg.property_list().Get(i);
		pRecord->SetObject(xRecordObject.row(), xRecordObject.col(), NFINetModule::PBToNF(xRecordObject.data()));
		m_pLogModule->LogInfo(nPlayerID, "Upload From Client Object set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFGameServerToWorldModule::OnWorldRecordVector2Process(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordVector2)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client vector2 set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordVector2 &xRecordVector2 = xMsg.property_list().Get(i);
		pRecord->SetVector2(xRecordVector2.row(), xRecordVector2.col(), NFINetModule::PBToNF(xRecordVector2.data()));
		m_pLogModule->LogInfo(nPlayerID, "Upload From Client vector2 set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}

void NFGameServerToWorldModule::OnWorldRecordVector3Process(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordVector3)

	auto pRecord = m_pKernelModule->FindRecord(nPlayerID, xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client vector3 set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::RecordVector3 &xRecordVector3 = xMsg.property_list().Get(i);
		pRecord->SetVector3(xRecordVector3.row(), xRecordVector3.col(), NFINetModule::PBToNF(xRecordVector3.data()));
		m_pLogModule->LogInfo(nPlayerID, "Upload From Client vector3 set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
	}
}
