/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

NFINT64 NFPropertyConfigModule::CalculateBaseValue(const int nJob, const int nLevel, const std::string& strProperty)
{
	auto xPropertyMap = mhtCoefficienData.find(nJob);
	if (xPropertyMap != mhtCoefficienData.end())
	{
		auto xRefPropertyIDName = xPropertyMap.second.find(nLevel);
		if (xRefPropertyIDName != xPropertyMap.second.end())
		{
			return m_pElementModule->GetPropertyInt(xRefPropertyIDName.second, strProperty);
		}
    }

    return 0;
}

const std::string& NFPropertyConfigModule::GetInitPropertyID(const int nJob,  const int nLevel)
{
	auto xPropertyMap = mhtCoefficienData.find(nJob);
	if (xPropertyMap != mhtCoefficienData.end())
	{
		auto element = xPropertyMap.second.find(nLevel);
        if (element != xPropertyMap.second.end())
        {
            element.get();
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
                const std::string& strID = m_pElementModule->GetPropertyString(strId, NFrame::InitProperty::ID());

				NF_SHARE_PTR <NFMapEx<int, std::string> > xPropertyMap = mhtCoefficienData.GetElement(nJob);
				if (!xPropertyMap)
				{
					xPropertyMap = NF_SHARE_PTR<NFMapEx<int, std::string>>(NF_NEW NFMapEx<int, std::string>());
					mhtCoefficienData.AddElement(nJob, xPropertyMap);
				}

				NF_SHARE_PTR<std::string> xRefPropertyIDName = xPropertyMap->GetElement(nLevel);
				if (!xRefPropertyIDName)
				{
					xRefPropertyIDName = NF_SHARE_PTR<std::string>(NF_NEW std::string(strID));
				}

				xPropertyMap->AddElement(nLevel, xRefPropertyIDName);
            }
        }
    }
}

bool NFPropertyConfigModule::LegalLevel(const int nJob, const int nLevel)
{
	NF_SHARE_PTR <NFMapEx<int, std::string> > xPropertyMap = mhtCoefficienData.GetElement(nJob);
	if (xPropertyMap)
	{
		NF_SHARE_PTR<std::string> xRefPropertyIDName = xPropertyMap->GetElement(nLevel);
		if (xRefPropertyIDName)
		{
			return true;
		}
	}


    return false;
}
