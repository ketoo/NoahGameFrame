/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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
#include "NFCLuaPBModule.h"
#include "NFCLuaScriptModule.h"
#include "NFLuaScriptPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

#define TRY_RUN_GLOBAL_SCRIPT_FUN0(strFuncName)   try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(); }   catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN1(strFuncName, arg1)  try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(arg1); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN2(strFuncName, arg1, arg2)  try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(arg1, arg2); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }

#define TRY_LOAD_SCRIPT_FLE(strFileName)  try{mLuaContext.doFile(strFileName);} catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }

bool NFCLuaScriptModule::Awake()
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

	NFCLuaPBModule* p = (NFCLuaPBModule*)(m_pLuaPBModule);
	p->SetLuaState(mLuaContext.state());

    Register();

	std::string strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/NFScriptSystem.lua";

	TRY_LOAD_SCRIPT_FLE(strRootFileh.c_str());

	TRY_RUN_GLOBAL_SCRIPT_FUN1("init_script_system", this);

	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_awake");

	return true;
}

bool NFCLuaScriptModule::Init()
{
    TRY_RUN_GLOBAL_SCRIPT_FUN0("module_init");

    return true;
}

bool NFCLuaScriptModule::AfterInit()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_after_init");

    return true;
}

bool NFCLuaScriptModule::Shut()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_shut");

    return true;
}

bool NFCLuaScriptModule::ReadyExecute()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_ready_execute");

	return true;
}

bool NFCLuaScriptModule::Execute()
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

bool NFCLuaScriptModule::BeforeShut()
{
    TRY_RUN_GLOBAL_SCRIPT_FUN0("module_before_shut");

    return true;
}

void NFCLuaScriptModule::RegisterModule(const std::string & tableName, const LuaIntf::LuaRef & luatbl)
{
	mxTableName[tableName] = luatbl;
}

NFGUID NFCLuaScriptModule::CreateObject(const NFGUID & self, const int nSceneID, const int nGroupID, const std::string & strClassName, const std::string & strIndex, const NFDataList & arg)
{
	NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->CreateObject(self, nSceneID, nGroupID, strClassName, strIndex, arg);
	if (xObject)
	{
		return xObject->Self();

	}

	return NFGUID();
}

bool NFCLuaScriptModule::ExistObject(const NFGUID & self)
{
	return m_pKernelModule->ExistObject(self);
}

bool NFCLuaScriptModule::DestroyObject(const NFGUID & self)
{
	return m_pKernelModule->DestroyObject(self);
}

bool NFCLuaScriptModule::EnterScene(const int nSceneID, const int nGroupID)
{
	return false;
}

bool NFCLuaScriptModule::DoEvent(const NFGUID & self, const int nEventID, const NFDataList & arg)
{
	m_pEventModule->DoEvent(self, (NFEventDefine)nEventID, arg);

	return true;
}

bool NFCLuaScriptModule::FindProperty(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->FindProperty(self, strPropertyName);
}

bool NFCLuaScriptModule::SetPropertyInt(const NFGUID & self, const std::string & strPropertyName, const NFINT64 nValue)
{
	return m_pKernelModule->SetPropertyInt(self, strPropertyName, nValue);
}

bool NFCLuaScriptModule::SetPropertyFloat(const NFGUID & self, const std::string & strPropertyName, const double dValue)
{
	return m_pKernelModule->SetPropertyFloat(self, strPropertyName, dValue);
}

bool NFCLuaScriptModule::SetPropertyString(const NFGUID & self, const std::string & strPropertyName, const std::string & strValue)
{
	return m_pKernelModule->SetPropertyString(self, strPropertyName, strValue);
}

bool NFCLuaScriptModule::SetPropertyObject(const NFGUID & self, const std::string & strPropertyName, const NFGUID & objectValue)
{
	return m_pKernelModule->SetPropertyObject(self, strPropertyName, objectValue);
}

bool NFCLuaScriptModule::SetPropertyVector2(const NFGUID & self, const std::string & strPropertyName, const NFVector2 & value)
{
	return m_pKernelModule->SetPropertyVector2(self, strPropertyName, value);
}

bool NFCLuaScriptModule::SetPropertyVector3(const NFGUID & self, const std::string & strPropertyName, const NFVector3 & value)
{
	return m_pKernelModule->SetPropertyVector3(self, strPropertyName, value);
}

NFINT64 NFCLuaScriptModule::GetPropertyInt(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->GetPropertyInt(self, strPropertyName);
}

int NFCLuaScriptModule::GetPropertyInt32(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->GetPropertyInt32(self, strPropertyName);
}

double NFCLuaScriptModule::GetPropertyFloat(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->GetPropertyFloat(self, strPropertyName);
}

std::string NFCLuaScriptModule::GetPropertyString(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->GetPropertyString(self, strPropertyName);
}

NFGUID NFCLuaScriptModule::GetPropertyObject(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->GetPropertyObject(self, strPropertyName);
}

NFVector2 NFCLuaScriptModule::GetPropertyVector2(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->GetPropertyVector2(self, strPropertyName);
}

NFVector3 NFCLuaScriptModule::GetPropertyVector3(const NFGUID & self, const std::string & strPropertyName)
{
	return m_pKernelModule->GetPropertyVector3(self, strPropertyName);
}

bool NFCLuaScriptModule::AddClassCallBack(std::string& className, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	auto funcNameList = mxClassEventFuncMap.GetElement(className);
	if (!funcNameList)
	{
		funcNameList = new NFList<string>();
		mxClassEventFuncMap.AddElement(className, funcNameList);

		m_pKernelModule->AddClassCallBack(className, this, &NFCLuaScriptModule::OnClassEventCB);
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

int NFCLuaScriptModule::OnClassEventCB(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    auto funcNameList = mxClassEventFuncMap.GetElement(strClassName);
    if (funcNameList)
    {
		std::string strFuncNme;
		bool ret = funcNameList->First(strFuncNme);
		while (ret)
		{
			try
			{
				LuaIntf::LuaRef func(mLuaContext, strFuncNme.c_str());
				func.call(self, strClassName, (int)eClassEvent, (NFDataList)var);
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

void NFCLuaScriptModule::OnScriptReload()
{
    NFINT64 nAppType = APPType();
    std::string strRootFileh = "";
    switch ((NF_SERVER_TYPES)(nAppType))
    {
        case NF_SERVER_TYPES::NF_ST_GAME:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/game/script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_LOGIN:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/login/script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_WORLD:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/world/script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_PROXY:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/proxy/script_reload.lua";
        }
        break;
        case NF_SERVER_TYPES::NF_ST_MASTER:
        {
            strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/master/script_reload.lua";
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

bool NFCLuaScriptModule::AddPropertyCallBack(const NFGUID& self, std::string& strPropertyName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaPropertyCallBackFuncMap, self, strPropertyName, luaFuncName))
		{
			m_pKernelModule->AddPropertyCallBack(self, strPropertyName, this, &NFCLuaScriptModule::OnLuaPropertyCB);
		}

		return true;
	}
    return false;
}

int NFCLuaScriptModule::OnLuaPropertyCB(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
    return CallLuaFuncFromMap(mxLuaPropertyCallBackFuncMap, strPropertyName, self, strPropertyName, oldVar, newVar);
}

bool NFCLuaScriptModule::AddRecordCallBack(const NFGUID& self, std::string& strRecordName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaRecordCallBackFuncMap, self, strRecordName, luaFuncName))
		{
			m_pKernelModule->AddRecordCallBack(self, strRecordName, this, &NFCLuaScriptModule::OnLuaRecordCB);
		}
		return true;
	}

	return false;
}

int NFCLuaScriptModule::OnLuaRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    return CallLuaFuncFromMap(mxLuaRecordCallBackFuncMap, xEventData.strRecordName, self, xEventData.strRecordName, xEventData.nOpType, xEventData.nRow, xEventData.nCol, oldVar, newVar);
}

bool NFCLuaScriptModule::AddEventCallBack(const NFGUID& self, const NFEventDefine nEventID, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaEventCallBackFuncMap, self, (int)nEventID, luaFuncName))
		{
			m_pEventModule->AddEventCallBack(self, nEventID, this, &NFCLuaScriptModule::OnLuaEventCB);
		}

		return true;
	}

	return false;
}

int NFCLuaScriptModule::OnLuaEventCB(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& argVar)
{
    return CallLuaFuncFromMap(mxLuaEventCallBackFuncMap, (int)nEventID, self, nEventID, (NFDataList&)argVar);
}

bool NFCLuaScriptModule::AddModuleSchedule(std::string& strHeartBeatName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc, const float fTime, const int nCount)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaHeartBeatCallBackFuncMap, strHeartBeatName, luaFuncName))
		{
			return m_pScheduleModule->AddSchedule(strHeartBeatName, this, &NFCLuaScriptModule::OnLuaHeartBeatCB, fTime, nCount);
		}
	}

	return false;
}

bool NFCLuaScriptModule::AddSchedule(const NFGUID& self, std::string& strHeartBeatName, const LuaIntf::LuaRef& luatbl, const LuaIntf::LuaRef& luaFunc, const float fTime, const int nCount)
{
	std::string luaFuncName = FindFuncName(luatbl, luaFunc);
	if (!luaFuncName.empty())
	{
		if (AddLuaFuncToMap(mxLuaHeartBeatCallBackFuncMap, self, strHeartBeatName, luaFuncName))
		{
			m_pScheduleModule->AddSchedule(self, strHeartBeatName, this, &NFCLuaScriptModule::OnLuaHeartBeatCB, fTime, nCount);
		}

		return true;
	}

	return false;
}

int NFCLuaScriptModule::OnLuaHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float fTime, const int nCount)
{
    return CallLuaFuncFromMap(mxLuaHeartBeatCallBackFuncMap, strHeartBeatName, self, strHeartBeatName, fTime, nCount);
}

int NFCLuaScriptModule::OnLuaHeartBeatCB(const std::string& strHeartBeatName, const float fTime, const int nCount)
{
    return CallLuaFuncFromMap(mxLuaHeartBeatCallBackFuncMap, strHeartBeatName, strHeartBeatName, fTime, nCount);
}

int NFCLuaScriptModule::AddRow(const NFGUID& self, std::string& strRecordName, const NFDataList& var)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (nullptr == pRecord)
    {
        return -1;
    }

    return pRecord->AddRow(-1, var);
}

bool NFCLuaScriptModule::RemRow(const NFGUID & self, std::string & strRecordName, const int nRow)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (nullptr == pRecord)
    {
        return false;
    }

    return pRecord->Remove(nRow);
}

bool NFCLuaScriptModule::SetRecordInt(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFINT64 value)
{
	return m_pKernelModule->SetRecordInt(self, strRecordName, nRow, strColTag, value);
}

bool NFCLuaScriptModule::SetRecordFloat(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const double value)
{
	return m_pKernelModule->SetRecordFloat(self, strRecordName, nRow, strColTag, value);
}

bool NFCLuaScriptModule::SetRecordString(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const std::string & value)
{
	return m_pKernelModule->SetRecordString(self, strRecordName, nRow, strColTag, value);
}

bool NFCLuaScriptModule::SetRecordObject(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFGUID & value)
{
	return m_pKernelModule->SetRecordObject(self, strRecordName, nRow, strColTag, value);
}

bool NFCLuaScriptModule::SetRecordVector2(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFVector2 & value)
{
	return m_pKernelModule->SetRecordVector2(self, strRecordName, nRow, strColTag, value);
}

bool NFCLuaScriptModule::SetRecordVector3(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFVector3 & value)
{
	return m_pKernelModule->SetRecordVector3(self, strRecordName, nRow, strColTag, value);
}

NFINT64 NFCLuaScriptModule::GetRecordInt(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return m_pKernelModule->GetRecordInt(self, strRecordName, nRow, strColTag);
}

double NFCLuaScriptModule::GetRecordFloat(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return m_pKernelModule->GetRecordFloat(self, strRecordName, nRow, strColTag);
}

std::string NFCLuaScriptModule::GetRecordString(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return m_pKernelModule->GetRecordString(self, strRecordName, nRow, strColTag);
}

NFGUID NFCLuaScriptModule::GetRecordObject(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return m_pKernelModule->GetRecordObject(self, strRecordName, nRow, strColTag);
}

NFVector2 NFCLuaScriptModule::GetRecordVector2(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return m_pKernelModule->GetRecordVector2(self, strRecordName, nRow, strColTag);
}

NFVector3 NFCLuaScriptModule::GetRecordVector3(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return m_pKernelModule->GetRecordVector3(self, strRecordName, nRow, strColTag);
}

NFINT64 NFCLuaScriptModule::GetNowTime()
{
	return pPluginManager->GetNowTime();
}

NFGUID NFCLuaScriptModule::CreateID()
{
	return m_pKernelModule->CreateGUID();
}

NFINT64 NFCLuaScriptModule::APPID()
{
	return pPluginManager->GetAppID();
}

NFINT64 NFCLuaScriptModule::APPType()
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (pPluginManager->GetAppID() == nServerID)
            {
                return nServerType;
            }
        }
    }
    
	return 0;
}

bool NFCLuaScriptModule::ExistElementObject(const std::string & strConfigName)
{
	return m_pElementModule->ExistElement(strConfigName);
}

std::vector<std::string> NFCLuaScriptModule::GetEleList(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		return xLogicClass->GetIDList();
    }

    return std::vector<std::string>();
}

NFINT64 NFCLuaScriptModule::GetElePropertyInt(const std::string & strConfigName, const std::string & strPropertyName)
{
	return m_pElementModule->GetPropertyInt(strConfigName, strPropertyName);
}

double NFCLuaScriptModule::GetElePropertyFloat(const std::string & strConfigName, const std::string & strPropertyName)
{
	return m_pElementModule->GetPropertyFloat(strConfigName, strPropertyName);
}

std::string NFCLuaScriptModule::GetElePropertyString(const std::string & strConfigName, const std::string & strPropertyName)
{
	return m_pElementModule->GetPropertyString(strConfigName, strPropertyName);
}

NFVector2 NFCLuaScriptModule::GetElePropertyVector2(const std::string & strConfigName, const std::string & strPropertyName)
{
	return m_pElementModule->GetPropertyVector2(strConfigName, strPropertyName);
}

NFVector3 NFCLuaScriptModule::GetElePropertyVector3(const std::string & strConfigName, const std::string & strPropertyName)
{
	return m_pElementModule->GetPropertyVector3(strConfigName, strPropertyName);
}

template<typename T>
bool NFCLuaScriptModule::AddLuaFuncToMap(NFMap<T, NFMap<NFGUID, NFList<string>>>& funcMap, const NFGUID& self, T key, string& luaFunc)
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
bool NFCLuaScriptModule::AddLuaFuncToMap(NFMap<T, NFMap<NFGUID, NFList<string>>>& funcMap, T key, string& luaFunc)
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
bool NFCLuaScriptModule::CallLuaFuncFromMap(NFMap<T1, NFMap<NFGUID, NFList<string>>>& funcMap, T1 key, const NFGUID& self, T2 ... arg)
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
bool NFCLuaScriptModule::CallLuaFuncFromMap(NFMap<T1, NFMap<NFGUID, NFList<string>>>& funcMap, T1 key, T2 ... arg)
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

void NFCLuaScriptModule::RemoveClientMsgCallBack(const int nMsgID)
{
	m_pNetModule->RemoveReceiveCallBack(nMsgID);
}

void NFCLuaScriptModule::AddClientMsgCallBack(const int nMsgID, const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
{
	//m_pNetModule->AddEventCallBack
}

void NFCLuaScriptModule::RemoveServerMsgCallBack(const int nServerType, const int nMsgID)
{
	m_pNetClientModule->RemoveReceiveCallBack((NF_SERVER_TYPES)nServerType, nMsgID);
}

void NFCLuaScriptModule::AddServerMsgCallBack(const int nServerType, const int nMsgID, const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
{
	//m_pNetClientModule->AddReceiveCallBack((NF_SERVER_TYPES)nServerType, )
}

void NFCLuaScriptModule::RemoveHttpCallBack(const std::string & path)
{
}

void NFCLuaScriptModule::AddHttpCallBack(const std::string & path, const int httpType, const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
{
}

void NFCLuaScriptModule::ImportProtoFile(const std::string& strFile)
{
	NFCLuaPBModule* p = (NFCLuaPBModule*)m_pLuaPBModule;
	p->ImportProtoFile(strFile);
}

const std::string NFCLuaScriptModule::Encode(const std::string& strMsgTypeName, const LuaIntf::LuaRef& luaTable)
{
	NFCLuaPBModule* p = (NFCLuaPBModule*)m_pLuaPBModule;
	return p->Encode(strMsgTypeName, luaTable);
}

LuaIntf::LuaRef NFCLuaScriptModule::Decode(const std::string& strMsgTypeName, const std::string& strData)
{
	NFCLuaPBModule* p = (NFCLuaPBModule*)m_pLuaPBModule;
	return p->Decode(strMsgTypeName, strData);
}

void NFCLuaScriptModule::SendByServerFD(const NFSOCK nFD, const uint16_t nMsgID, const std::string& strData)
{
    m_pNetModule->SendMsgWithOutHead(nMsgID, strData, nFD);
}

void NFCLuaScriptModule::SendByServerID(const int nServerID, const uint16_t nMsgID, const std::string& strData)
{
    if (pPluginManager->GetAppID() == nServerID)
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
			if (nServerID == nTempServerID)
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
                m_pNetClientModule->SendByServerID(nServerID, nMsgID, strData);
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
                m_pNetClientModule->SendByServerID(nServerID, nMsgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_PROXY:
        {
            if (NF_SERVER_TYPES::NF_ST_GAME == goalServerType)
            {
                m_pNetClientModule->SendByServerID(nServerID, nMsgID, strData);
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
                m_pNetClientModule->SendByServerID(nServerID, nMsgID, strData);
            }
        }
        break;
        default:
        break;
    }
}

void NFCLuaScriptModule::SendByServerType(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string & strData)
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
                m_pNetClientModule->SendToAllServer(nMsgID, strData);
            }
            else if (NF_SERVER_TYPES::NF_ST_PROXY == goalServerType)
            {
                m_pNetModule->SendMsgToAllClientWithOutHead(nMsgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_WORLD:
        {
            if (NF_SERVER_TYPES::NF_ST_GAME == goalServerType)
            {
                m_pNetModule->SendMsgToAllClientWithOutHead(nMsgID, strData);
            }
            else if (NF_SERVER_TYPES::NF_ST_MASTER == goalServerType)
            {
                m_pNetClientModule->SendToAllServer(nMsgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_PROXY:
        {
            if (NF_SERVER_TYPES::NF_ST_GAME == goalServerType)
            {
                m_pNetClientModule->SendToAllServer(nMsgID, strData);
            }
        }
        break;
        case NF_SERVER_TYPES::NF_ST_MASTER:
        {
            if (NF_SERVER_TYPES::NF_ST_WORLD == goalServerType)
            {
                //m_pNetModule->SendMsgToAllClientWithOutHead(nMsgID, strData);
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
                m_pNetClientModule->SendToAllServer(nMsgID, strData);
            }
        }
        break;
        default:
        break;
    }
}

void NFCLuaScriptModule::SendMsgToGate(const NFGUID player, const uint16_t nMsgID, const std::string& strData)
{
    //the app must be the game server
}

void NFCLuaScriptModule::SendGroupMsgToGate(const uint16_t nMsgID, const std::string& strData)
{
    //the app must be the game server

}

void NFCLuaScriptModule::SendToAllPlayer(const uint16_t nMsgID, const std::string& strData)
{
	m_pNetModule->SendMsgToAllClientWithOutHead(nMsgID, strData);
}

void NFCLuaScriptModule::LogInfo(const std::string& strData)
{
	m_pLogModule->LogInfo(strData);
}

void NFCLuaScriptModule::LogError(const std::string& strData)
{
	m_pLogModule->LogError(strData);
}

void NFCLuaScriptModule::LogWarning(const std::string& strData)
{
	m_pLogModule->LogWarning(strData);
}

void NFCLuaScriptModule::LogDebug(const std::string& strData)
{
	m_pLogModule->LogDebug(strData);
}

void NFCLuaScriptModule::SetVersionCode(const std::string& strData)
{
    strVersionCode = strData;
}

const std::string&  NFCLuaScriptModule::GetVersionCode()
{
    return strVersionCode;
}

bool NFCLuaScriptModule::Register()
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

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFCLuaScriptModule>("NFCLuaScriptModule")
		.addFunction("register_module", &NFCLuaScriptModule::RegisterModule)
		.addFunction("create_object", &NFCLuaScriptModule::CreateObject)
		.addFunction("exist_object", &NFCLuaScriptModule::ExistObject)
		.addFunction("destroy_object", &NFCLuaScriptModule::DestroyObject)
		.addFunction("enter_scene", &NFCLuaScriptModule::EnterScene)
		.addFunction("do_event", &NFCLuaScriptModule::DoEvent)

		.addFunction("set_prop_int", &NFCLuaScriptModule::SetPropertyInt)
		.addFunction("set_prop_float", &NFCLuaScriptModule::SetPropertyFloat)
		.addFunction("set_prop_string", &NFCLuaScriptModule::SetPropertyString)
		.addFunction("set_prop_object", &NFCLuaScriptModule::SetPropertyObject)
		.addFunction("set_prop_vector2", &NFCLuaScriptModule::SetPropertyVector2)
		.addFunction("set_prop_vector3", &NFCLuaScriptModule::SetPropertyVector3)

		.addFunction("get_prop_int", &NFCLuaScriptModule::GetPropertyInt)
		.addFunction("get_prop_float", &NFCLuaScriptModule::GetPropertyFloat)
		.addFunction("get_prop_string", &NFCLuaScriptModule::GetPropertyString)
		.addFunction("get_prop_object", &NFCLuaScriptModule::GetPropertyObject)
		.addFunction("get_prop_vector2", &NFCLuaScriptModule::GetPropertyVector2)
		.addFunction("get_prop_vector3", &NFCLuaScriptModule::GetPropertyVector3)

		.addFunction("set_record_int", &NFCLuaScriptModule::SetRecordInt)
		.addFunction("set_record_float",&NFCLuaScriptModule::SetRecordFloat)
		.addFunction("set_record_string", &NFCLuaScriptModule::SetRecordString)
		.addFunction("set_record_object", &NFCLuaScriptModule::SetRecordObject)
		.addFunction("set_record_vector2", &NFCLuaScriptModule::SetPropertyVector2)
		.addFunction("set_record_vector3", &NFCLuaScriptModule::SetPropertyVector3)

		.addFunction("get_record_int", &NFCLuaScriptModule::GetRecordInt)
		.addFunction("get_record_float", &NFCLuaScriptModule::GetRecordFloat)
		.addFunction("get_record_string", &NFCLuaScriptModule::GetRecordString)
		.addFunction("get_record_object", &NFCLuaScriptModule::GetRecordObject)
		.addFunction("get_record_vector2", &NFCLuaScriptModule::GetPropertyVector2)
		.addFunction("get_record_vector3", &NFCLuaScriptModule::GetPropertyVector3)

		.addFunction("add_prop_cb", &NFCLuaScriptModule::AddPropertyCallBack)
		.addFunction("add_record_cb", &NFCLuaScriptModule::AddRecordCallBack)
		.addFunction("add_event_cb", &NFCLuaScriptModule::AddEventCallBack)
		.addFunction("add_class_cb", &NFCLuaScriptModule::AddClassCallBack)
		.addFunction("add_schedule", &NFCLuaScriptModule::AddSchedule)
		.addFunction("add_module_schedule", &NFCLuaScriptModule::AddModuleSchedule)
		.addFunction("do_event", &NFCLuaScriptModule::DoEvent)
		.addFunction("add_row", &NFCLuaScriptModule::AddRow)
		.addFunction("rem_row", &NFCLuaScriptModule::RemRow)

		.addFunction("time", &NFCLuaScriptModule::GetNowTime)
		.addFunction("new_id", &NFCLuaScriptModule::CreateID)
		.addFunction("app_id", &NFCLuaScriptModule::APPID)
		.addFunction("app_type", &NFCLuaScriptModule::APPType)

		.addFunction("exist_ele", &NFCLuaScriptModule::ExistElementObject)
		.addFunction("get_ele_list", &NFCLuaScriptModule::GetEleList)
		.addFunction("get_ele_int", &NFCLuaScriptModule::GetElePropertyInt)
		.addFunction("get_ele_float", &NFCLuaScriptModule::GetElePropertyFloat)
		.addFunction("get_ele_string", &NFCLuaScriptModule::GetElePropertyString)
		.addFunction("get_ele_vector2", &NFCLuaScriptModule::GetElePropertyVector2)
		.addFunction("get_ele_vector3", &NFCLuaScriptModule::GetElePropertyVector3)

		.addFunction("remove_cli_msg_cb", &NFCLuaScriptModule::RemoveClientMsgCallBack)//as server
		.addFunction("add_cli_msg_cb", &NFCLuaScriptModule::AddClientMsgCallBack)//as server
		.addFunction("remove_svr_msg_cb", &NFCLuaScriptModule::RemoveServerMsgCallBack)//as client
		.addFunction("add_svr_msg_cb", &NFCLuaScriptModule::AddServerMsgCallBack)//as client

		.addFunction("remove_http_cb", &NFCLuaScriptModule::RemoveHttpCallBack)
		.addFunction("add_http_cb", &NFCLuaScriptModule::AddHttpCallBack)

		.addFunction("send_by_fd", &NFCLuaScriptModule::SendByServerFD)//as client
		.addFunction("send_by_id", &NFCLuaScriptModule::SendByServerID)//as clent
		.addFunction("send_by_type", &NFCLuaScriptModule::SendByServerType)//as client

		.addFunction("send_to_player", &NFCLuaScriptModule::SendMsgToGate)//as game
		.addFunction("send_to_group_player", &NFCLuaScriptModule::SendGroupMsgToGate)//as game
		.addFunction("send_to_all_player", &NFCLuaScriptModule::SendToAllPlayer)//as game

		.addFunction("log_info", &NFCLuaScriptModule::LogInfo)
		.addFunction("log_error", &NFCLuaScriptModule::LogError)
		.addFunction("log_warning", &NFCLuaScriptModule::LogWarning)
		.addFunction("log_debug", &NFCLuaScriptModule::LogDebug)

		.addFunction("get_version_code", &NFCLuaScriptModule::GetVersionCode)
		.addFunction("set_version_code", &NFCLuaScriptModule::SetVersionCode)

		.addFunction("import_proto_file", &NFCLuaScriptModule::ImportProtoFile)
		.addFunction("encode", &NFCLuaScriptModule::Encode)
		.addFunction("decode", &NFCLuaScriptModule::Decode)

		.endClass();

    return true;
}

std::string NFCLuaScriptModule::FindFuncName(const LuaIntf::LuaRef & luatbl, const LuaIntf::LuaRef & luaFunc)
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
