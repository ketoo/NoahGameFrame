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

#ifndef NFI_KERNEL_MODULE_H
#define NFI_KERNEL_MODULE_H

#include <iostream>
#include <string>
#include <functional>
#include "NFIModule.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"

class NFIKernelModule
    : public NFIModule
{

public:


	template<typename BaseType>
	bool AddPropertyCallBack(const NFGUID& self, const std::string& propertyName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFData&, const NFData&, const NFINT64))
	{
		NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
		if (pObject.get())
		{
			return pObject->AddPropertyCallBack(propertyName, pBase, handler);
		}

		return false;
	}

    template<typename BaseType>
    bool AddRecordCallBack(const NFGUID& self, const std::string& recordName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&))
    {
        NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
        if (pObject.get())
        {
            return pObject->AddRecordCallBack(recordName, pBase, handler);
        }

        return false;
    }

    ////////////////event//////////////////////////////////////////////////////////
    template<typename BaseType>
    bool AddClassCallBack(const std::string& className, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return AddClassCallBack(className, functorPtr);
    }

    virtual bool DoEvent(const NFGUID& self, const std::string& className, CLASS_OBJECT_EVENT eEvent, const NFDataList& valueList) = 0;

    //////////////////////////////////////////////////////////////////////////
    template<typename BaseType>
    bool RegisterCommonClassEvent(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return RegisterCommonClassEvent(functorPtr);
    }

    template<typename BaseType>
    bool RegisterCommonPropertyEvent(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFData&, const NFData&, const NFINT64))
    {
        PROPERTY_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        PROPERTY_EVENT_FUNCTOR_PTR functorPtr(new PROPERTY_EVENT_FUNCTOR(functor));
        return RegisterCommonPropertyEvent(functorPtr);
    }

    template<typename BaseType>
    bool RegisterCommonRecordEvent(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&))
    {
        RECORD_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        RECORD_EVENT_FUNCTOR_PTR functorPtr(new RECORD_EVENT_FUNCTOR(functor));
        return RegisterCommonRecordEvent(functorPtr);
    }

	template<typename BaseType>
	bool RegisterClassPropertyEvent(const std::string& className, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFData&, const NFData&))
	{
		PROPERTY_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		PROPERTY_EVENT_FUNCTOR_PTR functorPtr(new PROPERTY_EVENT_FUNCTOR(functor));
		return RegisterClassPropertyEvent(className, functorPtr);
	}

	template<typename BaseType>
	bool RegisterClassRecordEvent(const std::string& className, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&))
	{
		RECORD_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		RECORD_EVENT_FUNCTOR_PTR functorPtr(new RECORD_EVENT_FUNCTOR(functor));
		return RegisterClassRecordEvent(className, functorPtr);
	}

    /////////////////////////////////////////////////////////////////

    virtual bool ExistScene(const int sceneID) = 0;

	virtual bool ExistObject(const NFGUID& ident) = 0;
    virtual bool ObjectReady(const NFGUID& ident) = 0;
	virtual bool ExistObject(const NFGUID& ident, const int sceneID, const int groupID) = 0;

    virtual NF_SHARE_PTR<NFIObject> GetObject(const NFGUID& ident) = 0;
    virtual NF_SHARE_PTR<NFIObject> CreateObject(const NFGUID& self, const int sceneID, const int groupID, const std::string& className, const std::string& configIndex, const NFDataList& arg) = 0;

	virtual bool DestroySelf(const NFGUID& self) = 0;
    virtual bool DestroyObject(const NFGUID& self) = 0;
    virtual bool DestroyAll() = 0;

    virtual bool FindProperty(const NFGUID& self, const std::string& propertyName) = 0;

    virtual bool SetPropertyInt(const NFGUID& self, const std::string& propertyName, const NFINT64 nValue, const NFINT64 reason = 0) = 0;
    virtual bool SetPropertyFloat(const NFGUID& self, const std::string& propertyName, const double dValue, const NFINT64 reason = 0) = 0;
    virtual bool SetPropertyString(const NFGUID& self, const std::string& propertyName, const std::string& value, const NFINT64 reason = 0) = 0;
    virtual bool SetPropertyObject(const NFGUID& self, const std::string& propertyName, const NFGUID& objectValue, const NFINT64 reason = 0) = 0;
	virtual bool SetPropertyVector2(const NFGUID& self, const std::string& propertyName, const NFVector2& value, const NFINT64 reason = 0) = 0;
	virtual bool SetPropertyVector3(const NFGUID& self, const std::string& propertyName, const NFVector3& value, const NFINT64 reason = 0) = 0;

    virtual NFINT64 GetPropertyInt(const NFGUID& self, const std::string& propertyName) = 0;
	virtual int GetPropertyInt32(const NFGUID& self, const std::string& propertyName) = 0;
    virtual double GetPropertyFloat(const NFGUID& self, const std::string& propertyName) = 0;
    virtual const std::string& GetPropertyString(const NFGUID& self, const std::string& propertyName) = 0;
    virtual const NFGUID& GetPropertyObject(const NFGUID& self, const std::string& propertyName) = 0;
	virtual const NFVector2& GetPropertyVector2(const NFGUID& self, const std::string& propertyName) = 0;
	virtual const NFVector3& GetPropertyVector3(const NFGUID& self, const std::string& propertyName) = 0;

    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const NFGUID& self, const std::string& recordName) = 0;
    virtual bool ClearRecord(const NFGUID& self, const std::string& recordName) = 0;

    virtual bool SetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFINT64 nValue) = 0;
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const int col, const double dwValue) = 0;
    virtual bool SetRecordString(const NFGUID& self, const std::string& recordName, const int row, const int col, const std::string& value) = 0;
    virtual bool SetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFGUID& objectValue) = 0;
	virtual bool SetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFVector2& value) = 0;
	virtual bool SetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFVector3& value) = 0;

    virtual bool SetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value) = 0;
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const double value) = 0;
    virtual bool SetRecordString(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const std::string& value) = 0;
    virtual bool SetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value) = 0;
	virtual bool SetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value) = 0;
	virtual bool SetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value) = 0;

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const int col) = 0;
	inline int GetRecordInt32(const NFGUID& self, const std::string& recordName, const int row, const int col) { return (int)GetRecordInt(self, recordName, row, col); };
    virtual double GetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const int col) = 0;
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& recordName, const int row, const int col) = 0;
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const int col) = 0;
	virtual const NFVector2& GetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const int col) = 0;
	virtual const NFVector3& GetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const int col) = 0;

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag) = 0;
	inline int GetRecordInt32(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag) { return (int)GetRecordInt(self, recordName, row, colTag); };
	virtual double GetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag) = 0;
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag) = 0;
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag) = 0;
	virtual const NFVector2& GetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag) = 0;
	virtual const NFVector3& GetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag) = 0;

    ////////////////////////////////////////////////////////////////
    virtual NFGUID CreateGUID() = 0;

    virtual bool CreateScene(const int sceneID) = 0;
    virtual bool DestroyScene(const int sceneID) = 0;

    virtual int GetOnLineCount() = 0;
    virtual int GetMaxOnLineCount() = 0;

    virtual int RequestGroupScene(const int sceneID) = 0;
    virtual bool ReleaseGroupScene(const int sceneID, const int groupID) = 0;
    virtual bool ExitGroupScene(const int sceneID, const int groupID) = 0;

    virtual int GetGroupObjectList(const int sceneID, const int groupID, const bool bPlayer, const NFGUID& noSelf) = 0;
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list) = 0;
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list, const NFGUID& noSelf) = 0;
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list, const bool bPlayer) = 0;
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list, const bool bPlayer, const NFGUID& noSelf) = 0;
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, const std::string& className, NFDataList& list) = 0;
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, const std::string& className, NFDataList& list, const NFGUID& noSelf) = 0;
	
	virtual int GetObjectByProperty(const int sceneID, const int groupID, const std::string& propertyName, const NFDataList& valueArg, NFDataList& list) = 0;

    //0-10 ==>0,1,2,3,4,5,6,7,8,9
	virtual int Random(int nStart, int nEnd) = 0;
	virtual float Random() = 0;
    virtual bool LogInfo(const NFGUID ident) = 0;

protected:
    virtual bool AddClassCallBack(const std::string& className, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;


protected:
    virtual bool RegisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
    virtual bool RegisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;
    virtual bool RegisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool RegisterClassPropertyEvent(const std::string& className, const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool RegisterClassRecordEvent(const std::string& className, const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;
};

#endif
