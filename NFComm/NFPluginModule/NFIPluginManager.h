// -------------------------------------------------------------------------
//    @FileName			:    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef NFI_PLUGIN_MANAGER_H
#define NFI_PLUGIN_MANAGER_H

#include <functional>
#include "NFPlatform.h"

class NFIPlugin;
class NFIModule;

typedef std::function<bool (const std::string &strFileName, std::string &strContent)> GET_FILECONTENT_FUNCTOR;
typedef void (* CoroutineFunction)(void* arg);

template<typename DerivedType, typename BaseType>
class TIsDerived
{
public:
    static int AnyFunction(BaseType* base)
    {
        return 1;
    }

    static  char AnyFunction(void* t2)
    {
        return 0;
    }

    enum
    {
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType*)NULL))),
    };
};

#define FIND_MODULE(classBaseName, className)  \
	assert((TIsDerived<classBaseName, NFIModule>::Result));



class NFIPluginManager
{
public:
    NFIPluginManager()
    {

    }

	/////////////////////

	virtual bool Awake()
	{
		return true;
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

	virtual bool ReadyExecute()
	{
		return true;
	}

	virtual bool Execute()
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

	virtual bool Finalize()
	{
		return true;
	}

	virtual bool OnReloadPlugin()
	{
		return true;
	}

	/////////////////////

	template <typename T>
	T* FindModule()
	{
		NFIModule* pLogicModule = FindModule(typeid(T).name());
		if (pLogicModule)
		{
			if (!TIsDerived<T, NFIModule>::Result)
			{
				return NULL;
			}
            //TODO OSX上dynamic_cast返回了NULL
#if NF_PLATFORM == NF_PLATFORM_APPLE
            T* pT = (T*)pLogicModule;
#else
			T* pT = dynamic_cast<T*>(pLogicModule);
#endif
			assert(NULL != pT);

			return pT;
		}
		assert(NULL);
		return NULL;
	}

	virtual bool ReLoadPlugin(const std::string& strPluginDLLName) = 0;

    virtual void Registered(NFIPlugin* plugin) = 0;

    virtual void UnRegistered(NFIPlugin* plugin) = 0;

    virtual NFIPlugin* FindPlugin(const std::string& strPluginName) = 0;

    virtual void AddModule(const std::string& strModuleName, NFIModule* pModule) = 0;

    virtual void RemoveModule(const std::string& strModuleName) = 0;

    virtual NFIModule* FindModule(const std::string& strModuleName) = 0;

    virtual int GetAppID() const = 0;
    virtual void SetAppID(const int nAppID) = 0;

    virtual NFINT64 GetInitTime() const = 0;
    virtual NFINT64 GetNowTime() const = 0;

	virtual const std::string& GetConfigPath() const = 0;
	virtual void SetConfigPath(const std::string & strPath) = 0;

	virtual void SetConfigName(const std::string& strFileName) = 0;	
	virtual const std::string& GetConfigName() const = 0;

	virtual const std::string& GetAppName() const = 0;
	virtual void SetAppName(const std::string& strAppName) = 0;

	virtual const std::string& GetLogConfigName() const = 0;
	virtual void SetLogConfigName(const std::string& strName) = 0;

	virtual void SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun) = 0;
	virtual bool GetFileContent(const std::string &strFileName, std::string &strContent) = 0;

	virtual void ExecuteCoScheduler() = 0;
	virtual void YieldCo(const float nSecond) = 0;
	virtual void YieldCo() = 0;
};

#endif
