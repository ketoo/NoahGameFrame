// -------------------------------------------------------------------------
//    @FileName			:    NFCPluginManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#include "NFCPluginManager.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32.lib" )
#endif

#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN

#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
//#pragma comment( lib, "libtherond.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
//#pragma comment( lib, "libtherond.a" )
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN

#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
//#pragma comment( lib, "libtheron.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
//#pragma comment( lib, "libtheron.a" )
#endif

#endif


#ifndef NF_DYNAMIC_PLUGIN
//for nf-sdk plugins
#include "NFComm/NFActorPlugin/NFActorPlugin.h"
#include "NFComm/NFConfigPlugin/NFConfigPlugin.h"
#include "NFComm/NFKernelPlugin/NFKernelPlugin.h"
#include "NFComm/NFLogPlugin/NFLogPlugin.h"
#include "NFComm/NFLuaScriptPlugin/NFLuaScriptPlugin.h"
#include "NFComm/NFNavigationPlugin/NFNavigationPlugin.h"
#include "NFComm/NFNetPlugin/NFNetPlugin.h"
#include "NFComm/NFNoSqlPlugin/NFNoSqlPlugin.h"
#include "NFComm/NFScalePlugin/NFScalePlugin.h"
#include "NFComm/NFSecurityPlugin/NFSecurityPlugin.h"
#include "NFComm/NFTestPlugin/NFTestPlugin.h"
//DB
#include "NFServer/NFDBLogicPlugin/NFDBLogicPlugin.h"
#include "NFServer/NFDBNet_ClientPlugin/NFDBNet_ClientPlugin.h"
#include "NFServer/NFDBNet_ServerPlugin/NFDBNet_ServerPlugin.h"
//GAME
#include "NFServer/NFGameLogicPlugin/NFGameLogicPlugin.h"
#include "NFServer/NFGameServerNet_ClientPlugin/NFGameServerNet_ClientPlugin.h"
#include "NFServer/NFGameServerNet_ServerPlugin/NFGameServerNet_ServerPlugin.h"
#include "NFServer/NFGameServerPlugin/NFGameServerPlugin.h"
//LOGIN
#include "NFServer/NFLoginLogicPlugin/NFLoginLogicPlugin.h"
#include "NFServer/NFLoginNet_ClientPlugin/NFLoginNet_ClientPlugin.h"
#include "NFServer/NFLoginNet_ServerPlugin/NFLoginNet_ServerPlugin.h"
#include "NFServer/NFLoginNet_HttpServerPlugin/NFLoginNet_HttpServerPlugin.h"
//MASTER
#include "NFServer/NFMasterNet_HttpServerPlugin/NFMasterNet_HttpServerPlugin.h"
#include "NFServer/NFMasterNet_ServerPlugin/NFMasterNet_ServerPlugin.h"
//PROXY
#include "NFServer/NFProxyLogicPlugin/NFProxyLogicPlugin.h"
#include "NFServer/NFProxyServerNet_ClientPlugin/NFProxyServerNet_ClientPlugin.h"
#include "NFServer/NFProxyServerNet_ServerPlugin/NFProxyServerNet_ServerPlugin.h"
//WORLD
#include "NFServer/NFWorldLogicPlugin/NFWorldLogicPlugin.h"
#include "NFServer/NFWorldNet_ClientPlugin/NFWorldNet_ClientPlugin.h"
#include "NFServer/NFWorldNet_ServerPlugin/NFWorldNet_ServerPlugin.h"
//MIDWARE
#include "NFMidWare/NFAIPlugin/NFAIPlugin.h"
#include "NFMidWare/NFAOIPlugin/NFAOIPlugin.h"
#include "NFMidWare/NFChatPlugin/NFChatPlugin.h"
#include "NFMidWare/NFFriendPlugin/NFFriendPlugin.h"
#include "NFMidWare/NFGuildPlugin/NFGuildPlugin.h"
#include "NFMidWare/NFHeroPlugin/NFHeroPlugin.h"
#include "NFMidWare/NFItemBagPlugin/NFItemBagPlugin.h"
#include "NFMidWare/NFMailPlugin/NFMailPlugin.h"
#include "NFMidWare/NFRankPlugin/NFRankPlugin.h"
#include "NFMidWare/NFShopPlugin/NFShopPlugin.h"
#include "NFMidWare/NFSkillPlugin/NFSkillPlugin.h"
#include "NFMidWare/NFTaskPlugin/NFTaskPlugin.h"
#include "NFMidWare/NFUserGiftPlugin/NFUserGiftPlugin.h"
#endif

void CoroutineExecute(void* arg)
{
	NFCPluginManager::Instance()->Execute();
}

NFCPluginManager::NFCPluginManager() : NFIPluginManager()
{
	mnAppID = 0;
    mbIsDocker = false;

#ifdef NF_DYNAMIC_PLUGIN
	mbStaticPlugin = false;
#else
	mbStaticPlugin = true;
#endif

	mnInitTime = time(NULL);
	mnNowTime = mnInitTime;

	mGetFileContentFunctor = nullptr;

	mstrConfigPath = "../";

#ifdef NF_DEBUG_MODE
   mstrConfigName = "NFDataCfg/Debug/Plugin.xml";
#else
   mstrConfigName = "NFDataCfg/Release/Plugin.xml";
#endif
}

NFCPluginManager::~NFCPluginManager()
{

}

bool NFCPluginManager::Awake()
{
	LoadPluginConfig();

#ifndef NF_DYNAMIC_PLUGIN
	LoadStaticPlugin();
#endif

	PluginNameMap::iterator it = mPluginNameMap.begin();
	for (; it != mPluginNameMap.end(); ++it)
	{
#ifdef NF_DYNAMIC_PLUGIN
		LoadPluginLibrary(it->first);
#else
		LoadStaticPlugin(it->first);
#endif
	}

#ifndef NF_DYNAMIC_PLUGIN
	CheckStaticPlugin();
#endif

	PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
	for (; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
	{
		itAfterInstance->second->Awake();
	}

	return true;
}

inline bool NFCPluginManager::Init()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
		itInstance->second->Init();
	}

   mxCoroutineManager.Init(CoroutineExecute);

	return true;
}

bool NFCPluginManager::LoadPluginConfig()
{
	std::string strContent;
	std::string strFilePath = GetConfigPath() + mstrConfigName;
	GetFileContent(strFilePath, strContent);

	rapidxml::xml_document<> xDoc;
	xDoc.parse<0>((char*)strContent.c_str());

    rapidxml::xml_node<>* pRoot = xDoc.first_node();
    rapidxml::xml_node<>* pAppNameNode = pRoot->first_node(mstrAppName.c_str());
    if (!pAppNameNode)
    {
        NFASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    for (rapidxml::xml_node<>* pPluginNode = pAppNameNode->first_node("Plugin"); pPluginNode; pPluginNode = pPluginNode->next_sibling("Plugin"))
    {
        const char* strPluginName = pPluginNode->first_attribute("Name")->value();

        mPluginNameMap.insert(PluginNameMap::value_type(strPluginName, true));

    }

    return true;
}

bool NFCPluginManager::LoadStaticPlugin()
{

#ifndef NF_DYNAMIC_PLUGIN
	CREATE_PLUGIN(this, NFConfigPlugin)
	CREATE_PLUGIN(this, NFKernelPlugin)
	CREATE_PLUGIN(this, NFLogPlugin)
	CREATE_PLUGIN(this, NFNetPlugin)
	CREATE_PLUGIN(this, NFNoSqlPlugin)

	CREATE_PLUGIN(this, NFProxyLogicPlugin)
	CREATE_PLUGIN(this, NFProxyServerNet_ClientPlugin)
	CREATE_PLUGIN(this, NFProxyServerNet_ServerPlugin)
#endif

    return true;
}

bool NFCPluginManager::CheckStaticPlugin()
{
#ifndef NF_DYNAMIC_PLUGIN
	//plugin
	for (auto it = mPluginInstanceMap.begin(); it != mPluginInstanceMap.end();)
	{
		bool bFind = false;
		const std::string& strPluginName = it->first;
		for (int i = 0; i < mStaticPlugin.size(); ++i)
		{
			const std::string& tempPluginName = mStaticPlugin[i];
			if (tempPluginName == strPluginName)
			{
				bFind = true;
			}
		}

		if (!bFind)
		{
			it = mPluginInstanceMap.erase(it);  
		}
		else
		{
			it++;
		}
	}
	//////module
	for (auto it = mModuleInstanceMap.begin(); it != mModuleInstanceMap.end();)
	{
		bool bFind = false;
		const std::string& strModuleName = it->first;
		for (int i = 0; i < mStaticPlugin.size(); ++i)
		{
			const std::string& strPluginName = mStaticPlugin[i];
			NFIPlugin* pPlugin = this->FindPlugin(strPluginName);
			if (pPlugin)
			{
				NFIModule* pModule = pPlugin->GetElement(strModuleName);
				if (pModule)
				{
					bFind = true;
					break;
				}
			}
		}

		if (!bFind)
		{
			it = mModuleInstanceMap.erase(it);  
		}
		else
		{
			it++;
		}
	}
#endif

    return true;
}

bool NFCPluginManager::LoadStaticPlugin(const std::string& strPluginDLLName)
{
	mStaticPlugin.push_back(mStaticPlugin);

	return true;
}

void NFCPluginManager::Registered(NFIPlugin* plugin)
{
    std::string strPluginName = plugin->GetPluginName();
    if (!FindPlugin(strPluginName))
	{
		mPluginInstanceMap.insert(PluginInstanceMap::value_type(strPluginName, plugin));
        plugin->Install();
    }
	else
	{
		assert(0);
	}
}

void NFCPluginManager::UnRegistered(NFIPlugin* plugin)
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

bool NFCPluginManager::ReLoadPlugin(const std::string & strPluginDLLName)
{
	//1.shut all module of this plugin
	//2.unload this plugin
	//3.load new plugin
	//4.init new module
	//5.tell others who has been reloaded
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.find(strPluginDLLName);
	if (itInstance == mPluginInstanceMap.end())
	{
		return false;
	}
	//1
	NFIPlugin* pPlugin = itInstance->second;
	NFIModule* pModule = pPlugin->First();
	while (pModule)
	{
		pModule->BeforeShut();
		pModule->Shut();
		pModule->Finalize();

		pModule = pPlugin->Next();
	}

	//2
	PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
	if (it != mPluginLibMap.end())
	{
		NFCDynLib* pLib = it->second;
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
	NFCDynLib* pLib = new NFCDynLib(strPluginDLLName);
	bool bLoad = pLib->Load();
	if (bLoad)
	{
		mPluginLibMap.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

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
		if (strPluginDLLName != itReloadInstance->first)
		{
			itReloadInstance->second->OnReloadPlugin();
		}
	}
	return true;
}

NFIPlugin* NFCPluginManager::FindPlugin(const std::string& strPluginName)
{
    PluginInstanceMap::iterator it = mPluginInstanceMap.find(strPluginName);
    if (it != mPluginInstanceMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool NFCPluginManager::Execute()
{
    mnNowTime = time(NULL);

    bool bRet = true;

    PluginInstanceMap::iterator it = mPluginInstanceMap.begin();
    for (; it != mPluginInstanceMap.end(); ++it)
    {
        bool tembRet = it->second->Execute();
        bRet = bRet && tembRet;
    }

    return bRet;
}

inline int NFCPluginManager::GetAppID() const
{
	return mnAppID;
}

inline void NFCPluginManager::SetAppID(const int nAppID)
{
    mnAppID = nAppID;
}
bool NFCPluginManager::IsRunningDocker() const
{
	return mbIsDocker;
}

void NFCPluginManager::SetRunningDocker(bool bDocker)
{
	mbIsDocker = bDocker;
}

bool NFCPluginManager::IsStaticPlugin() const
{
	return mbStaticPlugin;
}

inline NFINT64 NFCPluginManager::GetInitTime() const
{
	return mnInitTime;
}

inline NFINT64 NFCPluginManager::GetNowTime() const
{
	return mnNowTime;
}

inline const std::string & NFCPluginManager::GetConfigPath() const
{
	return mstrConfigPath;
}

inline void NFCPluginManager::SetConfigPath(const std::string & strPath)
{
	mstrConfigPath = strPath;
}

void NFCPluginManager::SetConfigName(const std::string & strFileName)
{
	if (strFileName.empty())
	{
		return;
	}

	if (strFileName.find(".xml") == string::npos)
	{
		return;
	}

#ifdef NF_DEBUG_MODE
	mstrConfigName = "NFDataCfg/Debug/" + strFileName;
#else
	mstrConfigName = "NFDataCfg/Release/" + strFileName;
#endif
}

const std::string& NFCPluginManager::GetConfigName() const
{
	return mstrConfigName;
}

const std::string& NFCPluginManager::GetAppName() const
{
	return mstrAppName;
}

void NFCPluginManager::SetAppName(const std::string& strAppName)
{
	if (!mstrAppName.empty())
	{
		return;
	}

	mstrAppName = strAppName;
}

const std::string & NFCPluginManager::GetLogConfigName() const
{
	return mstrLogConfigName;
}

void NFCPluginManager::SetLogConfigName(const std::string & strName)
{
	mstrLogConfigName = strName;
}

void NFCPluginManager::SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun)
{
	mGetFileContentFunctor = fun;
}

bool NFCPluginManager::GetFileContent(const std::string &strFileName, std::string &strContent)
{
	if (mGetFileContentFunctor)
	{
		return mGetFileContentFunctor(strFileName, strContent);
	}

	FILE *fp = fopen(strFileName.c_str(), "rb");
	if (!fp)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	const long filelength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	strContent.resize(filelength);
	fread((void*)strContent.data(), filelength, 1, fp);
	fclose(fp);

	return true;
}

void NFCPluginManager::AddModule(const std::string& strModuleName, NFIModule* pModule)
{
    if (!FindModule(strModuleName))
    {
        mModuleInstanceMap.insert(ModuleInstanceMap::value_type(strModuleName, pModule));
    }
}

void NFCPluginManager::AddTestModule(const std::string& strModuleName, NFIModule* pModule)
{
	if (!FindTestModule(strModuleName))
	{
		mTestModuleInstanceMap.insert(TestModuleInstanceMap::value_type(strModuleName, pModule));
	}
}

void NFCPluginManager::RemoveModule(const std::string& strModuleName)
{
    ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strModuleName);
    if (it != mModuleInstanceMap.end())
    {
        mModuleInstanceMap.erase(it);
    }
}


NFIModule* NFCPluginManager::FindModule(const std::string& strModuleName)
{
	std::string strSubModuleName = strModuleName;

#if NF_PLATFORM == NF_PLATFORM_WIN
	std::size_t position = strSubModuleName.find(" ");
	if (string::npos != position)
	{
		strSubModuleName = strSubModuleName.substr(position + 1, strSubModuleName.length());
	}
#else
	for (int i = 0; i < strSubModuleName.length(); i++)
	{
		std::string s = strSubModuleName.substr(0, i + 1);
		int n = atof(s.c_str());
		if (strSubModuleName.length() == i + 1 + n)
		{
			strSubModuleName = strSubModuleName.substr(i + 1, strSubModuleName.length());
			break;
		}
	}
#endif

	ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strSubModuleName);
	if (it != mModuleInstanceMap.end())
	{
		return it->second;
	}

    return NULL;
}

NFIModule* NFCPluginManager::FindTestModule(const std::string& strModuleName)
{
	std::string strSubModuleName = strModuleName;

#if NF_PLATFORM == NF_PLATFORM_WIN
	std::size_t position = strSubModuleName.find(" ");
	if (string::npos != position)
	{
		strSubModuleName = strSubModuleName.substr(position + 1, strSubModuleName.length());
	}
#else
	for (int i = 0; i < strSubModuleName.length(); i++)
	{
		std::string s = strSubModuleName.substr(0, i + 1);
		int n = atof(s.c_str());
		if (strSubModuleName.length() == i + 1 + n)
		{
			strSubModuleName = strSubModuleName.substr(i + 1, strSubModuleName.length());
			break;
		}
	}
#endif

    TestModuleInstanceMap::iterator it = mTestModuleInstanceMap.find(strSubModuleName);
	if (it != mTestModuleInstanceMap.end())
	{
		return it->second;
	}

	return NULL;
}

std::list<NFIModule*> NFCPluginManager::Modules()
{
	std::list<NFIModule*> xModules;

	ModuleInstanceMap::iterator itCheckInstance = mModuleInstanceMap.begin();
	for (; itCheckInstance != mModuleInstanceMap.end(); itCheckInstance++)
	{
		xModules.push_back(itCheckInstance->second);
	}

	return xModules;
}

bool NFCPluginManager::AfterInit()
{
    PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
    for (; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
    {
        itAfterInstance->second->AfterInit();
    }

    return true;
}

bool NFCPluginManager::CheckConfig()
{
    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->CheckConfig();
    }

    return true;
}

bool NFCPluginManager::ReadyExecute()
{
    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->ReadyExecute();
    }

    return true;
}

bool NFCPluginManager::BeforeShut()
{
    PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
    for (; itBeforeInstance != mPluginInstanceMap.end(); itBeforeInstance++)
    {
        itBeforeInstance->second->BeforeShut();
    }

    return true;
}

bool NFCPluginManager::Shut()
{
    PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
    for (; itInstance != mPluginInstanceMap.end(); ++itInstance)
    {
        itInstance->second->Shut();
    }

    return true;
}

bool NFCPluginManager::Finalize()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
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

bool NFCPluginManager::LoadPluginLibrary(const std::string& strPluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
    if (it == mPluginLibMap.end())
    {
        NFCDynLib* pLib = new NFCDynLib(strPluginDLLName);
        bool bLoad = pLib->Load();

        if (bLoad)
        {
            mPluginLibMap.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

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

bool NFCPluginManager::UnLoadPluginLibrary(const std::string& strPluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
    if (it != mPluginLibMap.end())
    {
        NFCDynLib* pLib = it->second;
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

bool NFCPluginManager::UnLoadStaticPlugin(const std::string & strPluginDLLName)
{
	//     DESTROY_PLUGIN(this, NFConfigPlugin)
	//     DESTROY_PLUGIN(this, NFEventProcessPlugin)
	//     DESTROY_PLUGIN(this, NFKernelPlugin)
	return false;
}

void NFCPluginManager::ExecuteCoScheduler()
{
    mxCoroutineManager.ScheduleJob();
}

void NFCPluginManager::YieldCo(const int64_t nSecond)
{
	mxCoroutineManager.YieldCo(nSecond);
}

void NFCPluginManager::YieldCo()
{
   mxCoroutineManager.YieldCo();
}