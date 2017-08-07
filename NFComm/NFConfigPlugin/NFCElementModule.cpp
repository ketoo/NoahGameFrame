// -------------------------------------------------------------------------
//    @FileName			:    NFCElementModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCElementModule
//
// -------------------------------------------------------------------------

#include <algorithm>
#include <ctype.h>
#include "NFConfigPlugin.h"
#include "NFCElementModule.h"
#include "NFCClassModule.h"

////

NFCElementModule::NFCElementModule(NFIPluginManager* p)
{
    pPluginManager = p;
    mbLoaded = false;
}

NFCElementModule::~NFCElementModule()
{

}

bool NFCElementModule::Init()
{
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	
	Load();

    return true;
}

bool NFCElementModule::Shut()
{
    Clear();
    return true;
}

bool NFCElementModule::Load()
{
    if (mbLoaded)
    {
        return false;
    }

    NF_SHARE_PTR<NFIClass> pLogicClass = m_pClassModule->First();
    while (pLogicClass)
    {
        const std::string& strInstancePath = pLogicClass->GetInstancePath();
        if (strInstancePath.empty())
        {
            pLogicClass = m_pClassModule->Next();
            continue;
        }
        //////////////////////////////////////////////////////////////////////////
		std::string strFile = pPluginManager->GetConfigPath() + strInstancePath;
		std::string strContent;
		pPluginManager->GetFileContent(strFile, strContent);

		rapidxml::xml_document<> xDoc;
		xDoc.parse<0>((char*)strContent.c_str());
        //////////////////////////////////////////////////////////////////////////
        //support for unlimited layer class inherits
        rapidxml::xml_node<>* root = xDoc.first_node();
        for (rapidxml::xml_node<>* attrNode = root->first_node(); attrNode; attrNode = attrNode->next_sibling())
        {
            Load(attrNode, pLogicClass);
        }

        mbLoaded = true;
        pLogicClass = m_pClassModule->Next();
    }

    return true;
}

bool NFCElementModule::CheckRef()
{
    NF_SHARE_PTR<NFIClass> pLogicClass = m_pClassModule->First();
    while (pLogicClass)
    {
		NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = pLogicClass->GetPropertyManager();
		if (pClassPropertyManager)
		{
			NF_SHARE_PTR<NFIProperty> pProperty = pClassPropertyManager->First();
			while (pProperty)
			{
				//if one property is ref,check every config
				if (pProperty->GetRef())
				{
					const std::vector<std::string>& strIdList = pLogicClass->GetIDList();
					for (int i = 0; i < strIdList.size(); ++i)
					{
						const std::string& strId = strIdList[i];

						const std::string& strRefValue= this->GetPropertyString(strId, pProperty->GetKey());
						if (!this->GetElement(strRefValue))
						{
							std::string msg;
							msg.append("check ref failed id: ").append(strRefValue).append(" in ").append(pLogicClass->GetClassName());
							NFASSERT(nRet, msg.c_str(), __FILE__, __FUNCTION__);
							exit(0);
						}
					}
				}
				pProperty = pClassPropertyManager->Next();
			}
		}
        //////////////////////////////////////////////////////////////////////////
        pLogicClass = m_pClassModule->Next();
    }

    return false;
}

bool NFCElementModule::Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFIClass> pLogicClass)
{
    //attrNode is the node of a object
    std::string strConfigID = attrNode->first_attribute("Id")->value();
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
    pLogicClass->AddId(strConfigID);

    //ElementConfigInfo* pElementInfo = CreateElement( strConfigID, pElementInfo );
    NF_SHARE_PTR<NFIPropertyManager> pElementPropertyManager = pElementInfo->GetPropertyManager();
    NF_SHARE_PTR<NFIRecordManager> pElementRecordManager = pElementInfo->GetRecordManager();

    //1.add property
    //2.set the default value  of them
    NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = pLogicClass->GetPropertyManager();
    NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = pLogicClass->GetRecordManager();
    if (pClassPropertyManager && pClassRecordManager)
    {
        NF_SHARE_PTR<NFIProperty> pProperty = pClassPropertyManager->First();
        while (pProperty)
        {

            pElementPropertyManager->AddProperty(NFGUID(), pProperty);

            pProperty = pClassPropertyManager->Next();
        }

        NF_SHARE_PTR<NFIRecord> pRecord = pClassRecordManager->First();
        while (pRecord)
        {
            NF_SHARE_PTR<NFIRecord> xRecord = pElementRecordManager->AddRecord(NFGUID(), pRecord->GetName(), pRecord->GetInitData(), pRecord->GetTag(), pRecord->GetRows());

            xRecord->SetPublic(pRecord->GetPublic());
            xRecord->SetPrivate(pRecord->GetPrivate());
            xRecord->SetSave(pRecord->GetSave());
            xRecord->SetCache(pRecord->GetCache());
			xRecord->SetUpload(pRecord->GetUpload());

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

        NFData var;
        const NFDATA_TYPE eType = temProperty->GetType();
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
			case TDATA_VECTOR2:
			{
				if (strlen(pstrConfigValue))
				{
					NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
				}
				NFVector2 tmp;
				tmp.FromString(pstrConfigValue);
				var.SetVector2(tmp);
			}
			break;
			case TDATA_VECTOR3:
			{
				if (strlen(pstrConfigValue))
				{
					NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
				}
				NFVector3 tmp;
				tmp.FromString(pstrConfigValue);
				var.SetVector3(tmp);
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

    NFData xData;
    xData.SetString(pLogicClass->GetClassName());
    pElementPropertyManager->SetProperty("ClassName", xData);

    return true;
}

bool NFCElementModule::Save()
{
    return true;
}

NFINT64 NFCElementModule::GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty)
    {
        return pProperty->GetInt();
    }

    return 0;
}

int NFCElementModule::GetPropertyInt32(const std::string& strConfigName, const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
	if (pProperty)
	{
		return pProperty->GetInt32();
	}

	return 0;
}

double NFCElementModule::GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty)
    {
        return pProperty->GetFloat();
    }

    return 0.0;
}

const std::string& NFCElementModule::GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(strConfigName, strPropertyName);
    if (pProperty)
    {
        return pProperty->GetString();
    }

    return  NULL_STR;
}

const std::vector<std::string> NFCElementModule::GetListByProperty(const std::string & strClassName, const std::string & strPropertyName, NFINT64 nValue)
{
	std::vector<std::string> xList;

	NF_SHARE_PTR<NFIClass> xClass = m_pClassModule->GetElement(strClassName);
	if (nullptr != xClass)
	{
		const std::vector<std::string>& xElementList = xClass->GetIDList();
		for (int i = 0; i < xElementList.size(); ++i)
		{
			const std::string& strConfigID = xElementList[i];
			NFINT64 nElementValue = GetPropertyInt(strConfigID, strPropertyName);
			if (nValue == nElementValue)
			{
				xList.push_back(strConfigID);
			}
		}
	}

	return xList;
}

const std::vector<std::string> NFCElementModule::GetListByProperty(const std::string & strClassName, const std::string & strPropertyName, const std::string & nValue)
{
	std::vector<std::string> xList;

	NF_SHARE_PTR<NFIClass> xClass = m_pClassModule->GetElement(strClassName);
	if (nullptr != xClass)
	{
		const std::vector<std::string>& xElementList = xClass->GetIDList();
		for (int i = 0; i < xElementList.size(); ++i)
		{
			const std::string& strConfigID = xElementList[i];
			const std::string& strElementValue = GetPropertyString(strConfigID, strPropertyName);
			if (nValue == strElementValue)
			{
				xList.push_back(strConfigID);
			}
		}
	}

	return xList;
}

NF_SHARE_PTR<NFIProperty> NFCElementModule::GetProperty(const std::string& strConfigName, const std::string& strPropertyName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo)
    {
        return pElementInfo->GetPropertyManager()->GetElement(strPropertyName);
    }

    return NULL;
}

NF_SHARE_PTR<NFIPropertyManager> NFCElementModule::GetPropertyManager(const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo)
    {
        return pElementInfo->GetPropertyManager();
    }

    return NULL;
}

NF_SHARE_PTR<NFIRecordManager> NFCElementModule::GetRecordManager(const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo)
    {
        return pElementInfo->GetRecordManager();
    }
    return NULL;
}

bool NFCElementModule::LoadSceneInfo(const std::string& strFileName, const std::string& strClassName)
{
	std::string strContent;
	pPluginManager->GetFileContent(strFileName, strContent);

	rapidxml::xml_document<> xDoc;
	xDoc.parse<0>((char*)strContent.c_str());
	
    NF_SHARE_PTR<NFIClass> pLogicClass = m_pClassModule->GetElement(strClassName.c_str());
    if (pLogicClass)
    {
        //support for unlimited layer class inherits
        rapidxml::xml_node<>* root = xDoc.first_node();
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

bool NFCElementModule::ExistElement(const std::string& strConfigName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(strConfigName);
    if (pElementInfo)
    {
        return true;
    }

    return false;
}

bool NFCElementModule::ExistElement(const std::string& strClassName, const std::string& strConfigName)
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

bool NFCElementModule::LegalNumber(const char* str)
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

bool NFCElementModule::AfterInit()
{
    CheckRef();
    return true;

}

bool NFCElementModule::BeforeShut()
{
    return true;

}

bool NFCElementModule::Execute()
{
    return true;

}

bool NFCElementModule::Clear()
{
    ClearAll();

    mbLoaded = false;
    return true;
}