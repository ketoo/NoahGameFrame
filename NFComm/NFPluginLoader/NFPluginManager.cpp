/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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


#include "NFPluginManager.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "version.lib" )
//#pragma comment( lib, "SDL2maind.lib" )
#pragma comment( lib, "msimg32.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "imm32.lib" )
//#pragma comment( lib, "msvcrt.lib" )
#pragma comment (lib, "Setupapi.lib")

#ifndef NF_DYNAMIC_PLUGIN
#ifdef NF_DEBUG_MODE
#pragma comment( lib, "libprotobufd.lib" )
#pragma comment( lib, "SDL2d.lib" )
#else
#pragma comment( lib, "libprotobuf.lib" )
#pragma comment( lib, "SDL2.lib" )
#endif

#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "NFMessageDefine.lib" )
#pragma comment( lib, "event.lib" )
#pragma comment( lib, "event_core.lib" )
#pragma comment( lib, "navigation.lib" )
#pragma comment( lib, "hiredis.lib" )



#endif

#endif




NFPluginManager::NFPluginManager() : NFIPluginManager()
{
	appID = 0;
    mbIsDocker = false;

	currentPlugin = nullptr;
	currentModule = nullptr;

#ifdef NF_DYNAMIC_PLUGIN
	mbStaticPlugin = false;
#else
	mbStaticPlugin = true;
#endif

	mnInitTime = time(NULL);
	mnNowTime = mnInitTime;

	mGetFileContentFunctor = nullptr;

	configPath = "../";


	configName = "NFDataCfg/Debug/Plugin.xml";

}

NFPluginManager::~NFPluginManager()
{

}

bool NFPluginManager::LoadPlugin()
{
	std::cout << "----LoadPlugin----" << std::endl;

	PluginNameMap::iterator it = mPluginNameMap.begin();
	for (; it != mPluginNameMap.end(); ++it)
	{
#ifndef NF_DYNAMIC_PLUGIN
		LoadStaticPlugin(it->first);
#else
		LoadPluginLibrary(it->first);
#endif
	}

#ifndef NF_DYNAMIC_PLUGIN
	CheckStaticPlugin();
#endif

	return true;
}

bool NFPluginManager::Awake()
{
	std::cout << "----Awake----" << std::endl;

	PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
	for (; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
	{
		SetCurrentPlugin(itAfterInstance->second);
		itAfterInstance->second->Awake();
	}

	return true;
}

inline bool NFPluginManager::Init()
{
	std::cout << "----Init----" << std::endl;

	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
		SetCurrentPlugin(itInstance->second);
		std::cout << itInstance->first << std::endl;
		itInstance->second->Init();
	}

	return true;
}

bool NFPluginManager::LoadPluginConfig()
{
	std::string content;
	std::string strFilePath = GetConfigPath() + configName;
	GetFileContent(strFilePath, content);

	rapidxml::xml_document<> xDoc;
	xDoc.parse<0>((char*)content.c_str());

    rapidxml::xml_node<>* pRoot = xDoc.first_node();
    rapidxml::xml_node<>* pAppNameNode = pRoot->first_node(appName.c_str());
    if (pAppNameNode)
    {
		for (rapidxml::xml_node<>* pPluginNode = pAppNameNode->first_node("Plugin"); pPluginNode; pPluginNode = pPluginNode->next_sibling("Plugin"))
		{
			const char* pluginName = pPluginNode->first_attribute("Name")->value();

			mPluginNameMap.insert(PluginNameMap::value_type(pluginName, true));

			//std::cout << pluginName << std::endl;
		}
    }
	else
	{
		for (rapidxml::xml_node<>* pServerNode = pRoot->first_node(); pServerNode; pServerNode = pServerNode->next_sibling())
		{
			for (rapidxml::xml_node<>* pPluginNode = pServerNode->first_node("Plugin"); pPluginNode; pPluginNode = pPluginNode->next_sibling("Plugin"))
			{
				const char* pluginName = pPluginNode->first_attribute("Name")->value();
				if (mPluginNameMap.find(pluginName) == mPluginNameMap.end())
				{
					mPluginNameMap.insert(PluginNameMap::value_type(pluginName, true));
				}
			}
		}
	}

    return true;
}

bool NFPluginManager::CheckStaticPlugin()
{
#ifndef NF_DYNAMIC_PLUGIN
	//plugin
	for (auto it = mPluginInstanceMap.begin(); it != mPluginInstanceMap.end();)
	{
		bool bFind = false;
		const std::string& pluginName = it->first;
		for (int i = 0; i < mStaticPlugin.size(); ++i)
		{
			const std::string& tempPluginName = mStaticPlugin[i];
			if (tempPluginName == pluginName)
			{
				bFind = true;
				break;
			}
		}

		if (!bFind)
		{
			it->second->Uninstall();
			delete it->second;
			it->second = NULL;

			it = mPluginInstanceMap.erase(it);  
		}
		else
		{
			it++;
		}
	}

	for (auto it = mPluginInstanceMap.begin(); it != mPluginInstanceMap.end(); ++it)
	{
		std::cout << it->first << std::endl;
	}

	std::cout << "-------------" << std::endl;

	for (auto it = mModuleInstanceMap.begin(); it != mModuleInstanceMap.end(); ++it)
	{
		std::cout << it->first << std::endl;
	}
#endif

    return true;
}

bool NFPluginManager::LoadStaticPlugin(const std::string& pluginDLLName)
{
	mStaticPlugin.push_back(pluginDLLName);

	return true;
}

void NFPluginManager::Registered(NFIPlugin* plugin)
{
    const std::string& pluginName = plugin->GetPluginName();
    if (!FindPlugin(pluginName))
	{
		mPluginInstanceMap.insert(PluginInstanceMap::value_type(pluginName, plugin));
        plugin->Install();
    }
	else
	{
		std::cout << pluginName << std::endl;
		assert(0);
	}
}

void NFPluginManager::UnRegistered(NFIPlugin* plugin)
{
    PluginInstanceMap::iterator it = mPluginInstanceMap.find(plugin->GetPluginName());
    if (it != mPluginInstanceMap.end())
    {
        it->second->Uninstall();
        delete it->second;
        it->second = NULL;
        mPluginInstanceMap.erase(it);
    }
}

bool NFPluginManager::ReLoadPlugin(const std::string & pluginDLLName)
{
	//1.shut all module of this plugin
	//2.unload this plugin
	//3.load new plugin
	//4.init new module
	//5.tell others who has been reloaded
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.find(pluginDLLName);
	if (itInstance == mPluginInstanceMap.end())
	{
		return false;
	}
	//1
	NFIPlugin* pPlugin = itInstance->second;
	pPlugin->BeforeShut();
	pPlugin->Shut();
	pPlugin->Finalize();

	//2
	PluginLibMap::iterator it = mPluginLibMap.find(pluginDLLName);
	if (it != mPluginLibMap.end())
	{
		NFDynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = NULL;
		mPluginLibMap.erase(it);
	}

	//3
	NFDynLib* pLib = new NFDynLib(pluginDLLName);
	bool bLoad = pLib->Load();
	if (bLoad)
	{
		mPluginLibMap.insert(PluginLibMap::value_type(pluginDLLName, pLib));

		DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
		if (!pFunc)
		{
			std::cout << "Reload Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
			assert(0);
			return false;
		}

		pFunc(this);
	}
	else
	{
#if NF_PLATFORM == NF_PLATFORM_LINUX
		char* error = dlerror();
		if (error)
		{
			std::cout << stderr << " Reload shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
			std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
		}
#elif NF_PLATFORM == NF_PLATFORM_WIN
		std::cout << stderr << " Reload DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
		std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
		assert(0);
		return false;
#endif // NF_PLATFORM
	}

	//4
	PluginInstanceMap::iterator itReloadInstance = mPluginInstanceMap.begin();
	for (; itReloadInstance != mPluginInstanceMap.end(); itReloadInstance++)
	{
		if (pluginDLLName != itReloadInstance->first)
		{
			itReloadInstance->second->OnReloadPlugin();
		}
	}
	return true;
}

NFIPlugin* NFPluginManager::FindPlugin(const std::string& pluginName)
{
    PluginInstanceMap::iterator it = mPluginInstanceMap.find(pluginName);
    if (it != mPluginInstanceMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool NFPluginManager::Execute()
{
    mnNowTime = time(NULL);

    bool bRet = true;

    for (auto& xPair : mNeedExecuteModuleVec)
    {
        bool tembRet = xPair.second->Execute();
        bRet = bRet && tembRet;
    }

    return bRet;
}

inline int NFPluginManager::GetAppID() const
{
	return appID;
}

inline void NFPluginManager::SetAppID(const int id)
{
	appID = id;
}

bool NFPluginManager::IsRunningDocker() const
{
	return mbIsDocker;
}

void NFPluginManager::SetRunningDocker(bool bDocker)
{
	mbIsDocker = bDocker;
}

bool NFPluginManager::IsStaticPlugin() const
{
	return mbStaticPlugin;
}

inline NFINT64 NFPluginManager::GetInitTime() const
{
	return mnInitTime;
}

inline NFINT64 NFPluginManager::GetNowTime() const
{
	return mnNowTime;
}

inline const std::string & NFPluginManager::GetConfigPath() const
{
	return configPath;
}

inline void NFPluginManager::SetConfigPath(const std::string & strPath)
{
	configPath = strPath;
}

void NFPluginManager::SetConfigName(const std::string & fileName)
{
	if (fileName.empty())
	{
		return;
	}

	if (fileName.find(".xml") == string::npos)
	{
		return;
	}

#ifdef NF_DEBUG_MODE
	configName = "NFDataCfg/Debug/" + fileName;
#else
	configName = "NFDataCfg/Release/" + fileName;
#endif
}

const std::string& NFPluginManager::GetConfigName() const
{
	return configName;
}

const std::string& NFPluginManager::GetAppName() const
{
	return appName;
}

void NFPluginManager::SetAppName(const std::string& name)
{
	if (name.empty())
	{
		return;
	}

	appName = name;
}

const std::string & NFPluginManager::GetLogConfigName() const
{
	return logConfigName;
}

void NFPluginManager::SetLogConfigName(const std::string & name)
{
	logConfigName = name;
}

NFIPlugin * NFPluginManager::GetCurrentPlugin()
{
	return currentPlugin;
}

NFIModule * NFPluginManager::GetCurrentModule()
{
	return currentModule;
}

void NFPluginManager::SetCurrentPlugin(NFIPlugin * pPlugin)
{
	currentPlugin = pPlugin;
}

void NFPluginManager::SetCurrentModule(NFIModule * pModule)
{
	currentModule = pModule;
}

void NFPluginManager::SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun)
{
	mGetFileContentFunctor = fun;
}

bool NFPluginManager::GetFileContent(const std::string &fileName, std::string &content)
{
	if (mGetFileContentFunctor)
	{
		return mGetFileContentFunctor(this, fileName, content);
	}

	FILE *fp = fopen(fileName.c_str(), "rb");
	if (!fp)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	const long filelength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	content.resize(filelength);
	fread((void*)content.data(), filelength, 1, fp);
	fclose(fp);

	return true;
}

void NFPluginManager::AddModule(const std::string& moduleName, NFIModule* pModule)
{
    if (!FindModule(moduleName))
    {
        mModuleInstanceMap.insert(ModuleInstanceMap::value_type(moduleName, pModule));

        if (pModule->m_bIsExecute)
        {
	        mNeedExecuteModuleVec.push_back(std::make_pair(moduleName, pModule));
        }
    }
}

void NFPluginManager::AddTestModule(const std::string& moduleName, NFIModule* pModule)
{
	if (!FindTestModule(moduleName))
	{
		mTestModuleInstanceMap.insert(TestModuleInstanceMap::value_type(moduleName, pModule));
	}
}

void NFPluginManager::RemoveModule(const std::string& moduleName)
{
    ModuleInstanceMap::iterator it = mModuleInstanceMap.find(moduleName);
    if (it != mModuleInstanceMap.end())
    {
        mModuleInstanceMap.erase(it);

        auto iter = std::find_if(mNeedExecuteModuleVec.begin(),
            mNeedExecuteModuleVec.end(),
            [&moduleName](const std::pair<std::string, NFIModule*>& xPair) ->bool{
            return xPair.first == moduleName;
        });
        
        if(iter != mNeedExecuteModuleVec.end())
            mNeedExecuteModuleVec.erase(iter);
    }
}


NFIModule* NFPluginManager::FindModule(const std::string& moduleName)
{
	if (moduleName.empty())
	{
		return NULL;
	}

	ModuleInstanceMap::iterator it = mModuleInstanceMap.find(moduleName);
	if (it != mModuleInstanceMap.end())
	{
		return it->second;
	}
	
	if (this->GetCurrentModule())
	{
		std::cout << this->GetCurrentModule()->name << " want to find module: " << moduleName << " but got null_ptr!!!" << std::endl;
	}

    return NULL;
}

NFIModule* NFPluginManager::FindTestModule(const std::string& moduleName)
{
	if (moduleName.empty())
	{
		return NULL;
	}

    TestModuleInstanceMap::iterator it = mTestModuleInstanceMap.find(moduleName);
	if (it != mTestModuleInstanceMap.end())
	{
		return it->second;
	}

	return NULL;
}

std::list<NFIModule*> NFPluginManager::Modules()
{
	std::list<NFIModule*> xModules;

	ModuleInstanceMap::iterator itCheckInstance = mModuleInstanceMap.begin();
	for (; itCheckInstance != mModuleInstanceMap.end(); itCheckInstance++)
	{
		xModules.push_back(itCheckInstance->second);
	}

	return xModules;
}

bool NFPluginManager::AfterInit()
{
	std::cout << "----AfterInit----" << std::endl;

    PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
    for (; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
    {
		SetCurrentPlugin(itAfterInstance->second);
        itAfterInstance->second->AfterInit();
    }

    return true;
}

bool NFPluginManager::CheckConfig()
{
	std::cout << "----CheckConfig----" << std::endl;

    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
		SetCurrentPlugin(itCheckInstance->second);
        itCheckInstance->second->CheckConfig();
    }

    return true;
}

bool NFPluginManager::ReadyExecute()
{
	std::cout << "----ReadyExecute----" << std::endl;

    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
		SetCurrentPlugin(itCheckInstance->second);
        itCheckInstance->second->ReadyExecute();
    }

    return true;
}

bool NFPluginManager::BeforeShut()
{
    PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
    for (; itBeforeInstance != mPluginInstanceMap.end(); itBeforeInstance++)
    {
		SetCurrentPlugin(itBeforeInstance->second);
        itBeforeInstance->second->BeforeShut();
    }

    return true;
}

bool NFPluginManager::Shut()
{
    PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
    for (; itInstance != mPluginInstanceMap.end(); ++itInstance)
    {
		SetCurrentPlugin(itInstance->second);
        itInstance->second->Shut();
    }

    return true;
}

bool NFPluginManager::Finalize()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
		SetCurrentPlugin(itInstance->second);
		itInstance->second->Finalize();
	}

	////////////////////////////////////////////////

	PluginNameMap::iterator it = mPluginNameMap.begin();
	for (; it != mPluginNameMap.end(); it++)
	{
#ifdef NF_DYNAMIC_PLUGIN
		UnLoadPluginLibrary(it->first);
#else
		UnLoadStaticPlugin(it->first);
#endif
	}

	mPluginInstanceMap.clear();
	mPluginNameMap.clear();

	return true;
}

bool NFPluginManager::LoadPluginLibrary(const std::string& pluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(pluginDLLName);
    if (it == mPluginLibMap.end())
    {
        NFDynLib* pLib = new NFDynLib(pluginDLLName);
        bool bLoad = pLib->Load();

        if (bLoad)
        {
            mPluginLibMap.insert(PluginLibMap::value_type(pluginDLLName, pLib));

            DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
            if (!pFunc)
            {
                std::cout << "Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
                assert(0);
                return false;
            }

            pFunc(this);

            return true;
        }
        else
        {
#if NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_APPLE
            char* error = dlerror();
            if (error)
            {
                std::cout << stderr << " Load shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
                std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
                assert(0);
                return false;
            }
#elif NF_PLATFORM == NF_PLATFORM_WIN
            std::cout << stderr << " Load DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
            std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
            assert(0);
            return false;
#endif // NF_PLATFORM
        }
    }

    return false;
}

bool NFPluginManager::UnLoadPluginLibrary(const std::string& pluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(pluginDLLName);
    if (it != mPluginLibMap.end())
    {
        NFDynLib* pLib = it->second;
        DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

        if (pFunc)
        {
            pFunc(this);
        }

        pLib->UnLoad();

        delete pLib;
        pLib = NULL;
        mPluginLibMap.erase(it);

        return true;
    }

    return false;
}

bool NFPluginManager::UnLoadStaticPlugin(const std::string & pluginDLLName)
{
	//     DESTROY_PLUGIN(this, NFConfigPlugin)
	//     DESTROY_PLUGIN(this, NFEventProcessPlugin)
	//     DESTROY_PLUGIN(this, NFKernelPlugin)
	return false;
}

void NFPluginManager::AddFileReplaceContent(const std::string& fileName, const std::string& content, const std::string& newValue)
{
	auto it = mReplaceContent.find(fileName);
	if (it == mReplaceContent.end())
	{
		std::vector<NFReplaceContent> v;
		v.push_back(NFReplaceContent(content, newValue));

		mReplaceContent.insert(std::make_pair(fileName, v));
	}
	else
	{
		it->second.push_back(NFReplaceContent(content, newValue));
	}
}

std::vector<NFReplaceContent> NFPluginManager::GetFileReplaceContents(const std::string& fileName)
{
	auto it = mReplaceContent.find(fileName);
	if (it != mReplaceContent.end())
	{
		return it->second;
	}

	return std::vector<NFReplaceContent>();
}

std::list<NFIModule *> NFPluginManager::TestModules()
{
	std::list<NFIModule*> xModules;

	TestModuleInstanceMap::iterator itCheckInstance = mTestModuleInstanceMap.begin();
	for (; itCheckInstance != mTestModuleInstanceMap.end(); itCheckInstance++)
	{
		xModules.push_back(itCheckInstance->second);
	}

	return xModules;
}

int NFPluginManager::GetAppCPUCount() const
{
	return (int)mnCPUCount;
}

void NFPluginManager::SetAppCPUCount(const int count)
{
	mnCPUCount = count;
}

int NFPluginManager::GetAppType() const
{
	return appType;
}

void NFPluginManager::SetAppType(const int type)
{
	appType = type;
}

bool NFPluginManager::UsingBackThread() const
{
	return usingBackThread;
}

void NFPluginManager::SetUsingBackThread(const bool b)
{
	usingBackThread = b;
}
