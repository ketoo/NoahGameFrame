// -------------------------------------------------------------------------
//    @FileName			:    NFCPluginManager.h
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
#include <thread>
#include "NFCDynLib.h"
#include "NFCoroutineManager.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

void CoroutineExecute(void* arg);

class NFCPluginManager
    : public NFIPluginManager,
	public NFSingleton<NFCPluginManager>
{
public:
    NFCPluginManager();
    virtual ~NFCPluginManager();

	virtual bool Awake() override;

	virtual bool Init() override;

    virtual bool AfterInit() override;

    virtual bool CheckConfig() override;

    virtual bool ReadyExecute() override;

    virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Finalize() override;


    //////////////////////////////////////////////////////////////////////////

    virtual void Registered(NFIPlugin* pPlugin) override;

    virtual void UnRegistered(NFIPlugin* pPlugin) override;

    //////////////////////////////////////////////////////////////////////////

	virtual bool ReLoadPlugin(const std::string& strPluginDLLName) override;

    virtual NFIPlugin* FindPlugin(const std::string& strPluginName) override;

    virtual void AddModule(const std::string& strModuleName, NFIModule* pModule) override;

    virtual void RemoveModule(const std::string& strModuleName) override;

    virtual NFIModule* FindModule(const std::string& strModuleName) override;

    virtual bool Execute() override;

	virtual int GetAppID() const override;

    virtual void SetAppID(const int nAppID) override;

	virtual NFINT64 GetInitTime() const override;

	virtual NFINT64 GetNowTime() const override;

	virtual const std::string& GetConfigPath() const override;
	virtual void SetConfigPath(const std::string & strPath) override;

	virtual void SetConfigName(const std::string& strFileName) override;
	virtual const std::string& GetConfigName() const override;

	virtual const std::string& GetAppName() const override;

	virtual void SetAppName(const std::string& strAppName) override;

	virtual const std::string& GetLogConfigName() const override;

	virtual void SetLogConfigName(const std::string& strName) override;

	virtual void SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun) override;

	virtual bool GetFileContent(const std::string &strFileName, std::string &strContent) override;

	virtual void ExecuteCoScheduler() override;

	virtual void YieldCo(const int64_t nSecond) override;

	virtual void YieldCo() override;

protected:
	bool LoadPluginConfig();

	bool LoadStaticPlugin(const std::string& strPluginDLLName);
    bool LoadPluginLibrary(const std::string& strPluginDLLName);
    bool UnLoadPluginLibrary(const std::string& strPluginDLLName);
	bool UnLoadStaticPlugin(const std::string& strPluginDLLName);

private:
    int mnAppID;
    NFINT64 mnInitTime;
    NFINT64 mnNowTime;
    std::string mstrConfigPath;
	std::string mstrConfigName;
	std::string mstrAppName;
	std::string mstrLogConfigName;

    typedef std::map<std::string, bool> PluginNameMap;
    typedef std::map<std::string, NFCDynLib*> PluginLibMap;
    typedef std::map<std::string, NFIPlugin*> PluginInstanceMap;
    typedef std::map<std::string, NFIModule*> ModuleInstanceMap;

    typedef void(* DLL_START_PLUGIN_FUNC)(NFIPluginManager* pm);
    typedef void(* DLL_STOP_PLUGIN_FUNC)(NFIPluginManager* pm);

    PluginNameMap mPluginNameMap;
    PluginLibMap mPluginLibMap;
    PluginInstanceMap mPluginInstanceMap;
    ModuleInstanceMap mModuleInstanceMap;

	GET_FILECONTENT_FUNCTOR mGetFileContentFunctor;

	NFCoroutineManager mxCoroutineManager;
};

#endif
