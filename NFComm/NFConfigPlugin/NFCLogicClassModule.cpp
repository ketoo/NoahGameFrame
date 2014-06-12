// -------------------------------------------------------------------------
//    @FileName      :    NFCLogicClassModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogicClassModule
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
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
    return true;
}

NFCLogicClassModule::NFCLogicClassModule(NFIPluginManager* p)
{
    pPluginManager = p;
    msConfigFileName = "../../NFDataCfg/Struct/LogicClass.xml";
}

NFCLogicClassModule::~NFCLogicClassModule()
{

}

VARIANT_TYPE NFCLogicClassModule::ComputerType(const char* pstrTypeName, NFIValueList::VarData& var)
{
    if (0 == strcmp(pstrTypeName, "int"))
    {
        var.nType = VTYPE_INT;
        var.variantData = (int)0;
        return VTYPE_INT;
    }
    else if (0 == strcmp(pstrTypeName, "float"))
    {
        var.nType = VTYPE_FLOAT;
        var.variantData = (float)0.0f;
        return VTYPE_FLOAT;
    }
    else if (0 == strcmp(pstrTypeName, "string"))
    {
        var.nType = VTYPE_STRING;
        var.variantData = NULL_STR;
        return VTYPE_STRING;
    }
    else if (0 == strcmp(pstrTypeName, "double"))
    {
        var.nType = VTYPE_DOUBLE;
        var.variantData = (double)0.0f;
        return VTYPE_DOUBLE;
    }
    else if (0 == strcmp(pstrTypeName, "object"))
    {
        var.nType = VTYPE_OBJECT;
        var.variantData = NFINT64(0);
        return VTYPE_OBJECT;
    }

    return VTYPE_UNKNOWN;
}

bool NFCLogicClassModule::AddPropertys(rapidxml::xml_node<>* propertyRootNode, NFCLogicClass* pClass)
{
    for (rapidxml::xml_node<>* propertyNode = propertyRootNode->first_node(); propertyNode; propertyNode = propertyNode->next_sibling())
    {
        if (propertyNode)
        {
            const char* strPropertyName = propertyNode->first_attribute("Id")->value();
            if (pClass->GetPropertyManager()->GetElement(strPropertyName))
            {
                //error
                NFASSERT(0, strPropertyName, __FILE__, __FUNCTION__);
                continue;
            }

            const char* pstrType = propertyNode->first_attribute("Type")->value();
            const char* pstrPublic = propertyNode->first_attribute("Public")->value();
            const char* pstrPrivate = propertyNode->first_attribute("Private")->value();
            const char* pstrPropertyIndex = propertyNode->first_attribute("Index")->value();

            const char* pstrSave = propertyNode->first_attribute("Save")->value();
            const char* pstrScriptFunction = propertyNode->first_attribute("ScriptFunction")->value();

            bool bPublic = (0 == strcmp("true", pstrPublic)) ? true : false;
            bool bPrivate = (0 == strcmp("true", pstrPrivate)) ? true : false;
            bool bSave = (0 == strcmp("true", pstrSave)) ? true : false;
            int nIndex = atoi(pstrPropertyIndex);

            NFIValueList::VarData varProperty;
            if (VTYPE_UNKNOWN == ComputerType(pstrType, varProperty))
            {
                //std::cout << "error:" << pClass->GetTypeName() << "  " << pClass->GetInstancePath() << ": " << strPropertyName << " type error!!!" << std::endl;

                NFASSERT(0, strPropertyName, __FILE__, __FUNCTION__);
            }

            //printf( " Property:%s[%s]\n", pstrPropertyName, pstrType );

            pClass->GetPropertyManager()->AddProperty(0, strPropertyName,  varProperty.nType, bPublic,  bPrivate, bSave, nIndex, pstrScriptFunction);
        }
    }

    return true;
}

bool NFCLogicClassModule::AddRecords(rapidxml::xml_node<>* recordRootNode, NFCLogicClass* pClass)
{
    for (rapidxml::xml_node<>* recordNode = recordRootNode->first_node(); recordNode;  recordNode = recordNode->next_sibling())
    {
        if (recordNode)
        {
            const char* pstrRecordName = recordNode->first_attribute("Id")->value();

            if (pClass->GetRecordManager()->GetElement(pstrRecordName))
            {
                //error
                //file << pClass->mstrType << ":" << pstrRecordName << std::endl;
                //assert(0);
                NFASSERT(0, pstrRecordName, __FILE__, __FUNCTION__);
                continue;
            }

            const char* pstrRow = recordNode->first_attribute("Row")->value();
            const char* pstrCol = recordNode->first_attribute("Col")->value();

            const char* pstrPublic = recordNode->first_attribute("Public")->value();
            const char* pstrPrivate = recordNode->first_attribute("Private")->value();
            const char* pstrSave = recordNode->first_attribute("Save")->value();
            const char* pstrIndex = recordNode->first_attribute("Index")->value();

            bool bPublic = (0 == strcmp("true", pstrPublic)) ? true : false;
            bool bPrivate = (0 == strcmp("true", pstrPrivate)) ? true : false;
            bool bSave = (0 == strcmp("true", pstrSave)) ? true : false;
            int nIndex = atoi(pstrIndex);

            NFCValueList recordVar;
            NFCValueList recordKey;
            NFCValueList recordDesc;
            NFCValueList recordTag;
            for (rapidxml::xml_node<>* recordColNode = recordNode->first_node(); recordColNode;  recordColNode = recordColNode->next_sibling())
            {
                //const char* pstrColName = recordColNode->first_attribute( "Id" )->value();
                NFIValueList::VarData varData;
                const char* pstrColType = recordColNode->first_attribute("Type")->value();
                if (VTYPE_UNKNOWN == ComputerType(pstrColType, varData))
                {
                    //assert(0);
                    NFASSERT(0, pstrRecordName, __FILE__, __FUNCTION__);
                }

                recordVar.Append(varData);
                //////////////////////////////////////////////////////////////////////////
                if (recordColNode->first_attribute("Key") != NULL)
                {
                    const char* pstrKey = recordColNode->first_attribute("Key")->value();
                    bool bKey = (0 == strcmp("true", pstrKey)) ? true : false;
                    if (bKey)
                    {
                        recordKey.AddInt(1);
                    }
                    else
                    {
                        recordKey.AddInt(0);
                    }
                }

                if (recordColNode->first_attribute("Tag") != NULL)
                {
                    const char* pstrTag = recordColNode->first_attribute("Tag")->value();
                    recordTag.AddString(pstrTag);
                }

                //////////////////////////////////////////////////////////////////////////
                if (recordColNode->first_attribute("Desc"))
                {
                    const char* pstrColDesc = recordColNode->first_attribute("Desc")->value();
                    recordDesc.AddString(pstrColDesc);
                }
                else
                {
                    recordDesc.AddString("");
                }

                //////////////////////////////////////////////////////////////////////////
            }

            pClass->GetRecordManager()->AddRecord(0, pstrRecordName, recordVar, recordKey, recordDesc, recordTag, atoi(pstrRow), bPublic, bPrivate, bSave, nIndex);
        }
    }

    return true;
}

bool NFCLogicClassModule::AddComponents( rapidxml::xml_node<>* componentRootNode, NFCLogicClass* pClass )
{
    for (rapidxml::xml_node<>* componentNode = componentRootNode->first_node(); componentNode; componentNode = componentNode->next_sibling())
    {
        if (componentNode)
        {
            const char* strComponentName = componentNode->first_attribute("Id")->value();
            const char* strLanguageModule = componentNode->first_attribute("LanguageModule")->value();
            if (pClass->GetComponentManager()->GetElement(strComponentName))
            {
                //error
                NFASSERT(0, strComponentName, __FILE__, __FUNCTION__);
                continue;
            }

            pClass->GetComponentManager()->AddComponent(std::string(strComponentName), std::string(strLanguageModule));
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
        pChildClass = new NFCLogicClass(strClassName);
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

    NFCLogicClass* pClass = new NFCLogicClass(pstrLogicClassName);
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

NFIComponentManager* NFCLogicClassModule::GetClassComponentManager( const std::string& strClassName )
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

void NFCLogicClassModule::OnReload( const char* strModuleName, NFILogicModule* pModule )
{
	Clear();
	Load();
}

