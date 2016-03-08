// -------------------------------------------------------------------------
//    @FileName         :    NFIObject.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFIObject
//
// -------------------------------------------------------------------------

#ifndef NFI_OBJECT_H
#define NFI_OBJECT_H


#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFIDataList.h"
#include "NFIRecord.h"
#include "NFIRecordManager.h"
#include "NFIHeartBeatManager.h"
#include "NFIPropertyManager.h"
#include "NFIComponentManager.h"
#include "NFIEventManager.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIObject
{
public:
    NFIObject(NFGUID self)
    {

    }
    virtual ~NFIObject() {}

    virtual bool Execute() = 0;

    virtual NFGUID Self() = 0;

    template<typename BaseType>
    bool AddPropertyCallBack(const std::string& strPropertyName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFIDataList::TData&, const NFIDataList::TData&))
    {
        PROPERTY_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
        return AddPropertyCallBack(strPropertyName, functorPtr);
    }

    template<typename BaseType>
    bool AddRecordCallBack(const std::string& strRecordName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFIDataList::TData&, const NFIDataList::TData&))
    {
        RECORD_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
        return AddRecordCallBack(strRecordName, functorPtr);
    }

    template<typename BaseType>
    bool AddHeartBeat(const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fTime, const int nCount)
    {
        HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
        return AddHeartBeat(strHeartBeatName, functorPtr, fTime, nCount);
    }

    template <typename T>
    bool AddComponent()
    {
        return GetComponentManager()->AddComponent<T>();
    }

    template <typename T>
    NF_SHARE_PTR<T> AddComponent(const std::string& strComponentName)
    {
        NFIComponent* pComponent = m_pPluginManager->FindComponent(strComponentName);
        if (pComponent)
        {
            NF_SHARE_PTR<T> pNewCOmponent = pComponent->CreateNewInstance<T>();
            if (nullptr != pNewCOmponent && GetComponentManager()->AddComponent(strComponentName, pNewCOmponent))
            {
                return pNewCOmponent;
            }
        }

        return NF_SHARE_PTR<T>();
    }

    template <typename T>
    NF_SHARE_PTR<T> FindComponent(const std::string& strComponentName)
    {
        return GetComponentManager()->FindComponent<T>(strComponentName);
    }

    virtual bool FindHeartBeat(const std::string& strHeartBeatName) = 0;

    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName) = 0;

    /////////////////////////////////////////////////////////////////

    virtual bool FindProperty(const std::string& strPropertyName) = 0;

    virtual bool SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue) = 0;
    virtual bool SetPropertyFloat(const std::string& strPropertyName, const double dwValue) = 0;
    virtual bool SetPropertyString(const std::string& strPropertyName, const std::string& strValue) = 0;
    virtual bool SetPropertyObject(const std::string& strPropertyName, const NFGUID& obj) = 0;

    virtual NFINT64 GetPropertyInt(const std::string& strPropertyName) = 0;
    virtual double GetPropertyFloat(const std::string& strPropertyName) = 0;
    virtual const std::string& GetPropertyString(const std::string& strPropertyName) = 0;
    virtual const NFGUID& GetPropertyObject(const std::string& strPropertyName) = 0;

    virtual bool FindRecord(const std::string& strRecordName) = 0;

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue) = 0;
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue) = 0;
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj) = 0;

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value) = 0;
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value) = 0;
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value) = 0;
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value) = 0;

    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;

    //  virtual NF_SHARE_PTR<NFIComponent> AddComponent(const std::string& strComponentName, const std::string& strLanguageName) = 0;
    //  virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName) = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager() = 0;
    virtual NF_SHARE_PTR<NFIHeartBeatManager> GetHeartBeatManager() = 0;
    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager() = 0;
    virtual NF_SHARE_PTR<NFIComponentManager> GetComponentManager() = 0;
    virtual NF_SHARE_PTR<NFIEventManager> GetEventManager() = 0;


protected:
    virtual bool AddRecordCallBack(const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual bool AddPropertyCallBack(const std::string& strPropertyName, const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual bool AddHeartBeat(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;

protected:
    NFIPluginManager* m_pPluginManager;

private:
};

#endif