/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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


#include "NFAutoBroadcastModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFServer/NFDBLogicPlugin/NFCommonRedisModule.h"

bool NFAutoBroadcastModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();

	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	
	return true;
}

bool NFAutoBroadcastModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFAutoBroadcastModule::OnObjectClassEvent);

	m_pSceneModule->AddObjectEnterCallBack(this, &NFAutoBroadcastModule::OnObjectListEnter);
	m_pSceneModule->AddObjectDataFinishedCallBack(this, &NFAutoBroadcastModule::OnObjectDataFinished);
	m_pSceneModule->AddObjectLeaveCallBack(this, &NFAutoBroadcastModule::OnObjectListLeave);
	m_pSceneModule->AddPropertyEnterCallBack(this, &NFAutoBroadcastModule::OnPropertyEnter);
	m_pSceneModule->AddRecordEnterCallBack(this, &NFAutoBroadcastModule::OnRecordEnter);

	m_pSceneModule->AddPropertyEventCallBack(this, &NFAutoBroadcastModule::OnPropertyEvent);
	m_pSceneModule->AddRecordEventCallBack(this, &NFAutoBroadcastModule::OnRecordEvent);

	m_pSceneModule->AddSwapSceneEventCallBack(this, &NFAutoBroadcastModule::OnSceneEvent);

	return true;
}

bool NFAutoBroadcastModule::Shut()
{

	return true;
}

bool NFAutoBroadcastModule::Execute()
{
	return true;
}

int NFAutoBroadcastModule::OnObjectClassEvent(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == classEvent)
	{
		//only just to tell client, now player can enter world(without data) and you can start to load scene or mesh
		NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo> pDataBase = m_pGameServerNet_ServerModule->GetPlayerGateInfo(self);
		if (pDataBase)
		{
			NFMsg::AckEventResult xMsg;
			xMsg.set_event_code(NFMsg::EGameEventCode::SUCCESS);

			*xMsg.mutable_event_client() = NFINetModule::NFToPB(pDataBase->xClientID);
			*xMsg.mutable_event_object() = NFINetModule::NFToPB(self);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_ENTER_GAME, xMsg, self);
		}
	}

	return 0;
}

int NFAutoBroadcastModule::OnSceneEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList& argList)
{
	NFVector3 vRelivePos = m_pSceneModule->GetRelivePosition(sceneID);

	NFMsg::ReqAckSwapScene xAckSwapScene;
	xAckSwapScene.set_scene_id(sceneID);
	xAckSwapScene.set_transfer_type(type);
	xAckSwapScene.set_line_id(0);
	xAckSwapScene.set_x(vRelivePos.X());
	xAckSwapScene.set_y(vRelivePos.Y());
	xAckSwapScene.set_z(vRelivePos.Z());
	xAckSwapScene.set_data("");

	//buildings

	m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_SWAP_SCENE, xAckSwapScene, self);

	return 0;
}

int NFAutoBroadcastModule::OnPropertyEnter(const NFDataList& argVar, const NFGUID& self)
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
		//std::cout << "OnPropertyEnter auto " << pPropertyManager->Self().ToString() << std::endl;
		//std::cout << pPropertyManager->ToString() << std::endl;

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

				case TDATA_VECTOR2:
				{
					if (pPropertyInfo->GetPublic())
					{
						NFMsg::PropertyVector2* pDataObject = pPublicData->add_property_vector2_list();
						pDataObject->set_property_name(pPropertyInfo->GetKey());
						*(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetVector2());
					}

					if (pPropertyInfo->GetPrivate())
					{
						NFMsg::PropertyVector2* pDataObject = pPrivateData->add_property_vector2_list();
						pDataObject->set_property_name(pPropertyInfo->GetKey());
						*(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetVector2());
					}
				}
				break;
				case TDATA_VECTOR3:
				{
					if (pPropertyInfo->GetPublic())
					{
						NFMsg::PropertyVector3* pDataObject = pPublicData->add_property_vector3_list();
						pDataObject->set_property_name(pPropertyInfo->GetKey());
						*(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetVector3());
					}

					if (pPropertyInfo->GetPrivate())
					{
						NFMsg::PropertyVector3* pDataObject = pPrivateData->add_property_vector3_list();
						pDataObject->set_property_name(pPropertyInfo->GetKey());
						*(pDataObject->mutable_data()) = NFINetModule::NFToPB(pPropertyInfo->GetVector3());
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
				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_PROPERTY_ENTRY, xPrivateMsg, identOther);
			}
			else
			{
				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_PROPERTY_ENTRY, xPublicMsg, identOther);
			}
		}
	}

	return 0;
}

int NFAutoBroadcastModule::OnRecordEnter(const NFDataList& argVar, const NFGUID& self)
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

				NFCommonRedisModule::ConvertRecordToPB(pRecord, pPublicRecordBase);
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

				NFCommonRedisModule::ConvertRecordToPB(pRecord, pPrivateRecordBase);
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
					m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_RECORD_ENTRY, xPrivateMsg, identOther);
				}
			}
			else
			{
				if (xPublicMsg.multi_player_record_size() > 0)
				{
					m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_RECORD_ENTRY, xPublicMsg, identOther);
				}
			}
		}
	}

	return 0;
}

int NFAutoBroadcastModule::OnPropertyEvent(const NFGUID & self, const std::string & propertyName, const NFData & oldVar, const NFData & newVar, const NFDataList & argVar, const NFINT64 reason)
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
		pDataInt->set_property_name(propertyName);
		pDataInt->set_data(newVar.GetInt());
		pDataInt->set_reason(reason);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_INT, xPropertyInt, identOld);
		}
	}
	break;

	case TDATA_FLOAT:
	{
		NFMsg::ObjectPropertyFloat xPropertyFloat;
		NFMsg::Ident* pIdent = xPropertyFloat.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
		pDataFloat->set_property_name(propertyName);
		pDataFloat->set_data(newVar.GetFloat());
		pDataFloat->set_reason(reason);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_FLOAT, xPropertyFloat, identOld);
		}
	}
	break;

	case TDATA_STRING:
	{
		NFMsg::ObjectPropertyString xPropertyString;
		NFMsg::Ident* pIdent = xPropertyString.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
		pDataString->set_property_name(propertyName);
		pDataString->set_data(newVar.GetString());
		pDataString->set_reason(reason);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_STRING, xPropertyString, identOld);
		}
	}
	break;

	case TDATA_OBJECT:
	{
		NFMsg::ObjectPropertyObject xPropertyObject;
		NFMsg::Ident* pIdent = xPropertyObject.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
		pDataObject->set_property_name(propertyName);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());
		pDataObject->set_reason(reason);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_OBJECT, xPropertyObject, identOld);
		}
	}
	break;
	case TDATA_VECTOR2:
	{
		NFMsg::ObjectPropertyVector2 xPropertyVector2;
		NFMsg::Ident* pIdent = xPropertyVector2.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyVector2* pDataObject = xPropertyVector2.add_property_list();
		pDataObject->set_property_name(propertyName);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetVector2());
		pDataObject->set_reason(reason);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_VECTOR2, xPropertyVector2, identOld);
		}
	}
	break;
	case TDATA_VECTOR3:
	{
		NFMsg::ObjectPropertyVector3 xPropertyVector3;
		NFMsg::Ident* pIdent = xPropertyVector3.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		NFMsg::PropertyVector3* pDataObject = xPropertyVector3.add_property_list();
		pDataObject->set_property_name(propertyName);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetVector3());
		pDataObject->set_reason(reason);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOld = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_VECTOR3, xPropertyVector3, identOld);
		}
	}
	break;
	default:
		break;
	}

	return 0;
}

int NFAutoBroadcastModule::OnRecordEvent(const NFGUID & self, const std::string& recordName, const RECORD_EVENT_DATA & eventData, const NFData & oldVar, const NFData & newVar, const NFDataList & argVar)
{
	if (NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString(self, NFrame::Player::ClassName()))
	{
		NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
		if (xObject->GetState() != CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
		{
			return 0;
		}
	}

	switch (eventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		NFMsg::ObjectRecordAddRow xAddRecordRow;
		NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(self);

		xAddRecordRow.set_record_name(recordName);

		NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
		pAddRowData->set_row(eventData.row);


		NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, recordName);
		if (xRecord)
		{
			NFDataList rowDataList;
			if (xRecord->QueryRow(eventData.row, rowDataList))
			{
				for (int i = 0; i < rowDataList.GetCount(); i++)
				{
					switch (rowDataList.Type(i))
					{
					case TDATA_INT:
					{

						int64_t nValue = rowDataList.Int(i);

						NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
						pAddData->set_col(i);
						pAddData->set_row(eventData.row);
						pAddData->set_data(nValue);
					}
					break;
					case TDATA_FLOAT:
					{
						double fValue = rowDataList.Float(i);

						NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
						pAddData->set_col(i);
						pAddData->set_row(eventData.row);
						pAddData->set_data(fValue);
					}
					break;
					case TDATA_STRING:
					{
						const std::string& str = rowDataList.String(i);
						NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
						pAddData->set_col(i);
						pAddData->set_row(eventData.row);
						pAddData->set_data(str);
					}
					break;
					case TDATA_OBJECT:
					{
						NFGUID identValue = rowDataList.Object(i);
						NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
						pAddData->set_col(i);
						pAddData->set_row(eventData.row);

						*pAddData->mutable_data() = NFINetModule::NFToPB(identValue);
					}
					break;
					case TDATA_VECTOR2:
					{
						NFVector2 vPos = rowDataList.Vector2(i);
						NFMsg::RecordVector2* pAddData = pAddRowData->add_record_vector2_list();
						pAddData->set_col(i);
						pAddData->set_row(eventData.row);
						*pAddData->mutable_data() = NFINetModule::NFToPB(vPos);
					}
					break;
					case TDATA_VECTOR3:
					{
						NFVector3 vPos = rowDataList.Vector3(i);
						NFMsg::RecordVector3* pAddData = pAddRowData->add_record_vector3_list();
						pAddData->set_col(i);
						pAddData->set_row(eventData.row);
						*pAddData->mutable_data() = NFINetModule::NFToPB(vPos);
					}
					break;

					default:
						break;
					}
				}

				for (int i = 0; i < argVar.GetCount(); i++)
				{
					NFGUID identOther = argVar.Object(i);

					m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_ADD_ROW, xAddRecordRow, identOther);
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

		xReoveRecordRow.set_record_name(recordName);
		xReoveRecordRow.add_remove_row(eventData.row);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOther = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_REMOVE_ROW, xReoveRecordRow, identOther);
		}
	}
	break;
	case RECORD_EVENT_DATA::Swap:
	{

		NFMsg::ObjectRecordSwap xSwapRecord;
		*xSwapRecord.mutable_player_id() = NFINetModule::NFToPB(self);

		xSwapRecord.set_origin_record_name(recordName);
		xSwapRecord.set_target_record_name(recordName);
		xSwapRecord.set_row_origin(eventData.row);
		xSwapRecord.set_row_target(eventData.col);

		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOther = argVar.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_SWAP_ROW, xSwapRecord, identOther);
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

			xRecordChanged.set_record_name(recordName);
			NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(eventData.row);
			recordProperty->set_col(eventData.col);
			int64_t nData = newVar.GetInt();
			recordProperty->set_data(nData);

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_INT, xRecordChanged, identOther);
			}
		}
		break;

		case TDATA_FLOAT:
		{
			NFMsg::ObjectRecordFloat xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(recordName);
			NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(eventData.row);
			recordProperty->set_col(eventData.col);
			recordProperty->set_data(newVar.GetFloat());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_FLOAT, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_STRING:
		{
			NFMsg::ObjectRecordString xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(recordName);
			NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(eventData.row);
			recordProperty->set_col(eventData.col);
			recordProperty->set_data(newVar.GetString());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_STRING, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_OBJECT:
		{
			NFMsg::ObjectRecordObject xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(recordName);
			NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(eventData.row);
			recordProperty->set_col(eventData.col);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_OBJECT, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_VECTOR2:
		{
			NFMsg::ObjectRecordVector2 xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(recordName);
			NFMsg::RecordVector2* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(eventData.row);
			recordProperty->set_col(eventData.col);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetVector2());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_VECTOR2, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_VECTOR3:
		{
			NFMsg::ObjectRecordVector3 xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(self);

			xRecordChanged.set_record_name(recordName);
			NFMsg::RecordVector3* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(eventData.row);
			recordProperty->set_col(eventData.col);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetVector3());

			for (int i = 0; i < argVar.GetCount(); i++)
			{
				NFGUID identOther = argVar.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_VECTOR3, xRecordChanged, identOther);
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
		//             xRecordChanged.set_record_name( recordName );
		//
		//             for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
		//             {
		//                 NFGUID identOther = valueBroadCaseList.Object( i );
		//                 SendMsgPB(NFMsg::ACK_RECORD_CLEAR, xRecordChanged, 0);
		//             }
	}
	break;
	default:
		break;
	}
	return 0;
}

int NFAutoBroadcastModule::OnObjectListEnter(const NFDataList& self, const NFDataList& argVar)
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


		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_ENTRY, xPlayerEntryInfoList, ident);
	}

	return 1;
}

int NFAutoBroadcastModule::OnObjectDataFinished(const NFDataList & self, const NFDataList & argVar)
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


		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_DATA_FINISHED, xPlayerEntryInfoList, ident);
	}

	return 1;
}

int NFAutoBroadcastModule::OnObjectListLeave(const NFDataList& self, const NFDataList& argVar)
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

		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_LEAVE, xPlayerLeaveInfoList, ident);
	}

	return 1;
}
