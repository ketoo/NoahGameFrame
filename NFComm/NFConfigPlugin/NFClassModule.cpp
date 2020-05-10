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


#include <time.h>
#include <algorithm>
#include "NFConfigPlugin.h"
#include "NFClassModule.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"

NFClassModule::NFClassModule()
{
    msConfigFileName = "NFDataCfg/Struct/LogicClass.xml";
}

NFClassModule::NFClassModule(NFIPluginManager* p)
{
    pPluginManager = p;
    msConfigFileName = "NFDataCfg/Struct/LogicClass.xml";

    std::cout << "Using [" << pPluginManager->GetConfigPath() + msConfigFileName << "]" << std::endl;

	if (!this->mbBackup)
	{
		//NFIThreadPoolModule *threadPoolModule = pPluginManager->FindModule<NFIThreadPoolModule>();
		//const int threadCount = threadPoolModule->GetThreadCount();
		for (int i = 0; i < 10; ++i)
		{
			ThreadClassModule threadElement;
			threadElement.used = false;
			threadElement.classModule = new NFClassModule();
			threadElement.classModule->mbBackup = true;
			threadElement.classModule->pPluginManager = pPluginManager;

			threadElement.classModule->Awake();
			threadElement.classModule->Init();
			threadElement.classModule->AfterInit();

			mThreadClasses.push_back(threadElement);
		}
	}
}

NFClassModule::~NFClassModule()
{
    ClearAll();
}

bool NFClassModule::Awake()
{
    for (int i = 0; i < mThreadClasses.size(); ++i)
	{
		mThreadClasses[i].classModule->Awake();
	}

    Load();

	return true;
	
}

bool NFClassModule::Init()
{
	for (int i = 0; i < mThreadClasses.size(); ++i)
	{
		mThreadClasses[i].classModule->Awake();
	}
    return true;
}

bool NFClassModule::Shut()
{
	for (int i = 0; i < mThreadClasses.size(); ++i)
	{
		mThreadClasses[i].classModule->Awake();
	}

    ClearAll();

    return true;
}

NFIClassModule* NFClassModule::GetThreadClassModule()
{
	std::thread::id threadID = std::this_thread::get_id();

	for (int i = 0; i < mThreadClasses.size(); ++i)
	{
		if (mThreadClasses[i].used)
		{
			if (mThreadClasses[i].threadID == threadID)
			{
				return mThreadClasses[i].classModule;
			}
		}
		else
		{
			mThreadClasses[i].used = true;
			mThreadClasses[i].threadID = threadID;
			return mThreadClasses[i].classModule;
		}
	}

	return nullptr;
}

NFDATA_TYPE NFClassModule::ComputerType(const char* pstrTypeName, NFData& var)
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
	else if (0 == strcmp(pstrTypeName, "vector2"))
	{
		var.SetVector2(NULL_VECTOR2);
		return var.GetType();
	}
	else if (0 == strcmp(pstrTypeName, "vector3"))
	{
		var.SetVector3(NULL_VECTOR3);
		return var.GetType();
	}

    return TDATA_UNKNOWN;
}

bool NFClassModule::AddProperties(rapidxml::xml_node<>* pPropertyRootNode, NF_SHARE_PTR<NFIClass> pClass)
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
            const char* pstrSave = pPropertyNode->first_attribute("Save")->value();
            const char* pstrCache = pPropertyNode->first_attribute("Cache")->value();
			const char* pstrRef = pPropertyNode->first_attribute("Ref")->value();
			const char* pstrForce = pPropertyNode->first_attribute("Force")->value();
			const char* pstrUpload = pPropertyNode->first_attribute("Upload")->value();

            bool bPublic = lexical_cast<bool>(pstrPublic);
            bool bPrivate = lexical_cast<bool>(pstrPrivate);
            bool bSave = lexical_cast<bool>(pstrSave);
            bool bCache = lexical_cast<bool>(pstrCache);
			bool bRef = lexical_cast<bool>(pstrRef);
			bool bForce = lexical_cast<bool>(pstrForce);
			bool bUpload = lexical_cast<bool>(pstrUpload);

            NFData varProperty;
            if (TDATA_UNKNOWN == ComputerType(pstrType, varProperty))
            {
                //std::cout << "error:" << pClass->GetTypeName() << "  " << pClass->GetInstancePath() << ": " << strPropertyName << " type error!!!" << std::endl;

                NFASSERT(0, strPropertyName, __FILE__, __FUNCTION__);
            }

            //printf( " Property:%s[%s]\n", pstrPropertyName, pstrType );

            NF_SHARE_PTR<NFIProperty> xProperty = pClass->GetPropertyManager()->AddProperty(NFGUID(), strPropertyName, varProperty.GetType());
            xProperty->SetPublic(bPublic);
            xProperty->SetPrivate(bPrivate);
            xProperty->SetSave(bSave);
            xProperty->SetCache(bCache);
			xProperty->SetRef(bRef);
			xProperty->SetForce(bForce);
			xProperty->SetUpload(bUpload);

        }
    }

    return true;
}

bool NFClassModule::AddRecords(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFIClass> pClass)
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
			const char* pstrCache = pRecordNode->first_attribute("Cache")->value();
			const char* pstrRef = pRecordNode->first_attribute("Ref")->value();
			const char* pstrForce = pRecordNode->first_attribute("Force")->value();
			const char* pstrUpload = pRecordNode->first_attribute("Upload")->value();

            std::string strView;
            if (pRecordNode->first_attribute("View") != NULL)
            {
                strView = pRecordNode->first_attribute("View")->value();
            }

            bool bPublic = lexical_cast<bool>(pstrPublic);
            bool bPrivate = lexical_cast<bool>(pstrPrivate);
            bool bSave = lexical_cast<bool>(pstrSave);
			bool bCache = lexical_cast<bool>(pstrCache);
			bool bRef = lexical_cast<bool>(pstrCache);
			bool bForce = lexical_cast<bool>(pstrCache);
			bool bUpload = lexical_cast<bool>(pstrUpload);

			NF_SHARE_PTR<NFDataList> recordVar(NF_NEW NFDataList());
			NF_SHARE_PTR<NFDataList> recordTag(NF_NEW NFDataList());

            for (rapidxml::xml_node<>* recordColNode = pRecordNode->first_node(); recordColNode;  recordColNode = recordColNode->next_sibling())
            {
                //const char* pstrColName = recordColNode->first_attribute( "Id" )->value();
                NFData TData;
                const char* pstrColType = recordColNode->first_attribute("Type")->value();
                if (TDATA_UNKNOWN == ComputerType(pstrColType, TData))
                {
                    //assert(0);
                    NFASSERT(0, pstrRecordName, __FILE__, __FUNCTION__);
                }

                recordVar->Append(TData);
                //////////////////////////////////////////////////////////////////////////
                if (recordColNode->first_attribute("Tag") != NULL)
                {
                    const char* pstrTag = recordColNode->first_attribute("Tag")->value();
                    recordTag->Add(pstrTag);
                }
                else
                {
                    recordTag->Add("");
                }
            }

            NF_SHARE_PTR<NFIRecord> xRecord = pClass->GetRecordManager()->AddRecord(NFGUID(), pstrRecordName, recordVar, recordTag, atoi(pstrRow));

			xRecord->SetPublic(bPublic);
            xRecord->SetPrivate(bPrivate);
            xRecord->SetSave(bSave);
            xRecord->SetCache(bCache);
			xRecord->SetRef(bRef);
			xRecord->SetForce(bForce);
			xRecord->SetUpload(bUpload);
        }
    }

    return true;
}

bool NFClassModule::AddComponents(rapidxml::xml_node<>* pComponentRootNode, NF_SHARE_PTR<NFIClass> pClass)
{
	/*
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
	*/
    return true;
}

bool NFClassModule::AddClassInclude(const char* pstrClassFilePath, NF_SHARE_PTR<NFIClass> pClass)
{
    if (pClass->Find(pstrClassFilePath))
    {
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    std::string strFile = pPluginManager->GetConfigPath() + pstrClassFilePath;
	std::string strContent;
	pPluginManager->GetFileContent(strFile, strContent);

	rapidxml::xml_document<> xDoc;
    xDoc.parse<0>((char*)strContent.c_str());
    //////////////////////////////////////////////////////////////////////////

    //support for unlimited layer class inherits
    rapidxml::xml_node<>* root = xDoc.first_node();

    rapidxml::xml_node<>* pRropertyRootNode = root->first_node("Propertys");
    if (pRropertyRootNode)
    {
		AddProperties(pRropertyRootNode, pClass);
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

bool NFClassModule::AddClass(const char* pstrClassFilePath, NF_SHARE_PTR<NFIClass> pClass)
{
    NF_SHARE_PTR<NFIClass> pParent = pClass->GetParent();
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

    //file.close();

    return true;
}

bool NFClassModule::AddClass(const std::string& strClassName, const std::string& strParentName)
{
    NF_SHARE_PTR<NFIClass> pParentClass = GetElement(strParentName);
    NF_SHARE_PTR<NFIClass> pChildClass = GetElement(strClassName);
    if (!pChildClass)
    {
        pChildClass = NF_SHARE_PTR<NFIClass>(NF_NEW NFClass(strClassName));
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

bool NFClassModule::Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFIClass> pParentClass)
{
    const char* pstrLogicClassName = attrNode->first_attribute("Id")->value();
    const char* pstrPath = attrNode->first_attribute("Path")->value();
    const char* pstrInstancePath = attrNode->first_attribute("InstancePath")->value();

    //printf( "-----------------------------------------------------\n");
    //printf( "%s:\n", pstrLogicClassName );

    NF_SHARE_PTR<NFIClass> pClass(NF_NEW NFClass(pstrLogicClassName));
    AddElement(pstrLogicClassName, pClass);
    pClass->SetParent(pParentClass);
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

bool NFClassModule::Load()
{
    //////////////////////////////////////////////////////////////////////////
	std::string strFile = pPluginManager->GetConfigPath() + msConfigFileName;
	std::string strContent;
	pPluginManager->GetFileContent(strFile, strContent);

	rapidxml::xml_document<> xDoc;
	xDoc.parse<0>((char*)strContent.c_str());
    //////////////////////////////////////////////////////////////////////////
    //support for unlimited layer class inherits
    rapidxml::xml_node<>* root = xDoc.first_node();
    for (rapidxml::xml_node<>* attrNode = root->first_node(); attrNode; attrNode = attrNode->next_sibling())
    {
        Load(attrNode, NULL);
    }

	for (int i = 0; i < mThreadClasses.size(); ++i)
	{
		mThreadClasses[i].classModule->Load();
	}

    return true;
}

bool NFClassModule::Save()
{
    return true;
}

NF_SHARE_PTR<NFIPropertyManager> NFClassModule::GetClassPropertyManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIClass> pClass = GetElement(strClassName);
    if (pClass)
    {
        return pClass->GetPropertyManager();
    }

    return NULL;
}

NF_SHARE_PTR<NFIRecordManager> NFClassModule::GetClassRecordManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIClass> pClass = GetElement(strClassName);
    if (pClass)
    {
        return pClass->GetRecordManager();
    }

    return NULL;
}

bool NFClassModule::Clear()
{
    return true;
}

bool NFClassModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFIClass> pClass = GetElement(strClassName);
    if (nullptr == pClass)
    {
        return false;
    }

    return pClass->AddClassCallBack(cb);
}

bool NFClassModule::DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList)
{
    NF_SHARE_PTR<NFIClass> pClass = GetElement(strClassName);
    if (nullptr == pClass)
    {
        return false;
    }

    return pClass->DoEvent(objectID, eClassEvent, valueList);
}

bool NFClassModule::AfterInit()
{


	return true;
}

NFIClassModule *NFClassModule::GetThreadClassModule(const int index)
{
	return mThreadClasses[index].classModule;
}
