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


#ifndef NF_PROPERTY_CONFIG_MODULE_H
#define NF_PROPERTY_CONFIG_MODULE_H

#include "Dependencies/RapidXML/rapidxml.hpp"
//#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFPropertyConfigModule
    : public NFIPropertyConfigModule
{
public:
    NFPropertyConfigModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFPropertyConfigModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool LegalLevel(const int nJob, const int nLevel);

    virtual const std::string& GetInitPropertyID(const int nJob,  const int nLevel);

    virtual void ClearInitPropertyData();

    virtual void AddInitPropertyID(const int nJob, const int nLevel, const std::string& data);

    virtual void SetEx(const bool b);

protected:
    void Load();

    std::map<NFGUID, std::string>& GetData();

private:
    bool mbExtra = false;

	//[job,Level]->ConfigID
    std::map<NFGUID, std::string> mhtCoefficientData;

    NFIClassModule* m_pClassModule;
    NFIElementModule* m_pElementModule;
};


#endif
