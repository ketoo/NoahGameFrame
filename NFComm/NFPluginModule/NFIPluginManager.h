// -------------------------------------------------------------------------
//    @FileName			:    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef NFI_PLUGIN_MANAGER_H
#define NFI_PLUGIN_MANAGER_H

#include "NFIModule.h"

#define FIND_MODULE(classBaseName, className)  \
	assert((TIsDerived<classBaseName, NFIModule>::Result));

class NFIPlugin;

class NFIPluginManager : public NFIModule
{
public:
    NFIPluginManager()
    {

    }

	template <typename T>
	T* FindModule()
	{
#if NF_PLATFORM == NF_PLATFORM_WIN
		NFIModule* pLogicModule = FindModule(typeid(T).name());
#else
		std::string strModuleName = typeid(T).name();
		for (int i = 0; i < strModuleName.length(); i++)
		{
			std::string s = strModuleName.substr(0, i + 1);
			int n = atof(s.c_str());
			if (strModuleName.length() == i + 1 + n)
			{
				strModuleName = strModuleName.substr(i + 1, strModuleName.length());
				break;
			}
		}
		NFIModule* pLogicModule = FindModule(strModuleName.c_str());
#endif
		if (pLogicModule)
		{
			if (!TIsDerived<T, NFIModule>::Result)
			{
				return NULL;
			}

			T* pT = dynamic_cast<T*>(pLogicModule);
			assert(NULL != pT);

			return pT;
		}
		assert(NULL);
		return NULL;
	}

    virtual void Registered(NFIPlugin* plugin) = 0;

    virtual void UnRegistered(NFIPlugin* plugin) = 0;

    virtual NFIPlugin* FindPlugin(const std::string& strPluginName) = 0;

    virtual void AddModule(const std::string& strModuleName, NFIModule* pModule) = 0;

    virtual void RemoveModule(const std::string& strModuleName) = 0;

    virtual NFIModule* FindModule(const std::string& strModuleName) = 0;

    virtual int AppID() = 0;
    virtual NFINT64 GetInitTime() const = 0;
    virtual NFINT64 GetNowTime() const = 0;
    virtual const std::string& GetConfigPath() const = 0;
};

#endif
