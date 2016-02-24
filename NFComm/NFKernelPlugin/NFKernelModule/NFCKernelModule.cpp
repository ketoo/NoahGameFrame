// -------------------------------------------------------------------------
//    @FileName			:    NFCKernelModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCKernelModule.h"
#include "NFComm/NFCore/NFDefine.h"
#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFCore/NFCRecord.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFCore/NFCMemManger.h"

NFCKernelModule::NFCKernelModule(NFIPluginManager* p)
{
	pPluginManager = p;

	nLastTime = pPluginManager->GetNowTime();
	InitRandom();
}

NFCKernelModule::~NFCKernelModule()
{
	ClearAll();
}

void NFCKernelModule::InitRandom()
{
	mvRandom.clear();

	int nRandomMax = 100000;
	mnRandomPos = 0;

    std::random_device rd;
	std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.0f);

	for (int i = 0; i < nRandomMax; i++)
	{
		mvRandom.push_back(dis(gen));
	}
}

bool NFCKernelModule::Init()
{
	mtDeleteSelfList.clear();

	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>("NFCSceneModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
	m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");

	assert(NULL != m_pSceneModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pUUIDModule);

	return true;
}

bool NFCKernelModule::Shut()
{
	return true;
}

bool NFCKernelModule::Execute()
{
    ProcessMemFree();

	mnCurExeObject = NFGUID();

	if (mtDeleteSelfList.size() > 0)
	{
		std::list<NFGUID>::iterator it = mtDeleteSelfList.begin();
		for (it; it != mtDeleteSelfList.end(); it++)
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
		mnCurExeObject = NFGUID();

		pObject = Next();
	}

	return true;
}

bool NFCKernelModule::FindHeartBeat(const NFGUID& self, const std::string& strHeartBeatName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->FindHeartBeat(strHeartBeatName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
	return false;
}

bool NFCKernelModule::RemoveHeartBeat(const NFGUID& self, const std::string& strHeartBeatName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->RemoveHeartBeat(strHeartBeatName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

NF_SHARE_PTR<NFIObject> NFCKernelModule::CreateObject(const NFGUID& self, const int nSceneID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFIDataList& arg)
{
	NF_SHARE_PTR<NFIObject> pObject;
	NFGUID ident = self;

	NF_SHARE_PTR<NFCSceneInfo> pContainerInfo = m_pSceneModule->GetElement(nSceneID);
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

// 	if (!m_pElementInfoModule->ExistElement(strConfigIndex))
// 	{
// 		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSceneID), "There is no group", nGroupID, __FUNCTION__, __LINE__);
// 		return pObject;
// 	}

	//默认为1分组，0则是所有分组都看得见,-1则是容器
	if (ident.IsNull())
	{
		ident = m_pUUIDModule->CreateGUID();
	}

	if (GetElement(ident))
	{
		m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, ident, "The object has Exists", __FUNCTION__, __LINE__);
		return pObject;
	}

	NF_SHARE_PTR<NFIPropertyManager> pStaticClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);
	NF_SHARE_PTR<NFIRecordManager> pStaticClassRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);
	NF_SHARE_PTR<NFIComponentManager> pStaticClasComponentManager = m_pLogicClassModule->GetClassComponentManager(strClassName);
	if (pStaticClassPropertyManager && pStaticClassRecordManager && pStaticClasComponentManager)
	{

		pObject = NF_SHARE_PTR<NFIObject>(NF_NEW NFCObject(ident, pPluginManager));
		//是否是应该晚点等到事件2时才加入容器，这样能保证进入容器的对象都是有完整数据的，否则因为协程的原因，其他对象找到他时他却没数据或者部分数据
		AddElement(ident, pObject);
		pContainerInfo->AddObjectToGroup(nGroupID, ident, strClassName == "Player" ? true : false);

		NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();
		NF_SHARE_PTR<NFIComponentManager> pComponentManager = pObject->GetComponentManager();

		//默认属性
		NF_SHARE_PTR<NFIProperty> pStaticConfigPropertyInfo = pStaticClassPropertyManager->First();
		while (pStaticConfigPropertyInfo.get())
		{
			pPropertyManager->AddProperty(ident,
				pStaticConfigPropertyInfo->GetKey(),
				pStaticConfigPropertyInfo->GetType(),
				pStaticConfigPropertyInfo->GetPublic(),
				pStaticConfigPropertyInfo->GetPrivate(),
				pStaticConfigPropertyInfo->GetSave(),
				pStaticConfigPropertyInfo->GetIndex(),
				pStaticConfigPropertyInfo->GetView(),
				pStaticConfigPropertyInfo->GetRelationValue());

            //通用回调，方便NET同步
            pObject->AddPropertyCallBack(pStaticConfigPropertyInfo->GetKey(), this, &NFCKernelModule::OnPropertyCommonEvent);

			pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
		}

		NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
		while (pConfigRecordInfo)
		{
			pRecordManager->AddRecord(ident,
				pConfigRecordInfo->GetName(),
				pConfigRecordInfo->GetInitData(),
				pConfigRecordInfo->GetKeyState(),
				pConfigRecordInfo->GetInitDesc(),
				pConfigRecordInfo->GetTag(),
				pConfigRecordInfo->GetRelatedRecord(),
				pConfigRecordInfo->GetRows(),
				pConfigRecordInfo->GetPublic(),
				pConfigRecordInfo->GetPrivate(),
				pConfigRecordInfo->GetSave(),
				pConfigRecordInfo->GetView(),
				pConfigRecordInfo->GetIndex());

			//通用回调，方便NET同步
			pObject->AddRecordCallBack(pConfigRecordInfo->GetName(), this, &NFCKernelModule::OnRecordCommonEvent);

			pConfigRecordInfo = pStaticClassRecordManager->Next();
		}

		std::string strSrciptComponentName;
		NF_SHARE_PTR<NFIComponent> xComponent = pStaticClasComponentManager->First(strSrciptComponentName);
		while (!strSrciptComponentName.empty())
		{
			pComponentManager->AddComponent(strSrciptComponentName, xComponent);

			strSrciptComponentName.clear();
			NF_SHARE_PTR<NFIComponent> xComponent = pStaticClasComponentManager->Next(strSrciptComponentName);
		}
		//////////////////////////////////////////////////////////////////////////
		//配置属性
		NF_SHARE_PTR<NFIPropertyManager> pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigIndex);
		NF_SHARE_PTR<NFIRecordManager> pConfigRecordManager = m_pElementInfoModule->GetRecordManager(strConfigIndex);

		if (pConfigPropertyManager.get() && pConfigRecordManager.get())
		{
			NF_SHARE_PTR<NFIProperty> pConfigPropertyInfo = pConfigPropertyManager->First();
			while (pConfigPropertyInfo.get())
			{

				pPropertyManager->SetProperty(pConfigPropertyInfo->GetKey(), pConfigPropertyInfo->GetValue());

				pConfigPropertyInfo = pConfigPropertyManager->Next();
			}
		}

		DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_NODATA, arg);

		//传入的属性赋值
		for (int i = 0; i < arg.GetCount() - 1; i += 2)
		{
			const std::string& strPropertyName = arg.String(i);
			if ("ConfigID" != strPropertyName
				&& "ClassName" != strPropertyName
				&& "SceneID" != strPropertyName
				&& "GroupID" != strPropertyName)
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
					default:
						break;
					}
				}
			}
		}

		//放进容器//先进入场景，再进入层
		pObject->SetPropertyString("ConfigID", strConfigIndex);
		pObject->SetPropertyString("ClassName", strClassName);
		pObject->SetPropertyInt("SceneID", nSceneID);
		pObject->SetPropertyInt("GroupID", nGroupID);

		DoEvent(ident, strClassName, COE_CREATE_LOADDATA, arg);
		DoEvent(ident, strClassName, COE_CREATE_BEFORE_EFFECT, arg);
		DoEvent(ident, strClassName, COE_CREATE_EFFECTDATA, arg);
		DoEvent(ident, strClassName, COE_CREATE_AFTER_EFFECT, arg);
		DoEvent(ident, strClassName, COE_CREATE_HASDATA, arg);
	}

	return pObject;
}

bool NFCKernelModule::DestroyObject(const NFGUID& self)
{
	if (self == mnCurExeObject
		&& !self.IsNull())
	{
		//自己的循环过程中，不能删除自己，得等下一帧才行
		return DestroySelf(self);
	}

	//需要同时从容器中删掉
	NFINT64 nGroupID = GetPropertyInt(self, "GroupID");
	NFINT64 nSceneID = GetPropertyInt(self, "SceneID");

	NF_SHARE_PTR<NFCSceneInfo> pContainerInfo = m_pSceneModule->GetElement(nSceneID);
	if (pContainerInfo.get())
	{
		const std::string& strClassName = GetPropertyString(self, "ClassName");

        pContainerInfo->RemoveObjectFromGroup(nGroupID, self, strClassName == "Player" ? true : false);

		DoEvent(self, strClassName, COE_BEFOREDESTROY, NFCDataList());
		DoEvent(self, strClassName, COE_DESTROY, NFCDataList());

		RemoveElement(self);

		return true;

	}

    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "There is no scene", nSceneID, __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::FindProperty(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->FindProperty(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyInt(const NFGUID& self, const std::string& strPropertyName, const NFINT64 nValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyInt(strPropertyName, nValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyFloat(const NFGUID& self, const std::string& strPropertyName, const double dValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyFloat(strPropertyName, dValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyString(const NFGUID& self, const std::string& strPropertyName, const std::string& strValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyString(strPropertyName, strValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyObject(const NFGUID& self, const std::string& strPropertyName, const NFGUID& objectValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyObject(strPropertyName, objectValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

NFINT64 NFCKernelModule::GetPropertyInt(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyInt(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_INT;
}

double NFCKernelModule::GetPropertyFloat(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyFloat(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_FLOAT;
}

const std::string& NFCKernelModule::GetPropertyString(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyString(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

const NFGUID& NFCKernelModule::GetPropertyObject(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyObject(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_OBJECT;
}

NF_SHARE_PTR<NFIRecord> NFCKernelModule::FindRecord(const NFGUID& self, const std::string& strRecordName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordManager()->GetElement(strRecordName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

	return nullptr;
}

bool NFCKernelModule::ClearRecord(const NFGUID& self, const std::string& strRecordName)
{
	NF_SHARE_PTR<NFIRecord> pRecord =  FindRecord(self, strRecordName);
	if (pRecord.get())
	{
		return pRecord->Clear();
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no record", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

bool NFCKernelModule::SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

bool NFCKernelModule::SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

bool NFCKernelModule::SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

bool NFCKernelModule::SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

bool NFCKernelModule::SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

bool NFCKernelModule::SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

bool NFCKernelModule::SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
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

NFINT64 NFCKernelModule::GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordInt(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0;
}

NFINT64 NFCKernelModule::GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordInt(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0;
}

double NFCKernelModule::GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordFloat(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0;
}

double NFCKernelModule::GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordFloat(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0;
}

const std::string& NFCKernelModule::GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordString(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

const std::string& NFCKernelModule::GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordString(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

const NFGUID& NFCKernelModule::GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordObject(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

	return NULL_OBJECT;
}

const NFGUID& NFCKernelModule::GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordObject(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

	return NULL_OBJECT;
}

bool NFCKernelModule::SwitchScene(const NFGUID& self, const int nTargetSceneID, const int nTargetGroupID, const float fX, const float fY, const float fZ, const float fOrient, const NFIDataList& arg)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		NFINT64 nOldSceneID = pObject->GetPropertyInt("SceneID");
		NFINT64 nOldGroupID = pObject->GetPropertyInt("GroupID");

		NF_SHARE_PTR<NFCSceneInfo> pOldSceneInfo = m_pSceneModule->GetElement(nOldSceneID);
		NF_SHARE_PTR<NFCSceneInfo> pNewSceneInfo = m_pSceneModule->GetElement(nTargetSceneID);
		if (!pOldSceneInfo.get())
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this container", nOldSceneID);
			return false;
		}

		if (!pNewSceneInfo.get())
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this container", nTargetSceneID);
			return false;
		}

		if (!pNewSceneInfo->GetElement(nTargetGroupID))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this group", nTargetGroupID);
			return false;
		}

		pOldSceneInfo->RemoveObjectFromGroup(nOldGroupID, self, true);

		//可以在同一场景切换到不同的层
		if (nTargetSceneID != nOldSceneID)
		{
			//真的切场景
			//先退回到0层，才能修改场景ID
			pObject->SetPropertyInt("GroupID", 0);

			pObject->SetPropertyInt("SceneID", nTargetSceneID);
			//进新的场景0层
		}

		pObject->SetPropertyFloat("X", fX);
		pObject->SetPropertyFloat("Y", fY);
		pObject->SetPropertyFloat("Z", fZ);

		pObject->SetPropertyInt("GroupID", nTargetGroupID);

		pNewSceneInfo->AddObjectToGroup(nTargetGroupID, self, true);

		return true;
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::CreateScene(const int nSceneID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo.get())
	{
		return false;
	}

	//容器nSceneIndex
	pSceneInfo = NF_SHARE_PTR<NFCSceneInfo>(NF_NEW NFCSceneInfo(nSceneID));
	if (pSceneInfo.get())
	{
		m_pSceneModule->AddElement(nSceneID, pSceneInfo);

		//默认分组0
		NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = NF_SHARE_PTR<NFCSceneGroupInfo>(NF_NEW NFCSceneGroupInfo(nSceneID, 0));
		if (NULL != pGroupInfo.get())
		{
			pSceneInfo->AddElement(0, pGroupInfo);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Create scene success, groupId:0, scene id:", nSceneID, __FUNCTION__, __LINE__);

			return true;
		}
	}

	return false;
}

bool NFCKernelModule::DestroyScene(const int nSceneID)
{
	m_pSceneModule->RemoveElement(nSceneID);

	return true;
}

int NFCKernelModule::GetOnLineCount()
{
    int nCount = 0;
    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->First();
    while (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo.get())
        {
            nCount += pGroupInfo->mxPlayerList.Count();
            pGroupInfo = pSceneInfo->Next();
        }

        pSceneInfo = m_pSceneModule->Next();
    }

    return nCount;
}

int NFCKernelModule::GetMaxOnLineCount()
{
	// test count 5000
	// and it should be define in a xml file

	return 10000;
}

int NFCKernelModule::GetSceneOnLineCount(const int nSceneID)
{
    int nCount = 0;

    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo.get())
        {
            nCount += pGroupInfo->mxPlayerList.Count();
            pGroupInfo = pSceneInfo->Next();
        }
    }

    return nCount;
}

int NFCKernelModule::GetSceneOnLineCount(const int nSceneID, const int nGroupID)
{
    int nCount = 0;

    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo.get())
        {
            nCount = pGroupInfo->mxPlayerList.Count();
        }
    }

    return nCount;
}

//int NFCKernelModule::GetSceneOnLineList( const int nSceneID, type, NFIDataList& var )
int NFCKernelModule::GetSceneOnLineList(const int nSceneID, NFIDataList& var)
{
    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo.get())
        {
            NFGUID ident;

            NF_SHARE_PTR<int> pRet  = pGroupInfo->mxPlayerList.First(ident);
            while (!ident.IsNull())
            {
                var.Add(ident);

                ident = NFGUID();
                pRet = pGroupInfo->mxPlayerList.Next(ident);
            }

            pGroupInfo = pSceneInfo->Next();
        }
    }

    return var.GetCount();
}

int NFCKernelModule::RequestGroupScene(const int nSceneID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo.get())
	{
		int nNewGroupID = pSceneInfo->NewGroupID();
		NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
		if (pSceneInfo)
		{
			if(!pSceneInfo->GetElement(nNewGroupID))
			{
				 NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo(NF_NEW NFCSceneGroupInfo(nSceneID, nNewGroupID, pSceneInfo->GetWidth()));
				if (pGroupInfo)
				{
					pSceneInfo->AddElement(nNewGroupID, pGroupInfo);
					return nNewGroupID;
				}
			}
		}
	}

	return -1;
}

bool NFCKernelModule::ReleaseGroupScene(const int nSceneID, const int nGroupID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo.get())
	{
		if (pSceneInfo->GetElement(nGroupID))
		{
			NFCDataList listObject;
			if (GetGroupObjectList(nSceneID, nGroupID, listObject))
			{
				for (int i = 0; i < listObject.GetCount(); ++i)
				{
					NFGUID ident = listObject.Object(i);
					DestroyObject(ident);
				}
			}

			pSceneInfo->RemoveElement(nGroupID);

			return true;
		}
	}

	return false;
}

bool NFCKernelModule::ExitGroupScene(const int nSceneID, const int nGroupID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo.get())
	{
		NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
		if (pGroupInfo)
		{
			return true;
		}
	}

	return false;
}

bool NFCKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFIDataList& list)
{
    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo.get())
    {

        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo.get())
        {
            NFGUID ident = NFGUID();
            NF_SHARE_PTR<int> pRet = pGroupInfo->mxPlayerList.First(ident);
            while (!ident.IsNull())
            {
                list.Add(ident);

                ident = NFGUID();
                pRet = pGroupInfo->mxPlayerList.Next(ident);
            }

            pRet = pGroupInfo->mxOtherList.First(ident);
            while (!ident.IsNull())
            {
                list.Add(ident);

                ident = NFGUID();
                pRet = pGroupInfo->mxOtherList.Next(ident);
            }

            return true;
        }
    }

    return false;
}

bool NFCKernelModule::LogStack()
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

bool NFCKernelModule::LogInfo(const NFGUID ident)
{
	//看是容器还是普通对象，容器则打印所有对象
	NF_SHARE_PTR<NFIObject> pObject = GetObject(ident);
	if (pObject.get())
	{
		if (IsContainer(ident))
		{
			int nSceneID = GetPropertyInt(ident, "SceneID");

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//----------child object list-------- SceneID = ", nSceneID);

			NFCDataList valObjectList;
			int nCount = GetSceneOnLineList(nSceneID, valObjectList);
			for (int i  = 0; i < nCount; i++)
			{
				NFGUID targetIdent = valObjectList.Object(i);
				LogInfo(targetIdent);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//---------printf object start-------- ", "");


			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//---------printf object end-------- ", "");
		}
	}
	else
	{
		m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, ident, "", __FUNCTION__, __LINE__);
	}

	return true;
}

int NFCKernelModule::OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	if (IsContainer(self))
	{
		return 0;
	}

	std::list<PROPERTY_EVENT_FUNCTOR_PTR>::iterator it = mtCommonPropertyCallBackList.begin();
	for (it; it != mtCommonPropertyCallBackList.end(); it++)
	{
		PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, strPropertyName, oldVar, newVar);
	}

	return 0;
}

NF_SHARE_PTR<NFIObject> NFCKernelModule::GetObject(const NFGUID& ident)
{
	return GetElement(ident);
}

bool NFCKernelModule::IsContainer(const NFGUID& self)
{
	NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
	if (pObject.get())
	{
		if (pObject->GetPropertyInt("GroupID") < 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

int NFCKernelModule::GetObjectByProperty(const int nSceneID, const std::string& strPropertyName, const NFIDataList& valueArg, NFIDataList& list)
{
	NFCDataList varObjectList;
	GetSceneOnLineList(nSceneID, varObjectList);
	int nWorldCount = varObjectList.GetCount();
	for (int i = 0; i < nWorldCount; i++)
	{
		NFGUID ident = varObjectList.Object(i);
		if (this->FindProperty(ident, strPropertyName))
		{
			TDATA_TYPE eType = valueArg.Type(0);
			switch (eType)
			{
			case TDATA_INT:
				{
					int nValue = GetPropertyInt(ident, strPropertyName.c_str());
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

bool NFCKernelModule::ExistContainer(const int nSceneID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo.get())
	{
		return true;
	}

	return false;
}

bool NFCKernelModule::DestroySelf(const NFGUID& self)
{
	mtDeleteSelfList.push_back(self);
	return true;
}

int NFCKernelModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	if (IsContainer(self))
	{
		return 0;
	}

	std::list<RECORD_EVENT_FUNCTOR_PTR>::iterator it = mtCommonRecordCallBackList.begin();
	for (it; it != mtCommonRecordCallBackList.end(); it++)
	{
		RECORD_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		RECORD_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, xEventData, oldVar, newVar);
	}

	return 0;
}

int NFCKernelModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	if (IsContainer(self))
	{
		return 0;
	}

	std::list<CLASS_EVENT_FUNCTOR_PTR>::iterator it = mtCommonClassCallBackList.begin();
	for (it; it != mtCommonClassCallBackList.end(); it++)
	{
		CLASS_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		CLASS_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, strClassName, eClassEvent, var);
	}

	return 0;
}

bool NFCKernelModule::ResgisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb)
{
	mtCommonClassCallBackList.push_back(cb);
	return true;
}

bool NFCKernelModule::ResgisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
	mtCommonPropertyCallBackList.push_back(cb);
	return true;
}

bool NFCKernelModule::ResgisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb)
{
	mtCommonRecordCallBackList.push_back(cb);
	return true;
}

bool NFCKernelModule::LogSelfInfo(const NFGUID ident)
{

	return false;
}

bool NFCKernelModule::AddProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex, const std::string& strScriptFunction)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		pObject->GetPropertyManager()->AddProperty(self, strPropertyName, varType, bPublic, bPrivate, bSave, bView, nIndex, strScriptFunction);

		//通用回调，方便NET同步
		return pObject->AddPropertyCallBack(strPropertyName, this, &NFCKernelModule::OnPropertyCommonEvent);
	}

	return false;
}

bool NFCKernelModule::AddRecord(const NFGUID& self, const std::string& strRecordName, const NFIDataList& TData, const NFIDataList& varKey, const NFIDataList& varDesc, const NFIDataList& varTag, const NFIDataList& varRelatedRecord, const int nRows, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		pObject->GetRecordManager()->AddRecord(self, strRecordName, TData, varKey, varDesc, varTag, varRelatedRecord, nRows, bPublic, bPrivate, bSave, bView, nIndex);

		//通用回调，方便NET同步
		return pObject->AddRecordCallBack(strRecordName, this, &NFCKernelModule::OnRecordCommonEvent);
	}

	return false;
}

bool NFCKernelModule::AfterInit()
{
	NF_SHARE_PTR<NFILogicClass> pClass = m_pLogicClassModule->First();
	while (pClass.get())
	{
		NFIKernelModule::AddClassCallBack(pClass->GetClassName(), this, &NFCKernelModule::OnClassCommonEvent);

		pClass = m_pLogicClassModule->Next();
	}

	return true;
}

bool NFCKernelModule::DestroyAll()
{
	NF_SHARE_PTR<NFIObject> pObject = First();
	while (pObject.get())
	{
		mtDeleteSelfList.push_back(pObject->Self());

		pObject = Next();
	}

	// 为了释放object
	Execute();

	m_pSceneModule->ClearAll();

	mvRandom.clear();
	mtCommonClassCallBackList.clear();
	mtCommonPropertyCallBackList.clear();
	mtCommonRecordCallBackList.clear();

	return true;
}

bool NFCKernelModule::BeforeShut()
{
	DestroyAll();
	return true;
}

void NFCKernelModule::Random(int nStart, int nEnd, int nCount, NFIDataList& valueList)
{
	if (mnRandomPos + nCount >= mvRandom.size())
	{
		mnRandomPos = 0;
	}

	for (int i = mnRandomPos; i < mnRandomPos + nCount; i++)
	{
		float fRanValue = mvRandom.at(i);
		int nValue = (nEnd - nStart) * fRanValue + nStart;
		valueList.Add((NFINT64)nValue);
	}

	mnRandomPos += nCount;
}

bool NFCKernelModule::AddEventCallBack(const NFGUID& self, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetEventManager()->AddEventCallBack(nEventID, cb);
	}

	return false;
}

bool NFCKernelModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
	return m_pLogicClassModule->AddClassCallBack(strClassName, cb);
}

void NFCKernelModule::ProcessMemFree()
{
	if (nLastTime + 30 > pPluginManager->GetNowTime())
	{
		return;
	}

	nLastTime = pPluginManager->GetNowTime();

    NFCMemManger::GetSingletonPtr()->FreeMem();
}

bool NFCKernelModule::DoEvent( const NFGUID& self, const std::string& strClassName, CLASS_OBJECT_EVENT eEvent, const NFIDataList& valueList )
{
	return m_pLogicClassModule->DoEvent(self, strClassName, eEvent, valueList);
}

bool NFCKernelModule::DoEvent( const NFGUID& self, const int nEventID, const NFIDataList& valueList )
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetEventManager()->DoEvent(nEventID, valueList);
	}

	return false;
}
