// -------------------------------------------------------------------------
//    @FileName      :    NFCElementInfoModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCElementInfoModule
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
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
    m_pLogicClassModule = dynamic_cast<NFCLogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

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

    std::shared_ptr<NFILogicClass> pLogicClass = m_pLogicClassModule->First();
    while (pLogicClass.get())
    {
        const std::string& strInstancePath = pLogicClass->GetInstancePath();
        if (strInstancePath.length() == 0)
        {
            pLogicClass = m_pLogicClassModule->Next();
            continue;
        }

        rapidxml::file<> fdoc(strInstancePath.c_str());
        //std::cout << fdoc.data() << std::endl;
        rapidxml::xml_document<>  doc;
        doc.parse<0>(fdoc.data());

        //support for unlimited layer class inherits
        rapidxml::xml_node<>* root = doc.first_node();
        for (rapidxml::xml_node<>* attrNode = root->first_node(); attrNode; attrNode = attrNode->next_sibling())
        {
            Load(attrNode, pLogicClass);
        }

        mbLoaded = true;

        pLogicClass = m_pLogicClassModule->Next();
    }

    return true;
}

bool NFCElementInfoModule::Load(rapidxml::xml_node<>* attrNode, std::shared_ptr<NFILogicClass> pLogicClass)
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
    
    std::shared_ptr<ElementConfigInfo> pElementInfo(NF_NEW ElementConfigInfo());
    AddElement(strConfigID, pElementInfo);

    //can find all configid by class name
    pLogicClass->AddConfigName(strConfigID);

    //ElementConfigInfo* pElementInfo = CreateElement( strConfigID, pElementInfo );
    std::shared_ptr<NFIPropertyManager> pElementPropertyManager = pElementInfo->GetPropertyManager();
    std::shared_ptr<NFIRecordManager> pElementRecordManager = pElementInfo->GetRecordManager();

    //1.add property
    //2.set the default value  of them
    std::shared_ptr<NFIPropertyManager> pClassPropertyManager = pLogicClass->GetPropertyManager();
    std::shared_ptr<NFIRecordManager> pClassRecordManager = pLogicClass->GetRecordManager();
    if (pClassPropertyManager.get() && pClassRecordManager.get())
    {
        std::shared_ptr<NFIProperty> pProperty = pClassPropertyManager->First();
        while (pProperty.get())
        {

            pElementPropertyManager->AddProperty(0, pProperty);

            pProperty = pClassPropertyManager->Next();
        }

        std::shared_ptr<NFIRecord> pRecord = pClassRecordManager->First();
        while (pRecord.get())
        {
            pElementRecordManager->AddRecord(0, pRecord->GetName(), pRecord->GetInitData(), pRecord->GetKeyState(), pRecord->GetInitDesc(), pRecord->GetTag(), pRecord->GetRelatedRecord(), pRecord->GetRows(), pRecord->GetPublic(), pRecord->GetPrivate(), pRecord->GetSave(), pRecord->GetView(), pRecord->GetIndex());
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

        std::shared_ptr<NFIProperty> temProperty = pElementPropertyManager->GetElement(pstrConfigName);
        if (!temProperty)
        {
            continue;
        }

        NFIDataList::TData var;
        TDATA_TYPE eType = temProperty->GetType();
        var.nType = eType;
        switch (eType)
        {
            case TDATA_INT:
            {
                if (!LegalNumber(pstrConfigValue))
                {
                    NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                }
                var.variantData = boost::lexical_cast<NFINT64>(pstrConfigValue);
            }
            break;
            case TDATA_FLOAT:
            {
                if (strlen(pstrConfigValue) <= 0)
                {
                    NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                }
                var.variantData = (float)atof(pstrConfigValue);
            }
            break;
            case TDATA_DOUBLE:
            {
                if (strlen(pstrConfigValue) <= 0)
                {
                    NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                }
                var.variantData = (double)atof(pstrConfigValue);
            }
            break;
            case TDATA_STRING:
                var.variantData = std::string(pstrConfigValue);
                break;
            case TDATA_OBJECT:
            {
                if (strlen(pstrConfigValue) <= 0)
                {
                    NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                }
                var.variantData = NFIDENTID(0);
            }

            break;
            //case TDATA_POINTER:
            //    var.variantData = (void*)NULL;
            //    break;
            default:
                NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                break;
        }

        pElementPropertyManager->SetProperty(pstrConfigName, var);
    }

    NFCDataList varClassName;
    varClassName << pLogicClass->GetClassName().c_str();
    pElementPropertyManager->SetProperty("ClassName", *varClassName.GetStackConst(0));

    return true;
}

bool NFCElementInfoModule::Save()
{
    return true;
}

NFINT64 NFCElementInfoModule::GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetInt();
    }

    return 0;
}

float NFCElementInfoModule::GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetFloat();
    }

    return 0.0f;
}

double NFCElementInfoModule::GetPropertyDouble(const std::string& strConfigName, const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetDouble();
    }

    return 0.0;
}

const std::string& NFCElementInfoModule::GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName)
{
    std::shared_ptr<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetString();
    }

    return  NULL_STR;
}

std::shared_ptr<NFIProperty> NFCElementInfoModule::GetProperty(const std::string& strConfigName, const std::string& strPropertyName)
{
    std::shared_ptr<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return pElementInfo->GetPropertyManager()->GetElement(strPropertyName);
    }

    return NULL;
}

std::shared_ptr<NFIPropertyManager> NFCElementInfoModule::GetPropertyManager(const std::string& strConfigName)
{
    std::shared_ptr<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return pElementInfo->GetPropertyManager();
    }

    return NULL;
}

std::shared_ptr<NFIRecordManager> NFCElementInfoModule::GetRecordManager(const std::string& strConfigName)
{
    std::shared_ptr<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
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

    std::shared_ptr<NFILogicClass> pLogicClass = m_pLogicClassModule->GetElement(strClassName.c_str());
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
    std::shared_ptr<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return true;
    }

    return false;
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

bool NFCElementInfoModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;

}

bool NFCElementInfoModule::Clear()
{
    ClearAll();

    mbLoaded = false;
    return true;
}

std::shared_ptr<NFIComponentManager> NFCElementInfoModule::GetComponentManager(const std::string& strConfigName)
{
    std::shared_ptr<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo.get())
    {
        return pElementInfo->GetComponentManager();
    }

    return std::shared_ptr<NFIComponentManager>(NULL);
}
