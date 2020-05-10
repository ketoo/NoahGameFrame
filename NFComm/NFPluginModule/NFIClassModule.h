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

#ifndef NFI_LOGICCLASS_MODULE_H
#define NFI_LOGICCLASS_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFCore/NFIObject.h"

typedef std::function<int(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFDataList&)> CLASS_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<CLASS_EVENT_FUNCTOR> CLASS_EVENT_FUNCTOR_PTR;//EVENT

class NFIClass
    : public NFList<std::string>//include files
{
public:
    virtual ~NFIClass() {}

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager() = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager() = 0;

    virtual void SetParent(NF_SHARE_PTR<NFIClass> pClass) = 0;
    virtual NF_SHARE_PTR<NFIClass> GetParent() = 0;
    virtual void SetTypeName(const char* strType) = 0;
    virtual const std::string& GetTypeName() = 0;
    virtual const std::string& GetClassName() = 0;
    virtual const bool AddId(std::string& strConfigName) = 0;
    virtual const std::vector<std::string>& GetIDList() = 0;
    virtual const std::string& GetInstancePath() = 0;
	virtual void SetInstancePath(const std::string& strPath) = 0;

    virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
    virtual bool DoEvent(const NFGUID& objectID, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList) = 0;
};

class NFIClassModule
    : public NFIModule,
  public NFMapEx<std::string, NFIClass>
{
public:
    virtual ~NFIClassModule() {}
    virtual bool Load() = 0;
    virtual bool Save() = 0;
    virtual bool Clear() = 0;

    virtual NFIClassModule* GetThreadClassModule() = 0;
    virtual NFIClassModule* GetThreadClassModule(const int index) = 0;

    template<typename BaseType>
    bool AddClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return AddClassCallBack(strClassName, functorPtr);
    }

    virtual bool DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList) = 0;

    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& strClassName) = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& strClassName) = 0;

};

#endif