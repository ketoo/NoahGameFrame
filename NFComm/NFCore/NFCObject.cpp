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

NFCObject::NFCObject(const NFIDENTID& self, NFIPluginManager* pLuginManager)
{
    mSelf = self;
    m_pPluginManager = pLuginManager;

    m_pRecordManager = NF_NEW NFCRecordManager(mSelf);
    m_pHeartBeatManager = NF_NEW NFCHeartBeatManager(mSelf);
    m_pPropertyManager = NF_NEW NFCPropertyManager(mSelf);
    m_pComponentManager = NF_NEW NFCComponentManager(mSelf);
}

NFCObject::~NFCObject()
{
    delete m_pComponentManager;
    delete m_pPropertyManager;
    delete m_pRecordManager;
    delete m_pHeartBeatManager;
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

bool NFCObject::AddHeartBeat(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIValueList& var, const float fTime, const int nCount)
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
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        pRecord->AddRecordHook(cb);

        return true;
    }

    return false;
}

bool NFCObject::AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strCriticalName);
    if (pProperty)
    {
        pProperty->RegisterCallback(cb, NFCValueList());

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

bool NFCObject::SetPropertyInt(const std::string& strPropertyName, const int nValue)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        pProperty->SetInt(nValue);

        return true;
    }

    return false;
}

bool NFCObject::SetPropertyFloat(const std::string& strPropertyName,  const float fValue)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        pProperty->SetFloat(fValue);

        return true;
    }


    return false;
}

bool NFCObject::SetPropertyDouble(const std::string& strPropertyName, const double dwValue)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        pProperty->SetDouble(dwValue);

        return true;
    }


    return false;
}

bool NFCObject::SetPropertyString(const std::string& strPropertyName, const std::string& strValue)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        pProperty->SetString(strValue);

        return true;
    }

    return false;
}

bool NFCObject::SetPropertyObject(const std::string& strPropertyName, const NFIDENTID& obj)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        pProperty->SetObject(obj);

        return true;
    }

    return false;
}

int NFCObject::GetPropertyInt(const std::string& strPropertyName)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {

        return pProperty->GetInt();
    }

    return -1;
}

float NFCObject::GetPropertyFloat(const std::string& strPropertyName)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {

        return pProperty->GetFloat();
    }

    return 0.0f;
}

double NFCObject::GetPropertyDouble(const std::string& strPropertyName)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {

        return pProperty->GetDouble();
    }

    return 0.0;
}

const std::string& NFCObject::GetPropertyString(const std::string& strPropertyName)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {

        return pProperty->GetString();
    }

    return NULL_STR;
}

NFIDENTID NFCObject::GetPropertyObject(const std::string& strPropertyName)
{
    NFIProperty* pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {

        return pProperty->GetObject();
    }

    return 0;
}

bool NFCObject::FindRecord(const std::string& strRecordName)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return true;
    }

    return false;
}

bool NFCObject::SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const int nValue)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, nCol, nValue);
    }

    return false;
}

bool NFCObject::SetRecordInt( const std::string& strRecordName, const int nRow, const std::string& strColTag, const int value )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const float fValue)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, nCol, fValue);
    }

    return false;
}


bool NFCObject::SetRecordFloat( const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetDouble(nRow, nCol, dwValue);
    }

    return false;
}

bool NFCObject::SetRecordDouble( const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetDouble(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, nCol, strValue.c_str());
    }

    return false;
}

bool NFCObject::SetRecordString( const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, strColTag, value.c_str());
    }

    return false;
}

bool NFCObject::SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, nCol, obj);
    }

    return false;
}

bool NFCObject::SetRecordObject( const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, strColTag, value);
    }

    return false;
}

int NFCObject::GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetInt(nRow, nCol);
    }

    return 0;
}

int NFCObject::GetRecordInt( const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
       return pRecord->GetInt(nRow, strColTag);
    }

    return 0;
}


float NFCObject::GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, nCol);
    }

    return 0.0f;
}


float NFCObject::GetRecordFloat( const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0;
}

double NFCObject::GetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetDouble(nRow, nCol);
    }

    return 0.0;
}

double NFCObject::GetRecordDouble( const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0.0;
}

const std::string& NFCObject::GetRecordString(const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, nCol);
    }

    return NULL_STR;
}

const std::string& NFCObject::GetRecordString( const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, strColTag);
    }

    return NULL_STR;
}

NFIDENTID NFCObject::GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, nCol);
    }

    return 0;
}

NFIDENTID NFCObject::GetRecordObject( const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIRecord* pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, strColTag);
    }

    return 0;
}

NFIRecordManager* NFCObject::GetRecordManager()
{
    return m_pRecordManager;
}

NFIHeartBeatManager* NFCObject::GetHeartBeatManager()
{
    return m_pHeartBeatManager;
}

NFIPropertyManager* NFCObject::GetPropertyManager()
{
    return m_pPropertyManager;
}

NFIDENTID NFCObject::Self()
{
    return mSelf;
}

NFIComponentManager* NFCObject::GetComponentManager()
{
    return m_pComponentManager;
}

NFIComponent* NFCObject::AddComponent( const std::string& strComponentName, const std::string& strLanguageName )
{
    return m_pComponentManager->AddComponent(strComponentName, strLanguageName);
}

NFIComponent* NFCObject::FindComponent( const std::string& strComponentName )
{
    return m_pComponentManager->FindComponent(strComponentName);
}
