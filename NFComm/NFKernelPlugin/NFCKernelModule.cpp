// -------------------------------------------------------------------------
//    @FileName         :    NFCKernelModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCKernelModule.h"
#include "NFComm/NFCore/NFMemManager.hpp"
#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFCRecord.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFCKernelModule::NFCKernelModule(NFIPluginManager* p)
{
    nGUIDIndex = 0;
    mnRandomPos = 0;
    nLastTime = 0;

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
        mvRandom.push_back((float) dis(gen));
    }
}

bool NFCKernelModule::Init()
{
    mtDeleteSelfList.clear();

    m_pSceneModule = pPluginManager->FindModule<NFISceneAOIModule>();
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();

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

NF_SHARE_PTR<NFIObject> NFCKernelModule::CreateObject(const NFGUID& self, const int nSceneID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFDataList& arg)
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

        pObject = NF_SHARE_PTR<NFIObject>(NF_NEW NFCObject(ident, pPluginManager));
        
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

            
            pObject->AddPropertyCallBack(pStaticConfigPropertyInfo->GetKey(), this, &NFCKernelModule::OnPropertyCommonEvent);

            pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
        }

        NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
        while (pConfigRecordInfo)
        {
            NF_SHARE_PTR<NFIRecord> xRecord =  pRecordManager->AddRecord(ident,
                                      pConfigRecordInfo->GetName(),
                                      pConfigRecordInfo->GetInitData(),
                                      pConfigRecordInfo->GetTag(),
                                      pConfigRecordInfo->GetRows());

             xRecord->SetPublic(pConfigRecordInfo->GetPublic());
             xRecord->SetPrivate(pConfigRecordInfo->GetPrivate());
             xRecord->SetSave(pConfigRecordInfo->GetSave());
             xRecord->SetCache(pConfigRecordInfo->GetCache());
			 xRecord->SetUpload(pConfigRecordInfo->GetUpload());
            
            pObject->AddRecordCallBack(pConfigRecordInfo->GetName(), this, &NFCKernelModule::OnRecordCommonEvent);

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


		pObject->SetState(COE_CREATE_LOADDATA);
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

bool NFCKernelModule::DestroyObject(const NFGUID& self)
{
    if (self == mnCurExeObject
        && !self.IsNull())
    {
        
        return DestroySelf(self);
    }

    
    int nGroupID = GetPropertyInt32(self, NFrame::IObject::GroupID());
    int nSceneID = GetPropertyInt32(self, NFrame::IObject::SceneID());

    NF_SHARE_PTR<NFCSceneInfo> pContainerInfo = m_pSceneModule->GetElement(nSceneID);
    if (pContainerInfo)
    {
        const std::string& strClassName = GetPropertyString(self, NFrame::IObject::ClassName());

        pContainerInfo->RemoveObjectFromGroup(nGroupID, self, strClassName == NFrame::Player::ThisName() ? true : false);

        DoEvent(self, strClassName, COE_BEFOREDESTROY, NFDataList());
        DoEvent(self, strClassName, COE_DESTROY, NFDataList());

        RemoveElement(self);

		m_pEventModule->RemoveEventCallBack(self);
		m_pScheduleModule->RemoveSchedule(self);

        return true;

    }

    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "There is no scene", nSceneID, __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::FindProperty(const NFGUID& self, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->FindProperty(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyInt(const NFGUID& self, const std::string& strPropertyName, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyInt(strPropertyName, nValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyFloat(const NFGUID& self, const std::string& strPropertyName, const double dValue)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyFloat(strPropertyName, dValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyString(const NFGUID& self, const std::string& strPropertyName, const std::string& strValue)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyString(strPropertyName, strValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyObject(const NFGUID& self, const std::string& strPropertyName, const NFGUID& objectValue)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyObject(strPropertyName, objectValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyVector2(const NFGUID& self, const std::string& strPropertyName, const NFVector2& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyVector2(strPropertyName, value);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector2", __FUNCTION__, __LINE__);

	return false;
}

bool NFCKernelModule::SetPropertyVector3(const NFGUID& self, const std::string& strPropertyName, const NFVector3& value)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->SetPropertyVector3(strPropertyName, value);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector3", __FUNCTION__, __LINE__);

	return false;
}

NFINT64 NFCKernelModule::GetPropertyInt(const NFGUID& self, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyInt(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return NULL_INT;
}

int NFCKernelModule::GetPropertyInt32(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyInt32(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

	return (int)NULL_INT;
}

double NFCKernelModule::GetPropertyFloat(const NFGUID& self, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyFloat(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return NULL_FLOAT;
}

const std::string& NFCKernelModule::GetPropertyString(const NFGUID& self, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyString(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return NULL_STR;
}

const NFGUID& NFCKernelModule::GetPropertyObject(const NFGUID& self, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyObject(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return NULL_OBJECT;
}

const NFVector2& NFCKernelModule::GetPropertyVector2(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyVector2(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector2", __FUNCTION__, __LINE__);

	return NULL_VECTOR2;
}

const NFVector3& NFCKernelModule::GetPropertyVector3(const NFGUID& self, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetPropertyVector3(strPropertyName);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no vector3", __FUNCTION__, __LINE__);

	return NULL_VECTOR3;
}

NF_SHARE_PTR<NFIRecord> NFCKernelModule::FindRecord(const NFGUID& self, const std::string& strRecordName)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordManager()->GetElement(strRecordName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

    return nullptr;
}

bool NFCKernelModule::ClearRecord(const NFGUID& self, const std::string& strRecordName)
{
    NF_SHARE_PTR<NFIRecord> pRecord =  FindRecord(self, strRecordName);
    if (pRecord)
    {
        return pRecord->Clear();
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no record", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
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

bool NFCKernelModule::SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
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

bool NFCKernelModule::SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
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

bool NFCKernelModule::SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
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

bool NFCKernelModule::SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
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

bool NFCKernelModule::SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
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

bool NFCKernelModule::SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue)
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

bool NFCKernelModule::SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
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

bool NFCKernelModule::SetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& value)
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

bool NFCKernelModule::SetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value)
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

bool NFCKernelModule::SetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& value)
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

bool NFCKernelModule::SetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value)
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

NFINT64 NFCKernelModule::GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordInt(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0;
}

NFINT64 NFCKernelModule::GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordInt(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0;
}

double NFCKernelModule::GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordFloat(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0.0;
}

double NFCKernelModule::GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordFloat(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0.0;
}

const std::string& NFCKernelModule::GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordString(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return NULL_STR;
}

const std::string& NFCKernelModule::GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordString(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return NULL_STR;
}

const NFGUID& NFCKernelModule::GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordObject(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

    return NULL_OBJECT;
}

const NFGUID& NFCKernelModule::GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordObject(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

    return NULL_OBJECT;
}

const NFVector2& NFCKernelModule::GetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector2(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector2", __FUNCTION__, __LINE__);

	return NULL_VECTOR2;
}

const NFVector2& NFCKernelModule::GetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector2(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector2", __FUNCTION__, __LINE__);

	return NULL_VECTOR2;
}

const NFVector3& NFCKernelModule::GetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector3(strRecordName, nRow, nCol);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector3", __FUNCTION__, __LINE__);

	return NULL_VECTOR3;
}

const NFVector3& NFCKernelModule::GetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIObject> pObject = GetElement(self);
	if (pObject)
	{
		return pObject->GetRecordVector3(strRecordName, nRow, strColTag);
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no vector3", __FUNCTION__, __LINE__);

	return NULL_VECTOR3;
}

NFGUID NFCKernelModule::CreateGUID()
{
    int64_t value = 0;   
    uint64_t time = NFGetTime();

    
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

bool NFCKernelModule::CreateScene(const int nSceneID)
{
    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo)
    {
        return false;
    }

    pSceneInfo = NF_SHARE_PTR<NFCSceneInfo>(NF_NEW NFCSceneInfo(nSceneID));
    if (pSceneInfo)
    {
        m_pSceneModule->AddElement(nSceneID, pSceneInfo);
        
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = NF_SHARE_PTR<NFCSceneGroupInfo>(NF_NEW NFCSceneGroupInfo(nSceneID, 0));
        if (NULL != pGroupInfo)
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
    while (pSceneInfo)
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->First();
        while (pGroupInfo)
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

int NFCKernelModule::RequestGroupScene(const int nSceneID)
{
    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo)
    {
        int nNewGroupID = pSceneInfo->NewGroupID();
        NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
        if (pSceneInfo)
        {
            if (!pSceneInfo->GetElement(nNewGroupID))
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
	if (pSceneInfo)
	{
		if (nGroupID > 0)
		{
			m_pSceneModule->DestroySceneNPC(nSceneID, nGroupID);

			pSceneInfo->RemoveElement(nGroupID);
		}
	}

    return false;
}

bool NFCKernelModule::ExitGroupScene(const int nSceneID, const int nGroupID)
{
    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo)
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo)
        {
            return true;
        }
    }

    return false;
}

bool NFCKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList & list, const NFGUID & noSelf)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{

		NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
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

bool NFCKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList& list)
{
	return GetGroupObjectList(nSceneID, nGroupID, list, NFGUID());
}

bool NFCKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList & list, const bool bPlayer, const NFGUID & noSelf)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{

		NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(nGroupID);
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

bool NFCKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList & list, const bool bPlayer)
{
	return GetGroupObjectList(nSceneID, nGroupID, list, bPlayer, NFGUID());
}

bool NFCKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string & strClassName, NFDataList& list, const NFGUID& noSelf)
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

bool NFCKernelModule::GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string & strClassName, NFDataList & list)
{
	return GetGroupObjectList(nSceneID, nGroupID, strClassName, list, NFGUID());
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
    
    NF_SHARE_PTR<NFIObject> pObject = GetObject(ident);
    if (pObject)
    {
		int nSceneID = GetPropertyInt32(ident, NFrame::IObject::SceneID());
		int nGroupID = GetPropertyInt32(ident, NFrame::IObject::GroupID());

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//----------child object list-------- SceneID = ", nSceneID);

        NFDataList valObjectList;
		GetGroupObjectList(nSceneID, nGroupID, valObjectList);
        for (int i  = 0; i < valObjectList.GetCount(); i++)
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

int NFCKernelModule::OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
	NF_SHARE_PTR<NFIObject> xObject = GetElement(self);
	if (xObject)
	{
		if (xObject->GetState() == CLASS_OBJECT_EVENT::COE_CREATE_HASDATA
			|| xObject->GetState() == CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
		{
			std::list<PROPERTY_EVENT_FUNCTOR_PTR>::iterator it = mtCommonPropertyCallBackList.begin();
			for (it; it != mtCommonPropertyCallBackList.end(); it++)
			{
				PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *it;
				PROPERTY_EVENT_FUNCTOR* pFun = pFunPtr.get();
				pFun->operator()(self, strPropertyName, oldVar, newVar);
			}
		}
	}
	
    return 0;
}

NF_SHARE_PTR<NFIObject> NFCKernelModule::GetObject(const NFGUID& ident)
{
    return GetElement(ident);
}

int NFCKernelModule::GetObjectByProperty(const int nSceneID, const int nGroupID, const std::string& strPropertyName, const NFDataList& valueArg, NFDataList& list)
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

bool NFCKernelModule::ExistScene(const int nSceneID)
{
    NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
    if (pSceneInfo)
    {
        return true;
    }

    return false;
}

bool NFCKernelModule::ExistObject(const NFGUID & ident)
{
	return ExistElement(ident);
}

bool NFCKernelModule::ExistObject(const NFGUID & ident, const int nSceneID, const int nGroupID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = m_pSceneModule->GetElement(nSceneID);
	if (pSceneInfo)
	{
		return true;
	}

	return pSceneInfo->ExistObjectInGroup(nGroupID, ident);
}

bool NFCKernelModule::DestroySelf(const NFGUID& self)
{
    mtDeleteSelfList.push_back(self);
    return true;
}

int NFCKernelModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
	NF_SHARE_PTR<NFIObject> xObject = GetElement(self);
	if (xObject)
	{
		if (xObject->GetState() == CLASS_OBJECT_EVENT::COE_CREATE_HASDATA
			|| xObject->GetState() == CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
		{
			std::list<RECORD_EVENT_FUNCTOR_PTR>::iterator it = mtCommonRecordCallBackList.begin();
			for (it; it != mtCommonRecordCallBackList.end(); it++)
			{
				RECORD_EVENT_FUNCTOR_PTR& pFunPtr = *it;
				RECORD_EVENT_FUNCTOR* pFun = pFunPtr.get();
				pFun->operator()(self, xEventData, oldVar, newVar);
			}
		}
	}

    return 0;
}

int NFCKernelModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    std::list<CLASS_EVENT_FUNCTOR_PTR>::iterator it = mtCommonClassCallBackList.begin();
    for (it; it != mtCommonClassCallBackList.end(); it++)
    {
        CLASS_EVENT_FUNCTOR_PTR& pFunPtr = *it;
        CLASS_EVENT_FUNCTOR* pFun = pFunPtr.get();
        pFun->operator()(self, strClassName, eClassEvent, var);
    }

    return 0;
}

bool NFCKernelModule::RegisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    mtCommonClassCallBackList.push_back(cb);
    return true;
}

bool NFCKernelModule::RegisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    mtCommonPropertyCallBackList.push_back(cb);
    return true;
}

bool NFCKernelModule::RegisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb)
{
    mtCommonRecordCallBackList.push_back(cb);
    return true;
}

bool NFCKernelModule::LogSelfInfo(const NFGUID ident)
{

    return false;
}

bool NFCKernelModule::AfterInit()
{
    NF_SHARE_PTR<NFIClass> pClass = m_pClassModule->First();
    while (pClass)
    {
        NFIKernelModule::AddClassCallBack(pClass->GetClassName(), this, &NFCKernelModule::OnClassCommonEvent);

        pClass = m_pClassModule->Next();
    }

    return true;
}

bool NFCKernelModule::DestroyAll()
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

bool NFCKernelModule::BeforeShut()
{
    DestroyAll();

	mvRandom.clear();
	mtCommonClassCallBackList.clear();
	mtCommonPropertyCallBackList.clear();
	mtCommonRecordCallBackList.clear();

    return true;
}

void NFCKernelModule::Random(int nStart, int nEnd, int nCount, NFDataList& valueList)
{
    if (mnRandomPos + nCount >= mvRandom.size())
    {
        mnRandomPos = 0;
    }

    for (int i = mnRandomPos; i < mnRandomPos + nCount; i++)
    {
        float fRanValue = mvRandom.at(i);
        int nValue = int((nEnd - nStart) * fRanValue) + nStart;
        valueList.Add((NFINT64)nValue);
    }

    mnRandomPos += nCount;
}

int NFCKernelModule::Random(int nStart, int nEnd)
{
	if (mnRandomPos + 1 >= mvRandom.size())
	{
		mnRandomPos = 0;
	}

	float fRanValue = mvRandom.at(mnRandomPos);
	mnRandomPos++;

	int nValue = int((nEnd - nStart) * fRanValue) + nStart;
	return nValue;
}

float NFCKernelModule::Random()
{
	mnRandomPos++;

	if (mnRandomPos + 1 >= mvRandom.size())
	{
		mnRandomPos = 0;
	}
	return mvRandom.at(mnRandomPos);;
}

bool NFCKernelModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    return m_pClassModule->AddClassCallBack(strClassName, cb);
}

void NFCKernelModule::ProcessMemFree()
{
    if (nLastTime + 30 > pPluginManager->GetNowTime())
    {
        return;
    }

    nLastTime = pPluginManager->GetNowTime();

    NFMemManager::GetSingletonPtr()->FreeMem();
}

bool NFCKernelModule::DoEvent(const NFGUID& self, const std::string& strClassName, CLASS_OBJECT_EVENT eEvent, const NFDataList& valueList)
{
    return m_pClassModule->DoEvent(self, strClassName, eEvent, valueList);
}