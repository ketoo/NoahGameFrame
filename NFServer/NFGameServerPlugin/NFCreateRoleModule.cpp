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

#include "NFCreateRoleModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCreateRoleModule::Init()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pGameToDBModule = pPluginManager->FindModule<NFIGameServerToDBModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pDataTailModule = pPluginManager->FindModule<NFIDataTailModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();

    return true;
}

bool NFCreateRoleModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCreateRoleModule::OnObjectPlayerEvent);

	return true;
}

bool NFCreateRoleModule::ReadyExecute()
{
	m_pNetModule->RemoveReceiveCallBack(NFMsg::REQ_ROLE_LIST);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::REQ_CREATE_ROLE);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::REQ_DELETE_ROLE);
	m_pNetModule->RemoveReceiveCallBack(NFMsg::REQ_ENTER_GAME);


	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_ROLE_LIST, this, &NFCreateRoleModule::OnRequireRoleListProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_CREATE_ROLE, this, &NFCreateRoleModule::OnCreateRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_DELETE_ROLE, this, &NFCreateRoleModule::OnDeleteRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_ENTER_GAME, this, &NFCreateRoleModule::OnClientEnterGameProcess);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_DB, NFMsg::ACK_ROLE_LIST, this, &NFCreateRoleModule::OnResponseRoleListProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_DB, NFMsg::ACK_LOAD_ROLE_DATA, this, &NFCreateRoleModule::OnDBLoadRoleDataProcess);

	return true;
}

void NFCreateRoleModule::OnRequireRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, nMsgID, std::string(msg, nLen));
}

void NFCreateRoleModule::OnResponseRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::AckRoleLiteInfoList xData;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xData, nClientID))
	{
		return;
	}

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> xGateInfo = m_pGameServerNet_ServerModule->GetGateServerInfo(nClientID.GetHead());
	if (xGateInfo)
	{
		m_pNetModule->SendMsgWithOutHead (nMsgID, std::string(msg, nLen), xGateInfo->xServerData.nFD);
	}
}

void NFCreateRoleModule::OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, nMsgID, std::string(msg, nLen));
}

void NFCreateRoleModule::OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, nMsgID, std::string(msg, nLen));
}

void NFCreateRoleModule::OnClientEnterGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
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
		m_pKernelModule->DestroyObject(nRoleID);
	}

	//////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo>  pGateInfo = m_pGameServerNet_ServerModule->GetPlayerGateInfo(nRoleID);
	if (nullptr != pGateInfo)
	{
		if (pGateInfo->eStatus == NFIGameServerNet_ServerModule::GateBaseInfo::E_LOADING)
		{
			return;
		}

		m_pGameServerNet_ServerModule->RemovePlayerGateInfo(nRoleID);
	}

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> pGateServerinfo = m_pGameServerNet_ServerModule->GetGateServerInfoBySockIndex(nSockIndex);
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

	if (!m_pGameServerNet_ServerModule->AddPlayerGateInfo(nRoleID, nClientID, nGateID))
	{
		return;
	}

	m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_DB, nSockIndex, NFMsg::REQ_LOAD_ROLE_DATA, std::string(msg, nLen));

}

void NFCreateRoleModule::OnDBLoadRoleDataProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::RoleDataPack xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nClientID))
	{
		//releasing all the resource that allow when the user login, then kick off the user
		// TODO

		//m_pGameServerNet_ServerModule->RemovePlayerGateInfo();

		//Avtually, the developer may not know the user id in this situation, therefore the developer must record the login-time when the user coming
		//and check the time per min to kick off the user who are not active.

		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());
	if (m_pKernelModule->GetObject(nRoleID))
	{
		//it should be rebind with proxy's netobject
		m_pKernelModule->DestroyObject(nRoleID);
	}

	mxObjectDataCache[nRoleID] = xMsg;

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo>  pGateInfo = m_pGameServerNet_ServerModule->GetPlayerGateInfo(nRoleID);
	if (nullptr != pGateInfo)
	{
		if (pGateInfo->eStatus == NFIGameServerNet_ServerModule::GateBaseInfo::E_LOADING)
		{
			pGateInfo->eStatus = NFIGameServerNet_ServerModule::GateBaseInfo::E_LOADED;
		}


		NFDataList var;

		var.AddString(NFrame::Player::GateID());
		var.AddInt(pGateInfo->nGateID);

		var.AddString(NFrame::Player::GameID());
		var.AddInt(pPluginManager->GetAppID());

		var.AddString(NFrame::Player::Connection());
		var.AddInt(1);

		/*
		var.AddString(NFrame::Player::HomeSceneID());
		var.AddInt(1);

		var.AddString(NFrame::Player::SceneID());
		var.AddInt(1);
		*/

		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, 1, 0, NFrame::Player::ThisName(), "", var);
		if (nullptr == pObject)
		{
			//mRoleBaseData
			//mRoleFDData
			mxObjectDataCache.erase(nRoleID);
			return;
		}

		/////////////////////////////
		const int group = m_pKernelModule->GetPropertyInt(pObject->Self(), NFrame::IObject::GroupID());
		if (group <= 0)
		{
			/////////////////////////////
			//sometimes, the player might disconnected from game server and want to reconnect.
			//Basic on this reason, developer could move this kinds of players into the specific scene or group to avoid players move to the default scene.
			//If developers move that kinds of players into the specific scene or group, which means the group value will NOT ZERO!
			//COE_CREATE_FINISH

			/////////////////////////////
			const int nHomeSceneID = 1;
			const NFVector3& pos = m_pSceneModule->GetRelivePosition(nHomeSceneID, 0);
			m_pSceneProcessModule->RequestEnterScene(pObject->Self(), nHomeSceneID, -1, 0, pos, NFDataList::Empty());
		}
	}
}

int NFCreateRoleModule::OnObjectPlayerEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		//m_pDataTailModule->LogObjectData(self);

		SaveData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
		//m_pDataTailModule->StartTrail(self);
		//m_pDataTailModule->LogObjectData(self);

		AttachData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		auto it = mxObjectDataCache.find(self);
		if (it != mxObjectDataCache.end())
		{
			mxObjectDataCache.erase(it);
		}


		m_pScheduleModule->AddSchedule(self, "SaveDataOnTime", this, &NFCreateRoleModule::SaveDataOnTime, 180.0f, -1);
	}

	return 0;
}

void NFCreateRoleModule::AttachData(const NFGUID & self)
{
	auto it = mxObjectDataCache.find(self);
	if (it != mxObjectDataCache.end())
	{
		NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
		if (xObject)
		{
			NF_SHARE_PTR<NFIPropertyManager> xPropManager = xObject->GetPropertyManager();
			NF_SHARE_PTR<NFIRecordManager> xRecordManager = xObject->GetRecordManager();

			if (xPropManager)
			{
				ConvertPBToPropertyManager(it->second.property(), xPropManager);
			}

			if (xRecordManager)
			{
				ConvertPBToRecordManager(it->second.record(), xRecordManager);
			}

			mxObjectDataCache.erase(it);

		}
	}
}

void NFCreateRoleModule::SaveData(const NFGUID & self)
{
	NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
	if (xObject)
	{
		NF_SHARE_PTR<NFIPropertyManager> xPropManager = xObject->GetPropertyManager();
		NF_SHARE_PTR<NFIRecordManager> xRecordManager = xObject->GetRecordManager();
		NFMsg::RoleDataPack xDataPack;

		*xDataPack.mutable_id() = NFINetModule::NFToPB(self);

		*(xDataPack.mutable_property()->mutable_player_id()) = NFINetModule::NFToPB(self);
		*(xDataPack.mutable_record()->mutable_player_id()) = NFINetModule::NFToPB(self);

		if (xPropManager)
		{
			ConvertPropertyManagerToPB(xPropManager, xDataPack.mutable_property());
		}

		if (xRecordManager)
		{
			ConvertRecordManagerToPB(xRecordManager, xDataPack.mutable_record());
		}

		m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_DB, self.GetData(), NFMsg::REQ_SAVE_ROLE_DATA, xDataPack);
	}
}

int NFCreateRoleModule::SaveDataOnTime(const NFGUID & self, const std::string & name, const float fIntervalTime, const int nCount)
{
	SaveData(self);
	return 0;
}

bool NFCreateRoleModule::Shut()
{
    return true;
}

bool NFCreateRoleModule::Execute()
{
    return true;
}


bool NFCreateRoleModule::ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase * pRecordData)
{
	pRecordData->set_record_name(pRecord->GetName());

	for (int iRow = 0; iRow < pRecord->GetRows(); iRow++)
	{
		if (!pRecord->IsUsed(iRow))
		{
			continue;
		}

		NFMsg::RecordAddRowStruct* pRowData = pRecordData->add_row_struct();
		if (!pRowData)
		{
			continue;
		}

		pRowData->set_row(iRow);

		for (int iCol = 0; iCol < pRecord->GetCols(); iCol++)
		{
			const int nType = pRecord->GetColType(iCol);
			switch (nType)
			{
			case TDATA_INT:
			{
				NFMsg::RecordInt* pPropertyData = pRowData->add_record_int_list();
				const NFINT64 xPropertyValue = pRecord->GetInt(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);
				}
			}
			break;
			case TDATA_FLOAT:
			{
				NFMsg::RecordFloat* pPropertyData = pRowData->add_record_float_list();
				const double xPropertyValue = pRecord->GetFloat(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);
				}
			}
			break;
			case TDATA_STRING:
			{
				NFMsg::RecordString* pPropertyData = pRowData->add_record_string_list();
				const std::string& xPropertyValue = pRecord->GetString(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);

				}
			}
			break;
			case TDATA_OBJECT:
			{
				NFMsg::RecordObject* pPropertyData = pRowData->add_record_object_list();
				const NFGUID xPropertyValue = pRecord->GetObject(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					*pPropertyData->mutable_data() = NFINetModule::NFToPB(xPropertyValue);
				}
			}
			break;
			case TDATA_VECTOR2:
			{
				NFMsg::RecordVector2* pPropertyData = pRowData->add_record_vector2_list();
				const NFVector2 xPropertyValue = pRecord->GetVector2(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector2* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
				}
			}
			break;
			case TDATA_VECTOR3:
			{
				NFMsg::RecordVector3* pPropertyData = pRowData->add_record_vector3_list();
				const NFVector3 xPropertyValue = pRecord->GetVector3(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector3* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
					pVec->set_z(xPropertyValue.Z());
				}
			}
			break;
			default:
				break;
			}
		}
	}

	return true;
}

bool NFCreateRoleModule::ConvertPBToRecord(const NFMsg::ObjectRecordBase& pRecordData, NF_SHARE_PTR<NFIRecord> pRecord)
{
	for (int iStuct = 0; iStuct < pRecordData.row_struct_size(); iStuct++)
	{
		const NFMsg::RecordAddRowStruct& xAddRowStruct = pRecordData.row_struct(iStuct);

		const int nCommonRow = xAddRowStruct.row();
		pRecord->SetUsed(nCommonRow, true);
		pRecord->PreAllocMemoryForRow(nCommonRow);

		for (int i = 0; i < xAddRowStruct.record_int_list_size(); i++)
		{
			const NFMsg::RecordInt& xPropertyData = xAddRowStruct.record_int_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFINT64 xPropertyValue = xPropertyData.data();

			pRecord->SetInt(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_float_list_size(); i++)
		{
			const NFMsg::RecordFloat& xPropertyData = xAddRowStruct.record_float_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const float xPropertyValue = xPropertyData.data();

			pRecord->SetFloat(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_string_list_size(); i++)
		{
			const NFMsg::RecordString& xPropertyData = xAddRowStruct.record_string_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const std::string& xPropertyValue = xPropertyData.data();

			pRecord->SetString(nRow, nCol, xPropertyValue.c_str());
		}

		for (int i = 0; i < xAddRowStruct.record_object_list_size(); i++)
		{
			const NFMsg::RecordObject& xPropertyData = xAddRowStruct.record_object_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFGUID xPropertyValue = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetObject(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_vector2_list_size(); i++)
		{
			const NFMsg::RecordVector2& xPropertyData = xAddRowStruct.record_vector2_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector2 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector2(nRow, nCol, v);
		}

		for (int i = 0; i < xAddRowStruct.record_vector3_list_size(); i++)
		{
			const NFMsg::RecordVector3& xPropertyData = xAddRowStruct.record_vector3_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector3 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector3(nRow, nCol, v);
		}
	}

	return false;
}

bool NFCreateRoleModule::ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecordManager, NFMsg::ObjectRecordList * pRecordDataList)
{
	if (pRecordDataList == nullptr)
	{
		return false;
	}

	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if (!pRecord->GetCache() && !pRecord->GetSave())
		{
			continue;
		}

		NFMsg::ObjectRecordBase* pRecordData = pRecordDataList->add_record_list();
		if (!pRecordData)
		{
			continue;
		}

		ConvertRecordToPB(pRecord, pRecordData);
	}

	return true;
}

bool NFCreateRoleModule::ConvertPBToRecordManager(const NFMsg::ObjectRecordList & pRecordData, NF_SHARE_PTR<NFIRecordManager> pRecord)
{
	if (pRecord == nullptr)
	{
		return false;
	}

	for (int i = 0; i < pRecordData.record_list_size(); ++i)
	{
		const NFMsg::ObjectRecordBase& xRecordBase = pRecordData.record_list(i);
		NF_SHARE_PTR<NFIRecord> xRecord = pRecord->GetElement(xRecordBase.record_name());
		if (xRecord)
		{
			ConvertPBToRecord(xRecordBase, xRecord);
		}
	}

	return true;
}

bool NFCreateRoleModule::ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pProps, NFMsg::ObjectPropertyList * pPropertyData)
{
	if (pProps)
	{
		NF_SHARE_PTR<NFIProperty> xPropert = pProps->First();
		while (xPropert)
		{
			if (xPropert->GetCache() || xPropert->GetSave())
			{

				switch (xPropert->GetType())
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					NFMsg::PropertyInt* pData = pPropertyData->add_property_int_list();
					pData->set_property_name(xPropert->GetKey());
					pData->set_data(xPropert->GetInt());
				}
				break;

				case NFDATA_TYPE::TDATA_FLOAT:
				{
					NFMsg::PropertyFloat* pData = pPropertyData->add_property_float_list();
					pData->set_property_name(xPropert->GetKey());
					pData->set_data(xPropert->GetFloat());
				}
				break;

				case NFDATA_TYPE::TDATA_OBJECT:
				{
					NFMsg::PropertyObject* pData = pPropertyData->add_property_object_list();
					pData->set_property_name(xPropert->GetKey());
					*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetObject());
				}
				break;

				case NFDATA_TYPE::TDATA_STRING:
				{
					NFMsg::PropertyString* pData = pPropertyData->add_property_string_list();
					pData->set_property_name(xPropert->GetKey());
					pData->set_data(xPropert->GetString());
				}
				break;

				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					NFMsg::PropertyVector2* pData = pPropertyData->add_property_vector2_list();
					pData->set_property_name(xPropert->GetKey());
					*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetVector2());
				}
				break;

				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					NFMsg::PropertyVector3* pData = pPropertyData->add_property_vector3_list();
					pData->set_property_name(xPropert->GetKey());
					*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetVector3());
				}
				break;
				default:
					break;
				}
			}

			xPropert = pProps->Next();
		}
	}

	return false;
}

bool NFCreateRoleModule::ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& pPropertyData, NF_SHARE_PTR<NFIPropertyManager> pProps)
{
	if (pProps)
	{
		for (int i = 0; i < pPropertyData.property_int_list_size(); ++i)
		{
			const NFMsg::PropertyInt& xData = pPropertyData.property_int_list(i);

			if (!pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_INT);
			}

			pProps->SetPropertyInt(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_float_list_size(); ++i)
		{
			const NFMsg::PropertyFloat& xData = pPropertyData.property_float_list(i);

			if (!pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_FLOAT);
			}

			pProps->SetPropertyFloat(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_string_list_size(); ++i)
		{
			const NFMsg::PropertyString& xData = pPropertyData.property_string_list(i);

			if (!pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_STRING);
			}

			pProps->SetPropertyString(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_object_list_size(); ++i)
		{
			const NFMsg::PropertyObject& xData = pPropertyData.property_object_list(i);

			if (!pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_OBJECT);
			}

			pProps->SetPropertyObject(xData.property_name(), NFGUID(xData.data().svrid(), xData.data().index()));
		}

		for (int i = 0; i < pPropertyData.property_vector2_list_size(); ++i)
		{
			const NFMsg::PropertyVector2& xData = pPropertyData.property_vector2_list(i);

			if (!pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_VECTOR2);
			}

			pProps->SetPropertyVector2(xData.property_name(), NFVector2(xData.data().x(), xData.data().y()));
		}

		for (int i = 0; i < pPropertyData.property_vector3_list_size(); ++i)
		{
			const NFMsg::PropertyVector3& xData = pPropertyData.property_vector3_list(i);

			if (!pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_VECTOR3);
			}

			pProps->SetPropertyVector3(xData.property_name(), NFVector3(xData.data().x(), xData.data().y(), xData.data().z()));
		}
	}

	return true;
}
