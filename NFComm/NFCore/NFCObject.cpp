// -------------------------------------------------------------------------
//    @FileName         :    NFCObject.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCObject
//
// -------------------------------------------------------------------------

#include "NFCObject.h"
#include "NFCRecordManager.h"
#include "NFCHeartBeatManager.h"
#include "NFCPropertyManager.h"
#include "NFCComponentManager.h"

NFCObject::NFCObject(NFIDENTID self, NFIPluginManager* pLuginManager)
{
    mSelf = self;
    m_pPluginManager = pLuginManager;

    m_pRecordManager = std::shared_ptr<NFCRecordManager>(NF_NEW NFCRecordManager(mSelf));
    m_pHeartBeatManager = std::shared_ptr<NFCHeartBeatManager>(NF_NEW NFCHeartBeatManager(mSelf));
    m_pPropertyManager = std::shared_ptr<NFCPropertyManager>(NF_NEW NFCPropertyManager(mSelf));
    m_pComponentManager = std::shared_ptr<NFCComponentManager>(NF_NEW NFCComponentManager(mSelf));
}

NFCObject::~NFCObject()
{

}

bool NFCObject::Init()
{
    return true;
}

bool NFCObject::Shut()
{
    return true;
}

bool NFCObject::Execute(const float fLastTime, const float fAllTime)
{
    //循环的心跳中，可能删除自己
    GetHeartBeatManager()->Execute(fLastTime, fAllTime);
    GetComponentManager()->Execute(fLastTime, fAllTime);

    return true;
}

bool NFCObject::AddHeartBeat(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIDataList& var, const float fTime, const int nCount)
{
    return GetHeartBeatManager()->AddHeartBeat(mSelf , strHeartBeatName, cb, var, fTime, nCount);
}

bool NFCObject::FindHeartBeat(const std::string& strHeartBeatName)
{
    return GetHeartBeatManager()->Exist(strHeartBeatName);
}

bool NFCObject::RemoveHeartBeat(const std::string& strHeartBeatName)
{
    return GetHeartBeatManager()->RemoveHeartBeat(strHeartBeatName);
}

bool NFCObject::AddRecordCallBack(const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        pRecord->AddRecordHook(cb);

        return true;
    }

    return false;
}

bool NFCObject::AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strCriticalName);
    if (pProperty.get())
    {
        pProperty->RegisterCallback(cb, NFCDataList());

        return true;
    }

    return false;
}

bool NFCObject::FindProperty(const std::string& strPropertyName)
{
    if (GetPropertyManager()->GetElement(strPropertyName))
    {
        return true;
    }

    return false;
}

bool NFCObject::SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetInt(nValue);
    }

    return false;
}

bool NFCObject::SetPropertyFloat(const std::string& strPropertyName,  const float fValue)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetFloat(fValue);
    }

    return false;
}

bool NFCObject::SetPropertyDouble(const std::string& strPropertyName, const double dwValue)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetDouble(dwValue);
    }

    return false;
}

bool NFCObject::SetPropertyString(const std::string& strPropertyName, const std::string& strValue)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetString(strValue);
    }

    return false;
}

bool NFCObject::SetPropertyObject(const std::string& strPropertyName, const NFIDENTID& obj)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetObject(obj);
    }

    return false;
}

NFINT64 NFCObject::GetPropertyInt(const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetInt();
    }

    return -1;
}

float NFCObject::GetPropertyFloat(const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetFloat();
    }

    return 0.0f;
}

double NFCObject::GetPropertyDouble(const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetDouble();
    }

    return 0.0;
}

const std::string& NFCObject::GetPropertyString(const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetString();
    }

    return NULL_STR;
}

NFIDENTID NFCObject::GetPropertyObject(const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetObject();
    }

    return 0;
}

bool NFCObject::FindRecord(const std::string& strRecordName)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return true;
    }

    return false;
}

bool NFCObject::SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetInt(nRow, nCol, nValue);
    }

    return false;
}

bool NFCObject::SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetInt(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const float fValue)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetFloat(nRow, nCol, fValue);
    }

    return false;
}


bool NFCObject::SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetFloat(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetDouble(nRow, nCol, dwValue);
    }

    return false;
}

bool NFCObject::SetRecordDouble(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetDouble(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetString(nRow, nCol, strValue.c_str());
    }

    return false;
}

bool NFCObject::SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetString(nRow, strColTag, value.c_str());
    }

    return false;
}

bool NFCObject::SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetObject(nRow, nCol, obj);
    }

    return false;
}

bool NFCObject::SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetObject(nRow, strColTag, value);
    }

    return false;
}

NFINT64 NFCObject::GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetInt(nRow, nCol);
    }

    return 0;
}

NFINT64 NFCObject::GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetInt(nRow, strColTag);
    }

    return 0;
}


float NFCObject::GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetFloat(nRow, nCol);
    }

    return 0.0f;
}


float NFCObject::GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0;
}

double NFCObject::GetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetDouble(nRow, nCol);
    }

    return 0.0;
}

double NFCObject::GetRecordDouble(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0.0;
}

const std::string& NFCObject::GetRecordString(const std::string& strRecordName, const int nRow, const int nCol)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetString(nRow, nCol);
    }

    return NULL_STR;
}

const std::string& NFCObject::GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetString(nRow, strColTag);
    }

    return NULL_STR;
}

NFIDENTID NFCObject::GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetObject(nRow, nCol);
    }

    return 0;
}

NFIDENTID NFCObject::GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    std::shared_ptr<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetObject(nRow, strColTag);
    }

    return 0;
}

std::shared_ptr<NFIRecordManager> NFCObject::GetRecordManager()
{
    return m_pRecordManager;
}

std::shared_ptr<NFIHeartBeatManager> NFCObject::GetHeartBeatManager()
{
    return m_pHeartBeatManager;
}

std::shared_ptr<NFIPropertyManager> NFCObject::GetPropertyManager()
{
    return m_pPropertyManager;
}

NFIDENTID NFCObject::Self()
{
    return mSelf;
}

std::shared_ptr<NFIComponentManager> NFCObject::GetComponentManager()
{
    return m_pComponentManager;
}

std::shared_ptr<NFIComponent> NFCObject::AddComponent(const std::string& strComponentName, const std::string& strLanguageName)
{
    return m_pComponentManager->AddComponent(strComponentName, strLanguageName);
}

std::shared_ptr<NFIComponent> NFCObject::FindComponent(const std::string& strComponentName)
{
    return m_pComponentManager->FindComponent(strComponentName);
}
