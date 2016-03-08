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

bool NFCLogicClassModule::bCipher = false;
bool NFCLogicClassModule::Init()
{
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

    assert(NULL != m_pElementInfoModule);

    Load();

    return true;
}

bool NFCLogicClassModule::Shut()
{
    ClearAll();

    return true;
}

NFCLogicClassModule::NFCLogicClassModule(NFIPluginManager* p)
{
    mnPropertyIndex = NF_GetTickCount() % 10 + 1;

    pPluginManager = p;

    // 判断 *.xml 是否存在，如果不存在，用 *.NF
    msConfigFileName = "NFDataCfg/Struct/LogicClass.xml";
    bCipher = false;
    std::fstream file;
    file.open(pPluginManager->GetConfigPath() + msConfigFileName, std::ios::in);
    if (!file)
    {
        msConfigFileName = "NFDataCfg/Struct/LogicClass.NF";
        bCipher = true;
    }

    std::cout << "Using [" << pPluginManager->GetConfigPath() + msConfigFileName << "]" << std::endl;
}

NFCLogicClassModule::~NFCLogicClassModule()
{
    ClearAll();
}

TDATA_TYPE NFCLogicClassModule::ComputerType(const char* pstrTypeName, NFIDataList::TData& var)
{
    if (0 == strcmp(pstrTypeName, "int"))
    {
        var.SetInt(NULL_INT);
        return var.GetType();
    }
    else if (0 == strcmp(pstrTypeName, "string"))
    {
        var.SetString(NULL_STR);
        return var.GetType();
    }
    else if (0 == strcmp(pstrTypeName, "float"))
    {
        var.SetFloat(NULL_FLOAT);
        return var.GetType();
    }
    else if (0 == strcmp(pstrTypeName, "object"))
    {
        var.SetObject(NULL_OBJECT);
        return var.GetType();
    }

    return TDATA_UNKNOWN;
}

bool NFCLogicClassModule::AddPropertys(rapidxml::xml_node<>* pPropertyRootNode, NF_SHARE_PTR<NFILogicClass> pClass)
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

            bool bPublic = lexical_cast<bool>(pstrPublic);
            bool bPrivate = lexical_cast<bool>(pstrPrivate);
            bool bSave = lexical_cast<bool>(pstrSave);
            bool bView = (strView.empty() ? false : (lexical_cast<bool>(strView)));
            int nIndex = lexical_cast<int>(pstrPropertyIndex);

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

            pClass->GetPropertyManager()->AddProperty(NFGUID(), strPropertyName, varProperty.GetType(), bPublic,  bPrivate, bSave, bView, nIndex, pstrRelationValue);
        }
    }

    return true;
}

bool NFCLogicClassModule::AddRecords(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFILogicClass> pClass)
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

            bool bPublic = lexical_cast<bool>(pstrPublic);
            bool bPrivate = lexical_cast<bool>(pstrPrivate);
            bool bSave = lexical_cast<bool>(pstrSave);
            bool bView = (strView.empty() ? false : (lexical_cast<bool>(strView)));
            int nIndex = lexical_cast<int>(pstrIndex);

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
                    bool bKey = lexical_cast<bool>(pstrKey);
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

            pClass->GetRecordManager()->AddRecord(NFGUID(), pstrRecordName, recordVar, recordKey, recordDesc, recordTag, recordRelation, atoi(pstrRow), bPublic, bPrivate, bSave, bView, nIndex);
        }
    }

    return true;
}

bool NFCLogicClassModule::AddComponents(rapidxml::xml_node<>* pComponentRootNode, NF_SHARE_PTR<NFILogicClass> pClass)
{
    for (rapidxml::xml_node<>* pComponentNode = pComponentRootNode->first_node(); pComponentNode; pComponentNode = pComponentNode->next_sibling())
    {
        if (pComponentNode)
        {
            const char* strComponentName = pComponentNode->first_attribute("Name")->value();
            const char* strLanguage = pComponentNode->first_attribute("Language")->value();
            const char* strEnable = pComponentNode->first_attribute("Enable")->value();
            bool bEnable = lexical_cast<bool>(strEnable);
            if (bEnable)
            {
                if (pClass->GetComponentManager()->GetElement(strComponentName))
                {
                    //error
                    NFASSERT(0, strComponentName, __FILE__, __FUNCTION__);
                    continue;
                }
                NF_SHARE_PTR<NFIComponent> xComponent(NF_NEW NFIComponent(NFGUID(), strComponentName));
                pClass->GetComponentManager()->AddComponent(strComponentName, xComponent);
            }
        }
    }

    return true;
}

bool NFCLogicClassModule::AddClassInclude(const char* pstrClassFilePath, NF_SHARE_PTR<NFILogicClass> pClass)
{
    if (pClass->Find(pstrClassFilePath))
    {
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    rapidxml::xml_document<> xDoc;
    char* pData = NULL;
    int nDataSize = 0;

    std::string strFile = pPluginManager->GetConfigPath() + pstrClassFilePath;
    if (!bCipher)
    {
        rapidxml::file<> fdoc(strFile.c_str());
        nDataSize = fdoc.size();
        pData = new char[nDataSize + 1];
        strncpy(pData, fdoc.data(), nDataSize);
    }
    else
    {
        std::string strFileData;
        if (!ReadFileToString(strFile, strFileData))
        {
            return false;
        }

        std::string strDecode = Decode(strFileData);

        nDataSize = strDecode.length();
        pData = new char[nDataSize + 1];
        strncpy(pData, strDecode.data(), nDataSize);
    }

    pData[nDataSize] = 0;
    xDoc.parse<0>(pData);
    //////////////////////////////////////////////////////////////////////////

    //support for unlimited layer class inherits
    rapidxml::xml_node<>* root = xDoc.first_node();

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

    //////////////////////////////////////////////////////////////////////////
    if (NULL != pData)
    {
        delete []pData;
    }
    //////////////////////////////////////////////////////////////////////////

    return true;
}

bool NFCLogicClassModule::AddClass(const char* pstrClassFilePath, NF_SHARE_PTR<NFILogicClass> pClass)
{
    NF_SHARE_PTR<NFILogicClass> pParent = pClass->GetParent();
    while (pParent.get())
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

    //file.close();

    return true;
}

bool NFCLogicClassModule::AddClass(const std::string& strClassName, const std::string& strParentName)
{
    NF_SHARE_PTR<NFILogicClass> pParentClass = GetElement(strParentName);
    NF_SHARE_PTR<NFILogicClass> pChildClass = GetElement(strClassName);
    if (!pChildClass.get())
    {
        pChildClass = NF_SHARE_PTR<NFILogicClass>(NF_NEW NFCLogicClass(strClassName));
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

bool NFCLogicClassModule::Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFILogicClass> pParentClass)
{
    const char* pstrLogicClassName = attrNode->first_attribute("Id")->value();
    const char* pstrType = attrNode->first_attribute("Type")->value();
    const char* pstrPath = attrNode->first_attribute("Path")->value();
    const char* pstrInstancePath = attrNode->first_attribute("InstancePath")->value();

    //printf( "-----------------------------------------------------\n");
    //printf( "%s:\n", pstrLogicClassName );

    NF_SHARE_PTR<NFILogicClass> pClass(NF_NEW NFCLogicClass(pstrLogicClassName));
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
    //////////////////////////////////////////////////////////////////////////
    rapidxml::xml_document<> xDoc;
    char* pData = NULL;
    int nDataSize = 0;

    std::string strFile = pPluginManager->GetConfigPath() + msConfigFileName;
    if (!bCipher)
    {
        rapidxml::file<> fdoc(strFile.c_str());
        nDataSize = fdoc.size();
        pData = new char[nDataSize + 1];
        strncpy(pData, fdoc.data(), nDataSize);
    }
    else
    {
        std::string strFileData;
        if (!ReadFileToString(strFile, strFileData))
        {
            return false;
        }

        std::string strDecode = Decode(strFileData);

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
        Load(attrNode, NULL);
    }

    m_pElementInfoModule->Load();
    //////////////////////////////////////////////////////////////////////////
    if (NULL != pData)
    {
        delete []pData;
    }
    //////////////////////////////////////////////////////////////////////////
    return true;
}

bool NFCLogicClassModule::Save()
{
    return true;
}

NF_SHARE_PTR<NFIPropertyManager> NFCLogicClassModule::GetClassPropertyManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFILogicClass> pClass = GetElement(strClassName);
    if (pClass.get())
    {
        return pClass->GetPropertyManager();
    }

    return NULL;
}

NF_SHARE_PTR<NFIRecordManager> NFCLogicClassModule::GetClassRecordManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFILogicClass> pClass = GetElement(strClassName);
    if (pClass.get())
    {
        return pClass->GetRecordManager();
    }

    return NULL;
}

NF_SHARE_PTR<NFIComponentManager> NFCLogicClassModule::GetClassComponentManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFILogicClass> pClass = GetElement(strClassName);
    if (pClass.get())
    {
        return pClass->GetComponentManager();
    }

    return NULL;
}

bool NFCLogicClassModule::Clear()
{
    return true;
}

std::string NFCLogicClassModule::cepher = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bool NFCLogicClassModule::IsEncoded(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string NFCLogicClassModule::Encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--)
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4) ; i++)
            {
                ret += cepher[char_array_4[i]];
            }

            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
        {
            ret += cepher[char_array_4[j]];
        }

        while ((i++ < 3))
        {
            ret += '=';
        }
    }

    return ret;
}

std::string NFCLogicClassModule::Decode(const std::string& encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && IsEncoded(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                char_array_4[i] = cepher.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
            {
                ret += char_array_3[i];
            }

            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
        {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++)
        {
            char_array_4[j] = cepher.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
        {
            ret += char_array_3[j];
        }
    }

    return ret;
}

bool NFCLogicClassModule::ReadFileToString(const std::string& strFile, std::string& strOutData)
{
    std::ifstream stream(strFile, std::ios::binary);
    if (!stream)
    {
        throw std::runtime_error(std::string("cannot open file = ") + strFile);
        return false;
    }

    stream.unsetf(std::ios::skipws);

    // Determine stream size
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    stream.seekg(0);

    std::vector<char> m_data;
    // Load data and add terminating 0
    m_data.resize(size + 1);
    stream.read(&m_data.front(), static_cast<std::streamsize>(size));
    m_data[size] = 0;

    strOutData.append(&m_data.front(), size);

    return true;
}

bool NFCLogicClassModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFILogicClass> pClass = GetElement(strClassName);
    if (nullptr == pClass)
    {
        return false;
    }

    return pClass->AddClassCallBack(cb);
}

bool NFCLogicClassModule::DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList)
{
    NF_SHARE_PTR<NFILogicClass> pClass = GetElement(strClassName);
    if (nullptr == pClass)
    {
        return false;
    }

    return pClass->DoEvent(objectID, eClassEvent, valueList);
}
