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


#include <assert.h>
#include "NFLuaPBModule.h"
#include "NFLuaScriptModule.h"
#include "NFLuaScriptPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

#define TRY_RUN_GLOBAL_SCRIPT_FUN0(strFuncName)   try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(); }   catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN1(strFuncName, arg1)  try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(arg1); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN2(strFuncName, arg1, arg2)  try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(arg1, arg2); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }

#define TRY_LOAD_SCRIPT_FLE(fileName)  try{mLuaContext.doFile(fileName);} catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }

bool NFLuaScriptModule::Awake()
{
	mnTime = pPluginManager->GetNowTime();

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
    m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
    m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
    m_pNetModule = pPluginManager->FindModule<NFINetModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pLuaPBModule = pPluginManager->FindModule<NFILuaPBModule>();

	NFLuaPBModule* p = (NFLuaPBModule*)(m_pLuaPBModule);
	p->SetLuaState(mLuaContext.state());

    Register();

	std::string strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/NFScriptSystem.lua";

	TRY_LOAD_SCRIPT_FLE(strRootFileh.c_str());

	TRY_RUN_GLOBAL_SCRIPT_FUN1("init_script_system", this);

	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_awake");

	return true;
}

bool NFLuaScriptModule::Init()
{
    TRY_RUN_GLOBAL_SCRIPT_FUN0("module_init");

    return true;
}

bool NFLuaScriptModule::AfterInit()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_after_init");

    return true;
}

bool NFLuaScriptModule::Shut()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_shut");

    return true;
}

bool NFLuaScriptModule::ReadyExecute()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_ready_execute");

	return true;
}

bool NFLuaScriptModule::Execute()
{
    //1分钟reload一次
#ifdef NF_DEBUG_MODE
    if (pPluginManager->GetNowTime() - mnTime > 10)
#else
    if (pPluginManager->GetNowTime() - mnTime > 60)
#endif
    {
        mnTime = pPluginManager->GetNowTime();

        OnScriptReload();
    }

    return true;
}

bool NFLuaScriptModule::BeforeShut()
{
    TRY_RUN_GLOBAL_SCRIPT_FUN0("module_before_shut");

    return true;
}

void NFLuaScriptModule::RegisterModule(const std::string & tableName, const LuaIntf::LuaRef & luatbl)
{
	mxTableName[tableName] = luatbl;
}

NFGUID NFLuaScriptModule::CreateObject(const NFGUID & self, const int sceneID, const int groupID, const std::string & className, const std::string & strIndex, const NFDataList & arg)
{
	NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->CreateObject(self, sceneID, groupID, className, strIndex, arg);
	if (xObject)
	{
		return xObject->Self();

	}

	return NFGUID();
}

bool NFLuaScriptModule::ExistObject(const NFGUID & self)
{
	return m_pKernelModule->ExistObject(self);
}

bool NFLuaScriptModule::DestroyObject(const NFGUID & self)
{
	return m_pKernelModule->DestroyObject(self);
}

bool NFLuaScriptModule::EnterScene(const int sceneID, const int groupID)
{
	return false;
}

bool NFLuaScriptModule::DoEvent(const NFGUID & self, const int eventID, const NFDataList & arg)
{
	m_pEventModule->DoEvent(self, (int)eventID, arg);

	return true;
}

bool NFLuaScriptModule::FindProperty(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->FindProperty(self, propertyName);
}

bool NFLuaScriptModule::SetPropertyInt(const NFGUID & self, const std::string & propertyName, const NFINT64 nValue)
{
	return m_pKernelModule->SetPropertyInt(self, propertyName, nValue);
}

bool NFLuaScriptModule::SetPropertyFloat(const NFGUID & self, const std::string & propertyName, const double dValue)
{
	return m_pKernelModule->SetPropertyFloat(self, propertyName, dValue);
}

bool NFLuaScriptModule::SetPropertyString(const NFGUID & self, const std::string & propertyName, const std::string & value)
{
	return m_pKernelModule->SetPropertyString(self, propertyName, value);
}

bool NFLuaScriptModule::SetPropertyObject(const NFGUID & self, const std::string & propertyName, const NFGUID & objectValue)
{
	return m_pKernelModule->SetPropertyObject(self, propertyName, objectValue);
}

bool NFLuaScriptModule::SetPropertyVector2(const NFGUID & self, const std::string & propertyName, const NFVector2 & value)
{
	return m_pKernelModule->SetPropertyVector2(self, propertyName, value);
}

bool NFLuaScriptModule::SetPropertyVector3(const NFGUID & self, const std::string & propertyName, const NFVector3 & value)
{
	return m_pKernelModule->SetPropertyVector3(self, propertyName, value);
}

NFINT64 NFLuaScriptModule::GetPropertyInt(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->GetPropertyInt(self, propertyName);
}

int NFLuaScriptModule::GetPropertyInt32(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->GetPropertyInt32(self, propertyName);
}

double NFLuaScriptModule::GetPropertyFloat(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->GetPropertyFloat(self, propertyName);
}

std::string NFLuaScriptModule::GetPropertyString(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->GetPropertyString(self, propertyName);
}

NFGUID NFLuaScriptModule::GetPropertyObject(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->GetPropertyObject(self, propertyName);
}

NFVector2 NFLuaScriptModule::GetPropertyVector2(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->GetPropertyVector2(self, propertyName);
}

NFVector3 NFLuaScriptModule::GetPropertyVector3(const NFGUID & self, const std::string & propertyName)
{
	return m_pKernelModule->GetPropertyVector3(self, propertyName);
}

bool NFLuaScriptModule::AddClassCallBack(std::string& className, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	auto funcNameList = mxClassEventFuncMap.GetElement(className);
	if (!funcNameList)
	{
		funcNameList = new NFList<string>();
		mxClassEventFuncMap.AddElement(className, funcNameList);

		m_pKernelModule->AddClassCallBack(className, this, &NFLuaScriptModule::OnClassEventCB);
	}
	
	std::string strfuncName = FindFuncName(luatbl, luaFunc);
	if (!strfuncName.empty())
	{
		if (!funcNameList->Find(strfuncName))
		{
			funcNameList->Add(strfuncName);

			return true;
		}
	}

	return false;
}

int NFLuaScriptModule::OnClassEventCB(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var)
{
    auto funcNameList = mxClassEventFuncMap.GetElement(className);
    if (funcNameList)
    {
		std::string strFuncNme;
		bool ret = funcNameList->First(strFuncNme);
		while (ret)
		{
			try
			{
				LuaIntf::LuaRef func(mLuaContext, strFuncNme.c_str());
				func.call(self, className, (int)classEvent, (NFDataList)var);
			}
			catch (LuaIntf::LuaException& e)
			{
				cout << e.what() << endl;
				return 0;
			}
			catch (...)
			{
				return 0;
			}

			ret = funcNameList->Next(strFuncNme);
		}
    }

	return -1;
}

void NFLuaScriptModule::OnScriptReload()
{
    NFINT64 nAppType = APPType();
    std::string strRootFileh = "";
    switch ((NF_SERVER_TYPES)(nAppType))
    {
        case NF_SERVER_TYPES::NF_ST_GAME:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/game/game_script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_LOGIN:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/login/login_script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_WORLD:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/world/world_script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_PROXY:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/proxy/proxy_script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_MASTER:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/master/master_script_reload.lua";
        }
        break;
        default:
        break;
    }
    
    if (!strRootFileh.empty())
    {
		TRY_LOAD_SCRIPT_FLE(strRootFileh.c_str());
    }
}

bool NFLuaScriptModule::AddPropertyCallBack(const NFGUID& self, std::string& propertyName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaPropertyCallBackFuncMap, self, propertyName, luaFuncName))
		{
			m_pKernelModule->AddPropertyCallBack(self, propertyName, this, &NFLuaScriptModule::OnLuaPropertyCB);
		}

		return true;
	}
    return false;
}

int NFLuaScriptModule::OnLuaPropertyCB(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar)
{
    return CallLuaFuncFromMap(mxLuaPropertyCallBackFuncMap, propertyName, self, propertyName, oldVar, newVar);
}

bool NFLuaScriptModule::AddRecordCallBack(const NFGUID& self, std::string& recordName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaRecordCallBackFuncMap, self, recordName, luaFuncName))
		{
			m_pKernelModule->AddRecordCallBack(self, recordName, this, &NFLuaScriptModule::OnLuaRecordCB);
		}
		return true;
	}

	return false;
}

int NFLuaScriptModule::OnLuaRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    return CallLuaFuncFromMap(mxLuaRecordCallBackFuncMap, xEventData.recordName, self, xEventData.recordName, xEventData.nOpType, xEventData.row, xEventData.col, oldVar, newVar);
}

bool NFLuaScriptModule::AddEventCallBack(const NFGUID& self, const int eventID, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaEventCallBackFuncMap, self, (int)eventID, luaFuncName))
		{
			m_pEventModule->AddEventCallBack(self, eventID, this, &NFLuaScriptModule::OnLuaEventCB);
		}

		return true;
	}

	return false;
}

int NFLuaScriptModule::OnLuaEventCB(const NFGUID& self, const int eventID, const NFDataList& argVar)
{
    return CallLuaFuncFromMap(mxLuaEventCallBackFuncMap, (int)eventID, self, eventID, (NFDataList&)argVar);
}

bool NFLuaScriptModule::AddModuleSchedule(std::string& strHeartBeatName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc, const float time, const int count)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaHeartBeatCallBackFuncMap, strHeartBeatName, luaFuncName))
		{
			return m_pScheduleModule->AddSchedule(NFGUID(), strHeartBeatName, this, &NFLuaScriptModule::OnLuaHeartBeatCB, time, count);
		}
	}

	return false;
}

bool NFLuaScriptModule::AddSchedule(const NFGUID& self, std::string& strHeartBeatName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc, const float time, const int count)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaHeartBeatCallBackFuncMap, self, strHeartBeatName, luaFuncName))
		{
			m_pScheduleModule->AddSchedule(self, strHeartBeatName, this, &NFLuaScriptModule::OnLuaHeartBeatCB, time, count);
		}

		return true;
	}

	return false;
}

int NFLuaScriptModule::OnLuaHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float time, const int count)
{
    return CallLuaFuncFromMap(mxLuaHeartBeatCallBackFuncMap, strHeartBeatName, self, strHeartBeatName, time, count);
}

int NFLuaScriptModule::OnLuaHeartBeatCB(const std::string& strHeartBeatName, const float time, const int count)
{
    return CallLuaFuncFromMap(mxLuaHeartBeatCallBackFuncMap, strHeartBeatName, strHeartBeatName, time, count);
}

int NFLuaScriptModule::AddRow(const NFGUID& self, std::string& recordName, const NFDataList& var)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, recordName);
    if (nullptr == pRecord)
    {
        return -1;
    }

    return pRecord->AddRow(-1, var);
}

bool NFLuaScriptModule::RemRow(const NFGUID & self, std::string & recordName, const int row)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, recordName);
    if (nullptr == pRecord)
    {
        return false;
    }

    return pRecord->Remove(row);
}

bool NFLuaScriptModule::SetRecordInt(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag, const NFINT64 value)
{
	return m_pKernelModule->SetRecordInt(self, recordName, row, colTag, value);
}

bool NFLuaScriptModule::SetRecordFloat(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag, const double value)
{
	return m_pKernelModule->SetRecordFloat(self, recordName, row, colTag, value);
}

bool NFLuaScriptModule::SetRecordString(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag, const std::string & value)
{
	return m_pKernelModule->SetRecordString(self, recordName, row, colTag, value);
}

bool NFLuaScriptModule::SetRecordObject(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag, const NFGUID & value)
{
	return m_pKernelModule->SetRecordObject(self, recordName, row, colTag, value);
}

bool NFLuaScriptModule::SetRecordVector2(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag, const NFVector2 & value)
{
	return m_pKernelModule->SetRecordVector2(self, recordName, row, colTag, value);
}

bool NFLuaScriptModule::SetRecordVector3(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag, const NFVector3 & value)
{
	return m_pKernelModule->SetRecordVector3(self, recordName, row, colTag, value);
}

NFINT64 NFLuaScriptModule::GetRecordInt(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag)
{
	return m_pKernelModule->GetRecordInt(self, recordName, row, colTag);
}

double NFLuaScriptModule::GetRecordFloat(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag)
{
	return m_pKernelModule->GetRecordFloat(self, recordName, row, colTag);
}

std::string NFLuaScriptModule::GetRecordString(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag)
{
	return m_pKernelModule->GetRecordString(self, recordName, row, colTag);
}

NFGUID NFLuaScriptModule::GetRecordObject(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag)
{
	return m_pKernelModule->GetRecordObject(self, recordName, row, colTag);
}

NFVector2 NFLuaScriptModule::GetRecordVector2(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag)
{
	return m_pKernelModule->GetRecordVector2(self, recordName, row, colTag);
}

NFVector3 NFLuaScriptModule::GetRecordVector3(const NFGUID & self, const std::string & recordName, const int row, const std::string & colTag)
{
	return m_pKernelModule->GetRecordVector3(self, recordName, row, colTag);
}

NFINT64 NFLuaScriptModule::GetNowTime()
{
	return pPluginManager->GetNowTime();
}

NFGUID NFLuaScriptModule::CreateID()
{
	return m_pKernelModule->CreateGUID();
}

NFINT64 NFLuaScriptModule::APPID()
{
	return pPluginManager->GetAppID();
}

NFINT64 NFLuaScriptModule::APPType()
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (pPluginManager->GetAppID() == serverID)
            {
                return serverType;
            }
        }
    }
    
	return 0;
}

bool NFLuaScriptModule::ExistElementObject(const std::string & configName)
{
	return m_pElementModule->ExistElement(configName);
}

std::vector<std::string> NFLuaScriptModule::GetEleList(const std::string& className)
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		return xLogicClass->GetIDList();
    }

    return std::vector<std::string>();
}

NFINT64 NFLuaScriptModule::GetElePropertyInt(const std::string & configName, const std::string & propertyName)
{
	return m_pElementModule->GetPropertyInt(configName, propertyName);
}

double NFLuaScriptModule::GetElePropertyFloat(const std::string & configName, const std::string & propertyName)
{
	return m_pElementModule->GetPropertyFloat(configName, propertyName);
}

std::string NFLuaScriptModule::GetElePropertyString(const std::string & configName, const std::string & propertyName)
{
	return m_pElementModule->GetPropertyString(configName, propertyName);
}

NFVector2 NFLuaScriptModule::GetElePropertyVector2(const std::string & configName, const std::string & propertyName)
{
	return m_pElementModule->GetPropertyVector2(configName, propertyName);
}

NFVector3 NFLuaScriptModule::GetElePropertyVector3(const std::string & configName, const std::string & propertyName)
{
	return m_pElementModule->GetPropertyVector3(configName, propertyName);
}

template<typename T>
bool NFLuaScriptModule::AddLuaFuncToMap(NFMap<T, NFMap<NFGUID, NFList<string>>>& funcMap, const NFGUID& self, T key, string& luaFunc)
{
    auto funcList = funcMap.GetElement(key);
    if (!funcList)
    {
        NFList<string>* funcNameList = new NFList<string>;
        funcNameList->Add(luaFunc);
        funcList = new NFMap<NFGUID, NFList<string>>;
        funcList->AddElement(self, funcNameList);
        funcMap.AddElement(key, funcList);
        return true;
    }

    if (!funcList->GetElement(self))
    {
        NFList<string>* funcNameList = new NFList<string>;
        funcNameList->Add(luaFunc);
        funcList->AddElement(self, funcNameList);
        return true;
    }
    else
    {
        auto funcNameList = funcList->GetElement(self);
        if (!funcNameList->Find(luaFunc))
        {
            funcNameList->Add(luaFunc);
            return true;
        }
        else
        {
            return false;
        }
    }

}


template<typename T>
bool NFLuaScriptModule::AddLuaFuncToMap(NFMap<T, NFMap<NFGUID, NFList<string>>>& funcMap, T key, string& luaFunc)
{
    auto funcList = funcMap.GetElement(key);
    if (!funcList)
    {
        NFList<string>* funcNameList = new NFList<string>;
        funcNameList->Add(luaFunc);
        funcList = new NFMap<NFGUID, NFList<string>>;
        funcList->AddElement(NFGUID(), funcNameList);
        funcMap.AddElement(key, funcList);
        return true;
    }

    if (!funcList->GetElement(NFGUID()))
    {
        NFList<string>* funcNameList = new NFList<string>;
        funcNameList->Add(luaFunc);
        funcList->AddElement(NFGUID(), funcNameList);
        return true;
    }
    else
    {
        auto funcNameList = funcList->GetElement(NFGUID());
        if (!funcNameList->Find(luaFunc))
        {
            funcNameList->Add(luaFunc);
            return true;
        }
        else
        {
            return false;
        }
    }

}

template<typename T1, typename ...T2>
bool NFLuaScriptModule::CallLuaFuncFromMap(NFMap<T1, NFMap<NFGUID, NFList<string>>>& funcMap, T1 key, const NFGUID& self, T2 ... arg)
{
    auto funcList = funcMap.GetElement(key);
    if (funcList)
    {
        auto funcNameList = funcList->GetElement(self);
        if (funcNameList)
        {
            string funcName;
            auto Ret = funcNameList->First(funcName);
            while (Ret)
            {
                try
                {
                    LuaIntf::LuaRef func(mLuaContext, funcName.c_str());
                    func.call(self, key, arg ...);
					return true;
                }
                catch (LuaIntf::LuaException& e)
                {
                    cout << e.what() << endl;
                    return false;
                }
				catch (...)
				{
                    return false;
				}

                Ret = funcNameList->Next(funcName);
            }
        }
    }

    return true;
}

template<typename T1, typename ...T2>
bool NFLuaScriptModule::CallLuaFuncFromMap(NFMap<T1, NFMap<NFGUID, NFList<string>>>& funcMap, T1 key, T2 ... arg)
{
    auto funcList = funcMap.GetElement(key);
    if (funcList)
    {
        auto funcNameList = funcList->GetElement(NFGUID());
        if (funcNameList)
        {
            string funcName;
            auto Ret = funcNameList->First(funcName);
            while (Ret)
            {
                try
                {
                    LuaIntf::LuaRef func(mLuaContext, funcName.c_str());
                    func.call(key, arg...);
					return true;
                }
                catch (LuaIntf::LuaException& e)
                {
                    cout << e.what() << endl;
                    return false;
                }
				catch (...)
				{
                    return false;
				}

                Ret = funcNameList->Next(funcName);
            }
        }
    }

    return true;
}

void NFLuaScriptModule::RemoveClientMsgCallBack(const int msgID)
{
	m_pNetModule->RemoveReceiveCallBack(msgID);
}

void NFLuaScriptModule::AddClientMsgCallBack(const int msgID, const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
{
	//m_pNetModule->AddEventCallBack
}

void NFLuaScriptModule::RemoveServerMsgCallBack(const int serverType, const int msgID)
{
	m_pNetClientModule->RemoveReceiveCallBack((NF_SERVER_TYPES)serverType, msgID);
}

void NFLuaScriptModule::AddServerMsgCallBack(const int serverType, const int msgID, const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
{
	//m_pNetClientModule->AddReceiveCallBack((NF_SERVER_TYPES)serverType, )
}

void NFLuaScriptModule::RemoveHttpCallBack(const std::string & path)
{
}

void NFLuaScriptModule::AddHttpCallBack(const std::string & path, const int httpType, const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
{
}

void NFLuaScriptModule::ImportProtoFile(const std::string& strFile)
{
	NFLuaPBModule* p = (NFLuaPBModule*)m_pLuaPBModule;
	p->ImportProtoFile(strFile);
}

const std::string NFLuaScriptModule::Encode(const std::string& strMsgTypeName, const LuaIntf::LuaRef& luaTable)
{
	NFLuaPBModule* p = (NFLuaPBModule*)m_pLuaPBModule;
	return p->Encode(strMsgTypeName, luaTable);
}

LuaIntf::LuaRef NFLuaScriptModule::Decode(const std::string& strMsgTypeName, const std::string& strData)
{
	NFLuaPBModule* p = (NFLuaPBModule*)m_pLuaPBModule;
	return p->Decode(strMsgTypeName, strData);
}

void NFLuaScriptModule::SendByServerFD(const NFSOCK nFD, const uint16_t msgID, const std::string& strData)
{
    m_pNetModule->SendMsgWithOutHead(msgID, strData, nFD);
}

void NFLuaScriptModule::SendByServerID(const int serverID, const uint16_t msgID, const std::string& strData)
{
    if (pPluginManager->GetAppID() == serverID)
    {
        m_pLogModule->LogError("you can send message to youself");
        return;
    }

    NF_SERVER_TYPES nowServerType = NF_SERVER_TYPES::NF_ST_NONE;
    NF_SERVER_TYPES goalServerType = NF_SERVER_TYPES::NF_ST_NONE;

    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nTempServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nTempServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (pPluginManager->GetAppID() == nTempServerID)
			{
                nowServerType = (NF_SERVER_TYPES)(nTempServerType);
                break;
            }
        }

        for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nTempServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nTempServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (serverID == nTempServerID)
			{
                goalServerType = (NF_SERVER_TYPES)(nTempServerType);
                break;
            }
        }
    }

    if (goalServerType == NF_SERVER_TYPES::NF_ST_NONE || nowServerType == NF_SERVER_TYPES::NF_ST_NONE)
    {
        m_pLogModule->LogError("goal type: none or now type: none");
        return;
    }

    switch (nowServerType)
    {
        case NF_SERVER_TYPES::NF_ST_GAME:
        {
            if (NF_SERVER_TYPES::NF_ST_WORLD == goalServerType)
            {
                m_pNetClientModule->SendByServerID(serverID, msgID, strData);
            }
            else if (NF_SERVER_TYPES::NF_ST_PROXY == goalServerType)
            {
                //
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_WORLD:
        {
            if (NF_SERVER_TYPES::NF_ST_GAME == goalServerType)
            {
                //
            }
            else if (NF_SERVER_TYPES::NF_ST_MASTER == goalServerType)
            {
                m_pNetClientModule->SendByServerID(serverID, msgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_PROXY:
        {
            if (NF_SERVER_TYPES::NF_ST_GAME == goalServerType)
            {
                m_pNetClientModule->SendByServerID(serverID, msgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_MASTER:
        {
            if (NF_SERVER_TYPES::NF_ST_WORLD == goalServerType)
            {

            }
            else if (NF_SERVER_TYPES::NF_ST_LOGIN == goalServerType)
            {

            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_LOGIN:
        {
            if (NF_SERVER_TYPES::NF_ST_MASTER == goalServerType)
            {
                m_pNetClientModule->SendByServerID(serverID, msgID, strData);
            }
        }
        break;
        default:
        break;
    }
}

void NFLuaScriptModule::SendByServerType(const NF_SERVER_TYPES eType, const uint16_t msgID, const std::string & strData)
{
    NF_SERVER_TYPES nowServerType = NF_SERVER_TYPES::NF_ST_NONE;
    NF_SERVER_TYPES goalServerType = eType;

    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nTempServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nTempServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (pPluginManager->GetAppID() == nTempServerID)
			{
                nowServerType = (NF_SERVER_TYPES)(nTempServerType);
                break;
            }
        }
    }

    if (goalServerType == NF_SERVER_TYPES::NF_ST_NONE || nowServerType == NF_SERVER_TYPES::NF_ST_NONE)
    {
        m_pLogModule->LogError("goal type: none or now type: none");
        return;
    }

    if (goalServerType == nowServerType)
    {
        m_pLogModule->LogError("can not send the message to the same server type");
        return;
    }

    switch (nowServerType)
    {
        case NF_SERVER_TYPES::NF_ST_GAME:
        {
            if (NF_SERVER_TYPES::NF_ST_WORLD == goalServerType)
            {
                m_pNetClientModule->SendToAllServer(msgID, strData);
            }
            else if (NF_SERVER_TYPES::NF_ST_PROXY == goalServerType)
            {
                m_pNetModule->SendMsgToAllClientWithOutHead(msgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_WORLD:
        {
            if (NF_SERVER_TYPES::NF_ST_GAME == goalServerType)
            {
                m_pNetModule->SendMsgToAllClientWithOutHead(msgID, strData);
            }
            else if (NF_SERVER_TYPES::NF_ST_MASTER == goalServerType)
            {
                m_pNetClientModule->SendToAllServer(msgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_PROXY:
        {
            if (NF_SERVER_TYPES::NF_ST_GAME == goalServerType)
            {
                m_pNetClientModule->SendToAllServer(msgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_MASTER:
        {
            if (NF_SERVER_TYPES::NF_ST_WORLD == goalServerType)
            {
                //m_pNetModule->SendMsgToAllClientWithOutHead(msgID, strData);
            }
            else if (NF_SERVER_TYPES::NF_ST_LOGIN == goalServerType)
            {

            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_LOGIN:
        {
            if (NF_SERVER_TYPES::NF_ST_MASTER == goalServerType)
            {
                m_pNetClientModule->SendToAllServer(msgID, strData);
            }
        }
        break;
        default:
        break;
    }
}

void NFLuaScriptModule::SendMsgToGate(const NFGUID player, const uint16_t msgID, const std::string& strData)
{
    //the app must be the game server
}

void NFLuaScriptModule::SendGroupMsgToGate(const uint16_t msgID, const std::string& strData)
{
    //the app must be the game server

}

void NFLuaScriptModule::SendToAllPlayer(const uint16_t msgID, const std::string& strData)
{
	m_pNetModule->SendMsgToAllClientWithOutHead(msgID, strData);
}

void NFLuaScriptModule::LogInfo(const std::string& strData)
{
	m_pLogModule->LogInfo(strData);
}

void NFLuaScriptModule::LogError(const std::string& strData)
{
	m_pLogModule->LogError(strData);
}

void NFLuaScriptModule::LogWarning(const std::string& strData)
{
	m_pLogModule->LogWarning(strData);
}

void NFLuaScriptModule::LogDebug(const std::string& strData)
{
	m_pLogModule->LogDebug(strData);
}

void NFLuaScriptModule::SetVersionCode(const std::string& strData)
{
    strVersionCode = strData;
}

const std::string&  NFLuaScriptModule::GetVersionCode()
{
    return strVersionCode;
}

bool NFLuaScriptModule::Register()
{

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFGUID>("NFGUID")
		.addConstructor(LUA_ARGS())
		.addProperty("data", &NFGUID::GetData, &NFGUID::SetData)
		.addProperty("head", &NFGUID::GetHead, &NFGUID::SetHead)
		.addFunction("tostring", &NFGUID::ToString)
		.addFunction("fromstring", &NFGUID::FromString)
		.endClass();

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFDataList>("NFDataList")
		.endClass();

	LuaIntf::LuaBinding(mLuaContext).beginExtendClass<NFDataList, NFDataList>("NFDataList")
		.addConstructor(LUA_ARGS())
		.addFunction("empty", &NFDataList::IsEmpty)
		.addFunction("count", &NFDataList::GetCount)
		.addFunction("tye", &NFDataList::Type)

		.addFunction("add_int", &NFDataList::AddInt)
		.addFunction("add_float", &NFDataList::AddFloat)
		.addFunction("add_string", &NFDataList::AddStringFromChar)
		.addFunction("add_object", &NFDataList::AddObject)
		.addFunction("add_vector2", &NFDataList::AddVector2)
		.addFunction("add_vector3", &NFDataList::AddVector3)

		.addFunction("set_int", &NFDataList::SetInt)
		.addFunction("set_float", &NFDataList::SetFloat)
		.addFunction("set_string", &NFDataList::SetString)
		.addFunction("set_object", &NFDataList::SetObject)
		.addFunction("set_vector2", &NFDataList::SetVector2)
		.addFunction("set_vector3", &NFDataList::SetVector3)

		.addFunction("int", &NFDataList::Int)
		.addFunction("float", &NFDataList::Float)
		.addFunction("string", &NFDataList::String)
		.addFunction("object", &NFDataList::Object)
		.addFunction("vector2", &NFDataList::Vector2)
		.addFunction("vector3", &NFDataList::Vector3)
		.endClass();

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFData>("TData")
		.addConstructor(LUA_ARGS())
		.addFunction("float", &NFData::GetFloat)
		.addFunction("int", &NFData::GetInt)
		.addFunction("object", &NFData::GetObject)
		.addFunction("string", &NFData::GetString)
		.addFunction("vector2", &NFData::GetVector2)
		.addFunction("vector3", &NFData::GetVector3)

		.addFunction("type", &NFData::GetType)
		.addFunction("is_null", &NFData::IsNullValue)

		.addFunction("set_float", &NFData::SetFloat)
		.addFunction("set_int", &NFData::SetInt)
		.addFunction("set_object", &NFData::SetObject)
		.addFunction("set_string", &NFData::SetString)
		.addFunction("set_vector2", &NFData::SetVector2)
		.addFunction("set_vector3", &NFData::SetVector3)
		.endClass();
	//for kernel module

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFLuaScriptModule>("NFLuaScriptModule")
		.addFunction("register_module", &NFLuaScriptModule::RegisterModule)
		.addFunction("create_object", &NFLuaScriptModule::CreateObject)
		.addFunction("exist_object", &NFLuaScriptModule::ExistObject)
		.addFunction("destroy_object", &NFLuaScriptModule::DestroyObject)
		.addFunction("enter_scene", &NFLuaScriptModule::EnterScene)
		.addFunction("do_event", &NFLuaScriptModule::DoEvent)

		.addFunction("set_prop_int", &NFLuaScriptModule::SetPropertyInt)
		.addFunction("set_prop_float", &NFLuaScriptModule::SetPropertyFloat)
		.addFunction("set_prop_string", &NFLuaScriptModule::SetPropertyString)
		.addFunction("set_prop_object", &NFLuaScriptModule::SetPropertyObject)
		.addFunction("set_prop_vector2", &NFLuaScriptModule::SetPropertyVector2)
		.addFunction("set_prop_vector3", &NFLuaScriptModule::SetPropertyVector3)

		.addFunction("get_prop_int", &NFLuaScriptModule::GetPropertyInt)
		.addFunction("get_prop_float", &NFLuaScriptModule::GetPropertyFloat)
		.addFunction("get_prop_string", &NFLuaScriptModule::GetPropertyString)
		.addFunction("get_prop_object", &NFLuaScriptModule::GetPropertyObject)
		.addFunction("get_prop_vector2", &NFLuaScriptModule::GetPropertyVector2)
		.addFunction("get_prop_vector3", &NFLuaScriptModule::GetPropertyVector3)

		.addFunction("set_record_int", &NFLuaScriptModule::SetRecordInt)
		.addFunction("set_record_float",&NFLuaScriptModule::SetRecordFloat)
		.addFunction("set_record_string", &NFLuaScriptModule::SetRecordString)
		.addFunction("set_record_object", &NFLuaScriptModule::SetRecordObject)
		.addFunction("set_record_vector2", &NFLuaScriptModule::SetPropertyVector2)
		.addFunction("set_record_vector3", &NFLuaScriptModule::SetPropertyVector3)

		.addFunction("get_record_int", &NFLuaScriptModule::GetRecordInt)
		.addFunction("get_record_float", &NFLuaScriptModule::GetRecordFloat)
		.addFunction("get_record_string", &NFLuaScriptModule::GetRecordString)
		.addFunction("get_record_object", &NFLuaScriptModule::GetRecordObject)
		.addFunction("get_record_vector2", &NFLuaScriptModule::GetPropertyVector2)
		.addFunction("get_record_vector3", &NFLuaScriptModule::GetPropertyVector3)

		.addFunction("add_prop_cb", &NFLuaScriptModule::AddPropertyCallBack)
		.addFunction("add_record_cb", &NFLuaScriptModule::AddRecordCallBack)
		.addFunction("add_event_cb", &NFLuaScriptModule::AddEventCallBack)
		.addFunction("add_class_cb", &NFLuaScriptModule::AddClassCallBack)
		.addFunction("add_schedule", &NFLuaScriptModule::AddSchedule)
		.addFunction("add_module_schedule", &NFLuaScriptModule::AddModuleSchedule)
		.addFunction("do_event", &NFLuaScriptModule::DoEvent)
		.addFunction("add_row", &NFLuaScriptModule::AddRow)
		.addFunction("rem_row", &NFLuaScriptModule::RemRow)

		.addFunction("time", &NFLuaScriptModule::GetNowTime)
		.addFunction("new_id", &NFLuaScriptModule::CreateID)
		.addFunction("app_id", &NFLuaScriptModule::APPID)
		.addFunction("app_type", &NFLuaScriptModule::APPType)

		.addFunction("exist_ele", &NFLuaScriptModule::ExistElementObject)
		.addFunction("get_ele_list", &NFLuaScriptModule::GetEleList)
		.addFunction("get_ele_int", &NFLuaScriptModule::GetElePropertyInt)
		.addFunction("get_ele_float", &NFLuaScriptModule::GetElePropertyFloat)
		.addFunction("get_ele_string", &NFLuaScriptModule::GetElePropertyString)
		.addFunction("get_ele_vector2", &NFLuaScriptModule::GetElePropertyVector2)
		.addFunction("get_ele_vector3", &NFLuaScriptModule::GetElePropertyVector3)

		.addFunction("remove_cli_msg_cb", &NFLuaScriptModule::RemoveClientMsgCallBack)//as server
		.addFunction("add_cli_msg_cb", &NFLuaScriptModule::AddClientMsgCallBack)//as server
		.addFunction("remove_svr_msg_cb", &NFLuaScriptModule::RemoveServerMsgCallBack)//as client
		.addFunction("add_svr_msg_cb", &NFLuaScriptModule::AddServerMsgCallBack)//as client

		.addFunction("remove_http_cb", &NFLuaScriptModule::RemoveHttpCallBack)
		.addFunction("add_http_cb", &NFLuaScriptModule::AddHttpCallBack)

		.addFunction("send_by_fd", &NFLuaScriptModule::SendByServerFD)//as client
		.addFunction("send_by_id", &NFLuaScriptModule::SendByServerID)//as clent
		.addFunction("send_by_type", &NFLuaScriptModule::SendByServerType)//as client

		.addFunction("send_to_player", &NFLuaScriptModule::SendMsgToGate)//as game
		.addFunction("send_to_group_player", &NFLuaScriptModule::SendGroupMsgToGate)//as game
		.addFunction("send_to_all_player", &NFLuaScriptModule::SendToAllPlayer)//as game

		.addFunction("log_info", &NFLuaScriptModule::LogInfo)
		.addFunction("log_error", &NFLuaScriptModule::LogError)
		.addFunction("log_warning", &NFLuaScriptModule::LogWarning)
		.addFunction("log_debug", &NFLuaScriptModule::LogDebug)

		.addFunction("get_version_code", &NFLuaScriptModule::GetVersionCode)
		.addFunction("set_version_code", &NFLuaScriptModule::SetVersionCode)

		.addFunction("import_proto_file", &NFLuaScriptModule::ImportProtoFile)
		.addFunction("encode", &NFLuaScriptModule::Encode)
		.addFunction("decode", &NFLuaScriptModule::Decode)

		.endClass();

    return true;
}

std::string NFLuaScriptModule::FindFuncName(const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
{
	if (luatbl.isTable() && luaFunc.isFunction())
	{
		std::string strLuaTableName = "";
		std::map<std::string, LuaIntf::LuaRef>::iterator it = mxTableName.begin();
		for (it; it != mxTableName.end(); ++it)
		{
			if (it->second == luatbl)
			{
				strLuaTableName = it->first;
			}
		}
		
		if (!strLuaTableName.empty())
		{
			for (auto itr = luatbl.begin(); itr != luatbl.end(); ++itr)
			{
				const LuaIntf::LuaRef& key = itr.key();

				const std::string& sKey = key.toValue<std::string>();
				const LuaIntf::LuaRef& val = itr.value();
				if (val.isFunction() && luaFunc.isFunction() && val == luaFunc)
				{
					strLuaTableName.append(".");
					strLuaTableName.append(sKey);
					return strLuaTableName;
				}
			}
		}
	}

	return NULL_STR;
}
