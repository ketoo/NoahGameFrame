// -------------------------------------------------------------------------
//    @FileName      :    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_PLUGIN_MANAGER_H_
#define _NFI_PLUGIN_MANAGER_H_

#include "NFIActor.h"
//#include "NFIPlugin.h"

class NFIPlugin;

#ifdef NF_USE_ACTOR
class NFIPluginManager : public NFIActor
{
public:
    NFIPluginManager(Theron::Framework &framework, NFIActorManager* pManager, NFIActorManager::EACTOR eActor): NFIActor(framework, pManager, eActor)
    {

    }
#else
class NFIPluginManager : public NFIActor
{
public:
    NFIPluginManager(NFIActorManager* pManager): NFIActor(pManager)
    {
    }

    virtual bool Init()
    {
        return true;
    }

    virtual bool AfterInit()
    {
        return true;
    }

    virtual bool CheckConfig()
    {
        return true;
    }

    virtual bool BeforeShut()
    {
        return true;
    }

    virtual bool Shut()
    {
        return true;
    }

    virtual bool Execute(const float fLasFrametime, const float fStartedTime)
    {
        return true;
    }
#endif


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
};

#endif