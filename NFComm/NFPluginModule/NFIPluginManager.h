// -------------------------------------------------------------------------
//    @FileName      :    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef NFI_PLUGIN_MANAGER_H
#define NFI_PLUGIN_MANAGER_H

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

    template <typename T>
    T* FindModule(const std::string& strModuleName)
    {
        NFILogicModule* pLogicModule = FindModule(strModuleName);
        if (pLogicModule)
        {
            if (!TIsDerived<T, NFILogicModule>::Result)
            {
                return NULL;
            }

            return dynamic_cast<T*>(pLogicModule);
        }

        return NULL;
    }
	template <typename T>
	T* FindModule()
	{
		NFILogicModule* pLogicModule = FindModule(typeid(T).name());
		if (pLogicModule)
		{
			if (!TIsDerived<T, NFILogicModule>::Result)
			{
				return NULL;
			}

			return dynamic_cast<T*>(pLogicModule);
		}

		return NULL;
	}
    virtual void Registered(NFIPlugin* plugin) = 0;

    virtual void UnRegistered(NFIPlugin* plugin) = 0;

    virtual NFIPlugin* FindPlugin(const std::string& strPluginName) = 0;

    virtual void AddModule(const std::string& strModuleName, NFILogicModule* pModule) = 0;

    virtual void RemoveModule(const std::string& strModuleName) = 0;

    virtual NFILogicModule* FindModule(const std::string& strModuleName) = 0;

    //////////////////////////////////////////////////////////////////////////

    virtual void AddComponent(const std::string& strComponentName, NFIComponent* pComponent) = 0;

    virtual void RemoveComponent(const std::string& strComponentName) = 0;

    virtual NFIComponent* FindComponent(const std::string& strComponentName) = 0;

    //////////////////////////////////////////////////////////////////////////

    virtual NFIActorManager* GetActorManager() = 0;
    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from) = 0;

    virtual int AppID() = 0;
    virtual NFINT64 GetInitTime() const = 0;
    virtual NFINT64 GetNowTime() const = 0;
    virtual const std::string& GetConfigPath() const = 0;
};

#endif
