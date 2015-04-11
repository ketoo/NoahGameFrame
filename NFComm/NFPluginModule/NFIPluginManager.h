// -------------------------------------------------------------------------
//    @FileName      :    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_PLUGIN_MANAGER_H_
#define _NFI_PLUGIN_MANAGER_H_

#ifdef NF_USE_ACTOR
#include "NFIActor.h"
#endif

#include "NFILogicModule.h"
#include "NFIActorManager.h"

class NFIPlugin;

class NFIPluginManager : public NFILogicModule
{
public:
    //NFIPluginManager(Theron::Framework &framework, NFIActorManager* pManager, NFIActorManager::EACTOR eActor): NFIActor(framework, pManager, eActor)
	NFIPluginManager(NFIActorManager* pManager)
	{

    }

    virtual bool LoadPlugin() = 0;

    //virtual bool UnLoadPlugin() = 0;

    //virtual bool LoadPlugin( const std::string& strPluginName ) = 0;

    //virtual bool UnLoadPlugin( const std::string& strPluginName ) = 0;

    virtual void Registered(NFIPlugin* plugin) = 0;

    virtual void UnsRegistered(NFIPlugin* plugin) = 0;

    virtual NFIPlugin* FindPlugin(const std::string& strPluginName) = 0;

    virtual void AddModule(const std::string& strModuleName, NFILogicModule* pModule) = 0;

    virtual void RemoveModule(const std::string& strModuleName) = 0;

    virtual NFILogicModule* FindModule(const std::string& strModuleName) = 0;

    virtual bool ReInitialize() = 0;

#ifdef NF_USE_ACTOR
	virtual NFIActorManager* GetActorManager() = 0;
#endif
};

#endif