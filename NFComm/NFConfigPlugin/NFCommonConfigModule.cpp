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


#include "NFCommonConfigModule.h"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"

bool NFCommonConfigModule::Init()
{
    return true;
}


bool NFCommonConfigModule::Shut()
{
    return true;
}

bool NFCommonConfigModule::Execute()
{
    return true;
}

bool NFCommonConfigModule::AfterInit()
{

    return true;
}

bool NFCommonConfigModule::ClearConfig()
{
	mmData.ClearAll();

	return true;
}

const int NFCommonConfigModule::GetFieldInt(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute)
{
	NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
	if (pSDKInfo)
	{
		NF_SHARE_PTR<CAttributeList> pParam = pSDKInfo->GetElement(strStructItemName);
		if (pParam)
		{
			return pParam->GetInt(strAttribute);
		}
	}

	return 0;
}

const std::string& NFCommonConfigModule::GetFieldString(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute)
{
	NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
	if (pSDKInfo)
	{
		NF_SHARE_PTR<CAttributeList> pParam = pSDKInfo->GetElement(strStructItemName);
		if (pParam)
		{
			return pParam->GetString(strAttribute);
		}
	}

	return NULL_STR;
}


const int NFCommonConfigModule::GetFieldInt(const std::string& strStructName, const std::string& strSDKAttribute)
{
	NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
	if (pSDKInfo)
	{
		return pSDKInfo->mmStructAttribute.GetInt(strSDKAttribute);
	}

	return 0;
}
const std::string& NFCommonConfigModule::GetFieldString(const std::string& strStructName,const std::string& strSDKAttribute)
{
	NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
	if (pSDKInfo)
	{
		return pSDKInfo->mmStructAttribute.GetString(strSDKAttribute);
	}

	return NULL_STR;
}

bool NFCommonConfigModule::LoadConfig(const std::string& strFile)
{
	try
	{
		rapidxml::file<> xFile(strFile.c_str());
		rapidxml::xml_document<> xDoc;
		xDoc.parse<0>(xFile.data());

		rapidxml::xml_node<>* pRoot = xDoc.first_node();
		if (NULL == pRoot)
		{
			return false;
		}

		for (rapidxml::xml_node<>* pRootNode = pRoot->first_node(); pRootNode; pRootNode = pRootNode->next_sibling())
		{
			NF_SHARE_PTR<CStructInfo> pAppSDKConfigInfo = mmData.GetElement(pRootNode->name());
			if (!pAppSDKConfigInfo)
			{
				pAppSDKConfigInfo = NF_SHARE_PTR<CStructInfo>(NF_NEW CStructInfo());
				mmData.AddElement(pRootNode->name(), pAppSDKConfigInfo);
			}

			for (rapidxml::xml_attribute<>* pAttribute = pRootNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
			{
				const char* pstrConfigName = pAttribute->name();
				const char* pstrConfigValue = pAttribute->value();

				pAppSDKConfigInfo->mmStructAttribute.AddElement(pstrConfigName, NF_SHARE_PTR<std::string>(NF_NEW std::string(pstrConfigValue)));
			}
			
			for (rapidxml::xml_node<>* pSDKInterfaceNode = pRootNode->first_node(); pSDKInterfaceNode; pSDKInterfaceNode = pSDKInterfaceNode->next_sibling())
			{
				if (pSDKInterfaceNode->first_attribute("ID") != NULL)
				{
					std::string strStructItemName = pSDKInterfaceNode->first_attribute("ID")->value();   
					NF_SHARE_PTR<CAttributeList> pConfigData = pAppSDKConfigInfo->GetElement(strStructItemName);
					if (!pConfigData)
					{
						pConfigData = NF_SHARE_PTR<CAttributeList>(NF_NEW CAttributeList());
						pAppSDKConfigInfo->AddElement(strStructItemName, pConfigData);
					}

					for (rapidxml::xml_attribute<>* pAttribute = pSDKInterfaceNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
					{
						const char* pstrConfigName = pAttribute->name();
						const char* pstrConfigValue = pAttribute->value();

						pConfigData->AddElement(pstrConfigName, NF_SHARE_PTR<std::string>(NF_NEW std::string(pstrConfigValue)));
					}
				}
				else
				{
					for (rapidxml::xml_attribute<>* pAttribute = pSDKInterfaceNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
					{
						const char* pstrConfigName = pAttribute->name();
						const char* pstrConfigValue = pAttribute->value();

						pAppSDKConfigInfo->mmStructAttribute.AddElement(pstrConfigName, NF_SHARE_PTR<std::string>(NF_NEW std::string(pstrConfigValue)));
					}
				}
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

std::vector<std::string> NFCommonConfigModule::GetSubKeyList(const std::string&strStructName)
{
	std::vector<std::string> xList;
	NF_SHARE_PTR<CStructInfo> pStructTypeData = mmData.GetElement(strStructName);
	if (pStructTypeData)
	{
		std::string strStructName;
		for (NF_SHARE_PTR<CAttributeList> pData = pStructTypeData->First(strStructName); pData != NULL; pData = pStructTypeData->Next(strStructName))
		{
			xList.push_back(strStructName);
		}
	}

	return xList;
}

std::vector<std::string> NFCommonConfigModule::GetFieldList(const std::string & strStructName)
{
	std::vector<std::string> xList;
	NF_SHARE_PTR<CStructInfo> pStructTypeData = mmData.GetElement(strStructName);
	if (pStructTypeData)
	{
		std::string strStructName;
		for (NF_SHARE_PTR<std::string> pData = pStructTypeData->mmStructAttribute.First(strStructName); pData != NULL; pData = pStructTypeData->mmStructAttribute.Next(strStructName))
		{
			xList.push_back(strStructName);
		}
	}

	return xList;
}
