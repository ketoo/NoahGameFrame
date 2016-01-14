// -------------------------------------------------------------------------
//    @FileName         £∫    NFCCommonConfig.h
//    @Author           £∫    eliteYang
//    @Date             £∫    2014/07/16
//    @Module           £∫    NFCCommonConfig
//
// -------------------------------------------------------------------------
#ifndef NFC_COMMONCONFIG_H
#define NFC_COMMONCONFIG_H

#include "boost/lexical_cast.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFSingleton.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCCommonConfig : public NFSingleton<NFCCommonConfig>
{
public:
        class CAttributeList : public NFMapEx<std::string, std::string>
        {
        public:
            const int GetInt(const std::string& strName)
            {
                NF_SHARE_PTR<std::string> pStr = GetElement(strName);
                if (pStr && !pStr->empty())
                {
                    int nData = 0;
                    NF_StrTo(*pStr, nData);
                    return nData;
                }

                return 0;
            }

            const std::string& GetString(const std::string& strName)
            {
                NF_SHARE_PTR<std::string> pStr = GetElement(strName);
                if (pStr)
                {
                    return *pStr;
                }

                return NULL_STR;
            }
        };

        struct CStructInfo : public NFMapEx<std::string, CAttributeList>
        {
            CAttributeList  mmStructAttribute;
        };

public:
    NFCCommonConfig() 
    {
    }

    virtual ~NFCCommonConfig() {}

    virtual bool ClearConfig()
    {
        mmData.ClearAll();

        return true;
    }

    virtual const int GetAttributeInt(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute)
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

    virtual const std::string& GetAttributeString(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute)
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


    virtual const int GetAttributeInt(const std::string& strStructName, const std::string& strSDKAttribute)
    {
        NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
        if (pSDKInfo)
        {
            return pSDKInfo->mmStructAttribute.GetInt(strSDKAttribute);
        }

        return 0;
    }

    virtual const std::string& GetAttributeString(const std::string& strStructName,const std::string& strSDKAttribute)
    {
        NF_SHARE_PTR<CStructInfo> pSDKInfo = mmData.GetElement(strStructName);
        if (pSDKInfo)
        {
            return pSDKInfo->mmStructAttribute.GetString(strSDKAttribute);
        }

        return NULL_STR;
    }

    virtual bool LoadConfig(const std::string& strFile)
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

                    //CStruct Ù–‘
                    for (rapidxml::xml_attribute<>* pAttribute = pAppSDKRootNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
                    {
                        if (pAttribute)
                        {
                            const char* pstrConfigName = pAttribute->name();
                            const char* pstrConfigValue = pAttribute->value();

                            pAppSDKConfigInfo->mmStructAttribute.AddElement(pstrConfigName, NF_SHARE_PTR<std::string>(NF_NEW std::string(pstrConfigValue)));
                        }
                    }

                    //CStruct.itemdµƒ Ù–‘
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
            }        }        catch(...)        {            return false;        }

        return true;
    }

    virtual const int GetStructItemList(const std::string&strStructName, std::vector<std::string>& xList)
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

private:
    NFMapEx<std::string, CStructInfo> mmData; //strStructName<-->CStructInfo
};

#endif //