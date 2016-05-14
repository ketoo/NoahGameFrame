// -------------------------------------------------------------------------
//    @FileName      :    NFCPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#ifndef NFC_PLUGIN_MANAGER_H
#define NFC_PLUGIN_MANAGER_H

#include <map>
#include <string>
#include <time.h>
#include "NFCDynLib.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFCore/NFQueue.h"

class NFCPluginManager
    : public NFIPluginManager
{
public:
    NFCPluginManager(NFIActorManager* pManager) : NFIPluginManager(pManager);
    virtual ~NFCPluginManager();

    virtual bool Init();

    virtual bool AfterInit();

    virtual bool CheckConfig();

    virtual bool BeforeShut();

    virtual bool Shut();

    //////////////////////////////////////////////////////////////////////////

    virtual void Registered(NFIPlugin* pPlugin);

    virtual void UnRegistered(NFIPlugin* pPlugin);

    virtual bool StartReLoadState();

    virtual bool EndReLoadState();
    //////////////////////////////////////////////////////////////////////////

    virtual NFIPlugin* FindPlugin(const std::string& strPluginName);

    virtual void AddModule(const std::string& strModuleName, NFILogicModule* pModule);

    virtual void RemoveModule(const std::string& strModuleName);

    virtual NFILogicModule* FindModule(const std::string& strModuleName);

    virtual void AddComponent(const std::string& strComponentName, NFIComponent* pComponent);

    virtual void RemoveComponent(const std::string& strComponentName);

    virtual NFIComponent* FindComponent(const std::string& strComponentName);

    virtual bool Execute();

    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);

    virtual NFIActorManager* GetActorManager()
    {
        return m_pActorManager;
    }

    virtual int AppID()
    {
        return mnAppID;
    }

    virtual NFINT64 GetInitTime() const
    {
        return mnInitTime;
    }

    virtual NFINT64 GetNowTime() const
    {
        return mnNowTime;
    }

    virtual const std::string& GetConfigPath() const
    {
        return mstrConfigPath;
    }

protected:
    bool LoadPlugin();
    bool LoadPluginLibrary(const std::string& strPluginDLLName);
    bool UnLoadPluginLibrary(const std::string& strPluginDLLName);

    bool ExecuteEvent();

private:
    int mnAppID;
    NFINT64 mnInitTime;
    NFINT64 mnNowTime;
    std::string mstrConfigPath;

    typedef std::map<std::string, bool> PluginNameMap;
    typedef std::map<std::string, NFCDynLib*> PluginLibMap;
    typedef std::map<std::string, NFIPlugin*> PluginInstanceMap;
    typedef std::map<std::string, NFILogicModule*> ModuleInstanceMap;
    typedef std::map<std::string, NFIComponent*> ComponentInstanceMap;

    typedef void(* DLL_START_PLUGIN_FUNC)(NFIPluginManager* pm);
    typedef void(* DLL_STOP_PLUGIN_FUNC)(NFIPluginManager* pm);

    NFIActorManager* m_pActorManager;
    NFQueue<NFIActorMessage> mxQueue;

    PluginNameMap mPluginNameMap;
    PluginLibMap mPluginLibMap;
    PluginInstanceMap mPluginInstanceMap;
    ModuleInstanceMap mModuleInstanceMap;
    ComponentInstanceMap mComponentInstanceMap;
};

#endif
