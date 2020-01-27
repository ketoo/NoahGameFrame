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


#include "NFPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFPropertyConfigModule::Init()
{
    return true;
}

bool NFPropertyConfigModule::Shut()
{
    return true;
}

bool NFPropertyConfigModule::Execute()
{
    return true;
}

bool NFPropertyConfigModule::AfterInit()
{
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    Load();

    return true;
}

const std::string& NFPropertyConfigModule::GetInitPropertyID(const int nJob,  const int nLevel)
{
	auto it = mhtCoefficienData.find(nJob);
	if (it != mhtCoefficienData.end())
	{
		auto refPropertyIDName = it->second.find(nLevel);
		if (refPropertyIDName != it->second.end())
		{
			return refPropertyIDName->second;
        }
	}

    return NULL_STR;
}

void NFPropertyConfigModule::Load()
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::InitProperty::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager(strId);
            if (pPropertyManager)
            {
                const int nJob = m_pElementModule->GetPropertyInt32(strId, NFrame::InitProperty::Job());
                const int nLevel = m_pElementModule->GetPropertyInt32(strId, NFrame::InitProperty::Level());

				auto xPropertyMap = mhtCoefficienData.find(nJob);
				if (xPropertyMap == mhtCoefficienData.end())
				{
					std::map<int, std::string> propertyMap;
					mhtCoefficienData.insert(std::make_pair(nJob, propertyMap));
				}
				
				xPropertyMap = mhtCoefficienData.find(nJob);

				auto xRefPropertyIDName = xPropertyMap->second.find(nLevel);
				if (xRefPropertyIDName == xPropertyMap->second.end())
				{
					xPropertyMap->second.insert(std::make_pair(nLevel, strId));
				}
            }
        }
    }
}

bool NFPropertyConfigModule::LegalLevel(const int nJob, const int nLevel)
{
	auto it = mhtCoefficienData.find(nJob);
	if (it != mhtCoefficienData.end())
	{
		auto refPropertyIDName = it->second.find(nLevel);
		if (refPropertyIDName != it->second.end())
		{
			return true;
		}
	}


    return false;
}
