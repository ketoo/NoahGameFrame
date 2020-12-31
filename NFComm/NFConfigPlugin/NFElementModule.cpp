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


#include <algorithm>
#include <ctype.h>
#include "NFConfigPlugin.h"
#include "NFElementModule.h"
#include "NFClassModule.h"
#include "NFIThreadPoolModule.h"

NFElementModule::NFElementModule(NFElementModule* p)
{
    mbLoaded = false;
	originalElementModule = p;
}

NFElementModule::NFElementModule(NFIPluginManager* p)
{
	originalElementModule = this;
    pPluginManager = p;
    mbLoaded = false;

	if (!this->mbBackup)
	{
		for (int i = 0; i < pPluginManager->GetAppCPUCount(); ++i)
		{
			ThreadElementModule threadElement;
			threadElement.used = false;
			threadElement.elementModule = new NFElementModule(this);
			threadElement.elementModule->mbBackup = true;
			threadElement.elementModule->pPluginManager = pPluginManager;

			mThreadElements.push_back(threadElement);
		}
	}
}

NFElementModule::~NFElementModule()
{
    if (!this->mbBackup)
    {
		for (int i = 0; i < mThreadElements.size(); ++i)
		{
			delete mThreadElements[i].elementModule;
		}

		mThreadElements.clear();
    }
}

bool NFElementModule::Awake()
{
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	if (this->mbBackup)
	{
		for (int i = 0; i < originalElementModule->mThreadElements.size(); ++i)
		{
			if (originalElementModule->mThreadElements[i].elementModule == this)
			{
				m_pClassModule = m_pClassModule->GetThreadClassModule(i);
				break;
			}
		}
	}

	for (int i = 0; i < mThreadElements.size(); ++i)
	{
		mThreadElements[i].elementModule->Awake();
	}

	Load();

	return true;
}

bool NFElementModule::Init()
{
	for (int i = 0; i < mThreadElements.size(); ++i)
	{
		mThreadElements[i].elementModule->Init();
	}

    return true;
}

bool NFElementModule::AfterInit()
{
	CheckRef();

	for (int i = 0; i < mThreadElements.size(); ++i)
	{
		mThreadElements[i].elementModule->AfterInit();
	}

	return true;
}

bool NFElementModule::Shut()
{
    Clear();

    return true;
}

NFIElementModule* NFElementModule::GetThreadElementModule()
{
	std::thread::id threadID = std::this_thread::get_id();

	for (int i = 0; i < mThreadElements.size(); ++i)
	{
		if (mThreadElements[i].used)
		{
			if (mThreadElements[i].threadID == threadID)
			{
				return mThreadElements[i].elementModule;
			}
		}
		else
		{
			mThreadElements[i].used = true;
			mThreadElements[i].threadID = threadID;
			return mThreadElements[i].elementModule;
		}
	}

	return nullptr;
}

bool NFElementModule::Load()
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
		std::string content;
		pPluginManager->GetFileContent(strFile, content);

		rapidxml::xml_document<> xDoc;
		xDoc.parse<0>((char*)content.c_str());
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

	for (int i = 0; i < mThreadElements.size(); ++i)
	{
		mThreadElements[i].elementModule->Load();
	}

    return true;
}

bool NFElementModule::CheckRef()
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
						if (!strRefValue.empty() && !this->GetElement(strRefValue))
						{
							std::string msg = "check ref failed id:" + strRefValue + ", in " + pLogicClass->GetClassName() + "=>" + strId;
							NFASSERT(nRet, msg.c_str(), __FILE__, __FUNCTION__);

							m_pLogModule->LogError(msg, __FUNCTION__, __LINE__);
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

bool NFElementModule::LoadRecordData(rapidxml::xml_node<>* rootNode, NF_SHARE_PTR<NFIClass> pLogicClass){
    std::string configID = rootNode->first_attribute("Id")->value();
    NF_SHARE_PTR<NFIRecordManager> pElementRecordManager = pLogicClass->GetRecordManager();
    NF_SHARE_PTR<NFIRecord> record = pElementRecordManager->GetElement(configID);
    if(!record){
        return false;
    }
    int row = 0;
    for (rapidxml::xml_node<>* attrNode = rootNode->first_node(); attrNode; attrNode = attrNode->next_sibling())
    {
        int col = 0;
        record->AddRow(-1);
        for (rapidxml::xml_attribute<>* pAttribute = attrNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute()){
            
            const char* pstrConfigName = pAttribute->name();
            const char* pstrConfigValue = pAttribute->value();
            NFDATA_TYPE type = record->GetColType(pstrConfigName);
            switch (type)
            {
                case NFDATA_TYPE::TDATA_INT:
                    if (!LegalNumber(pstrConfigValue))
                    {
                        NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                    }
                    record->SetInt(row,pstrConfigName,lexical_cast<NFINT64>(pstrConfigValue));
                    break;
                case NFDATA_TYPE::TDATA_FLOAT:
                    if (strlen(pstrConfigValue) <= 0 || !LegalFloat(pstrConfigValue))
                    {
                        NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
                    }
                    record->SetFloat(row,pstrConfigName,(double)atof(pstrConfigValue));
                    break;
               case NFDATA_TYPE::TDATA_STRING:
                    record->SetString(row,pstrConfigName,pstrConfigValue);
                    break;
               case NFDATA_TYPE::TDATA_OBJECT:
                    {
                        NFGUID id;
                        if (id.FromString(pstrConfigValue))
                        {
                            record->SetObject(row,pstrConfigName,id);
                        }
                        break;
                    }
                default:
                    break;
            }
            col++;
        }
        row++;
    }
    record->SetStatic(true);
    return true;
}

bool NFElementModule::Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFIClass> pLogicClass)
{
    //attrNode is the node of a object
    std::string configID = attrNode->first_attribute("Id")->value();
    std::string configName = attrNode->name();
    if(configName == "Record"){
        LoadRecordData(attrNode,pLogicClass);
        return true;
    }
    if (configID.empty())
    {
        NFASSERT(0, configID, __FILE__, __FUNCTION__);
        return false;
    }

    if (ExistElement(configID))
    {
        NFASSERT(0, configID, __FILE__, __FUNCTION__);
        return false;
    }

    NF_SHARE_PTR<ElementConfigInfo> pElementInfo(NF_NEW ElementConfigInfo());
    AddElement(configID, pElementInfo);

    //can find all configid by class name
    pLogicClass->AddId(configID);

    //ElementConfigInfo* pElementInfo = CreateElement( configID, pElementInfo );
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
			xRecord->SetRef(pRecord->GetRef());
			xRecord->SetForce(pRecord->GetForce());
			xRecord->SetUpload(pRecord->GetUpload());
			xRecord->SetReadOnly(pRecord->GetReadOnly());
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
                if (strlen(pstrConfigValue) <= 0 || !LegalFloat(pstrConfigValue))
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
				if (strlen(pstrConfigValue) <= 0)
				{
					NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
				}

				NFVector2 tmp;
				if (!tmp.FromString(pstrConfigValue))
				{
					NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
				}
				var.SetVector2(tmp);
			}
			break;
			case TDATA_VECTOR3:
			{
				if (strlen(pstrConfigValue) <= 0)
				{
					NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
				}
				NFVector3 tmp;
				if (!tmp.FromString(pstrConfigValue))
				{
					NFASSERT(0, temProperty->GetKey(), __FILE__, __FUNCTION__);
				}
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

    NFData xDataClassName;
    xDataClassName.SetString(pLogicClass->GetClassName());
    pElementPropertyManager->SetProperty("ClassName", xDataClassName);


    NFData xDataID;
    xDataID.SetString(configID);
    pElementPropertyManager->SetProperty("ID", xDataID);
    pElementPropertyManager->SetProperty("ConfigID", xDataID);

    return true;
}

bool NFElementModule::Save()
{
    return true;
}

NFINT64 NFElementModule::GetPropertyInt(const std::string& configName, const std::string& propertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(configName, propertyName);
    if (pProperty)
    {
        return pProperty->GetInt();
    }

    return 0;
}

int NFElementModule::GetPropertyInt32(const std::string& configName, const std::string& propertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(configName, propertyName);
	if (pProperty)
	{
		return pProperty->GetInt32();
	}

	return 0;
}

double NFElementModule::GetPropertyFloat(const std::string& configName, const std::string& propertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(configName, propertyName);
    if (pProperty)
    {
        return pProperty->GetFloat();
    }

    return 0.0;
}

const std::string& NFElementModule::GetPropertyString(const std::string& configName, const std::string& propertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(configName, propertyName);
    if (pProperty)
    {
        return pProperty->GetString();
    }

    return  NULL_STR;
}

const NFVector2 NFElementModule::GetPropertyVector2(const std::string & configName, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(configName, propertyName);
	if (pProperty)
	{
		return pProperty->GetVector2();
	}

	return NFVector2();
}

const NFVector3 NFElementModule::GetPropertyVector3(const std::string & configName, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetProperty(configName, propertyName);
	if (pProperty)
	{
		return pProperty->GetVector3();
	}

	return NFVector3();
}

const std::vector<std::string> NFElementModule::GetListByProperty(const std::string & className, const std::string & propertyName, NFINT64 nValue)
{
	std::vector<std::string> xList;

	NF_SHARE_PTR<NFIClass> xClass = m_pClassModule->GetElement(className);
	if (nullptr != xClass)
	{
		const std::vector<std::string>& xElementList = xClass->GetIDList();
		for (int i = 0; i < xElementList.size(); ++i)
		{
			const std::string& configID = xElementList[i];
			NFINT64 nElementValue = GetPropertyInt(configID, propertyName);
			if (nValue == nElementValue)
			{
				xList.push_back(configID);
			}
		}
	}

	return xList;
}

const std::vector<std::string> NFElementModule::GetListByProperty(const std::string & className, const std::string & propertyName, const std::string & nValue)
{
	std::vector<std::string> xList;

	NF_SHARE_PTR<NFIClass> xClass = m_pClassModule->GetElement(className);
	if (nullptr != xClass)
	{
		const std::vector<std::string>& xElementList = xClass->GetIDList();
		for (int i = 0; i < xElementList.size(); ++i)
		{
			const std::string& configID = xElementList[i];
			const std::string& strElementValue = GetPropertyString(configID, propertyName);
			if (nValue == strElementValue)
			{
				xList.push_back(configID);
			}
		}
	}

	return xList;
}

NF_SHARE_PTR<NFIProperty> NFElementModule::GetProperty(const std::string& configName, const std::string& propertyName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(configName);
    if (pElementInfo)
    {
        return pElementInfo->GetPropertyManager()->GetElement(propertyName);
    }

    return NULL;
}

NF_SHARE_PTR<NFIPropertyManager> NFElementModule::GetPropertyManager(const std::string& configName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(configName);
    if (pElementInfo)
    {
        return pElementInfo->GetPropertyManager();
    }

    return NULL;
}

NF_SHARE_PTR<NFIRecordManager> NFElementModule::GetRecordManager(const std::string& configName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(configName);
    if (pElementInfo)
    {
        return pElementInfo->GetRecordManager();
    }
    return NULL;
}

bool NFElementModule::LoadSceneInfo(const std::string& fileName, const std::string& className)
{
	std::string content;
	pPluginManager->GetFileContent(fileName, content);

	rapidxml::xml_document<> xDoc;
	xDoc.parse<0>((char*)content.c_str());
	
    NF_SHARE_PTR<NFIClass> pLogicClass = m_pClassModule->GetElement(className.c_str());
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
        std::cout << "error load scene info failed, name is:" << className << " file name is :" << fileName << std::endl;
    }

    return true;
}

bool NFElementModule::ExistElement(const std::string& configName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(configName);
    if (pElementInfo)
    {
        return true;
    }

    return false;
}

bool NFElementModule::ExistElement(const std::string& className, const std::string& configName)
{
    NF_SHARE_PTR<ElementConfigInfo> pElementInfo = GetElement(configName);
    if (!pElementInfo)
    {
        return false;
    }

    const std::string& strClass = pElementInfo->GetPropertyManager()->GetPropertyString("ClassName");
    if (strClass != className)
    {
        return ExistElement(configName);
    }

    return true;
}

bool NFElementModule::LegalNumber(const char* str)
{
    int len = int(strlen(str));
    if (len <= 0)
    {
        return false;
    }

    int nStart = 0;
    if ('-' == str[0])
    {
        nStart = 1;
    }

    for (int i = nStart; i < len; ++i)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

bool NFElementModule::LegalFloat(const char * str)
{

	int len = int(strlen(str));
	if (len <= 0)
	{
		return false;
	}

	int nStart = 0;
	int nEnd = len;
	if ('-' == str[0])
	{
		nStart = 1;
	}
	if ('f' == std::tolower(str[nEnd -1]))
	{
		nEnd--;
	}

	if (nEnd <= nStart)
	{
		return false;
	}

	int pointNum = 0;
	for (int i = nStart; i < nEnd; ++i)
	{
		if ('.' == str[i])
		{
			pointNum++;
		}

		if (!isdigit(str[i]) && '.' != str[i])
		{
			return false;
		}
	}

	if (pointNum > 1)
	{
		return false;
	}

	return true;
}

bool NFElementModule::BeforeShut()
{
    return true;

}

bool NFElementModule::Execute()
{
    return true;

}

bool NFElementModule::Clear()
{
    ClearAll();

    mbLoaded = false;
    return true;
}