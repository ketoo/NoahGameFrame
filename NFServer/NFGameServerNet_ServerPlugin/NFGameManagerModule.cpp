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

#include "NFGameManagerModule.h"

bool NFGameManagerModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();

	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	return true;
}

bool NFGameManagerModule::Shut()
{
	return true;
}

bool NFGameManagerModule::Execute()
{
	return true;
}

bool NFGameManagerModule::AfterInit()
{
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_PROPERTY_INT, this, &NFGameManagerModule::OnClientPropertyIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_PROPERTY_FLOAT, this, &NFGameManagerModule::OnClientPropertyFloatProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_PROPERTY_STRING, this, &NFGameManagerModule::OnClientPropertyStringProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_PROPERTY_OBJECT, this, &NFGameManagerModule::OnClientPropertyObjectProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_PROPERTY_VECTOR2, this, &NFGameManagerModule::OnClientPropertyVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_PROPERTY_VECTOR3, this, &NFGameManagerModule::OnClientPropertyVector3Process);

	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_ADD_ROW, this, &NFGameManagerModule::OnClientAddRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_REMOVE_ROW, this, &NFGameManagerModule::OnClientRemoveRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_SWAP_ROW, this, &NFGameManagerModule::OnClientSwapRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_RECORD_INT, this, &NFGameManagerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_RECORD_FLOAT, this, &NFGameManagerModule::OnClientRecordFloatProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_RECORD_STRING, this, &NFGameManagerModule::OnClientRecordStringProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_RECORD_OBJECT, this, &NFGameManagerModule::OnClientRecordObjectProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_RECORD_VECTOR2, this, &NFGameManagerModule::OnClientRecordVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_RECORD_VECTOR3, this, &NFGameManagerModule::OnClientRecordVector3Process);

	return true;
}

void NFGameManagerModule::OnClientPropertyIntProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyInt)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyInt &xProperty = xMsg.property_list().Get(i);
		NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());

		auto pProperty = CalProperty(pObject, objectID, xProperty.property_name(), nGMLevel);
		if (pProperty)
		{
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				m_pLogModule->LogInfo(objectID, "Upload From Client int set " + xProperty.property_name() + std::to_string(xProperty.data()), __FUNCTION__, __LINE__);

				if (!objectID.IsNull() && objectID == pObject->Self())
				{
					m_pKernelModule->SetPropertyInt(objectID, xProperty.property_name(), xProperty.data());
				}
				else
				{
					m_pSceneModule->SetPropertyInt(scene, group, xProperty.property_name(), xProperty.data());
				}
			}
			else
			{
				m_pLogModule->LogError(nPlayerID, "Upload From Client int set Upload error " + xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogError(nPlayerID, "Upload From Client int set Property error " + xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameManagerModule::OnClientPropertyFloatProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyFloat)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyFloat &xProperty = xMsg.property_list().Get(i);
		NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());

		auto pProperty = CalProperty(pObject, objectID, xProperty.property_name(), nGMLevel);
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				if (!objectID.IsNull() && objectID == pObject->Self())
				{
					m_pKernelModule->SetPropertyInt(objectID, xProperty.property_name(), xProperty.data());
				}
				else
				{
					m_pSceneModule->SetPropertyInt(scene, group, xProperty.property_name(), xProperty.data());
				}
			}
			else
			{
				m_pLogModule->LogError(nPlayerID, "Upload From Client float set Upload error " + xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogError(nPlayerID, "Upload From Client float set Property error " + xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameManagerModule::OnClientPropertyStringProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyString)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyString &xProperty = xMsg.property_list().Get(i);
		NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());

		auto pProperty = CalProperty(pObject, objectID, xProperty.property_name(), nGMLevel);
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				if (!objectID.IsNull() && objectID == pObject->Self())
				{
					m_pKernelModule->SetPropertyString(objectID, xProperty.property_name(), xProperty.data());
				}
				else
				{
					m_pSceneModule->SetPropertyString(scene, group, xProperty.property_name(), xProperty.data());
				}
			}
			else
			{
				m_pLogModule->LogError(nPlayerID, "Upload From Client string set Upload error " + xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogError(nPlayerID, "Upload From Client string set Property error" + xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameManagerModule::OnClientPropertyObjectProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyObject)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyObject &xProperty = xMsg.property_list().Get(i);
		NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());

		auto pProperty = CalProperty(pObject, objectID, xProperty.property_name(), nGMLevel);
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				if (!objectID.IsNull() && objectID == pObject->Self())
				{
					m_pKernelModule->SetPropertyObject(objectID, xProperty.property_name(), NFINetModule::PBToNF(xProperty.data()));
				}
				else
				{
					m_pSceneModule->SetPropertyObject(scene, group, xProperty.property_name(), NFINetModule::PBToNF(xProperty.data()));
				}
			}
			else
			{
				m_pLogModule->LogError(nPlayerID, "Upload From Client object set Upload error " + xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogError(nPlayerID, "Upload From Client object set Property error " + xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}


void NFGameManagerModule::OnClientPropertyVector2Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyVector2)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		NFMsg::PropertyVector2 xProperty = xMsg.property_list().Get(i);
		NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());

		auto pProperty = CalProperty(pObject, objectID, xProperty.property_name(), nGMLevel);
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				if (!objectID.IsNull() && objectID == pObject->Self())
				{
					m_pKernelModule->SetPropertyVector2(objectID, xProperty.property_name(), NFINetModule::PBToNF(xProperty.data()));
				}
				else
				{
					m_pSceneModule->SetPropertyVector2(scene, group, xProperty.property_name(), NFINetModule::PBToNF(xProperty.data()));
				}
			}
			else
			{
				m_pLogModule->LogError(nPlayerID, "Upload From Client object set Upload error " + xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogError(nPlayerID, "Upload From Client object set Property error " + xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameManagerModule::OnClientPropertyVector3Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectPropertyVector3)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		NFMsg::PropertyVector3 xProperty = xMsg.property_list().Get(i);
		NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());

		auto pProperty = CalProperty(pObject, objectID, xProperty.property_name(), nGMLevel);
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				if (!objectID.IsNull() && objectID == pObject->Self())
				{
					m_pKernelModule->SetPropertyVector3(objectID, xProperty.property_name(), NFINetModule::PBToNF(xProperty.data()));
				}
				else
				{
					m_pSceneModule->SetPropertyVector3(scene, group, xProperty.property_name(), NFINetModule::PBToNF(xProperty.data()));
				}
			}
			else
			{
				m_pLogModule->LogError(nPlayerID, "Upload From Client object set Upload error " + xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogError(nPlayerID, "Upload From Client object set Property error " + xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameManagerModule::OnClientAddRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordAddRow)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client add row record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (pRecord->GetUpload() || nGMLevel > 0)
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

void NFGameManagerModule::OnClientRemoveRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordRemove)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client remove row record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
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

void NFGameManagerModule::OnClientSwapRowProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{

}

void NFGameManagerModule::OnClientRecordIntProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordInt)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client int set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordInt &xRecordInt = xMsg.property_list().Get(i);
			pRecord->SetInt(xRecordInt.row(), xRecordInt.col(), xRecordInt.data());
			m_pLogModule->LogInfo(nPlayerID, "Upload From Client int set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}

}


void NFGameManagerModule::OnClientRecordFloatProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordFloat)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client float set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordFloat &xRecordFloat = xMsg.property_list().Get(i);
			pRecord->SetFloat(xRecordFloat.row(), xRecordFloat.col(), xRecordFloat.data());
			m_pLogModule->LogInfo(nPlayerID, "Upload From Client float set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}

}

void NFGameManagerModule::OnClientRecordStringProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordString)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client String set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordString &xRecordString = xMsg.property_list().Get(i);
			pRecord->SetString(xRecordString.row(), xRecordString.col(), xRecordString.data());
			m_pLogModule->LogInfo(nPlayerID, "Upload From Client String set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}

}

void NFGameManagerModule::OnClientRecordObjectProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordObject)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client Object set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordObject &xRecordObject = xMsg.property_list().Get(i);
			pRecord->SetObject(xRecordObject.row(), xRecordObject.col(), NFINetModule::PBToNF(xRecordObject.data()));
			m_pLogModule->LogInfo(nPlayerID, "Upload From Client Object set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameManagerModule::OnClientRecordVector2Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordVector2)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client vector2 set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (pRecord->GetUpload() || nGMLevel > 0)
	{
		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordVector2 &xRecordVector2 = xMsg.property_list().Get(i);
			pRecord->SetVector2(xRecordVector2.row(), xRecordVector2.col(), NFINetModule::PBToNF(xRecordVector2.data()));
			m_pLogModule->LogInfo(nPlayerID, "Upload From Client vector2 set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameManagerModule::OnClientRecordVector3Process(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ObjectRecordVector3)

	int nGMLevel = 0;
	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NFGUID objectID = NFINetModule::PBToNF(xMsg.player_id());
	auto pRecord = CalRecord(pObject, objectID, xMsg.record_name(), nGMLevel);
	if (!pRecord)
	{
		m_pLogModule->LogError(nPlayerID, "Upload From Client vector3 set record error " + xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordVector3 &xRecordVector3 = xMsg.property_list().Get(i);
			pRecord->SetVector3(xRecordVector3.row(), xRecordVector3.col(), NFINetModule::PBToNF(xRecordVector3.data()));
			m_pLogModule->LogInfo(nPlayerID, "Upload From Client vector3 set record " + xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}
}

std::shared_ptr<NFIProperty> NFGameManagerModule::CalProperty(NF_SHARE_PTR<NFIObject> pObject, const NFGUID objectID, const std::string& propertyName, int &gmLevel)
{
	const std::string& account = pObject->GetPropertyString(NFrame::Player::Account());
	gmLevel = m_pElementModule->GetPropertyInt(account, NFrame::GM::Level());
#ifdef NF_DEBUG_MODE
	gmLevel = 1;
#endif

	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NF_SHARE_PTR<NFIProperty> pProperty;
	if (!objectID.IsNull())
	{
		auto gameObject = m_pKernelModule->GetObject(objectID);
		if (gameObject)
		{
			pProperty = gameObject->GetPropertyManager()->GetElement(propertyName);
		}
	}
	else
	{
		auto propertyManager = m_pSceneModule->FindPropertyManager(scene, group);
		if (propertyManager)
		{
			pProperty = propertyManager->GetElement(propertyName);
		}
	}

	return pProperty;
}

std::shared_ptr<NFIRecord> NFGameManagerModule::CalRecord(NF_SHARE_PTR<NFIObject> pObject, const NFGUID objectID, const std::string &recordName, int &gmLevel)
{
	const std::string& account = pObject->GetPropertyString(NFrame::Player::Account());
	gmLevel = m_pElementModule->GetPropertyInt(account, NFrame::GM::Level());
#ifdef NF_DEBUG_MODE
	gmLevel = 1;
#endif

	const int scene = pObject->GetPropertyInt(NFrame::IObject::SceneID());
	const int group = pObject->GetPropertyInt(NFrame::IObject::GroupID());

	NF_SHARE_PTR<NFIRecord> pRecord;
	if (!objectID.IsNull())
	{
		auto gameObject = m_pKernelModule->GetObject(objectID);
		if (gameObject)
		{
			pRecord = gameObject->GetRecordManager()->GetElement(recordName);
		}
	}
	else
	{
		auto recordManager = m_pSceneModule->FindRecordManager(scene, group);
		if (recordManager)
		{
			pRecord = recordManager->GetElement(recordName);
		}
	}

	return pRecord;
}
