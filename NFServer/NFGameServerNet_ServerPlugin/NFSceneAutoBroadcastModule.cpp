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


#include "NFSceneAutoBroadcastModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFSceneAutoBroadcastModule::Init()
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

bool NFSceneAutoBroadcastModule::AfterInit()
{
	m_pSceneModule->AddGroupPropertyCommCallBack(this, &NFSceneAutoBroadcastModule::OnPropertyEvent);
	m_pSceneModule->AddGroupRecordCommCallBack(this, &NFSceneAutoBroadcastModule::OnRecordEvent);

	m_pSceneModule->AddBeforeLeaveSceneGroupCallBack(this, &NFSceneAutoBroadcastModule::OnBeforeLeaveSceneEvent);
	m_pSceneModule->AddAfterEnterSceneGroupCallBack(this, &NFSceneAutoBroadcastModule::OnAfterEntrySceneEvent);
	return true;
}

bool NFSceneAutoBroadcastModule::Shut()
{

	return true;
}

bool NFSceneAutoBroadcastModule::Execute()
{
	return true;
}

int NFSceneAutoBroadcastModule::OnPropertyEvent(const NFGUID & self, const std::string & strProperty, const NFData & oldVar, const NFData & newVar)
{
	NFDataList players;
	m_pKernelModule->GetGroupObjectList(self.nHead64, self.nData64, players, true);

	switch (oldVar.GetType())
	{
	case TDATA_INT:
	{
		NFMsg::ObjectPropertyInt xPropertyInt;
		NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(NFGUID());

		NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
		pDataInt->set_property_name(strProperty);
		pDataInt->set_data(newVar.GetInt());

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOld = players.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_INT, xPropertyInt, identOld);
		}
	}
	break;

	case TDATA_FLOAT:
	{
		NFMsg::ObjectPropertyFloat xPropertyFloat;
		NFMsg::Ident* pIdent = xPropertyFloat.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(NFGUID());

		NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
		pDataFloat->set_property_name(strProperty);
		pDataFloat->set_data(newVar.GetFloat());

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOld = players.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_FLOAT, xPropertyFloat, identOld);
		}
	}
	break;

	case TDATA_STRING:
	{
		NFMsg::ObjectPropertyString xPropertyString;
		NFMsg::Ident* pIdent = xPropertyString.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(NFGUID());

		NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
		pDataString->set_property_name(strProperty);
		pDataString->set_data(newVar.GetString());

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOld = players.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_STRING, xPropertyString, identOld);
		}
	}
	break;

	case TDATA_OBJECT:
	{
		NFMsg::ObjectPropertyObject xPropertyObject;
		NFMsg::Ident* pIdent = xPropertyObject.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(NFGUID());

		NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
		pDataObject->set_property_name(strProperty);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOld = players.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_OBJECT, xPropertyObject, identOld);
		}
	}
	break;
	case TDATA_VECTOR2:
	{
		NFMsg::ObjectPropertyVector2 xPropertyVector2;
		NFMsg::Ident* pIdent = xPropertyVector2.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(NFGUID());

		NFMsg::PropertyVector2* pDataObject = xPropertyVector2.add_property_list();
		pDataObject->set_property_name(strProperty);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetVector2());

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOld = players.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_VECTOR2, xPropertyVector2, identOld);
		}
	}
	break;
	case TDATA_VECTOR3:
	{
		NFMsg::ObjectPropertyVector3 xPropertyVector3;
		NFMsg::Ident* pIdent = xPropertyVector3.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(NFGUID());

		NFMsg::PropertyVector3* pDataObject = xPropertyVector3.add_property_list();
		pDataObject->set_property_name(strProperty);
		*pDataObject->mutable_data() = NFINetModule::NFToPB(newVar.GetVector3());

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOld = players.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_VECTOR3, xPropertyVector3, identOld);
		}
	}
	break;
	default:
		break;
	}

	return 0;
}

int NFSceneAutoBroadcastModule::OnRecordEvent(const NFGUID & self, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar)
{
	NFDataList players;
	m_pKernelModule->GetGroupObjectList(self.nHead64, self.nData64, players, true);

	switch (xEventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		NFMsg::ObjectRecordAddRow xAddRecordRow;
		NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
		*pIdent = NFINetModule::NFToPB(NFGUID());

		xAddRecordRow.set_record_name(xEventData.strRecordName);

		NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
		pAddRowData->set_row(xEventData.nRow);


		NF_SHARE_PTR<NFIRecord> xRecord = m_pSceneModule->FindRecord(self.nHead64, self.nData64, xEventData.strRecordName);
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
					case TDATA_VECTOR2:
					{
						NFVector2 vPos = xRowDataList.Vector2(i);
						NFMsg::RecordVector2* pAddData = pAddRowData->add_record_vector2_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						*pAddData->mutable_data() = NFINetModule::NFToPB(vPos);
					}
					break;
					case TDATA_VECTOR3:
					{
						NFVector3 vPos = xRowDataList.Vector3(i);
						NFMsg::RecordVector3* pAddData = pAddRowData->add_record_vector3_list();
						pAddData->set_col(i);
						pAddData->set_row(xEventData.nRow);
						*pAddData->mutable_data() = NFINetModule::NFToPB(vPos);
					}
					break;

					default:
						break;
					}
				}

				for (int i = 0; i < players.GetCount(); i++)
				{
					NFGUID identOther = players.Object(i);

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
		*pIdent = NFINetModule::NFToPB(NFGUID());

		xReoveRecordRow.set_record_name(xEventData.strRecordName);
		xReoveRecordRow.add_remove_row(xEventData.nRow);

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOther = players.Object(i);

			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_REMOVE_ROW, xReoveRecordRow, identOther);
		}
	}
	break;
	case RECORD_EVENT_DATA::Swap:
	{

		NFMsg::ObjectRecordSwap xSwapRecord;
		*xSwapRecord.mutable_player_id() = NFINetModule::NFToPB(NFGUID());

		xSwapRecord.set_origin_record_name(xEventData.strRecordName);
		xSwapRecord.set_target_record_name(xEventData.strRecordName);
		xSwapRecord.set_row_origin(xEventData.nRow);
		xSwapRecord.set_row_target(xEventData.nCol);

		for (int i = 0; i < players.GetCount(); i++)
		{
			NFGUID identOther = players.Object(i);

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
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(NFGUID());

			xRecordChanged.set_record_name(xEventData.strRecordName);
			NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			int64_t nData = newVar.GetInt();
			recordProperty->set_data(nData);

			for (int i = 0; i < players.GetCount(); i++)
			{
				NFGUID identOther = players.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_INT, xRecordChanged, identOther);
			}
		}
		break;

		case TDATA_FLOAT:
		{
			NFMsg::ObjectRecordFloat xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(NFGUID());

			xRecordChanged.set_record_name(xEventData.strRecordName);
			NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			recordProperty->set_data(newVar.GetFloat());

			for (int i = 0; i < players.GetCount(); i++)
			{
				NFGUID identOther = players.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_FLOAT, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_STRING:
		{
			NFMsg::ObjectRecordString xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(NFGUID());

			xRecordChanged.set_record_name(xEventData.strRecordName);
			NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			recordProperty->set_data(newVar.GetString());

			for (int i = 0; i < players.GetCount(); i++)
			{
				NFGUID identOther = players.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_STRING, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_OBJECT:
		{
			NFMsg::ObjectRecordObject xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(NFGUID());

			xRecordChanged.set_record_name(xEventData.strRecordName);
			NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetObject());

			for (int i = 0; i < players.GetCount(); i++)
			{
				NFGUID identOther = players.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_OBJECT, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_VECTOR2:
		{
			NFMsg::ObjectRecordVector2 xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(NFGUID());

			xRecordChanged.set_record_name(xEventData.strRecordName);
			NFMsg::RecordVector2* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetVector2());

			for (int i = 0; i < players.GetCount(); i++)
			{
				NFGUID identOther = players.Object(i);

				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_VECTOR2, xRecordChanged, identOther);
			}
		}
		break;
		case TDATA_VECTOR3:
		{
			NFMsg::ObjectRecordVector3 xRecordChanged;
			*xRecordChanged.mutable_player_id() = NFINetModule::NFToPB(NFGUID());

			xRecordChanged.set_record_name(xEventData.strRecordName);
			NFMsg::RecordVector3* recordProperty = xRecordChanged.add_property_list();
			recordProperty->set_row(xEventData.nRow);
			recordProperty->set_col(xEventData.nCol);
			*recordProperty->mutable_data() = NFINetModule::NFToPB(newVar.GetVector3());

			for (int i = 0; i < players.GetCount(); i++)
			{
				NFGUID identOther = players.Object(i);

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
		//             xRecordChanged.set_record_name( strRecordName );
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

int NFSceneAutoBroadcastModule::OnBeforeLeaveSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	ClearProperty(self, nSceneID, nGroupID);
	ClearRecord(self, nSceneID, nGroupID);

	return 0;
}

int NFSceneAutoBroadcastModule::OnAfterEntrySceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	NFDataList argVar;
	argVar << self;

	OnPropertyEnter(argVar, nSceneID, nGroupID);
	OnRecordEnter(argVar, nSceneID, nGroupID);

	return 0;
}

int NFSceneAutoBroadcastModule::OnPropertyEnter(const NFDataList & argVar, const int nSceneID, const int nGroupID)
{
	NFMsg::MultiObjectPropertyList xPublicMsg;
	NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pSceneModule->FindPropertyManager(nSceneID, nGroupID);
	if (pPropertyManager)
	{
		NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();

		*(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(NFGUID(0, 0));

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
			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_PROPERTY_ENTRY, xPublicMsg, identOther);
		}
	}

	return 0;
}

bool OnSceneRecordEnterPack(NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase)
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
					NFINT64 nValue = pRecord->GetInt(i, j);
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
				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					NFVector2 vPos = pRecord->GetVector2(i, j);
					//if ( !ident.IsNull() )
					{
						NFMsg::RecordVector2* pAddData = pAddRowStruct->add_record_vector2_list();
						pAddData->set_row(i);
						pAddData->set_col(j);
						*(pAddData->mutable_data()) = NFINetModule::NFToPB(vPos);
					}
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					NFVector3 vPos = pRecord->GetVector3(i, j);
					//if ( !ident.IsNull() )
					{
						NFMsg::RecordVector3* pAddData = pAddRowStruct->add_record_vector3_list();
						pAddData->set_row(i);
						pAddData->set_col(j);
						*(pAddData->mutable_data()) = NFINetModule::NFToPB(vPos);
					}
				}
				break;

				default:
					break;
				}
			}
		}
	}
	return 0;
}

int NFSceneAutoBroadcastModule::OnRecordEnter(const NFDataList & argVar, const int nSceneID, const int nGroupID)
{
	NFMsg::MultiObjectRecordList xPublicMsg;

	NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pSceneModule->FindRecordManager(nSceneID, nGroupID);
	if (pRecordManager)
	{
		NFMsg::ObjectRecordList* pPublicData = NULL;

		NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
		while (pRecord)
		{
			if (!pRecord->GetPublic())
			{
				pRecord = pRecordManager->Next();
				continue;
			}

			NFMsg::ObjectRecordBase* pPublicRecordBase = NULL;
			if (pRecord->GetPublic())
			{
				if (!pPublicData)
				{
					pPublicData = xPublicMsg.add_multi_player_record();
					*(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(NFGUID(0, 0));
				}
				pPublicRecordBase = pPublicData->add_record_list();
				pPublicRecordBase->set_record_name(pRecord->GetName());

				OnSceneRecordEnterPack(pRecord, pPublicRecordBase);
			}

			pRecord = pRecordManager->Next();
		}


		for (int i = 0; i < argVar.GetCount(); i++)
		{
			NFGUID identOther = argVar.Object(i);
			if (xPublicMsg.multi_player_record_size() > 0)
			{
				m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_OBJECT_RECORD_ENTRY, xPublicMsg, identOther);
			}
		}
	}

	return 0;
}

int NFSceneAutoBroadcastModule::ClearProperty(const NFGUID & self, const int nSceneID, const int nGroupID)
{
	NFMsg::MultiObjectPropertyList xPublicMsg;
	NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pSceneModule->FindPropertyManager(nSceneID, nGroupID);
	if (pPropertyManager)
	{
		NFMsg::ObjectPropertyList* pPublicData = xPublicMsg.add_multi_player_property();

		*(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(NFGUID(0, 0));


		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_PROPERTY_CLEAR, xPublicMsg, self);
	}

	return 0;
}

int NFSceneAutoBroadcastModule::ClearRecord(const NFGUID & self, const int nSceneID, const int nGroupID)
{
	NFMsg::MultiObjectRecordList xPublicMsg;
	NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pSceneModule->FindRecordManager(nSceneID, nGroupID);
	if (pRecordManager)
	{
		NFMsg::ObjectRecordList* pPublicData = xPublicMsg.add_multi_player_record();
		*(pPublicData->mutable_player_id()) = NFINetModule::NFToPB(NFGUID(0, 0));

		NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First();
		while (pRecord)
		{
			if (!pRecord->GetPublic())
			{
				pRecord = pRecordManager->Next();
				continue;
			}

			if (pRecord->GetPublic())
			{
				NFMsg::ObjectRecordBase* pPublicRecordBase = pPublicData->add_record_list();
				pPublicRecordBase->set_record_name(pRecord->GetName());
			}

			pRecord = pRecordManager->Next();
		}


		if (xPublicMsg.multi_player_record_size() > 0)
		{
			m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_RECORD_CLEAR, xPublicMsg, self);
		}
	}

	return 0;
}