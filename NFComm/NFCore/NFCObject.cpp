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
#include "NFCEventManager.h"

NFCObject::NFCObject(NFGUID self, NFIPluginManager* pLuginManager)
    : NFIObject(self)
{
    mSelf = self;
    m_pPluginManager = pLuginManager;

    m_pRecordManager = NF_SHARE_PTR<NFCRecordManager>(NF_NEW NFCRecordManager(mSelf));
    m_pHeartBeatManager = NF_SHARE_PTR<NFCHeartBeatManager>(NF_NEW NFCHeartBeatManager(mSelf));
    m_pPropertyManager = NF_SHARE_PTR<NFCPropertyManager>(NF_NEW NFCPropertyManager(mSelf));
    m_pComponentManager = NF_SHARE_PTR<NFCComponentManager>(NF_NEW NFCComponentManager(mSelf));
    m_pEventManager = NF_SHARE_PTR<NFIEventManager>(NF_NEW NFCEventManager(mSelf));
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

bool NFCObject::Execute()
{
    GetHeartBeatManager()->Execute();
    GetComponentManager()->Execute();
    GetEventManager()->Execute();

    return true;
}

bool NFCObject::AddHeartBeat(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
    return GetHeartBeatManager()->AddHeartBeat(mSelf , strHeartBeatName, cb, fTime, nCount);
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
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        pRecord->AddRecordHook(cb);

        return true;
    }

    return false;
}

bool NFCObject::AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strCriticalName);
    if (pProperty.get())
    {
        pProperty->RegisterCallback(cb);

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
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetInt(nValue);
    }

    return false;
}

bool NFCObject::SetPropertyFloat(const std::string& strPropertyName, const double dwValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetFloat(dwValue);
    }

    return false;
}

bool NFCObject::SetPropertyString(const std::string& strPropertyName, const std::string& strValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetString(strValue);
    }

    return false;
}

bool NFCObject::SetPropertyObject(const std::string& strPropertyName, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetObject(obj);
    }

    return false;
}

NFINT64 NFCObject::GetPropertyInt(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetInt();
    }

    return 0;
}

double NFCObject::GetPropertyFloat(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetFloat();
    }

    return 0.0;
}

const std::string& NFCObject::GetPropertyString(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetString();
    }

    return NULL_STR;
}

const NFGUID& NFCObject::GetPropertyObject(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetObject();
    }

    return NULL_OBJECT;
}

bool NFCObject::FindRecord(const std::string& strRecordName)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return true;
    }

    return false;
}

bool NFCObject::SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetInt(nRow, nCol, nValue);
    }

    return false;
}

bool NFCObject::SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetInt(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetFloat(nRow, nCol, dwValue);
    }

    return false;
}

bool NFCObject::SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetFloat(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetString(nRow, nCol, strValue.c_str());
    }

    return false;
}

bool NFCObject::SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetString(nRow, strColTag, value.c_str());
    }

    return false;
}

bool NFCObject::SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetObject(nRow, nCol, obj);
    }

    return false;
}

bool NFCObject::SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetObject(nRow, strColTag, value);
    }

    return false;
}

NFINT64 NFCObject::GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetInt(nRow, nCol);
    }

    return 0;
}

NFINT64 NFCObject::GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetInt(nRow, strColTag);
    }

    return 0;
}

double NFCObject::GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetFloat(nRow, nCol);
    }

    return 0.0;
}

double NFCObject::GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0.0;
}

const std::string& NFCObject::GetRecordString(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetString(nRow, nCol);
    }

    return NULL_STR;
}

const std::string& NFCObject::GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetString(nRow, strColTag);
    }

    return NULL_STR;
}

const NFGUID& NFCObject::GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetObject(nRow, nCol);
    }

    return NULL_OBJECT;
}

const NFGUID& NFCObject::GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetObject(nRow, strColTag);
    }

    return NULL_OBJECT;
}

NF_SHARE_PTR<NFIRecordManager> NFCObject::GetRecordManager()
{
    return m_pRecordManager;
}

NF_SHARE_PTR<NFIHeartBeatManager> NFCObject::GetHeartBeatManager()
{
    return m_pHeartBeatManager;
}

NF_SHARE_PTR<NFIPropertyManager> NFCObject::GetPropertyManager()
{
    return m_pPropertyManager;
}

NFGUID NFCObject::Self()
{
    return mSelf;
}

NF_SHARE_PTR<NFIComponentManager> NFCObject::GetComponentManager()
{
    return m_pComponentManager;
}

NF_SHARE_PTR<NFIEventManager> NFCObject::GetEventManager()
{
    return m_pEventManager;
}
