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

    NFClass(const std::string& strClassName)
    {
        m_pParentClass = NULL;
        mstrClassName = strClassName;

        m_pPropertyManager = NF_SHARE_PTR<NFIPropertyManager>(NF_NEW NFPropertyManager(NFGUID()));
        m_pRecordManager = NF_SHARE_PTR<NFIRecordManager>(NF_NEW NFRecordManager(NFGUID()));
    }

    virtual ~NFClass()
    {
        ClearAll();
    }

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager()
    {
        return m_pPropertyManager;
    }

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager()
    {
        return m_pRecordManager;
    }

    virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb)
    {
        return mxClassEventInfo.Add(cb);
    }

    virtual bool DoEvent(const NFGUID& objectID, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList)
    {
        CLASS_EVENT_FUNCTOR_PTR cb;
        bool bRet = mxClassEventInfo.First(cb);
        while (bRet)
        {
            cb->operator()(objectID, mstrClassName, eClassEvent,  valueList);

            bRet = mxClassEventInfo.Next(cb);
        }

        return true;
    }

    void SetParent(NF_SHARE_PTR<NFIClass> pClass)
    {
        m_pParentClass = pClass;
    }

    NF_SHARE_PTR<NFIClass> GetParent()
    {
        return m_pParentClass;
    }

    void SetTypeName(const char* strType)
    {
        mstrType = strType;
    }

    const std::string& GetTypeName()
    {
        return mstrType;
    }

    const std::string& GetClassName()
    {
        return mstrClassName;
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
        mstrClassInstancePath = strPath;
    }

    const std::string& GetInstancePath()
    {
        return mstrClassInstancePath;
    }

private:
    NF_SHARE_PTR<NFIPropertyManager> m_pPropertyManager;
    NF_SHARE_PTR<NFIRecordManager> m_pRecordManager;

    NF_SHARE_PTR<NFIClass> m_pParentClass;
    std::string mstrType;
    std::string mstrClassName;
    std::string mstrClassInstancePath;

    std::vector<std::string> mIdList;

    NFList<CLASS_EVENT_FUNCTOR_PTR> mxClassEventInfo;
};

class NFClassModule
    : public NFIClassModule
{
private:
    NFClassModule();
public:
    NFClassModule(NFIPluginManager* p);
    virtual ~NFClassModule();
	
	
	virtual bool Awake();
    virtual bool Init();
    virtual bool AfterInit();

	virtual bool Shut();

    virtual bool Load();
    virtual bool Save();
    virtual bool Clear();

    virtual NFIClassModule* GetThreadClassModule() override;
	virtual NFIClassModule* GetThreadClassModule(const int index) override;

    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);
    virtual bool DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList);

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& strClassName);

    virtual bool AddClass(const std::string& strClassName, const std::string& strParentName);

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
    std::string msConfigFileName;
    bool mbBackup = false;
};

#endif
