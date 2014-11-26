// -------------------------------------------------------------------------
//    @FileName      :    NFCLogicClassModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogicClassModule
//
// -------------------------------------------------------------------------

#include <time.h>
#include <algorithm>
#include "NFConfigPlugin.h"
#include "NFCElementInfoModule.h"
#include "NFCLogicClassModule.h"

////

bool NFCLogicClassModule::Init()
{
    m_pElementInfoModule = dynamic_cast<NFCElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    assert(NULL != m_pElementInfoModule);

    Load();

    return true;
}

bool NFCLogicClassModule::Shut()
{
    NFILogicClass* pLogicClass = First();
    while (pLogicClass)
    {
        pLogicClass->ClearAll();

        delete pLogicClass;
        pLogicClass = NULL;

        pLogicClass = Next();
    }

    ClearAll();

    return true;
}

NFCLogicClassModule::NFCLogicClassModule(NFIPluginManager* p)
{
    mnPropertyIndex = NF_GetTickCount() % 10 + 1;

    pPluginManager = p;
    msConfigFileName = "../../NFDataCfg/Struct/LogicClass.xml";
}

NFCLogicClassModule::~NFCLogicClassModule()
{
    NFILogicClass* pLogicClass = First();
    while (NULL != pLogicClass)
    {
        pLogicClass->ClearAll();
        delete pLogicClass;
        pLogicClass = NULL;

        pLogicClass = Next();
    }

    ClearAll();
}

TDATA_TYPE NFCLogicClassModule::ComputerType(const char* pstrTypeName, NFIDataList::TData& var)
{
    if (0 == strcmp(pstrTypeName, "int"))
    {
        var.nType = TDATA_INT;
        var.variantData = (NFINT64)0;
        return TDATA_INT;
    }
    else if (0 == strcmp(pstrTypeName, "float"))
    {
        var.nType = TDATA_FLOAT;
        var.variantData = (float)0.0f;
        return TDATA_FLOAT;
    }
    else if (0 == strcmp(pstrTypeName, "string"))
    {
        var.nType = TDATA_STRING;
        var.variantData = NULL_STR;
        return TDATA_STRING;
    }
    else if (0 == strcmp(pstrTypeName, "double"))
    {
        var.nType = TDATA_DOUBLE;
        var.variantData = (double)0.0f;
        return TDATA_DOUBLE;
    }
    else if (0 == strcmp(pstrTypeName, "object"))
    {
        var.nType = TDATA_OBJECT;
        var.variantData = NFIDENTID();
        return TDATA_OBJECT;
    }

    return TDATA_UNKNOWN;
}

bool NFCLogicClassModule::AddPropertys(rapidxml::xml_node<>* pPropertyRootNode, NFCLogicClass* pClass)
{
    for (rapidxml::xml_node<>* pPropertyNode = pPropertyRootNode->first_node(); pPropertyNode; pPropertyNode = pPropertyNode->next_sibling())
    {
        if (pPropertyNode)
        {
            const char* strPropertyName = pPropertyNode->first_attribute("Id")->value();
            if (pClass->GetPropertyManager()->GetElement(strPropertyName))
            {
                //error
                NFASSERT(0, strPropertyName, __FILE__, __FUNCTION__);
                continue;
            }

            const char* pstrType = pPropertyNode->first_attribute("Type")->value();
            const char* pstrPublic = pPropertyNode->first_attribute("Public")->value();
            const char* pstrPrivate = pPropertyNode->first_attribute("Private")->value();
            const char* pstrPropertyIndex = pPropertyNode->first_attribute("Index")->value();

            const char* pstrSave = pPropertyNode->first_attribute("Save")->value();
            
            std::string strView;
            if (pPropertyNode->first_attribute("View") != NULL)
            {
                strView = pPropertyNode->first_attribute("View")->value();
            }

            const char* pstrRelationValue = NULL_STR.c_str();
            if (pPropertyNode->first_attribute("RelationValue"))
            {
                pstrRelationValue = pPropertyNode->first_attribute("RelationValue")->value();
            }

            bool bPublic = boost::lexical_cast<bool>(pstrPublic);
            bool bPrivate = boost::lexical_cast<bool>(pstrPrivate);
            bool bSave = boost::lexical_cast<bool>(pstrSave);
            bool bView = (strView.empty() ? false : (boost::lexical_cast<bool>(strView)));
            int nIndex = boost::lexical_cast<int>(pstrPropertyIndex);

            if (bPublic || bPrivate)
            {
                ++mnPropertyIndex;

                nIndex = mnPropertyIndex;
                mxPropertyIndexMap.insert(std::map<std::string, int>::value_type(strPropertyName, mnPropertyIndex));
            }

            NFIDataList::TData varProperty;
            if (TDATA_UNKNOWN == ComputerType(pstrType, varProperty))
            {
                //std::cout << "error:" << pClass->GetTypeName() << "  " << pClass->GetInstancePath() << ": " << strPropertyName << " type error!!!" << std::endl;

                NFASSERT(0, strPropertyName, __FILE__, __FUNCTION__);
            }

            //printf( " Property:%s[%s]\n", pstrPropertyName, pstrType );

            pClass->GetPropertyManager()->AddProperty(0, strPropertyName, varProperty.nType, bPublic,  bPrivate, bSave, bView, nIndex, pstrRelationValue);
        }
    }

    return true;
}

bool NFCLogicClassModule::AddRecords(rapidxml::xml_node<>* pRecordRootNode, NFCLogicClass* pClass)
{
    for (rapidxml::xml_node<>* pRecordNode = pRecordRootNode->first_node(); pRecordNode;  pRecordNode = pRecordNode->next_sibling())
    {
        if (pRecordNode)
        {
            const char* pstrRecordName = pRecordNode->first_attribute("Id")->value();

            if (pClass->GetRecordManager()->GetElement(pstrRecordName))
            {
                //error
                //file << pClass->mstrType << ":" << pstrRecordName << std::endl;
                //assert(0);
                NFASSERT(0, pstrRecordName, __FILE__, __FUNCTION__);
                continue;
            }

            const char* pstrRow = pRecordNode->first_attribute("Row")->value();
            const char* pstrCol = pRecordNode->first_attribute("Col")->value();

            const char* pstrPublic = pRecordNode->first_attribute("Public")->value();
            const char* pstrPrivate = pRecordNode->first_attribute("Private")->value();
            const char* pstrSave = pRecordNode->first_attribute("Save")->value();
            
            std::string strView;
            if (pRecordNode->first_attribute("View") != NULL)
            {
                strView = pRecordNode->first_attribute("View")->value();
            }

            const char* pstrIndex = pRecordNode->first_attribute("Index")->value();

            bool bPublic = boost::lexical_cast<bool>(pstrPublic);
            bool bPrivate = boost::lexical_cast<bool>(pstrPrivate);
            bool bSave = boost::lexical_cast<bool>(pstrSave);
            bool bView = (strView.empty() ? false : (boost::lexical_cast<bool>(strView)));
            int nIndex = boost::lexical_cast<int>(pstrIndex);

            NFCDataList recordVar;
            NFCDataList recordKey;
            NFCDataList recordDesc;
            NFCDataList recordTag;
            NFCDataList recordRelation;
            for (rapidxml::xml_node<>* recordColNode = pRecordNode->first_node(); recordColNode;  recordColNode = recordColNode->next_sibling())
            {
                //const char* pstrColName = recordColNode->first_attribute( "Id" )->value();
                NFIDataList::TData TData;
                const char* pstrColType = recordColNode->first_attribute("Type")->value();
                if (TDATA_UNKNOWN == ComputerType(pstrColType, TData))
                {
                    //assert(0);
                    NFASSERT(0, pstrRecordName, __FILE__, __FUNCTION__);
                }

                recordVar.Append(TData);
                //////////////////////////////////////////////////////////////////////////
                if (recordColNode->first_attribute("Key") != NULL)
                {
                    const char* pstrKey = recordColNode->first_attribute("Key")->value();
                    bool bKey = boost::lexical_cast<int>(pstrKey);
                    if (bKey)
                    {
                        recordKey.Add(NFINT64(1));
                    }
                    else
                    {
                        recordKey.Add(NFINT64(0));
                    }
                }

                if (recordColNode->first_attribute("Tag") != NULL)
                {
                    const char* pstrTag = recordColNode->first_attribute("Tag")->value();
                    recordTag.Add(pstrTag);
                }
                else
                {
                    recordTag.Add("");
                }

                if (recordColNode->first_attribute("RelateRecord") != NULL)
                {
                    std::string strRelationRecord = recordColNode->first_attribute("RelatedRecord")->value();
                    recordRelation.Add(strRelationRecord.c_str());
                }
                else
                {
                    recordRelation.Add("");
                }

                //////////////////////////////////////////////////////////////////////////
                if (recordColNode->first_attribute("Desc"))
                {
                    const char* pstrColDesc = recordColNode->first_attribute("Desc")->value();
                    recordDesc.Add(pstrColDesc);
                }
                else
                {
                    recordDesc.Add("");
                }

                //////////////////////////////////////////////////////////////////////////
            }

            pClass->GetRecordManager()->AddRecord(0, pstrRecordName, recordVar, recordKey, recordDesc, recordTag, recordRelation, atoi(pstrRow), bPublic, bPrivate, bSave, bView, nIndex);
        }
    }

    return true;
}

bool NFCLogicClassModule::AddComponents(rapidxml::xml_node<>* pComponentRootNode, NFCLogicClass* pClass)
{
    for (rapidxml::xml_node<>* pComponentNode = pComponentRootNode->first_node(); pComponentNode; pComponentNode = pComponentNode->next_sibling())
    {
        if (pComponentNode)
        {
            const char* strComponentName = pComponentNode->first_attribute("Name")->value();
            const char* strLanguage = pComponentNode->first_attribute("Language")->value();
            const char* strEnable = pComponentNode->first_attribute("Enable")->value();
            bool bEnable = boost::lexical_cast<int>(strEnable);
            if (bEnable)
            {
                if (pClass->GetComponentManager()->GetElement(strComponentName))
                {
                    //error
                    NFASSERT(0, strComponentName, __FILE__, __FUNCTION__);
                    continue;
                }

                pClass->GetComponentManager()->AddComponent(strComponentName, strLanguage);
            }
        }
    }

    return true;
}

bool NFCLogicClassModule::AddClassInclude(const char* pstrClassFilePath, NFCLogicClass* pClass)
{
    if (pClass->Find(pstrClassFilePath))
    {
        return false;
    }

    rapidxml::file<> fdoc(pstrClassFilePath);
    //std::cout << fdoc.data() << std::endl;
    rapidxml::xml_document<>  doc;
    doc.parse<0>(fdoc.data());

    //support for unlimited layer class inherits
    rapidxml::xml_node<>* root = doc.first_node();

    rapidxml::xml_node<>* pRropertyRootNode = root->first_node("Propertys");
    if (pRropertyRootNode)
    {
        AddPropertys(pRropertyRootNode, pClass);
    }

    //////////////////////////////////////////////////////////////////////////
    //and record
    rapidxml::xml_node<>* pRecordRootNode = root->first_node("Records");
    if (pRecordRootNode)
    {
        AddRecords(pRecordRootNode, pClass);
    }

    rapidxml::xml_node<>* pComponentRootNode = root->first_node("Components");
    if (pComponentRootNode)
    {
        AddComponents(pComponentRootNode, pClass);
    }

    //pClass->mvIncludeFile.push_back( pstrClassFilePath );
    //and include file
    rapidxml::xml_node<>* pIncludeRootNode = root->first_node("Includes");
    if (pIncludeRootNode)
    {
        for (rapidxml::xml_node<>* includeNode = pIncludeRootNode->first_node(); includeNode; includeNode = includeNode->next_sibling())
        {
            const char* pstrIncludeFile = includeNode->first_attribute("Id")->value();
            //std::vector<std::string>::iterator it = std::find( pClass->mvIncludeFile.begin(), pClass->mvIncludeFile..end(), pstrIncludeFile );

            if (AddClassInclude(pstrIncludeFile, pClass))
            {
                pClass->Add(pstrIncludeFile);
            }
        }
    }


    return true;
}

bool NFCLogicClassModule::AddClass(const char* pstrClassFilePath, NFCLogicClass* pClass)
{

    std::ofstream file;
    file.open("./Log/NFLogicClass.log");

    NFILogicClass* pParent = pClass->GetParent();
    while (pParent)
    {
        //inherited some properties form class of parent
        std::string strFileName = "";
        pParent->First(strFileName);
        while (!strFileName.empty())
        {
            if (pClass->Find(strFileName))
            {
                strFileName.clear();
                continue;
            }

            if (AddClassInclude(strFileName.c_str(), pClass))
            {
                pClass->Add(strFileName);
            }

            strFileName.clear();
            pParent->Next(strFileName);
        }

        pParent = pParent->GetParent();
    }

    //////////////////////////////////////////////////////////////////////////
    if (AddClassInclude(pstrClassFilePath, pClass))
    {
        pClass->Add(pstrClassFilePath);
    }

    file.close();

    return true;
}

bool NFCLogicClassModule::AddClass(const std::string& strClassName, const std::string& strParentName)
{
    NFILogicClass* pParentClass = GetElement(strParentName);
    NFILogicClass* pChildClass = GetElement(strClassName);
    if (!pChildClass)
    {
        pChildClass = NF_NEW NFCLogicClass(strClassName);
        AddElement(strClassName, pChildClass);
        //pChildClass = CreateElement( strClassName );

        pChildClass->SetTypeName("");
        pChildClass->SetInstancePath("");

        if (pParentClass)
        {
            pChildClass->SetParent(pParentClass);
        }

    }

    return true;
}

bool NFCLogicClassModule::Load(rapidxml::xml_node<>* attrNode, NFCLogicClass* pParentClass)
{

    const char* pstrLogicClassName = attrNode->first_attribute("Id")->value();
    const char* pstrType = attrNode->first_attribute("Type")->value();
    const char* pstrPath = attrNode->first_attribute("Path")->value();
    const char* pstrInstancePath = attrNode->first_attribute("InstancePath")->value();

    //printf( "-----------------------------------------------------\n");
    //printf( "%s:\n", pstrLogicClassName );

    NFCLogicClass* pClass = NF_NEW NFCLogicClass(pstrLogicClassName);
    AddElement(pstrLogicClassName, pClass);
    pClass->SetParent(pParentClass);
    pClass->SetTypeName(pstrType);
    pClass->SetInstancePath(pstrInstancePath);

    AddClass(pstrPath, pClass);

    for (rapidxml::xml_node<>* pDataNode = attrNode->first_node(); pDataNode; pDataNode = pDataNode->next_sibling())
    {
        //her children
        Load(pDataNode, pClass);
    }
    //printf( "-----------------------------------------------------\n");
    return true;
}

bool NFCLogicClassModule::Load()
{
    rapidxml::file<> fdoc(msConfigFileName.c_str());
    //std::cout << fdoc.data() << std::endl;
    rapidxml::xml_document<>  doc;
    doc.parse<0>(fdoc.data());

    //support for unlimited layer class inherits
    rapidxml::xml_node<>* root = doc.first_node();
    for (rapidxml::xml_node<>* attrNode = root->first_node(); attrNode; attrNode = attrNode->next_sibling())
    {
        Load(attrNode, NULL);
    }

    m_pElementInfoModule->Load();
    return true;
}

bool NFCLogicClassModule::Save()
{
    return true;
}

NFIPropertyManager* NFCLogicClassModule::GetClassPropertyManager(const std::string& strClassName)
{
    NFILogicClass* pClass = GetElement(strClassName);
    if (pClass)
    {
        return pClass->GetPropertyManager();
    }

    return NULL;
}

NFIRecordManager* NFCLogicClassModule::GetClassRecordManager(const std::string& strClassName)
{
    NFILogicClass* pClass = GetElement(strClassName);
    if (pClass)
    {
        return pClass->GetRecordManager();
    }

    return NULL;
}

NFIComponentManager* NFCLogicClassModule::GetClassComponentManager(const std::string& strClassName)
{
    NFILogicClass* pClass = GetElement(strClassName);
    if (pClass)
    {
        return pClass->GetComponentManager();
    }

    return NULL;
}

bool NFCLogicClassModule::Clear()
{
    return true;

}
