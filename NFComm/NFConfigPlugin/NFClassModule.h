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


#ifndef NF_LOGICCLASS_MODULE_H
#define NF_LOGICCLASS_MODULE_H

#include <string>
#include <map>
#include <iostream>
#include <thread>
#include "NFElementModule.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFRecord.h"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFPropertyManager.h"
#include "NFComm/NFCore/NFRecordManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFClass : public NFIClass
{
public:

    NFClass(const std::string& className)
    {
        mParentClass = NULL;
        mClassName = className;

        mPropertyManager = NF_SHARE_PTR<NFIPropertyManager>(NF_NEW NFPropertyManager(NFGUID()));
        mRecordManager = NF_SHARE_PTR<NFIRecordManager>(NF_NEW NFRecordManager(NFGUID()));
    }

    virtual ~NFClass()
    {
        ClearAll();
    }

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager()
    {
        return mPropertyManager;
    }

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager()
    {
        return mRecordManager;
    }

    virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb)
    {
        return mClassEventInfo.Add(cb);
    }

    virtual bool DoEvent(const NFGUID& objectID, const CLASS_OBJECT_EVENT classEvent, const NFDataList& valueList)
    {
        CLASS_EVENT_FUNCTOR_PTR cb;
        bool bRet = mClassEventInfo.First(cb);
        while (bRet)
        {
            cb->operator()(objectID, mClassName, classEvent, valueList);

            bRet = mClassEventInfo.Next(cb);
        }

        return true;
    }

    void SetParent(NF_SHARE_PTR<NFIClass> pClass)
    {
        mParentClass = pClass;
    }

    NF_SHARE_PTR<NFIClass> GetParent()
    {
        return mParentClass;
    }

    void SetTypeName(const char* strType)
    {
        mType = strType;
    }

    const std::string& GetTypeName()
    {
        return mType;
    }

    const std::string& GetClassName()
    {
        return mClassName;
    }

    const bool AddId(std::string& strId)
    {
		mIdList.push_back(strId);
        return true;
    }

	const std::vector<std::string>& GetIDList()
    {
        return mIdList;
    }

    void SetInstancePath(const std::string& strPath)
    {
        mClassInstancePath = strPath;
    }

    const std::string& GetInstancePath()
    {
        return mClassInstancePath;
    }

private:
    NF_SHARE_PTR<NFIPropertyManager> mPropertyManager;
    NF_SHARE_PTR<NFIRecordManager> mRecordManager;

    NF_SHARE_PTR<NFIClass> mParentClass;
    std::string mType;
    std::string mClassName;
    std::string mClassInstancePath;

    std::vector<std::string> mIdList;

    NFList<CLASS_EVENT_FUNCTOR_PTR> mClassEventInfo;
};

class NFClassModule
    : public NFIClassModule
{
private:
    NFClassModule();
public:
    NFClassModule(NFIPluginManager* p);
    virtual ~NFClassModule() override ;
	
	
	virtual bool Awake() override ;
    virtual bool Init() override ;
    virtual bool AfterInit() override ;

	virtual bool Shut() override ;

    virtual bool Load() override ;
    virtual bool Save() override ;
    virtual bool Clear() override ;

    virtual NFIClassModule* GetThreadClassModule() override;
	virtual NFIClassModule* GetThreadClassModule(const int index) override;

    virtual bool AddClassCallBack(const std::string& className, const CLASS_EVENT_FUNCTOR_PTR& cb) override ;
    virtual bool DoEvent(const NFGUID& objectID, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& valueList) override ;

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& className) override ;
    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& className) override ;

    virtual bool AddClass(const std::string& className, const std::string& strParentName) override ;

protected:
    virtual NFDATA_TYPE ComputerType(const char* pstrTypeName, NFData& var);
    virtual bool AddProperties(rapidxml::xml_node<>* pPropertyRootNode, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddRecords(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddComponents(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddClassInclude(const char* pstrClassFilePath, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddClass(const char* pstrClassFilePath, NF_SHARE_PTR<NFIClass> pClass);

    
    virtual bool Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFIClass> pParentClass);

protected:
	struct ThreadClassModule
	{
		bool used;
		std::thread::id threadID;
		NFClassModule* classModule;
	};

	std::vector<ThreadClassModule> mThreadClasses;

protected:
    std::string mConfigFileName;
    bool mbBackup = false;
};

#endif
