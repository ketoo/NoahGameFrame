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


#ifndef NFI_OBJECT_H
#define NFI_OBJECT_H

#include "NFDataList.hpp"
#include "NFIRecord.h"
#include "NFIRecordManager.h"
#include "NFIPropertyManager.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

enum CLASS_OBJECT_EVENT
{
	COE_CREATE_NODATA,
	COE_CREATE_BEFORE_ATTACHDATA,
	COE_CREATE_LOADDATA,
	COE_CREATE_AFTER_ATTACHDATA,
	COE_CREATE_BEFORE_EFFECT,
	COE_CREATE_EFFECTDATA,
	COE_CREATE_AFTER_EFFECT,
	COE_CREATE_READY,
	COE_CREATE_HASDATA,
	COE_CREATE_FINISH,
	COE_CREATE_CLIENT_FINISH,
	COE_BEFOREDESTROY,
	COE_DESTROY,
};

#pragma warning(disable: 4275)
class _NFExport NFIObject :public NFMemoryCounter
{
#pragma warning(default: 4275)
private:
	NFIObject() : NFMemoryCounter(GET_CLASS_NAME(NFIObject), 1)
	{
	}

public:
    NFIObject(NFGUID self) : NFMemoryCounter(GET_CLASS_NAME(NFIObject), 1)
    {
    }

    virtual ~NFIObject()
	{
	}

    virtual NFGUID Self() = 0;

    template<typename BaseType>
    bool AddPropertyCallBack(const std::string& propertyName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFData&, const NFData&, const NFINT64))
    {
        PROPERTY_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
        return AddPropertyCallBack(propertyName, functorPtr);
    }

    template<typename BaseType>
    bool AddRecordCallBack(const std::string& recordName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&))
    {
        RECORD_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
        return AddRecordCallBack(recordName, functorPtr);
    }

    /////////////////////////////////////////////////////////////////
	virtual CLASS_OBJECT_EVENT GetState() = 0;
	virtual void SetState(const CLASS_OBJECT_EVENT eState) = 0;
    virtual bool ObjectReady() = 0;

    virtual bool FindProperty(const std::string& propertyName) = 0;
    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const std::string& propertyName, const NFDATA_TYPE varType) = 0;

    virtual bool SetPropertyInt(const std::string& propertyName, const NFINT64 nValue, const NFINT64 reason = 0) = 0;
    virtual bool SetPropertyFloat(const std::string& propertyName, const double dwValue, const NFINT64 reason = 0) = 0;
    virtual bool SetPropertyString(const std::string& propertyName, const std::string& value, const NFINT64 reason = 0) = 0;
    virtual bool SetPropertyObject(const std::string& propertyName, const NFGUID& obj, const NFINT64 reason = 0) = 0;
	virtual bool SetPropertyVector2(const std::string& propertyName, const NFVector2& value, const NFINT64 reason = 0) = 0;
	virtual bool SetPropertyVector3(const std::string& propertyName, const NFVector3& value, const NFINT64 reason = 0) = 0;

    virtual NFINT64 GetPropertyInt(const std::string& propertyName) = 0;
	virtual int GetPropertyInt32(const std::string& propertyName) = 0;
    virtual double GetPropertyFloat(const std::string& propertyName) = 0;
    virtual const std::string& GetPropertyString(const std::string& propertyName) = 0;
    virtual const NFGUID& GetPropertyObject(const std::string& propertyName) = 0;
	virtual const NFVector2& GetPropertyVector2(const std::string& propertyName) = 0;
	virtual const NFVector3& GetPropertyVector3(const std::string& propertyName) = 0;

    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const std::string& recordName) = 0;

    virtual bool SetRecordInt(const std::string& recordName, const int row, const int col, const NFINT64 nValue) = 0;
    virtual bool SetRecordFloat(const std::string& recordName, const int row, const int col, const double dwValue) = 0;
    virtual bool SetRecordString(const std::string& recordName, const int row, const int col, const std::string& value) = 0;
    virtual bool SetRecordObject(const std::string& recordName, const int row, const int col, const NFGUID& obj) = 0;
	virtual bool SetRecordVector2(const std::string& recordName, const int row, const int col, const NFVector2& value) = 0;
	virtual bool SetRecordVector3(const std::string& recordName, const int row, const int col, const NFVector3& value) = 0;

    virtual bool SetRecordInt(const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value) = 0;
    virtual bool SetRecordFloat(const std::string& recordName, const int row, const std::string& colTag, const double value) = 0;
    virtual bool SetRecordString(const std::string& recordName, const int row, const std::string& colTag, const std::string& value) = 0;
    virtual bool SetRecordObject(const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value) = 0;
	virtual bool SetRecordVector2(const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value) = 0;
	virtual bool SetRecordVector3(const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value) = 0;

    virtual NFINT64 GetRecordInt(const std::string& recordName, const int row, const int col) = 0;
    virtual double GetRecordFloat(const std::string& recordName, const int row, const int col) = 0;
    virtual const std::string& GetRecordString(const std::string& recordName, const int row, const int col) = 0;
    virtual const NFGUID& GetRecordObject(const std::string& recordName, const int row, const int col) = 0;
	virtual const NFVector2& GetRecordVector2(const std::string& recordName, const int row, const int col) = 0;
	virtual const NFVector3& GetRecordVector3(const std::string& recordName, const int row, const int col) = 0;

    virtual NFINT64 GetRecordInt(const std::string& recordName, const int row, const std::string& colTag) = 0;
    virtual double GetRecordFloat(const std::string& recordName, const int row, const std::string& colTag) = 0;
    virtual const std::string& GetRecordString(const std::string& recordName, const int row, const std::string& colTag) = 0;
    virtual const NFGUID& GetRecordObject(const std::string& recordName, const int row, const std::string& colTag) = 0;
	virtual const NFVector2& GetRecordVector2(const std::string& recordName, const int row, const std::string& colTag) = 0;
	virtual const NFVector3& GetRecordVector3(const std::string& recordName, const int row, const std::string& colTag) = 0;

    //  virtual NF_SHARE_PTR<NFIComponent> AddComponent(const std::string& componentName, const std::string& strLanguageName) = 0;
    //  virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& componentName) = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager() = 0;
    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager() = 0;
	virtual void SetRecordManager(NF_SHARE_PTR<NFIRecordManager> xRecordManager) = 0;
	virtual void SetPropertyManager(NF_SHARE_PTR<NFIPropertyManager> xPropertyManager) = 0;

protected:
    virtual bool AddRecordCallBack(const std::string& recordName, const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual bool AddPropertyCallBack(const std::string& propertyName, const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;

protected:
    NFIPluginManager* m_pPluginManager;

private:
};

#endif
