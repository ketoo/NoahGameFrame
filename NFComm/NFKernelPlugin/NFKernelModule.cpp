/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "NFKernelModule.h"
#include "NFSceneModule.h"
#include "NFComm/NFCore/NFMemManager.hpp"
#include "NFComm/NFCore/NFObject.h"
#include "NFComm/NFCore/NFRecord.h"
#include "NFComm/NFCore/NFPerformance.hpp"
#include "NFComm/NFCore/NFPropertyManager.h"
#include "NFComm/NFCore/NFRecordManager.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFKernelModule::NFKernelModule(NFIPluginManager* p)
{
	nGUIDIndex = 0;
	nLastTime = 0;

	pPluginManager = p;

	nLastTime = pPluginManager->GetNowTime();
	InitRandom();
}

NFKernelModule::~NFKernelModule()
{
	ClearAll();
}

void NFKernelModule::InitRandom()
{
	mvRandom.clear();

	constexpr int nRandomMax = 100000;
	mvRandom.reserve(nRandomMax);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1.0f);

	for (int i = 0; i < nRandomMax; i++)
	{
		mvRandom.emplace_back((float)dis(gen));
	}

	mxRandomItor = mvRandom.cbegin();
}

bool NFKernelModule::Init()
{
	mtDeleteSelfList.clear();

	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pCellModule = pPluginManager->FindModule<NFICellModule>();
	
	return true;
}

bool NFKernelModule::Shut()
{
	return true;
}

bool NFKernelModule::Execute()
{
	ProcessMemFree();

	mnCurExeObject.nHead64 = 0;
	mnCurExeObject.nData64 = 0;

	if (mtDeleteSelfList.size() > 0)
	{
		std::list<NFGUID>::iterator it = mtDeleteSelfList.begin();
		for (; it != mtDeleteSelfList.end(); it++)
		{
			DestroyObject(*it);
		}
		mtDeleteSelfList.clear();
	}

	m_pSceneModule->Execute();

	NF_SHARE_PTR<NFIObject> pObject = First();
	while (pObject)
	{
		mnCurExeObject = pObject->Self();
		pObject->Execute();

		mnCurExeObject.nHead64 = 0;
		mnCurExeObject.nData64 = 0;

		pObject = Next();
	}

	return true;
}

NF_SHARE_PTR<NFIObject> NFKernelModule::CreateObject(const NFGUID& self, const int nSceneID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFDataList& arg)
{
	NF_SHARE_PTR<NFIObject> pObject;
	NFGUID ident = self;

	NF_SHARE_PTR<NFSceneInfo> pContainerInfo = m_pSceneModule->GetElement(nSceneID);
	if (!pContainerInfo)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSceneID), "There is no scene", nSceneID, __FUNCTION__, __LINE__);
		return pObject;
	}

	if (!pContainerInfo->GetElement(nGroupID))
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSceneID), "There is no group", nGroupID, __FUNCTION__, __LINE__);
		return pObject;
	}

	//  if (!m_pElementModule->ExistElement(strConfigIndex))
	//  {
	//      m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSceneID), "There is no group", nGroupID, __FUNCTION__, __LINE__);
	//      return pObject;
	//  }


	if (ident.IsNull())
	{
		ident = CreateGUID();
	}

	if (GetElement(ident))
	{
		m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, ident, "The object has Exists", __FUNCTION__, __LINE__);
		return pObject;
	}

	NF_SHARE_PTR<NFIPropertyManager> pStaticClassPropertyManager = m_pClassModule->GetClassPropertyManager(strClassName);
	NF_SHARE_PTR<NFIRecordManager> pStaticClassRecordManager = m_pClassModule->GetClassRecordManager(strClassName);
	if (pStaticClassPropertyManager && pStaticClassRecordManager)
	{

		pObject = NF_SHARE_PTR<NFIObject>(NF_NEW NFObject(ident, pPluginManager));

		AddElement(ident, pObject);
		pContainerInfo->AddObjectToGroup(nGroupID, ident, strClassName == NFrame::Player::ThisName() ? true : false);

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();


		NF_SHARE_PTR<NFIProperty> pStaticConfigPropertyInfo = pStaticClassPropertyManager->First();
		while (pStaticConfigPropertyInfo)
		{
			NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->AddProperty(ident, pStaticConfigPropertyInfo->GetKey(), pStaticConfigPropertyInfo->GetType());

			xProperty->SetPublic(pStaticConfigPropertyInfo->GetPublic());
			xProperty->SetPrivate(pStaticConfigPropertyInfo->GetPrivate());
			xProperty->SetSave(pStaticConfigPropertyInfo->GetSave());
			xProperty->SetCache(pStaticConfigPropertyInfo->GetCache());
			xProperty->SetRef(pStaticConfigPropertyInfo->GetRef());
			xProperty->SetUpload(pStaticConfigPropertyInfo->GetUpload());


			pObject->AddPropertyCallBack(pStaticConfigPropertyInfo->GetKey(), this, &NFKernelModule::OnPropertyCommonEvent);

			pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
		}

		NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
		while (pConfigRecordInfo)
		{
			NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->AddRecord(ident,
				pConfigRecordInfo->GetName(),
				pConfigRecordInfo->GetInitData(),
				pConfigRecordInfo->GetTag(),
				pConfigRecordInfo->GetRows());

			xRecord->SetPublic(pConfigRecordInfo->GetPublic());
			xRecord->SetPrivate(pConfigRecordInfo->GetPrivate());
			xRecord->SetSave(pConfigRecordInfo->GetSave());
			xRecord->SetCache(pConfigRecordInfo->GetCache());
			xRecord->SetUpload(pConfigRecordInfo->GetUpload());

			pObject->AddRecordCallBack(pConfigRecordInfo->GetName(), this, &NFKernelModule::OnRecordCommonEvent);

			pConfigRecordInfo = pStaticClassRecordManager->Next();
		}

		NFVector3 vRelivePos = m_pSceneModule->GetRelivePosition(nSceneID, 0);

		pObject->SetPropertyObject(NFrame::IObject::ID(), self);
		pObject->SetPropertyString(NFrame::IObject::ConfigID(), strConfigIndex);
		pObject->SetPropertyString(NFrame::IObject::ClassName(), strClassName);
		pObject->SetPropertyInt(NFrame::IObject::SceneID(), nSceneID);
		pObject->SetPropertyInt(NFrame::IObject::GroupID(), nGroupID);
		pObject->SetPropertyVector3(NFrame::IObject::Position(), vRelivePos);

		//no data
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		//////////////////////////////////////////////////////////////////////////

		NF_SHARE_PTR<NFIPropertyManager> pConfigPropertyManager = m_pElementModule->GetPropertyManager(strConfigIndex);
		NF_SHARE_PTR<NFIRecordManager> pConfigRecordManager = m_pElementModule->GetRecordManager(strConfigIndex);

		if (pConfigPropertyManager && pConfigRecordManager)
		{
			NF_SHARE_PTR<NFIProperty> pConfigPropertyInfo = pConfigPropertyManager->First();
			while (nullptr != pConfigPropertyInfo)
			{
				if (pConfigPropertyInfo->Changed())
				{
					pPropertyManager->SetProperty(pConfigPropertyInfo->GetKey(), pConfigPropertyInfo->GetValue());
				}

				pConfigPropertyInfo = pConfigPropertyManager->Next();
			}
		}

		for (int i = 0; i < arg.GetCount() - 1; i += 2)
		{
			const std::string& strPropertyName = arg.String(i);
			if (NFrame::IObject::ConfigID() != strPropertyName
				&& NFrame::IObject::ClassName() != strPropertyName
				&& NFrame::IObject::SceneID() != strPropertyName
				&& NFrame::IObject::ID() != strPropertyName
				&& NFrame::IObject::GroupID() != strPropertyName)
			{
				NF_SHARE_PTR<NFIProperty> pArgProperty = pStaticClassPropertyManager->GetElement(strPropertyName);
				if (pArgProperty)
				{
					switch (pArgProperty->GetType())
					{
					case TDATA_INT:
						pObject->SetPropertyInt(strPropertyName, arg.Int(i + 1));
						break;
					case TDATA_FLOAT:
						pObject->SetPropertyFloat(strPropertyName, arg.Float(i + 1));
						break;
					case TDATA_STRING:
						pObject->SetPropertyString(strPropertyName, arg.String(i + 1));
						break;
					case TDATA_OBJECT:
						pObject->SetPropertyObject(strPropertyName, arg.Object(i + 1));
						break;
					case TDATA_VECTOR2:
						pObject->SetPropertyVector2(strPropertyName, arg.Vector2(i + 1));
						break;
					case TDATA_VECTOR3:
						pObject->SetPropertyVector3(strPropertyName, arg.Vector3(i + 1));
						break;
					default:
						break;
					}
				}
			}
		}

		std::ostringstream stream;
		stream << " create object: " << ident.ToString();
		stream << " config_name: " << strConfigIndex;
		stream << " scene_id: " << nSceneID;
		stream << " group_id: " << nGroupID;
		stream << " position: " << pObject->GetPropertyVector3(NFrame::IObject::Position()).ToString();

		m_pLogModule->LogInfo(stream);

		pObject->SetState(COE_CREATE_BEFORE_ATTACHDATA);
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		pObject->SetState(COE_CREATE_LOADDATA);
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		pObject->SetState(COE_CREATE_AFTER_ATTACHDATA);
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		pObject->SetState(COE_CREATE_BEFORE_EFFECT);
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		pObject->SetState(COE_CREATE_EFFECTDATA);
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		pObject->SetState(COE_CREATE_AFTER_EFFECT);
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		pObject->SetState(COE_CREATE_HASDATA);
		DoEvent(ident, strClassName, pObject->GetState(), arg);

		pObject->SetState(COE_CREATE_FINISH);
		DoEvent(ident, strClassName, pObject->GetState(), arg);
	}

	return pObject;
}

bool NFKernelModule::DestroyObject(const NFGUID& self)
{
	if (self == mnCurExeObject
		&& !self.IsNull())
	{

		return DestroySelf(self);
	}

	const int nSceneID = GetPropertyInt32(self, NFrame::IObject::SceneID());

	NF_SHARE_PTR<NFSceneInfo> pContainerInfo = m_pSceneModule->GetElement(nSceneID);
	if (pContainerInfo)
	{
		const std::string& strClassName = GetPropertyString(self, NFrame::IObject::ClassName());
		if (strClassName == NFrame::Player::ThisName())
		{
			m_pSceneModule->LeaveSceneGroup(self);
		}

		DoEvent(self, strClassName, COE_BEFOREDESTROY, NFDataList::Empty());
		DoEvent(self, strClassName, COE_DESTROY, NFDataList::Empty());

		RemoveElement(self);

		m_pEventModule->RemoveEventCallBack(self);
		m_pScheduleModule->RemoveSchedule(self);

		return true;

	}

	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "There is no scene", nSceneID, __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::FindProperty(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->FindProperty(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetPropertyInt(const NFGUID& self, const std::string& strPropertyName, const NFINT64 nValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyInt(strPropertyName, nValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetPropertyFloat(const NFGUID& self, const std::string& strPropertyName, const double dValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyFloat(strPropertyName, dValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetPropertyString(const NFGUID& self, const std::string& strPropertyName, const std::string& strValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyString(strPropertyName, strValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetPropertyObject(const NFGUID& self, const std::string& strPropertyName, const NFGUID& objectValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyObject(strPropertyName, objectValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetPropertyVector2(const NFGUID& self, const std::string& strPropertyName, const NFVector2& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyVector2(strPropertyName, value);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector2", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetPropertyVector3(const NFGUID& self, const std::string& strPropertyName, const NFVector3& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyVector3(strPropertyName, value);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector3", __FUNCTION__, __LINE__);

	return false;
}

NFINT64 NFKernelModule::GetPropertyInt(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyInt(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_INT;
}

int NFKernelModule::GetPropertyInt32(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyInt32(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return (int)NULL_INT;
}

double NFKernelModule::GetPropertyFloat(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyFloat(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_FLOAT;
}

const std::string& NFKernelModule::GetPropertyString(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyString(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

const NFGUID& NFKernelModule::GetPropertyObject(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyObject(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_OBJECT;
}

const NFVector2& NFKernelModule::GetPropertyVector2(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyVector2(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector2", __FUNCTION__, __LINE__);

	return NULL_VECTOR2;
}

const NFVector3& NFKernelModule::GetPropertyVector3(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyVector3(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector3", __FUNCTION__, __LINE__);

	return NULL_VECTOR3;
}

NF_SHARE_PTR<NFIRecord> NFKernelModule::FindRecord(const NFGUID& self, const std::string& strRecordName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordManager()->GetElement(strRecordName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

	return nullptr;
}

bool NFKernelModule::ClearRecord(const NFGUID& self, const std::string& strRecordName)
{
	NF_SHARE_PTR<NFIRecord> pRecord = FindRecord(self, strRecordName);
	if (pRecord)
	{
		return pRecord->Clear();
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no record", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordInt(strRecordName, nRow, nCol, nValue))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error for row or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordInt(strRecordName, nRow, strColTag, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error for row or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordFloat(strRecordName, nRow, nCol, dwValue))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordFloat for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordFloat(strRecordName, nRow, strColTag, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordFloat for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordString(strRecordName, nRow, nCol, strValue))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordString for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordString(strRecordName, nRow, strColTag, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordObject for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordObject(strRecordName, nRow, nCol, objectValue))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordObject for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordObject(strRecordName, nRow, strColTag, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordObject for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordVector2(strRecordName, nRow, nCol, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordVector2 for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector2", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordVector2(strRecordName, nRow, strColTag, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordVector2 for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector2", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordVector3(strRecordName, nRow, nCol, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordVector3 for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector3", __FUNCTION__, __LINE__);

	return false;
}

bool NFKernelModule::SetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		if (!pObject->SetRecordVector3(strRecordName, nRow, strColTag, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordVector3 for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector3", __FUNCTION__, __LINE__);

	return false;
}

NFINT64 NFKernelModule::GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordInt(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0;
}

NFINT64 NFKernelModule::GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordInt(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0;
}

double NFKernelModule::GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordFloat(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0;
}

double NFKernelModule::GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordFloat(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0;
}

const std::string& NFKernelModule::GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordString(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

const std::string& NFKernelModule::GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordString(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

const NFGUID& NFKernelModule::GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordObject(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_OBJECT;
}

const NFGUID& NFKernelModule::GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordObject(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_OBJECT;
}

const NFVector2& NFKernelModule::GetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector2(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector2", __FUNCTION__, __LINE__);

	return NULL_VECTOR2;
}

const NFVector2& NFKernelModule::GetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector2(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector2", __FUNCTION__, __LINE__);

	return NULL_VECTOR2;
}

const NFVector3& NFKernelModule::GetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector3(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector3", __FUNCTION__, __LINE__);

	return NULL_VECTOR3;
}

const NFVector3& NFKernelModule::GetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector3(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector3", __FUNCTION__, __LINE__);

	return NULL_VECTOR3;
}

NFGUID NFKernelModule::CreateGUID()
{
	int64_t value = 0;
	uint64_t time = NFGetTimeMS();


	//value = time << 16;
	value = time * 1000000;


	//value |= nGUIDIndex++;
	value += nGUIDIndex++;

	//if (sequence_ == 0x7FFF)
	if (nGUIDIndex == 999999)
	{
		nGUIDIndex = 0;
	}

	NFGUID xID;
	xID.nHead64 = pPluginManager->GetAppID();
	xID.nData64 = value;

	return xID;
}

bool NFKernelModule::CreateScene(const int nSceneID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{
		return false;
	}

	pSceneInfo = NF_SHARE_PTR<NFSceneInfo>(NF_NEW NFSceneInfo(nSceneID));
	if (pSceneInfo)
	{
		m_pSceneModule->AddElement(nSceneID, pSceneInfo);
		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager(NF_NEW NFPropertyManager(NFGUID(nSceneID, 0)));
		NF_SHARE_PTR<NFIRecordManager> pRecordManager(NF_NEW NFRecordManager(NFGUID(nSceneID, 0)));
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = NF_SHARE_PTR<NFSceneGroupInfo>(NF_NEW NFSceneGroupInfo(nSceneID, 0, pPropertyManager, pRecordManager));
		if (NULL != pGroupInfo)
		{
			pSceneInfo->AddElement(0, pGroupInfo);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Create scene success, groupId:0, scene id:", nSceneID, __FUNCTION__, __LINE__);

			return true;
		}
	}

	return false;
}

bool NFKernelModule::DestroyScene(const int nSceneID)
{
	m_pSceneModule->RemoveElement(nSceneID);

	return true;
}

int NFKernelModule::GetOnLineCount()
{
	int nCount = 0;
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->First();
	while (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->First();
		while (pGroupInfo)
		{
			nCount += pGroupInfo->mxPlayerList.Count();
			pGroupInfo = pSceneInfo->Next();
		}

		pSceneInfo = m_pSceneModule->Next();
	}

	return nCount;
}

int NFKernelModule::GetMaxOnLineCount()
{
	// test count 5000
	// and it should be define in a xml file

	return 10000;
}

int NFKernelModule::RequestGroupScene(const int nSceneID)
{
	return m_pSceneModule->RequestGroupScene(nSceneID);
}

bool NFKernelModule::ReleaseGroupScene(const int nSceneID, const int nGroupID)
{
	return m_pSceneModule->ReleaseGroupScene(nSceneID, nGroupID);
}

bool NFKernelModule::ExitGroupScene(const int nSceneID, const int nGroupID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
		if (pGroupInfo)
		{
			return true;
		}
	}

	return false;
}

bool NFKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList & list, const NFGUID & noSelf)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{

		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
		if (pGroupInfo)
		{
			NFGUID ident = NFGUID();
			NF_SHARE_PTR<int> pRet = pGroupInfo->mxPlayerList.First(ident);
			while (!ident.IsNull())
			{
				if (ident != noSelf)
				{
					list.Add(ident);
				}

				ident = NFGUID();
				pRet = pGroupInfo->mxPlayerList.Next(ident);
			}

			ident = NFGUID();
			pRet = pGroupInfo->mxOtherList.First(ident);
			while (!ident.IsNull())
			{
				if (ident != noSelf)
				{
					list.Add(ident);
				}

				ident = NFGUID();
				pRet = pGroupInfo->mxOtherList.Next(ident);
			}

			return true;
		}
	}

	return false;
}

int NFKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, const bool bPlayer, const NFGUID & noSelf)
{
	int objectCount = 0;
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
		if (pGroupInfo)
		{
			NFGUID ident = NFGUID();
			NF_SHARE_PTR<int> pRet = pGroupInfo->mxPlayerList.First(ident);
			while (!ident.IsNull())
			{
				if (ident != noSelf)
				{
					objectCount++;
				}

				ident = NFGUID();
				pRet = pGroupInfo->mxPlayerList.Next(ident);
			}

			ident = NFGUID();
			pRet = pGroupInfo->mxOtherList.First(ident);
			while (!ident.IsNull())
			{
				if (ident != noSelf)
				{
					objectCount++;
				}

				ident = NFGUID();
				pRet = pGroupInfo->mxOtherList.Next(ident);
			}
		}
	}

	return objectCount;
}

bool NFKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList& list)
{
	return GetGroupObjectList(nSceneID, nGroupID, list, NFGUID());
}

bool NFKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList & list, const bool bPlayer, const NFGUID & noSelf)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{

		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
		if (pGroupInfo)
		{
			if (bPlayer)
			{
				NFGUID ident = NFGUID();
				NF_SHARE_PTR<int> pRet = pGroupInfo->mxPlayerList.First(ident);
				while (!ident.IsNull())
				{
					if (ident != noSelf)
					{
						list.Add(ident);
					}

					ident = NFGUID();
					pRet = pGroupInfo->mxPlayerList.Next(ident);
				}
			}
			else
			{
				NFGUID ident = NFGUID();
				NF_SHARE_PTR<int> pRet = pGroupInfo->mxOtherList.First(ident);
				while (!ident.IsNull())
				{
					if (ident != noSelf)
					{
						list.Add(ident);
					}
					ident = NFGUID();
					pRet = pGroupInfo->mxOtherList.Next(ident);
				}
			}

			return true;
		}
	}
	return false;
}

bool NFKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList & list, const bool bPlayer)
{
	return GetGroupObjectList(nSceneID, nGroupID, list, bPlayer, NFGUID());
}

bool NFKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string & strClassName, NFDataList& list, const NFGUID& noSelf)
{
	NFDataList xDataList;
	if (GetGroupObjectList(nSceneID, nGroupID, xDataList))
	{
		for (int i = 0; i < xDataList.GetCount(); i++)
		{
			NFGUID xID = xDataList.Object(i);
			if (xID.IsNull())
			{
				continue;
			}

			if (this->GetPropertyString(xID, NFrame::IObject::ClassName()) == strClassName
				&& xID != noSelf)
			{
				list.AddObject(xID);
			}
		}

		return true;
	}

	return false;
}

bool NFKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string & strClassName, NFDataList & list)
{
	return GetGroupObjectList(nSceneID, nGroupID, strClassName, list, NFGUID());
}

bool NFKernelModule::LogStack()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
#endif

#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
#endif // NF_PLATFORM

	return true;
}

bool NFKernelModule::LogInfo(const NFGUID ident)
{

	NF_SHARE_PTR<NFIObject> pObject = GetObject(ident);
	if (pObject)
	{
		int nSceneID = GetPropertyInt32(ident, NFrame::IObject::SceneID());
		int nGroupID = GetPropertyInt32(ident, NFrame::IObject::GroupID());

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//----------child object list-------- SceneID = ", nSceneID);

		NFDataList valObjectList;
		GetGroupObjectList(nSceneID, nGroupID, valObjectList);
		for (int i = 0; i < valObjectList.GetCount(); i++)
		{
			NFGUID targetIdent = valObjectList.Object(i);
			LogInfo(targetIdent);
		}
	}
	else
	{
		m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, ident, "", __FUNCTION__, __LINE__);
	}

	return true;
}

int NFKernelModule::OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
	NFPerformance performance;

	NF_SHARE_PTR<NFIObject> xObject = GetElement(self);
	if (xObject)
	{
		if (xObject->GetState() >= CLASS_OBJECT_EVENT::COE_CREATE_HASDATA)
		{
			std::list<PROPERTY_EVENT_FUNCTOR_PTR>::iterator it = mtCommonPropertyCallBackList.begin();
			for (; it != mtCommonPropertyCallBackList.end(); it++)
			{
				PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *it;
				PROPERTY_EVENT_FUNCTOR* pFun = pFunPtr.get();
				pFun->operator()(self, strPropertyName, oldVar, newVar);
			}

			const std::string& strClassName = xObject->GetPropertyString(NFrame::IObject::ClassName());
			std::map<std::string, std::list<PROPERTY_EVENT_FUNCTOR_PTR>>::iterator itClass = mtClassPropertyCallBackList.find(strClassName);
			if (itClass != mtClassPropertyCallBackList.end())
			{
				std::list<PROPERTY_EVENT_FUNCTOR_PTR>::iterator itList = itClass->second.begin();
				for (; itList != itClass->second.end(); itList++)
				{
					PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *itList;
					PROPERTY_EVENT_FUNCTOR* pFun = pFunPtr.get();
					pFun->operator()(self, strPropertyName, oldVar, newVar);
				}
			}
		}
	}

	if (performance.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "--------------- performance problem------------------- ";
		os << performance.TimeScope();
		os << "---------- ";
		os << strPropertyName;
		m_pLogModule->LogWarning(self, os, __FUNCTION__, __LINE__);
	}


	return 0;
}

NF_SHARE_PTR<NFIObject> NFKernelModule::GetObject(const NFGUID& ident)
{
	return GetElement(ident);
}

int NFKernelModule::GetObjectByProperty(const int nSceneID, const int nGroupID, const std::string& strPropertyName, const NFDataList& valueArg, NFDataList& list)
{
	NFDataList varObjectList;
	GetGroupObjectList(nSceneID, nGroupID, varObjectList);

	int nWorldCount = varObjectList.GetCount();
	for (int i = 0; i < nWorldCount; i++)
	{
		NFGUID ident = varObjectList.Object(i);
		if (this->FindProperty(ident, strPropertyName))
		{
			NFDATA_TYPE eType = valueArg.Type(0);
			switch (eType)
			{
			case TDATA_INT:
			{
				int64_t nValue = GetPropertyInt(ident, strPropertyName.c_str());
				if (valueArg.Int(0) == nValue)
				{
					list.Add(ident);
				}
			}
			break;
			case TDATA_STRING:
			{
				std::string strValue = GetPropertyString(ident, strPropertyName.c_str());
				std::string strCompareValue = valueArg.String(0);
				if (strValue == strCompareValue)
				{
					list.Add(ident);
				}
			}
			break;
			case TDATA_OBJECT:
			{
				NFGUID identObject = GetPropertyObject(ident, strPropertyName.c_str());
				if (valueArg.Object(0) == identObject)
				{
					list.Add(ident);
				}
			}
			break;
			default:
				break;
			}
		}
	}

	return list.GetCount();
}

bool NFKernelModule::ExistScene(const int nSceneID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{
		return true;
	}

	return false;
}

bool NFKernelModule::ExistObject(const NFGUID & ident)
{
	return ExistElement(ident);
}

bool NFKernelModule::ExistObject(const NFGUID & ident, const int nSceneID, const int nGroupID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (!pSceneInfo)
	{
		return false;
	}

	return pSceneInfo->ExistObjectInGroup(nGroupID, ident);
}

bool NFKernelModule::DestroySelf(const NFGUID& self)
{
	mtDeleteSelfList.push_back(self);
	return true;
}

int NFKernelModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
	NFPerformance performance;

	NF_SHARE_PTR<NFIObject> xObject = GetElement(self);
	if (xObject)
	{
		if (xObject->GetState() >= CLASS_OBJECT_EVENT::COE_CREATE_HASDATA)
		{
			std::list<RECORD_EVENT_FUNCTOR_PTR>::iterator it = mtCommonRecordCallBackList.begin();
			for (; it != mtCommonRecordCallBackList.end(); it++)
			{
				RECORD_EVENT_FUNCTOR_PTR& pFunPtr = *it;
				RECORD_EVENT_FUNCTOR* pFun = pFunPtr.get();
				pFun->operator()(self, xEventData, oldVar, newVar);
			}
		}

		const std::string& strClassName = xObject->GetPropertyString(NFrame::IObject::ClassName());
		std::map<std::string, std::list<RECORD_EVENT_FUNCTOR_PTR>>::iterator itClass = mtClassRecordCallBackList.find(strClassName);
		if (itClass != mtClassRecordCallBackList.end())
		{
			std::list<RECORD_EVENT_FUNCTOR_PTR>::iterator itList = itClass->second.begin();
			for (; itList != itClass->second.end(); itList++)
			{
				RECORD_EVENT_FUNCTOR_PTR& pFunPtr = *itList;
				RECORD_EVENT_FUNCTOR* pFun = pFunPtr.get();
				pFun->operator()(self, xEventData, oldVar, newVar);
			}
		}
	}

	if (performance.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "--------------- performance problem------------------- ";
		os << performance.TimeScope();
		os << "---------- ";
		os << xEventData.strRecordName;
		os << " event type " << xEventData.nOpType;
		m_pLogModule->LogWarning(self, os, __FUNCTION__, __LINE__);
	}

	return 0;
}

int NFKernelModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	NFPerformance performance;

	std::list<CLASS_EVENT_FUNCTOR_PTR>::iterator it = mtCommonClassCallBackList.begin();
	for (; it != mtCommonClassCallBackList.end(); it++)
	{
		CLASS_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		CLASS_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, strClassName, eClassEvent, var);
	}

	if (performance.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "--------------- performance problem------------------- ";
		os << performance.TimeScope();
		os << "---------- ";
		os << strClassName;
		os << " event type " << eClassEvent;
		m_pLogModule->LogWarning(self, os, __FUNCTION__, __LINE__);
	}

	return 0;
}

bool NFKernelModule::RegisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb)
{
	mtCommonClassCallBackList.push_back(cb);
	return true;
}

bool NFKernelModule::RegisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
	mtCommonPropertyCallBackList.push_back(cb);
	return true;
}

bool NFKernelModule::RegisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb)
{
	mtCommonRecordCallBackList.push_back(cb);
	return true;
}

bool NFKernelModule::RegisterClassPropertyEvent(const std::string & strClassName, const PROPERTY_EVENT_FUNCTOR_PTR & cb)
{
	if (mtClassPropertyCallBackList.find(strClassName) == mtClassPropertyCallBackList.end())
	{
		std::list<PROPERTY_EVENT_FUNCTOR_PTR> xList;
		xList.push_back(cb);

		mtClassPropertyCallBackList.insert(std::map< std::string, std::list<PROPERTY_EVENT_FUNCTOR_PTR>>::value_type(strClassName, xList));

		return true;
	}


	std::map< std::string, std::list<PROPERTY_EVENT_FUNCTOR_PTR>>::iterator it = mtClassPropertyCallBackList.find(strClassName);
	it->second.push_back(cb);


	return false;
}

bool NFKernelModule::RegisterClassRecordEvent(const std::string & strClassName, const RECORD_EVENT_FUNCTOR_PTR & cb)
{
	if (mtClassRecordCallBackList.find(strClassName) == mtClassRecordCallBackList.end())
	{
		std::list<RECORD_EVENT_FUNCTOR_PTR> xList;
		xList.push_back(cb);

		mtClassRecordCallBackList.insert(std::map< std::string, std::list<RECORD_EVENT_FUNCTOR_PTR>>::value_type(strClassName, xList));

		return true;
	}


	std::map< std::string, std::list<RECORD_EVENT_FUNCTOR_PTR>>::iterator it = mtClassRecordCallBackList.find(strClassName);
	it->second.push_back(cb);

	return true;
}

bool NFKernelModule::LogSelfInfo(const NFGUID ident)
{

	return false;
}

bool NFKernelModule::AfterInit()
{
	NF_SHARE_PTR<NFIClass> pClass = m_pClassModule->First();
	while (pClass)
	{
		NFIKernelModule::AddClassCallBack(pClass->GetClassName(), this, &NFKernelModule::OnClassCommonEvent);

		pClass = m_pClassModule->Next();
	}

	return true;
}

bool NFKernelModule::DestroyAll()
{
	NF_SHARE_PTR<NFIObject> pObject = First();
	while (pObject)
	{
		mtDeleteSelfList.push_back(pObject->Self());

		pObject = Next();
	}


	Execute();

	m_pSceneModule->ClearAll();

	return true;
}

bool NFKernelModule::BeforeShut()
{
	DestroyAll();

	mvRandom.clear();
	mtCommonClassCallBackList.clear();
	mtCommonPropertyCallBackList.clear();
	mtCommonRecordCallBackList.clear();

	mtClassPropertyCallBackList.clear();
	mtClassRecordCallBackList.clear();

	return true;
}

int NFKernelModule::Random(int nStart, int nEnd)
{
	if (++mxRandomItor == mvRandom.cend())
	{
		mxRandomItor = mvRandom.cbegin();
	}

	return static_cast<int>((nEnd - nStart) * *mxRandomItor) + nStart;
}

float NFKernelModule::Random()
{
	if (++mxRandomItor == mvRandom.cend())
	{
		mxRandomItor = mvRandom.cbegin();
	}

	return *mxRandomItor;
}

bool NFKernelModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
	return m_pClassModule->AddClassCallBack(strClassName, cb);
}

void NFKernelModule::ProcessMemFree()
{
	if (nLastTime + 30 > pPluginManager->GetNowTime())
	{
		return;
	}

	nLastTime = pPluginManager->GetNowTime();

	//MemManager::GetSingletonPtr()->FreeMem();
}

bool NFKernelModule::DoEvent(const NFGUID& self, const std::string& strClassName, CLASS_OBJECT_EVENT eEvent, const NFDataList& valueList)
{
	return m_pClassModule->DoEvent(self, strClassName, eEvent, valueList);
}