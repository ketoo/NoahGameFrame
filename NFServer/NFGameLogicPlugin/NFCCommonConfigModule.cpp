// -------------------------------------------------------------------------
//    @FileName         :    NFCCommonConfigModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCCommonConfigModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCCommonConfigModule.h"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"

bool NFCCommonConfigModule::Init()
{
    return true;
}


bool NFCCommonConfigModule::Shut()
{
    return true;
}

bool NFCCommonConfigModule::Execute()
{
    return true;
}

bool NFCCommonConfigModule::AfterInit()
{
	std::string strPlayerPath = pPluginManager->GetConfigPath();
	strPlayerPath += "NFDataCfg/Ini/Common/AwardPackConfig.xml";
	LoadConfig(strPlayerPath);

    return true;
}

bool NFCCommonConfigModule::ClearConfig()
{
	mmData.ClearAll();

	return true;
}

const int NFCCommonConfigModule::GetAttributeInt(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute)
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

const std::string& NFCCommonConfigModule::GetAttributeString(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute)
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


const int NFCCommonConfigModule::GetAttributeInt(const std::string& strStructName, const std::string& strSDKAttribute)
{
	NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
	if (pSDKInfo)
	{
		return pSDKInfo->mmStructAttribute.GetInt(strSDKAttribute);
	}

	return 0;
}
const std::string& NFCCommonConfigModule::GetAttributeString(const std::string& strStructName,const std::string& strSDKAttribute)
{
	NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
	if (pSDKInfo)
	{
		return pSDKInfo->mmStructAttribute.GetString(strSDKAttribute);
	}

	return NULL_STR;
}

bool NFCCommonConfigModule::LoadConfig(const std::string& strFile)
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

		for (rapidxml::xml_node<>* pAppSDKRootNode = pRoot->first_node(); pAppSDKRootNode; pAppSDKRootNode = pAppSDKRootNode->next_sibling())
		{
			if (pAppSDKRootNode)
			{
				rapidxml::xml_attribute<>* pNameAttribute = pAppSDKRootNode->first_attribute("Name");
				if (NULL == pNameAttribute)
				{
					continue;
				}

				std::string strName = pNameAttribute->value();
				NF_SHARE_PTR<CStructInfo> pAppSDKConfigInfo = mmData.GetElement(strName);
				if (!pAppSDKConfigInfo)
				{
					pAppSDKConfigInfo = NF_SHARE_PTR<CStructInfo>(NF_NEW CStructInfo());
					mmData.AddElement(strName, pAppSDKConfigInfo);
				}

				
				for (rapidxml::xml_attribute<>* pAttribute = pAppSDKRootNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
				{
					if (pAttribute)
					{
						const char* pstrConfigName = pAttribute->name();
						const char* pstrConfigValue = pAttribute->value();

						pAppSDKConfigInfo->mmStructAttribute.AddElement(pstrConfigName, NF_SHARE_PTR<std::string>(NF_NEW std::string(pstrConfigValue)));
					}
				}

				
				for (rapidxml::xml_node<>* pSDKInterfaceNode = pAppSDKRootNode->first_node(); pSDKInterfaceNode; pSDKInterfaceNode = pSDKInterfaceNode->next_sibling())
				{
					if (pSDKInterfaceNode)
					{
						if (pSDKInterfaceNode->first_attribute("Name") != NULL)
						{
							std::string strStructItemName = pSDKInterfaceNode->first_attribute("Name")->value();   
							NF_SHARE_PTR<CAttributeList> pConfigData = pAppSDKConfigInfo->GetElement(strStructItemName);
							if (!pConfigData)
							{
								pConfigData = NF_SHARE_PTR<CAttributeList>(NF_NEW CAttributeList());
								pAppSDKConfigInfo->AddElement(strStructItemName, pConfigData);
							}

							for (rapidxml::xml_attribute<>* pAttribute = pSDKInterfaceNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
							{
								if (pAttribute)
								{
									const char* pstrConfigName = pAttribute->name();
									const char* pstrConfigValue = pAttribute->value();

									pConfigData->AddElement(pstrConfigName, NF_SHARE_PTR<std::string>(NF_NEW std::string(pstrConfigValue)));
								}
							}
						}
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

const int NFCCommonConfigModule::GetStructItemList(const std::string&strStructName, std::vector<std::string>& xList)
{
	NF_SHARE_PTR<CStructInfo> pStructTypeData = mmData.GetElement(strStructName);
	if (pStructTypeData)
	{
		std::string strStructName;
		for (NF_SHARE_PTR<CAttributeList> pData = pStructTypeData->First(strStructName); pData != NULL; pData = pStructTypeData->Next(strStructName))
		{
			xList.push_back(strStructName);
		}
	}

	return 0;
}