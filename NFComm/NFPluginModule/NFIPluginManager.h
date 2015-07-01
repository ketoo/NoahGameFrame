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
#include "NFILogicModule.h"
#include "NFIActorManager.h"

class NFIPlugin;

class NFIPluginManager : public NFILogicModule
{
public:
	NFIPluginManager(NFIActorManager* pManager)
	{

    }

    virtual bool LoadPlugin() = 0;

    virtual void Registered(NFIPlugin* plugin) = 0;

    virtual void UnsRegistered(NFIPlugin* plugin) = 0;

    virtual NFIPlugin* FindPlugin(const std::string& strPluginName) = 0;

    virtual void AddModule(const std::string& strModuleName, NFILogicModule* pModule) = 0;

    virtual void RemoveModule(const std::string& strModuleName) = 0;

    virtual NFILogicModule* FindModule(const std::string& strModuleName) = 0;

    virtual bool ReInitialize() = 0;

	virtual NFIActorManager* GetActorManager() = 0;
	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from) = 0;

    virtual int AppID() = 0;

};

#endif
