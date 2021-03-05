/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NF_PLUGIN_MANAGER_H
#define NF_PLUGIN_MANAGER_H

#include <map>
#include <string>
#include <time.h>
#include <thread>
#include "NFDynLib.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFPluginManager
    : public NFIPluginManager
{
public:
    NFPluginManager();
    virtual ~NFPluginManager();

    virtual bool LoadPluginConfig() override;

    virtual bool LoadPlugin() override;

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

    virtual bool ReLoadPlugin(const std::string& pluginDLLName) override;

    virtual NFIPlugin* FindPlugin(const std::string& pluginName) override;

    virtual void AddModule(const std::string& moduleName, NFIModule* pModule) override;

    virtual void AddTestModule(const std::string& moduleName, NFIModule* pModule) override;

    virtual void RemoveModule(const std::string& moduleName) override;

    virtual NFIModule* FindModule(const std::string& moduleName) override;

    virtual NFIModule* FindTestModule(const std::string& moduleName) override;

	virtual std::list<NFIModule*> Modules() override;
	virtual std::list<NFIModule*> TestModules() override;

    virtual bool Execute() override;

    virtual int GetAppID() const override;
    virtual void SetAppID(const int appID) override;

	virtual int GetAppType() const override;
	virtual void SetAppType(const int type) override;

    virtual bool IsRunningDocker() const override;
    virtual void SetRunningDocker(bool bDocker) override;

    virtual bool IsStaticPlugin() const override;

    virtual NFINT64 GetInitTime() const override;
    virtual NFINT64 GetNowTime() const override;

    virtual const std::string& GetConfigPath() const override;
    virtual void SetConfigPath(const std::string & strPath) override;

    virtual void SetConfigName(const std::string& fileName) override;
    virtual const std::string& GetConfigName() const override;

    virtual const std::string& GetAppName() const override;

    virtual void SetAppName(const std::string& appName) override;

    virtual const std::string& GetLogConfigName() const override;

    virtual void SetLogConfigName(const std::string& name) override;

    virtual NFIPlugin* GetCurrentPlugin() override;
    virtual NFIModule* GetCurrentModule() override;

    virtual void SetCurrentPlugin(NFIPlugin* pPlugin) override;
    virtual void SetCurrentModule(NFIModule* pModule) override;

	virtual int GetAppCPUCount() const override;
	virtual void SetAppCPUCount(const int count) override;

	virtual bool UsingBackThread() const override;
	virtual void SetUsingBackThread(const bool b) override;

    virtual void SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun) override;

    virtual bool GetFileContent(const std::string &fileName, std::string &content) override;

	virtual void AddFileReplaceContent(const std::string& fileName, const std::string& content, const std::string& newValue) override;
	virtual std::vector<NFReplaceContent> GetFileReplaceContents(const std::string& fileName) override;

protected:

    bool LoadStaticPlugin();
    bool CheckStaticPlugin();

    bool LoadStaticPlugin(const std::string& pluginDLLName);
    bool LoadPluginLibrary(const std::string& pluginDLLName);
    bool UnLoadPluginLibrary(const std::string& pluginDLLName);
    bool UnLoadStaticPlugin(const std::string& pluginDLLName);

private:
    int appID = 0;
	int appType = 0;
    bool mbIsDocker = false;
	bool mbStaticPlugin = false;
	bool usingBackThread = false;

    NFINT64 mnInitTime = 0;
	NFINT64 mnNowTime = 0;
	NFINT64 mnCPUCount = 1;

    std::string configPath;
    std::string configName;
    std::string appName;
    std::string logConfigName;

    NFIPlugin* currentPlugin;
    NFIModule* currentModule;

    typedef std::map<std::string, bool> PluginNameMap;
    typedef std::map<std::string, NFDynLib*> PluginLibMap;
    typedef std::map<std::string, NFIPlugin*> PluginInstanceMap;
    typedef std::map<std::string, NFIModule*> ModuleInstanceMap;
    typedef std::map<std::string, NFIModule*> TestModuleInstanceMap;
    typedef std::vector<std::pair<std::string, NFIModule*>> NeedExectuteModuleVec;

    typedef void(* DLL_START_PLUGIN_FUNC)(NFIPluginManager* pm);
    typedef void(* DLL_STOP_PLUGIN_FUNC)(NFIPluginManager* pm);

    std::vector<std::string> mStaticPlugin;
	std::map<std::string, std::vector<NFReplaceContent>> mReplaceContent;

    PluginNameMap mPluginNameMap;
    PluginLibMap mPluginLibMap;
    PluginInstanceMap mPluginInstanceMap;
    ModuleInstanceMap mModuleInstanceMap;
    TestModuleInstanceMap mTestModuleInstanceMap;
    NeedExectuteModuleVec mNeedExecuteModuleVec;

    GET_FILECONTENT_FUNCTOR mGetFileContentFunctor;
};

#endif
