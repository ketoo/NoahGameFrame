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


#ifndef NF_ELEMENT_MODULE_H
#define NF_ELEMENT_MODULE_H

#include <map>
#include <string>
#include <iostream>
#include <thread>
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFRecord.h"
#include "NFComm/NFCore/NFPropertyManager.h"
#include "NFComm/NFCore/NFRecordManager.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFClass;

class ElementConfigInfo
{
public:
    ElementConfigInfo()
    {
        m_pPropertyManager = NF_SHARE_PTR<NFIPropertyManager>(NF_NEW NFPropertyManager(NFGUID()));
        m_pRecordManager = NF_SHARE_PTR<NFIRecordManager>(NF_NEW NFRecordManager(NFGUID()));
    }

    virtual ~ElementConfigInfo()
    {
    }

    NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager()
    {
        return m_pPropertyManager;
    }

    NF_SHARE_PTR<NFIRecordManager> GetRecordManager()
    {
        return m_pRecordManager;
    }

protected:

    //std::string mstrConfigID;

    NF_SHARE_PTR<NFIPropertyManager> m_pPropertyManager;
    NF_SHARE_PTR<NFIRecordManager> m_pRecordManager;
};

class NFElementModule
    : public NFIElementModule,
      NFMapEx<std::string, ElementConfigInfo>
{
private:
    NFElementModule(NFElementModule* p);

public:
    NFElementModule(NFIPluginManager* p);
    virtual ~NFElementModule();
	
	virtual bool Awake() override ;
    virtual bool Init() override ;
    virtual bool Shut() override ;

    virtual bool AfterInit() override ;
    virtual bool BeforeShut() override ;
    virtual bool Execute() override ;

    virtual bool Load() override ;
    virtual bool Save() override ;
    virtual bool Clear() override ;

    NFIElementModule* GetThreadElementModule() override;

    virtual bool LoadSceneInfo(const std::string& fileName, const std::string& className) override ;

    virtual bool ExistElement(const std::string& configName) override ;
    virtual bool ExistElement(const std::string& className, const std::string& configName) override ;

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager(const std::string& configName) override ;
    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager(const std::string& configName) override ;

    virtual NFINT64 GetPropertyInt(const std::string& configName, const std::string& propertyName) override ;
	virtual int GetPropertyInt32(const std::string& configName, const std::string& propertyName) override ;
    virtual double GetPropertyFloat(const std::string& configName, const std::string& propertyName) override ;
	virtual const std::string& GetPropertyString(const std::string& configName, const std::string& propertyName) override ;
	virtual const NFVector2 GetPropertyVector2(const std::string& configName, const std::string& propertyName) override ;
	virtual const NFVector3 GetPropertyVector3(const std::string& configName, const std::string& propertyName) override ;

	virtual const std::vector<std::string> GetListByProperty(const std::string& className, const std::string& propertyName, const NFINT64 nValue) override ;
	virtual const std::vector<std::string> GetListByProperty(const std::string& className, const std::string& propertyName, const std::string& nValue) override ;

protected:
    virtual NF_SHARE_PTR<NFIProperty> GetProperty(const std::string& configName, const std::string& propertyName);

    virtual bool Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFIClass> pLogicClass);
    virtual bool CheckRef();
	virtual bool LegalNumber(const char* str);
	virtual bool LegalFloat(const char* str);

protected:
	struct ThreadElementModule
	{
		bool used;
		std::thread::id threadID;
		NFElementModule* elementModule;
	};

	std::vector<ThreadElementModule> mThreadElements;
	NFElementModule* originalElementModule;

protected:
    NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;

    bool mbLoaded;
    bool mbBackup = false;
};

#endif
