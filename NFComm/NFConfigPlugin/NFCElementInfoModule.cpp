// -------------------------------------------------------------------------
//    @FileName      :    NFCElementInfoModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCElementInfoModule
//
// -------------------------------------------------------------------------

#include <algorithm>
#include <ctype.h>
#include "NFConfigPlugin.h"
#include "NFCElementInfoModule.h"
#include "NFCLogicClassModule.h"

////

NFCElementInfoModule::NFCElementInfoModule(NFIPluginManager* p)
{
    pPluginManager = p;
    mbLoaded = false;
}

NFCElementInfoModule::~NFCElementInfoModule()
{

}

bool NFCElementInfoModule::Init()
{
    m_pLogicClassModule = pPluginManager->FindModule<NFCLogicClassModule>("NFCLogicClassModule");

    assert(NULL != m_pLogicClassModule);

    //     Clear();
    //     Load();

    return true;
}

bool NFCElementInfoModule::Shut()
{
    Clear();
    return true;
}

bool NFCElementInfoModule::Load()
{
    if (mbLoaded)
    {
        return false;
    }

    NF_SHARE_PTR<NFILogicClass> pLogicClass = m_pLogicClassModule->First();
    while (pLogicClass.get())
    {
        const std::string& strInstancePath = pLogicClass->GetInstancePath();
        if (strInstancePath.empty())
        {
            pLogicClass = m_pLogicClassModule->Next();
            continue;
        }
        //////////////////////////////////////////////////////////////////////////
        rapidxml::xml_document<> xDoc;
        char* pData = NULL;
        int nDataSize = 0;

        std::string strFile = pPluginManager->GetConfigPath() + strInstancePath;
        if (!NFCLogicClassModule::bCipher)
        {
            rapidxml::file<> fdoc(strFile.c_str());
            nDataSize = fdoc.size();
            pData = new char[nDataSize + 1];
            strncpy(pData, fdoc.data(), nDataSize);
        }
        else
        {
            std::string strFileData;
            if (!NFCLogicClassModule::ReadFileToString(strFile, strFileData))
            {
                return false;
            }

            std::string strDecode = NFCLogicClassModule::Decode(strFileData);

            nDataSize = strDecode.length();
            pData = new char[nDataSize + 1];
            strncpy(pData, strDecode.data(), nDataSize);
        }

        pData[nDataSize] = 0;
        xDoc.parse<0>(pData);
        //////////////////////////////////////////////////////////////////////////
        //support for unlimited layer class inherits
        rapidxml::xml_node<>* root = xDoc.first_node();
        for (rapidxml::xml_node<>* attrNode = root->first_node(); attrNode; attrNode = attrNode->next_sibling())
        {
            Load(attrNode, pLogicClass);
        }

        mbLoaded = true;
        //////////////////////////////////////////////////////////////////////////
        if (NULL != pData)
        {
            delete []pData;
        }
        //////////////////////////////////////////////////////////////////////////
        pLogicClass = m_pLogicClassModule->Next();
    }

    return true;
}

bool NFCElementInfoModule::Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFILogicClass> pLogicClass)
{
    //attrNode is the node of a object
    std::string strConfigID = attrNode->first_attribute("ID")->value();
    if (strConfigID.empty())
    {
        NFASSERT(0, strConfigID, __FILE__, __FUNCTION__);
        return false;
    }

    if (ExistElement(strConfigID))
    {
        NFASSERT(0, strConfigID, __FILE__, __FUNCTION__);
        return false;
    }

    NF_SHARE_PTR<ElementConfigInfo> pElementInfo(NF_NEW ElementConfigInfo());
    AddElement(strConfigID, pElementInfo);

    //can find all configid by class name
    pLogicClass->AddConfigName(strConfigID);

    //ElementConfigInfo* pElementInfo = CreateElement( strConfigID, pElementInfo );
    NF_SHARE_PTR<NFIPropertyManager> pElementPropertyManager = pElementInfo->GetPropertyManager();
    NF_SHARE_PTR<NFIRecordManager> pElementRecordManager = pElementInfo->GetRecordManager();

    //1.add property
    //2.set the default value  of them
    NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = pLogicClass->GetPropertyManager();
    NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = pLogicClass->GetRecordManager();
    if (pClassPropertyManager.get() && pClassRecordManager.get())
    {
        NF_SHARE_PTR<NFIProperty> pProperty = pClassPropertyManager->First();
        while (pProperty.get())
        {

            pElementPropertyManager->AddProperty(NFGUID(), pProperty);

            pProperty = pClassPropertyManager->Next();
        }

        NF_SHARE_PTR<NFIRecord> pRecord = pClassRecordManager->First();
        while (pRecord.get())
        {
            NF_SHARE_PTR<NFIRecord> xRecord = pElementRecordManager->AddRecord(NFGUID(), pRecord->GetName(), pRecord->GetInitData(), pRecord->GetTag(), pRecord->GetRows());

            xRecord->SetPublic(pRecord->GetPublic());
            xRecord->SetPrivate(pRecord->GetPrivate());
            xRecord->SetSave(pRecord->GetSave());
            xRecord->SetCache(pRecord->GetCache());

            pRecord = pClassRecordManager->Next();
        }

    }

    //3.set the config value to them

    //const char* pstrConfigID = attrNode->first_attribute( "ID" );
    for (rapidxml::xml_attribute<>* pAttribute = attrNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
    {
        const char* pstrConfigName = pAttribute->name();
        const char* pstrConfigValue = pAttribute->value();
        //printf( "%s : %s\n", pstrConfigName, pstrConfigValue );

        NF_SHARE_PTR<NFIProperty> temProperty = pElementPropertyManager->GetElement(pstrConfigName);
        if (!temProperty)
        {
            continue;
        }

        NFIDataList::TData var;
        const TDATA_TYPE eType = temProperty->GetType();
        switch (eType)
        {
            case TDATA_INT:
            {
                if (!LegalNumber(pstrConfigValue))
                {
                    NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                }
                var.SetInt(lexical_cast<NFINT64>(pstrConfigValue));
            }
            break;
            case TDATA_FLOAT:
            {
                if (strlen(pstrConfigValue) <= 0)
                {
                    NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                }
                var.SetFloat((double)atof(pstrConfigValue));
            }
            break;
            case TDATA_STRING:
                {
                    var.SetString(pstrConfigValue);
                }
                break;
            case TDATA_OBJECT:
            {
                if (strlen(pstrConfigValue) <= 0)
                {
                    NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                }
                var.SetObject(NFGUID());
            }
            break;
            default:
                NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                break;
        }

        temProperty->SetValue(var);
        if (eType == TDATA_STRING)
        {
            temProperty->DeSerialization();
        }
    }

    NFIDataList::TData xData;
    xData.SetString(pLogicClass->GetClassName());
    pElementPropertyManager->SetProperty("ClassName", xData);

    return true;
}

bool NFCElementInfoModule::Save()
{
    return true;
}

NFINT64 NFCElementInfoModule::GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetInt();
    }

    return 0;
}

double NFCElementInfoModule::GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetFloat();
    }

    return 0.0;
}

const std::string& NFCElementInfoModule::GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetString();
    }

    return  NULL_STR;
}

NF_SHARE_PTR<NFIProperty> NFCElementInfoModule::GetProperty(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return pElementInfo->GetPropertyManager()->GetElement(strPropertyName);
    }

    return NULL;
}

NF_SHARE_PTR<NFIPropertyManager> NFCElementInfoModule::GetPropertyManager(const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return pElementInfo->GetPropertyManager();
    }

    return NULL;
}

NF_SHARE_PTR<NFIRecordManager> NFCElementInfoModule::GetRecordManager(const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return pElementInfo->GetRecordManager();
    }
    return NULL;
}

bool NFCElementInfoModule::LoadSceneInfo(const std::string& strFileName, const std::string& strClassName)
{
    rapidxml::file<> fdoc(strFileName.c_str());
    //std::cout << fdoc.data() << std::endl;
    rapidxml::xml_document<>  doc;
    doc.parse<0>(fdoc.data());

    NF_SHARE_PTR<NFILogicClass> pLogicClass = m_pLogicClassModule->GetElement(strClassName.c_str());
    if (pLogicClass.get())
    {
        //support for unlimited layer class inherits
        rapidxml::xml_node<>* root = doc.first_node();
        for (rapidxml::xml_node<>* attrNode = root->first_node(); attrNode; attrNode = attrNode->next_sibling())
        {
            Load(attrNode, pLogicClass);
        }
    }
    else
    {
        std::cout << "error load scene info failed, name is:" << strClassName << " file name is :" << strFileName << std::endl;
    }

    return true;
}

bool NFCElementInfoModule::ExistElement(const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return true;
    }

    return false;
}

bool NFCElementInfoModule::ExistElement(const std::string& strClassName, const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (!pElementInfo)
    {
        return false;
    }

    const std::string& strClass = pElementInfo->GetPropertyManager()->GetPropertyString("ClassName");
    if (strClass != strClassName)
    {
        return false;
    }

    return true;
}

bool NFCElementInfoModule::LegalNumber(const char* str)
{
    int nLen = int(strlen(str));
    if (nLen <= 0)
    {
        return false;
    }

    int nStart = 0;
    if ('-' == str[0])
    {
        nStart = 1;
    }

    for (int i = nStart; i < nLen; ++i)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

bool NFCElementInfoModule::AfterInit()
{
    return true;

}

bool NFCElementInfoModule::BeforeShut()
{
    return true;

}

bool NFCElementInfoModule::Execute()
{
    return true;

}

bool NFCElementInfoModule::Clear()
{
    ClearAll();

    mbLoaded = false;
    return true;
}

NF_SHARE_PTR<NFIComponentManager> NFCElementInfoModule::GetComponentManager(const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return pElementInfo->GetComponentManager();
    }

    return NF_SHARE_PTR<NFIComponentManager>(NULL);
}
