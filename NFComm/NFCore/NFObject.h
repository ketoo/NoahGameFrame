/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License") override ;
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/



#ifndef NF_OBJECT_H
#define NF_OBJECT_H

#include <iostream>
#include <string>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFObject
    : public NFIObject
{
private:
    NFObject() : NFIObject(NFGUID())
    {
		mObjectEventState = COE_CREATE_NODATA;
    }

public:
    NFObject(NFGUID self, NFIPluginManager* pLuginManager);
    virtual ~NFObject();

    ///////////////////////////////////////////////////////////////////////

    virtual NFGUID Self() override ;

    virtual void ToMemoryCounterString(std::string& info) override ;
    /////////////////////////////////////////////////////////////////

	virtual CLASS_OBJECT_EVENT GetState() override ;
	virtual void SetState(const CLASS_OBJECT_EVENT eState) override ;
    virtual bool ObjectReady() override ;

    virtual bool FindProperty(const std::string& propertyName) override ;
    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const std::string& propertyName, const NFDATA_TYPE varType) override;

    virtual bool SetPropertyInt(const std::string& propertyName, const NFINT64 nValue, const NFINT64 reason = 0) override ;
    virtual bool SetPropertyFloat(const std::string& propertyName, const double dwValue, const NFINT64 reason = 0) override ;
    virtual bool SetPropertyString(const std::string& propertyName, const std::string& value, const NFINT64 reason = 0) override ;
    virtual bool SetPropertyObject(const std::string& propertyName, const NFGUID& obj, const NFINT64 reason = 0) override ;
	virtual bool SetPropertyVector2(const std::string& propertyName, const NFVector2& value, const NFINT64 reason = 0) override ;
	virtual bool SetPropertyVector3(const std::string& propertyName, const NFVector3& value, const NFINT64 reason = 0) override ;

    virtual NFINT64 GetPropertyInt(const std::string& propertyName) override ;
	virtual int GetPropertyInt32(const std::string& propertyName) override ;
    virtual double GetPropertyFloat(const std::string& propertyName) override ;
    virtual const std::string& GetPropertyString(const std::string& propertyName) override ;
    virtual const NFGUID& GetPropertyObject(const std::string& propertyName) override ;
	virtual const NFVector2& GetPropertyVector2(const std::string& propertyName) override ;
	virtual const NFVector3& GetPropertyVector3(const std::string& propertyName) override ;

    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const std::string& recordName) override ;

    virtual bool SetRecordInt(const std::string& recordName, const int row, const int col, const NFINT64 nValue) override ;
    virtual bool SetRecordFloat(const std::string& recordName, const int row, const int col, const double dwValue) override ;
    virtual bool SetRecordString(const std::string& recordName, const int row, const int col, const std::string& value) override ;
    virtual bool SetRecordObject(const std::string& recordName, const int row, const int col, const NFGUID& obj) override ;
	virtual bool SetRecordVector2(const std::string& recordName, const int row, const int col, const NFVector2& value) override ;
	virtual bool SetRecordVector3(const std::string& recordName, const int row, const int col, const NFVector3& value) override ;

    virtual bool SetRecordInt(const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value) override ;
    virtual bool SetRecordFloat(const std::string& recordName, const int row, const std::string& colTag, const double value) override ;
    virtual bool SetRecordString(const std::string& recordName, const int row, const std::string& colTag, const std::string& value) override ;
    virtual bool SetRecordObject(const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value) override ;
	virtual bool SetRecordVector2(const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value) override ;
	virtual bool SetRecordVector3(const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value) override ;


    virtual NFINT64 GetRecordInt(const std::string& recordName, const int row, const int col) override ;
    virtual double GetRecordFloat(const std::string& recordName, const int row, const int col) override ;
    virtual const std::string& GetRecordString(const std::string& recordName, const int row, const int col) override ;
    virtual const NFGUID& GetRecordObject(const std::string& recordName, const int row, const int col) override ;
	virtual const NFVector2& GetRecordVector2(const std::string& recordName, const int row, const int col) override ;
	virtual const NFVector3& GetRecordVector3(const std::string& recordName, const int row, const int col) override ;

    virtual NFINT64 GetRecordInt(const std::string& recordName, const int row, const std::string& colTag) override ;
    virtual double GetRecordFloat(const std::string& recordName, const int row, const std::string& colTag) override ;
    virtual const std::string& GetRecordString(const std::string& recordName, const int row, const std::string& colTag) override ;
    virtual const NFGUID& GetRecordObject(const std::string& recordName, const int row, const std::string& colTag) override ;
	virtual const NFVector2& GetRecordVector2(const std::string& recordName, const int row, const std::string& colTag) override ;
	virtual const NFVector3& GetRecordVector3(const std::string& recordName, const int row, const std::string& colTag) override ;

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager() override ;
    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager() override ;

	virtual void SetRecordManager(NF_SHARE_PTR<NFIRecordManager> xRecordManager) override ;
	virtual void SetPropertyManager(NF_SHARE_PTR<NFIPropertyManager> xPropertyManager) override ;

protected:
    virtual bool AddRecordCallBack(const std::string& recordName, const RECORD_EVENT_FUNCTOR_PTR& cb) override ;

    virtual bool AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb) override ;

private:
    NFGUID mSelf;
	CLASS_OBJECT_EVENT mObjectEventState;
    NF_SHARE_PTR<NFIRecordManager> m_pRecordManager;
    NF_SHARE_PTR<NFIPropertyManager> m_pPropertyManager;

};

#endif