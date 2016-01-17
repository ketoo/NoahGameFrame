// -------------------------------------------------------------------------
//    @FileName      :    NFCPluginManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#include "NFCPluginManager.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIActorDataModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#pragma comment( lib, "ws2_32.lib" )


#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "Theron_d.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "Theron_d.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#pragma comment( lib, "Theron_d.a" )
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "Theron.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "Theron.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#pragma comment( lib, "Theron.a" )
#endif

#endif


bool NFCPluginManager::LoadPlugin()
{
    rapidxml::file<> fdoc( "Plugin.xml" );
    rapidxml::xml_document<>  doc;
    doc.parse<0>( fdoc.data() );

    rapidxml::xml_node<>* pRoot = doc.first_node();
    for ( rapidxml::xml_node<>* pPluginNode = pRoot->first_node("Plugin"); pPluginNode; pPluginNode = pPluginNode->next_sibling("Plugin") )
    {
        const char* strPluginName = pPluginNode->first_attribute( "Name" )->value();
        const char* strMain = pPluginNode->first_attribute( "Main" )->value();

		mPluginNameMap.insert(PluginNameMap::value_type(strPluginName, true));

    }

    rapidxml::xml_node<>* pPluginAppNode = pRoot->first_node("APPID");
    if (!pPluginAppNode)
    {
        NFASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    const char* strAppID = pPluginAppNode->first_attribute( "Name" )->value();
    if (!strAppID)
    {
        NFASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    if (!NF_StrTo(strAppID, mnAppID))
    {
        NFASSERT(0, "App ID Convert Error", __FILE__, __FUNCTION__);
        return false;
    }

    rapidxml::xml_node<>* pPluginConfigPathNode = pRoot->first_node("ConfigPath");
    if (!pPluginConfigPathNode)
    {
        NFASSERT(0, "There are no ConfigPath", __FILE__, __FUNCTION__);
        return false;
    }

    if (NULL == pPluginConfigPathNode->first_attribute( "Name" ))
    {
        NFASSERT(0, "There are no ConfigPath.Name", __FILE__, __FUNCTION__);
        return false;
    }

    mstrConfigPath = pPluginConfigPathNode->first_attribute( "Name" )->value();

    return true;
}

void NFCPluginManager::Registered(NFIPlugin* plugin)
{
    std::string strPluginName = plugin->GetPluginName();
    if (!FindPlugin(strPluginName))
    {
        bool bFind = false;
        PluginNameMap::iterator it = mPluginNameMap.begin();
        for (it; it != mPluginNameMap.end(); ++it)
        {
            if (strPluginName == it->first)
            {
                bFind = true;
                break;
            }
        }

        if (bFind)
        {
            mPluginInstanceMap.insert(PluginInstanceMap::value_type(strPluginName, plugin));
            plugin->Install();
        }
    }

}

void NFCPluginManager::UnsRegistered(NFIPlugin* plugin)
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

	ExecuteEvent();

    return bRet;
}

void NFCPluginManager::AddModule(const std::string& strModuleName, NFILogicModule* pModule)
{
    if (!FindModule(strModuleName))
    {
        mModuleInstanceMap.insert(ModuleInstanceMap::value_type(strModuleName, pModule));
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


NFILogicModule* NFCPluginManager::FindModule(const std::string& strModuleName)
{
    ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strModuleName);
    if (it != mModuleInstanceMap.end())
    {
        return it->second;
    }
    return NULL;
}

bool NFCPluginManager::Init()
{
#ifdef NF_DYNAMIC_PLUGIN
    LoadPlugin();


#if NF_PLATFORM == NF_PLATFORM_WIN || NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_APPLE
    PluginNameMap::iterator it = mPluginNameMap.begin();
    for (it; it != mPluginNameMap.end(); ++it)
    {
        LoadPluginLibrary(it->first);
    }

#endif

#else
    //     PluginNameList::iterator it = mPluginNameList.begin();
    //     for (it; it != mPluginNameList.end(); it++)
    //     {
    //         const std::string& strPluginName = *it;
    //         CREATE_PLUGIN( this, strPluginName );
    //     }

    //     CREATE_PLUGIN(this, NFKernelPlugin)
    //     CREATE_PLUGIN(this, NFEventProcessPlugin)
    //     CREATE_PLUGIN(this, NFConfigPlugin)

#endif

    PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
    for (itInstance; itInstance != mPluginInstanceMap.end(); itInstance++)
    {
        itInstance->second->Init();
    }

    return true;
}

bool NFCPluginManager::AfterInit()
{
    PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
    for (itAfterInstance; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
    {
        itAfterInstance->second->AfterInit();
    }

    return true;
}

bool NFCPluginManager::CheckConfig()
{
    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (itCheckInstance; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->CheckConfig();
    }

    return true;
}

bool NFCPluginManager::BeforeShut()
{
    PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
    for (itBeforeInstance; itBeforeInstance != mPluginInstanceMap.end(); itBeforeInstance++)
    {
        itBeforeInstance->second->BeforeShut();
    }

    return true;
}

bool NFCPluginManager::Shut()
{


    PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
    for (itInstance; itInstance != mPluginInstanceMap.end(); ++itInstance)
    {
        itInstance->second->Shut();
    }

#ifdef NF_DYNAMIC_PLUGIN

    PluginNameMap::iterator it = mPluginNameMap.begin();
    for (it; it != mPluginNameMap.end(); it++)
    {
        UnLoadPluginLibrary(it->first);
    }


#else

    //     DESTROY_PLUGIN(this, NFConfigPlugin)
    //     DESTROY_PLUGIN(this, NFEventProcessPlugin)
    //     DESTROY_PLUGIN(this, NFKernelPlugin)

#endif
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
#if NF_PLATFORM == NF_PLATFORM_LINUX
            char* error = dlerror();
            if (error)
            {
				std::cout << stderr << " Open shared lib failed " << pLib->GetName() << " " << error << std::endl;
                assert(0);
                return false;
            }
#elif NF_PLATFORM == NF_PLATFORM_WIN
            std::cout << stderr << " Open DLL " << pLib->GetName() << " failed, ErrorNo = "<< GetLastError() << std::endl;
#endif // NF_PLATFORM
            std::cout << "Load [" << pLib->GetName() << "] Failed" << std::endl;
            assert(0);
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

bool NFCPluginManager::ReInitialize()
{
    PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
    for ( itBeforeInstance; itBeforeInstance != mPluginInstanceMap.end(); itBeforeInstance++ )
    {
        itBeforeInstance->second->BeforeShut();
    }

    PluginInstanceMap::iterator itShutDownInstance = mPluginInstanceMap.begin();
    for ( itShutDownInstance; itShutDownInstance != mPluginInstanceMap.end(); itShutDownInstance++ )
    {
        itShutDownInstance->second->Shut();
    }

    PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
    for ( itInstance; itInstance != mPluginInstanceMap.end(); itInstance++ )
    {
        itInstance->second->Init();
    }

    PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
    for ( ; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++ )
    {
        itAfterInstance->second->AfterInit();
    }

    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (itCheckInstance; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->CheckConfig();
    }

    return true;
}


void NFCPluginManager::HandlerEx( const NFIActorMessage& message, const Theron::Address from )
{
	//添加到队列，每帧执行
	mxQueue.Push(message);
}

bool NFCPluginManager::ExecuteEvent()
{
	NFIActorMessage xMsg;
	bool bRet = false;
	bRet = mxQueue.Pop(xMsg);
	while (bRet)
	{
		if (xMsg.eType == NFIActorMessage::EACTOR_RETURN_EVENT_MSG)
		{
			xMsg.xEndFuncptr->operator()(xMsg.self, xMsg.nFormActor, xMsg.nSubMsgID, xMsg.data);
			m_pActorManager->ReleaseActor(xMsg.nFormActor);
		}

		bRet = mxQueue.Pop(xMsg);
	}



	return true;
}

void NFCPluginManager::AddComponent( const std::string& strComponentName, NFIComponent* pComponent )
{
	if (!FindComponent(strComponentName))
	{
		mComponentInstanceMap.insert(ComponentInstanceMap::value_type(strComponentName, pComponent));
	}
	
}

void NFCPluginManager::RemoveComponent( const std::string& strComponentName )
{
	ComponentInstanceMap::iterator it = mComponentInstanceMap.find(strComponentName);
	if (it != mComponentInstanceMap.end())
	{
		mComponentInstanceMap.erase(it);
	}
}

NFIComponent* NFCPluginManager::FindComponent( const std::string& strComponentName )
{
	ComponentInstanceMap::iterator it = mComponentInstanceMap.find(strComponentName);
	if (it != mComponentInstanceMap.end())
	{
		return it->second;
	}
	return NULL;
}
