// -------------------------------------------------------------------------
//    @FileName      :    NFCPluginManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCPluginManager.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIActorDataModule.h"

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

        int nMain = boost::lexical_cast<int>( strMain );
        bool bMain = (nMain > 0 ? true : false);
        if (bMain)
        {
            //主模块只能运行在主actor上只
            //非主模块则所有的actor都创建
            if (GetActor() == NFIActorManager::EACTOR_MAIN)
            {
                mPluginNameMap.insert(PluginNameMap::value_type(strPluginName, bMain));
            }
        }
        else
        {
            mPluginNameMap.insert(PluginNameMap::value_type(strPluginName, bMain));
        }
    }

    rapidxml::xml_node<>* pActorDataNode = pRoot->first_node("ActorDataModule");
    if (pActorDataNode)
    {
        strDataModule = pActorDataNode->first_attribute( "Name" )->value();
    }

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

bool NFCPluginManager::Execute(const float fLasFrametime, const float fStartedTime)
{
    bool bRet = true;

    PluginInstanceMap::iterator it = mPluginInstanceMap.begin();
    for (; it != mPluginInstanceMap.end(); ++it)
    {
        bool tembRet = it->second->Execute(fLasFrametime, fStartedTime);
        bRet = bRet && tembRet;
    }

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
    for (itInstance; itInstance != mPluginInstanceMap.end(); itInstance++)
    {
        itInstance->second->Shut();
    }

#ifdef NF_DYNAMIC_PLUGIN
#if NF_PLATFORM == NF_PLATFORM_WIN || NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_APPLE

    PluginNameMap::iterator it = mPluginNameMap.begin();
    for (it; it != mPluginNameMap.end(); it++)
    {
        UnLoadPluginLibrary(it->first);
    }

#endif
#else

    //     DESTROY_PLUGIN(this, NFConfigPlugin)
    //     DESTROY_PLUGIN(this, NFEventProcessPlugin)
    //     DESTROY_PLUGIN(this, NFKernelPlugin)

#endif
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
                return false;
            }

            pFunc(this);

            return true;
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

        pFunc(this);

        delete pLib;
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

    return true;
}

void NFCPluginManager::HandlerEx( const NFIActorMessage& message, const Theron::Address from )
{
    if (message.eType != NFIActorMessage::EACTOR_UNKNOW)
    {
        //给哪个模块接这个消息呢..难道广播...他也不知道，哪个模块要写是接受数据的啊，除非...配置写好
        NFIActorDataModule* pModule = dynamic_cast<NFIActorDataModule*>(pPluginManager->FindModule(strDataModule));
        if (pModule)
        {
            pModule->Handler(message, from);
        }
    }
}