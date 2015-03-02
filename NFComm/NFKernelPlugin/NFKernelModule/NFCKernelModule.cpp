// -------------------------------------------------------------------------
//    @FileName      :    NFCKernelModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <boost/bind.hpp>
#include "../NFKernelPlugin.h"

#include "NFCKernelModule.h"
#include "NFComm/NFCore/NFIdentID.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFCore/NFCRecord.h"
#include "NFComm/NFCore/NFCMemory .h"

NFCKernelModule::NFCKernelModule(NFIPluginManager* p)
{
	fLastTotal = 0.0f;
	pPluginManager = p;

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
	boost::uniform_real<> distribution(0, 1.0f) ;
	boost::mt19937 engine ;
	boost::variate_generator<boost::mt19937, boost::uniform_real<> > myrandom(engine, distribution);

	for (int i = 0; i < nRandomMax; i++)
	{
		mvRandom.push_back(myrandom());
	}
}

bool NFCKernelModule::Init()
{
	m_pContainerModule = NF_SHARE_PTR<NFCContainerModule>(NF_NEW NFCContainerModule());

	mtDeleteSelfList.clear();
	//mLogFile.open( "./Log/NFKernel.log" );

	// every process has a mnIdentIndex, and the ident maybe conflict
	// so...
	//mnIdentIndex

	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
	m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
	m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pUUIDModule = dynamic_cast<NFIUUIDModule*>(pPluginManager->FindModule("NFCUUIDModule"));

	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pUUIDModule);

	return true;
}

bool NFCKernelModule::Shut()
{
	//   mLogFile.close();

	return true;
}

bool NFCKernelModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	mnCurExeObject = NFIDENTID();

	if (mtDeleteSelfList.size() > 0)
	{
		std::list<NFIDENTID>::iterator it = mtDeleteSelfList.begin();
		for (it; it != mtDeleteSelfList.end(); it++)
		{
			DestroyObject(*it);
		}
		mtDeleteSelfList.clear();
	}


	if (fLastTotal < 0.1f)
	{
		fLastTotal += fLasFrametime;
		return false;
	}

	//所有场景
	NF_SHARE_PTR<NFCContainerInfo> pContainerInfo = m_pContainerModule->First();
	while (pContainerInfo.get())
	{
		pContainerInfo->Execute(fLastTotal, fStartedTime);

		pContainerInfo = m_pContainerModule->Next();
	}

	NF_SHARE_PTR<NFIObject> pObject = First();
	while (pObject.get())
	{
		mnCurExeObject = pObject->Self();
		pObject->Execute(fLastTotal, fStartedTime);
		mnCurExeObject = NFIDENTID();

		pObject = Next();
	}

	fLastTotal = 0.0f;

	return true;
}

bool NFCKernelModule::SetComponentEnable(const NFIDENTID& self, const std::string& strComponentName, const bool bEnable)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetComponentManager()->SetEnable(strComponentName, bEnable);
	}

	return false;
}

bool NFCKernelModule::QueryComponentEnable(const NFIDENTID& self, const std::string& strComponentName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetComponentManager()->Enable(strComponentName);
	}

	return false;
}

bool NFCKernelModule::FindHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->FindHeartBeat(strHeartBeatName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
	return false;
}

bool NFCKernelModule::RemoveHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->RemoveHeartBeat(strHeartBeatName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

NF_SHARE_PTR<NFIObject> NFCKernelModule::CreateObject(const NFIDENTID& self, const int nContainerID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFIDataList& arg)
{
	NF_SHARE_PTR<NFIObject> pObject;
	NFIDENTID ident = self;

	NF_SHARE_PTR<NFCContainerInfo> pContainerInfo = m_pContainerModule->GetElement(nContainerID);
	if (!pContainerInfo.get())
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFIDENTID(0, nContainerID), "There is no scene", nContainerID, __FUNCTION__, __LINE__);
		return pObject;
	}

	if (!pContainerInfo->GetElement(nGroupID))
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFIDENTID(0, nContainerID), "There is no group", nGroupID, __FUNCTION__, __LINE__);
		return pObject;
	}

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
	if (pStaticClassPropertyManager.get() && pStaticClassRecordManager.get() && pStaticClasComponentManager.get())
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
			PROPERTY_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnPropertyCommonEvent, this, _1, _2, _3, _4, _5);
			PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
			pObject->AddPropertyCallBack(pStaticConfigPropertyInfo->GetKey(), functorPtr);

			pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
		}

		NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
		while (pConfigRecordInfo.get())
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
			RECORD_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnRecordCommonEvent, this, _1, _2, _3, _4, _5, _6, _7, _8);
			RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
			pObject->AddRecordCallBack(pConfigRecordInfo->GetName(), functorPtr);

			pConfigRecordInfo = pStaticClassRecordManager->Next();
		}

		NF_SHARE_PTR<NFIComponent> pConfigComponentInfo = pStaticClasComponentManager->First();
		while (pConfigComponentInfo.get())
		{
			pComponentManager->AddComponent(pConfigComponentInfo->ComponentName(), pConfigComponentInfo->LanguageName());

			pConfigComponentInfo = pStaticClasComponentManager->Next();
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

		m_pEventProcessModule->DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_NODATA, arg);

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
					case TDATA_DOUBLE:
						pObject->SetPropertyDouble(strPropertyName, arg.Double(i + 1));
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
		pObject->SetPropertyInt("SceneID", nContainerID);
		pObject->SetPropertyInt("GroupID", nGroupID);

		m_pEventProcessModule->DoEvent(ident, strClassName, COE_CREATE_LOADDATA, arg);
		m_pEventProcessModule->DoEvent(ident, strClassName, COE_CREATE_BEFORE_EFFECT, arg);
		m_pEventProcessModule->DoEvent(ident, strClassName, COE_CREATE_EFFECTDATA, arg);
		m_pEventProcessModule->DoEvent(ident, strClassName, COE_CREATE_AFTER_EFFECT, arg);
		m_pEventProcessModule->DoEvent(ident, strClassName, COE_CREATE_HASDATA, arg);
	}

	return pObject;
}

bool NFCKernelModule::DestroyObject(const NFIDENTID& self)
{
	if (self == mnCurExeObject
		&& !self.IsNull())
	{
		//自己的循环过程中，不能删除自己，得等下一帧才行
		return DestroySelf(self);
	}

	//需要同时从容器中删掉
	NFINT64 nGroupID = GetPropertyInt(self, "GroupID");
	NFINT64 nContainerID = GetPropertyInt(self, "SceneID");

	NF_SHARE_PTR<NFCContainerInfo> pContainerInfo = m_pContainerModule->GetElement(nContainerID);
	if (pContainerInfo.get())
	{
		const std::string& strClassName = GetPropertyString(self, "ClassName");
        
        pContainerInfo->RemoveObjectFromGroup(nGroupID, self, strClassName == "Player" ? true : false);

		m_pEventProcessModule->DoEvent(self, strClassName, COE_BEFOREDESTROY, NFCDataList());
		m_pEventProcessModule->DoEvent(self, strClassName, COE_DESTROY, NFCDataList());

		m_pEventProcessModule->RemoveEvent(self);

		RemoveElement(self);

		return true;

	}

    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "There is no scene", nContainerID, __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::FindProperty(const NFIDENTID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->FindProperty(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const NFINT64 nValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyInt(strPropertyName, nValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);
	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName, __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName,  const float fValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyFloat(strPropertyName, fValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName, const double dValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyDouble(strPropertyName, dValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyString(strPropertyName, strValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& objectValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->SetPropertyObject(strPropertyName, objectValue);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

NFINT64 NFCKernelModule::GetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyInt(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return 0;
}

float NFCKernelModule::GetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyFloat(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return 0.0f;
}

double NFCKernelModule::GetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyDouble(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return 0.0;
}

const std::string& NFCKernelModule::GetPropertyString(const NFIDENTID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyString(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

NFIDENTID NFCKernelModule::GetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetPropertyObject(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return NFIDENTID();
}

NF_SHARE_PTR<NFIRecord> NFCKernelModule::FindRecord(const NFIDENTID& self, const std::string& strRecordName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordManager()->GetElement(strRecordName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

	return NULL;
}

bool NFCKernelModule::ClearRecord(const NFIDENTID& self, const std::string& strRecordName)
{
	NF_SHARE_PTR<NFIRecord> pRecord =  FindRecord(self, strRecordName);
	if (pRecord.get())
	{
		return pRecord->Clear();
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no record", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
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

bool NFCKernelModule::SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
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

bool NFCKernelModule::SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		if (!pObject->SetRecordFloat(strRecordName, nRow, nCol, fValue))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordFloat for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value)
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

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		if (!pObject->SetRecordDouble(strRecordName, nRow, nCol, dwValue))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordDouble for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		if (!pObject->SetRecordDouble(strRecordName, nRow, strColTag, value))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordDouble for row  or col", __FUNCTION__, __LINE__);
		}
		else
		{
			return true;
		}
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
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

bool NFCKernelModule::SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
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

bool NFCKernelModule::SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& objectValue)
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

bool NFCKernelModule::SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value)
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

NFINT64 NFCKernelModule::GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordInt(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0;
}

NFINT64 NFCKernelModule::GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordInt(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0;
}

float NFCKernelModule::GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordFloat(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0f;
}

float NFCKernelModule::GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordFloat(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0f;
}

double NFCKernelModule::GetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordDouble(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0;
}

double NFCKernelModule::GetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordDouble(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return 0.0;
}

const std::string& NFCKernelModule::GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordString(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

const std::string& NFCKernelModule::GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordString(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return NULL_STR;
}

NFIDENTID NFCKernelModule::GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordObject(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

	return NFIDENTID();
}

NFIDENTID NFCKernelModule::GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		return pObject->GetRecordObject(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

	return NFIDENTID();
}

bool NFCKernelModule::SwitchScene(const NFIDENTID& self, const int nTargetSceneID, const int nTargetGroupID, const float fX, const float fY, const float fZ, const float fOrient, const NFIDataList& arg)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		NFINT64 nOldSceneID = pObject->GetPropertyInt("SceneID");
		NFINT64 nOldGroupID = pObject->GetPropertyInt("GroupID");

		NF_SHARE_PTR<NFCContainerInfo> pOldSceneInfo = m_pContainerModule->GetElement(nOldSceneID);
		NF_SHARE_PTR<NFCContainerInfo> pNewSceneInfo = m_pContainerModule->GetElement(nTargetSceneID);
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

bool NFCKernelModule::CreateContainer(const int nContainerIndex, const std::string& strSceneConfigID)
{
	std::string strClassName = "Scene";

	NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerIndex);
	if (pSceneInfo.get())
	{
		return false;
	}

	int nWidth = 0;
	NF_SHARE_PTR<NFIPropertyManager> pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager(strSceneConfigID);
	if (pConfigPropertyManager.get())
	{
		NF_SHARE_PTR<NFIProperty> pProperty = pConfigPropertyManager->GetElement("Width");
		if (!pProperty.get())
		{
			return false;
		}

		nWidth = pProperty->GetInt();
	}


	//容器nSceneIndex
	pSceneInfo = NF_SHARE_PTR<NFCContainerInfo>(NF_NEW NFCContainerInfo(nContainerIndex, nWidth));
	if (pSceneInfo.get())
	{
		m_pContainerModule->AddElement(nContainerIndex, pSceneInfo);

		//默认分组0
		NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = NF_SHARE_PTR<NFCContainerGroupInfo>(NF_NEW NFCContainerGroupInfo(nContainerIndex, 0, nWidth));
		if (NULL != pGroupInfo.get())
		{
			pSceneInfo->AddElement(0, pGroupInfo);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "Create scene success, groupId:0, scene id:", nContainerIndex, __FUNCTION__, __LINE__);

			return true;
		}
	}

	return false;
}

bool NFCKernelModule::DestroyContainer(const int nContainerIndex)
{
	m_pContainerModule->RemoveElement(nContainerIndex);

	return true;
}

int NFCKernelModule::GetOnLineCount()
{
    int nCount = 0;
    NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->First();
    while (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo.get())
        {
            nCount += pGroupInfo->mxPlayerList.Count();
            pGroupInfo = pSceneInfo->Next();
        }

        pSceneInfo = m_pContainerModule->Next();
    }

    return nCount;
}

int NFCKernelModule::GetMaxOnLineCount()
{
	// test count 5000
	// and it should be define in a xml file

	return 10000;
}

int NFCKernelModule::GetContainerOnLineCount(const int nContainerID)
{
    int nCount = 0;

    NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo.get())
        {
            nCount += pGroupInfo->mxPlayerList.Count();
            pGroupInfo = pSceneInfo->Next();
        }
    }

    return nCount;
}

int NFCKernelModule::GetContainerOnLineCount(const int nContainerID, const int nGroupID)
{
    int nCount = 0;

    NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo.get())
        {
            nCount = pGroupInfo->mxPlayerList.Count();
        }
    }

    return nCount;
}

//int NFCKernelModule::GetContainerOnLineList( const int nContainerID, type, NFIDataList& var )
int NFCKernelModule::GetContainerOnLineList(const int nContainerID, NFIDataList& var)
{
    NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo.get())
        {
            NFIDENTID ident;

            NF_SHARE_PTR<int> pRet  = pGroupInfo->mxPlayerList.First(ident);
            while (!ident.IsNull())
            {
                var.Add(ident);

                ident = NFIDENTID();
                pRet = pGroupInfo->mxPlayerList.Next(ident);
            }

            pGroupInfo = pSceneInfo->Next();
        }
    }

    return var.GetCount();
}

int NFCKernelModule::RequestGroupScene(const int nContainerID)
{
	NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
	if (pSceneInfo.get())
	{
		int nNewGroupID = pSceneInfo->NewGroupID();
		NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo(NF_NEW NFCContainerGroupInfo(nContainerID, nNewGroupID, pSceneInfo->GetWidth()));
		if (pGroupInfo.get())
		{
			pSceneInfo->AddElement(nNewGroupID, pGroupInfo);
			return nNewGroupID;
		}
	}

	return -1;
}

bool NFCKernelModule::ReleaseGroupScene(const int nContainerID, const int nGroupID)
{
	NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
	if (pSceneInfo.get())
	{
		if (pSceneInfo->GetElement(nGroupID))
		{
			NFCDataList listObject;
			if (GetGroupObjectList(nContainerID, nGroupID, listObject))
			{
				for (int i = 0; i < listObject.GetCount(); ++i)
				{
					NFIDENTID ident = listObject.Object(i);
					DestroyObject(ident);
				}
			}

			pSceneInfo->RemoveElement(nGroupID);

			return true;
		}
	}

	return false;
}

bool NFCKernelModule::GetGroupObjectList(const int nContainerID, const int nGroupID, NFIDataList& list)
{
    NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo.get())
    {

        NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo.get())
        {
            NFIDENTID ident = NFIDENTID();
            NF_SHARE_PTR<int> pRet = pGroupInfo->mxPlayerList.First(ident);
            while (!ident.IsNull())
            {
                list.Add(ident);

                ident = NFIDENTID();
                pRet = pGroupInfo->mxPlayerList.Next(ident);
            }

            pRet = pGroupInfo->mxOtherList.First(ident);
            while (!ident.IsNull())
            {
                list.Add(ident);

                ident = NFIDENTID();
                pRet = pGroupInfo->mxOtherList.Next(ident);
            }

            return true;
        }
    }

    return false;
}

//bool NFCKernelModule::GetGridObjectList(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIDataList& list)
//{
//	NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
//	if (pSceneInfo.get())
//	{
//		NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
//		if (pGroupInfo.get())
//		{
//			NF_SHARE_PTR<NFIGridModule> pGridModule =  pGroupInfo->GetGridModule();
//			if (pGridModule.get())
//			{
//				return pGridModule->GetAroundObject(nGridID, list, EGRID_SELF) > 0 ? true : false;
//			}
//		}
//	}
//
//	return false;
//}

//bool NFCKernelModule::GetAroundGrid(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIDataList& list)
//{
//	NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
//	if (pSceneInfo.get())
//	{
//		NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
//		if (pGroupInfo.get())
//		{
//			NF_SHARE_PTR<NFIGridModule> pGridModule =  pGroupInfo->GetGridModule();
//			if (pGridModule.get())
//			{
//				return pGridModule->GetAroundGrid(nGridID, list, EGRID_AROUND_9) > 0 ? true : false;
//			}
//		}
//	}
//
//	return false;
//}

bool NFCKernelModule::GetRangObjectList(const NFIDENTID& self, const int nContainerID, const int nGroupID, const float fRang, NFIDataList& list)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		float fX = pObject->GetPropertyManager()->GetElement("X")->GetFloat();
		float fY = pObject->GetPropertyManager()->GetElement("Y")->GetFloat();
		float fZ = pObject->GetPropertyManager()->GetElement("Z")->GetFloat();

		// collect info
		return GetRangObjectList(fX, fY, fZ, nContainerID, nGroupID, fRang, list);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::GetRangObjectList(const float fX, const float fY, const float fZ, const int nContainerID, const int nGroupID, const float fRang, NFIDataList& list)
{
	//NFIDENTID gridIdent = NFCGridModule::ComputerGridID(fX, fY, fZ);
	//NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerID);
	//if (pSceneInfo.get())
	//{
	//	NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
	//	if (pGroupInfo.get())
	//	{
	//		NF_SHARE_PTR<NFIGridModule> pGridModule =  pGroupInfo->GetGridModule();
	//		if (pGridModule.get())
	//		{
	//			return pGridModule->GetAroundObject(gridIdent, list) > 0 ? true : false;
	//		}
	//	}
	//}

	return false;
}

bool NFCKernelModule::LogStack()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
#endif


	//NFCallStack::CALLSTACK_PTR call = NFCallStack::generate();
	//if (call)
	//{
	//    int i = 0;
	//    NFCallStack::CONST_ITERATOR it = call->Begin();
	//    for (; it != call->End(); ++it)
	//    {
	//        if (i > 2)
	//        {
	//            std::string strCallStack(*it);
	//            if (std::string::npos != strCallStack.find("NFCNet")
	//                    || std::string::npos != strCallStack.find("main"))
	//            {
	//                break;
	//            }

	//            std::string strHead = strCallStack.substr(0, strCallStack.find("()") + 2);
	//            strHead += "  line: " + strCallStack.substr(strCallStack.rfind('.') + 4, strCallStack.length());

	//            std::cout << strHead << std::endl;
	//            mLogFile << strHead << std::endl;
	//        }

	//        i++;
	//    }
	//}

	// stack walker
	//NFStackWalker sw;
	//sw.ShowCallstack();
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
#endif // NF_PLATFORM

	return true;
}

bool NFCKernelModule::LogInfo(const NFIDENTID ident)
{
	//看是容器还是普通对象，容器则打印所有对象
	NF_SHARE_PTR<NFIObject> pObject = GetObject(ident);
	if (pObject.get())
	{
		if (IsContainer(ident))
		{
			int nContainerID = GetPropertyInt(ident, "SceneID");

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//----------child object list-------- SceneID = ", nContainerID);

			NFCDataList valObjectList;
			int nCount = GetContainerOnLineList(nContainerID, valObjectList);
			for (int i  = 0; i < nCount; i++)
			{
				NFIDENTID targetIdent = valObjectList.Object(i);
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

int NFCKernelModule::OnPropertyCommonEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar)
{
	if (IsContainer(self))
	{
		return 0;
	}

	std::list<PROPERTY_EVENT_FUNCTOR_PTR>::iterator it = mtCommonPropertyCallBackList.begin();
	for (it; it != mtCommonPropertyCallBackList.end(); it++)
	{
		PROPERTY_EVENT_FUNCTOR_PTR pFunPtr = *it;
		PROPERTY_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, strPropertyName, oldVar, newVar, argVar);
	}

	return 0;
}

NF_SHARE_PTR<NFIObject> NFCKernelModule::GetObject(const NFIDENTID& ident)
{
	return GetElement(ident);
}

bool NFCKernelModule::IsContainer(const NFIDENTID& self)
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

int NFCKernelModule::Command(const NFIDataList& var)
{
	if (var.GetCount() > 0 && TDATA_STRING ==  var.Type(0))
	{
		std::string strCommand = var.String(0);
		if ("queryObject" == strCommand && 3 == var.GetCount())
		{
			const std::string& strObjectIdent = var.String(1);
			const std::string& strObjectSerial = var.String(2);

			NFIDENTID ident;
			//             ident.nIdent = atoi(strObjectIdent.c_str());
			//             ident.nSerial = atoi(strObjectSerial.c_str());
			// 
			//             LogInfo(ident);

		}
		else if ("queryObjectProperty" == strCommand)
		{
			if (2 == var.GetCount())
			{
			}
			else
			{

			}
		}
	}

	return 0;
}

int NFCKernelModule::GetObjectByProperty(const int nContainerID, const std::string& strPropertyName, const NFIDataList& valueArg, NFIDataList& list)
{
	NFCDataList varObjectList;
	GetContainerOnLineList(nContainerID, varObjectList);
	int nWorldCount = varObjectList.GetCount();
	for (int i = 0; i < nWorldCount; i++)
	{
		NFIDENTID ident = varObjectList.Object(i);
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
					NFIDENTID identObject = GetPropertyObject(ident, strPropertyName.c_str());
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

bool NFCKernelModule::ExistContainer(const int nContainerIndex)
{
	NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->GetElement(nContainerIndex);
	if (pSceneInfo.get())
	{
		return true;
	}

	return false;
}

bool NFCKernelModule::DestroySelf(const NFIDENTID& self)
{
	mtDeleteSelfList.push_back(self);
	return true;
}

int NFCKernelModule::OnRecordCommonEvent(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg)
{
	if (IsContainer(self))
	{
		return 0;
	}

	std::list<RECORD_EVENT_FUNCTOR_PTR>::iterator it = mtCommonRecordCallBackList.begin();
	for (it; it != mtCommonRecordCallBackList.end(); it++)
	{
		RECORD_EVENT_FUNCTOR_PTR pFunPtr = *it;
		RECORD_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, strRecordName, nOpType, nRow, nCol, oldVar, newVar, arg);
	}

	return 0;
}

int NFCKernelModule::OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	if (IsContainer(self))
	{
		return 0;
	}

	std::list<CLASS_EVENT_FUNCTOR_PTR>::iterator it = mtCommonClassCallBackList.begin();
	for (it; it != mtCommonClassCallBackList.end(); it++)
	{
		CLASS_EVENT_FUNCTOR_PTR pFunPtr = *it;
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

bool NFCKernelModule::LogSelfInfo(const NFIDENTID ident)
{

	return false;
}

bool NFCKernelModule::AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex, const std::string& strScriptFunction)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		pObject->GetPropertyManager()->AddProperty(self, strPropertyName, varType, bPublic, bPrivate, bSave, bView, nIndex, strScriptFunction);

		//通用回调，方便NET同步
		PROPERTY_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnPropertyCommonEvent, this, _1, _2, _3, _4, _5);
		PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
		return pObject->AddPropertyCallBack(strPropertyName, functorPtr);
	}

	return false;
}

bool NFCKernelModule::AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& TData, const NFIDataList& varKey, const NFIDataList& varDesc, const NFIDataList& varTag, const NFIDataList& varRelatedRecord, const int nRows, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject.get())
	{
		pObject->GetRecordManager()->AddRecord(self, strRecordName, TData, varKey, varDesc, varTag, varRelatedRecord, nRows, bPublic, bPrivate, bSave, bView, nIndex);

		//通用回调，方便NET同步
		RECORD_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnRecordCommonEvent, this, _1, _2, _3, _4, _5, _6, _7, _8);
		RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
		return pObject->AddRecordCallBack(strRecordName, functorPtr);
	}

	return false;
}

NFIDENTID NFCKernelModule::GetGridID(const float fX, const float fY, const float fZ)
{
	return NFCGridModule::ComputerGridID((int)fX, (int)fY, (int)fZ);
}

bool NFCKernelModule::AfterInit()
{
	NF_SHARE_PTR<NFILogicClass> pClass = m_pLogicClassModule->First();
	while (pClass.get())
	{
		m_pEventProcessModule->AddClassCallBack(pClass->GetClassName(), this, &NFCKernelModule::OnClassCommonEvent);

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
	Execute(0.1f, 0.1f);

	//NF_SHARE_PTR<NFCContainerInfo> pContainerInfo = m_pContainerModule->First();
	//while (pContainerInfo.get())
	//{
	//	NFIDENTID ident = pContainerInfo->GetObjectSelf();
	//	RemoveElement(ident);
	//	pContainerInfo->ClearAll();

	//	pContainerInfo = m_pContainerModule->Next();
	//}

	m_pContainerModule->ClearAll();

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

int NFCKernelModule::GetAllContainerObjectList(NFIDataList& var)
{
    NF_SHARE_PTR<NFCContainerInfo> pSceneInfo = m_pContainerModule->First();
    while (pSceneInfo.get())
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo.get())
        {
            NFIDENTID ident;

            NF_SHARE_PTR<int> pRet = pGroupInfo->mxPlayerList.First(ident);
            while (!ident.IsNull())
            {
                var.Add(ident);

                ident = NFIDENTID();
                pRet = pGroupInfo->mxPlayerList.Next(ident);
            }

            pRet = pGroupInfo->mxOtherList.First(ident);
            while (!ident.IsNull())
            {
                var.Add(ident);

                ident = NFIDENTID();
                pRet = pGroupInfo->mxOtherList.Next(ident);
            }

            pGroupInfo = pSceneInfo->Next();
        }

        pSceneInfo = m_pContainerModule->Next();
    }

    return 0;
}

bool NFCKernelModule::AddEventCallBack(const NFIDENTID& self, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
{
	return m_pEventProcessModule->AddEventCallBack(self, nEventID, cb);
}

bool NFCKernelModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
	return m_pEventProcessModule->AddClassCallBack(strClassName, cb);
}

// bool NFCKernelModule::ResgisterCommonHeartBeat( const HEART_BEAT_FUNCTOR_PTR& cb )
// {
//     mtCommonHeartBeatCallBackList.push_back(cb);
//
//     return true;
// }
//
// bool NFCKernelModule::ResgisterCommonEvent( const EVENT_PROCESS_FUNCTOR_PTR& cb )
// {
//     mtCommonEventCallBackList.push_back(cb);
//
//     return true;
// }

// int NFCKernelModule::OnHeartBeatCommonCB( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var )
// {
//     if (IsContainer(self))
//     {
//         return 0;
//     }
//
//     std::list<HEART_BEAT_FUNCTOR_PTR>::iterator it = mtCommonHeartBeatCallBackList.begin();
//     for (it; it != mtCommonHeartBeatCallBackList.end(); it++)
//     {
//         HEART_BEAT_FUNCTOR_PTR pFunPtr = *it;
//         HEART_BEAT_FUNCTOR* pFun = pFunPtr.get();
//         pFun->operator()(self, strHeartBeat, fTime, nCount, var);
//     }
//
//     return 0;
// }
//
// int NFCKernelModule::OnEventCommonCB( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
// {
//     if (IsContainer(self))
//     {
//         return 0;
//     }
//
//     std::list<EVENT_PROCESS_FUNCTOR_PTR>::iterator it = mtCommonEventCallBackList.begin();
//     for (it; it != mtCommonEventCallBackList.end(); it++)
//     {
//         EVENT_PROCESS_FUNCTOR_PTR pFunPtr = *it;
//         EVENT_PROCESS_FUNCTOR* pFun = pFunPtr.get();
//         pFun->operator()(self, nEventID, var);
//     }
//
//     return 0;
// }

bool NFCKernelModule::AddRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb)
{
	NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
	if (pObject.get())
	{
		return pObject->AddRecordCallBack(strRecordName, cb);
	}

	return false;
}

bool NFCKernelModule::AddPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
	NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
	if (pObject.get())
	{
		return pObject->AddPropertyCallBack(strPropertyName, cb);
	}

	return false;
}

bool NFCKernelModule::AddHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIDataList& var, const float fTime, const int nCount)
{
	NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
	if (pObject.get())
	{
		//共有回调添加后，删除时也要删除
		//ResgisterCommonHeartBeat(cb);

		pObject->AddHeartBeat(strHeartBeatName, cb, var, fTime, nCount);
	}

	return false;
}
