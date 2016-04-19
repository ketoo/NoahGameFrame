// -------------------------------------------------------------------------
//    @FileName      :    NFCLuaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include <assert.h>
#include "NFCLuaScriptModule.h"
#include "NFLuaScriptPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

#define TRY_RUN_GLOBAL_SCRIPT_FUN0(strFuncName)   try {LuaRef func(l, strFuncName);  func.call<LuaRef>(); }	catch (LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN1(strFuncName, arg1)  try {LuaRef func(l, strFuncName);  func.call<LuaRef>(arg1); }catch (LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN2(strFuncName, arg1, arg2)  try {LuaRef func(l, strFuncName);  func.call<LuaRef>(arg1, arg2); }catch (LuaException& e) { cout << e.what() << endl; }

#define TRY_LOAD_SCRIPT_FLE(strFileName)  try{l.doFile(strFileName);} catch (LuaException& e) { cout << e.what() << endl; }
#define TRY_ADD_PACKAGE_PATH(strFilePath)  try{l.addPackagePath(strFilePath);} catch (LuaException& e) { cout << e.what() << endl; }
LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
bool NFCLuaScriptModule::Init()
{
	mnTime = pPluginManager->GetNowTime();

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);

	Regisger();
	TRY_ADD_PACKAGE_PATH("../../NFDataCfg/ScriptModule");
	TRY_LOAD_SCRIPT_FLE("script_module.lua");
	TRY_LOAD_SCRIPT_FLE("script_init.lua");

	TRY_RUN_GLOBAL_SCRIPT_FUN2("init_script_system", pPluginManager, this);
	TRY_LOAD_SCRIPT_FLE("script_list.lua");

	TRY_RUN_GLOBAL_SCRIPT_FUN0("ScriptModule.Init");

	return true;
}

bool NFCLuaScriptModule::AfterInit()
{
	//add all callback
	m_pKernelModule->ResgisterCommonPropertyEvent(this, &NFCLuaScriptModule::OnPropertyCommEvent);
	m_pKernelModule->ResgisterCommonRecordEvent(this, &NFCLuaScriptModule::OnRecordCommonEvent);
	m_pKernelModule->ResgisterCommonClassEvent(this, &NFCLuaScriptModule::OnClassCommonEvent);


	TRY_RUN_GLOBAL_SCRIPT_FUN0("ScriptModule.AfterInit");

	return true;
}

bool NFCLuaScriptModule::Shut()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("ScriptModule.Shut");

	return true;
}

bool NFCLuaScriptModule::Execute()
{
	//10秒钟reload一次
	if (pPluginManager->GetNowTime() - mnTime > 5)
	{
		mnTime = pPluginManager->GetNowTime();
		TRY_RUN_GLOBAL_SCRIPT_FUN0("ScriptModule.Execute");
		TRY_LOAD_SCRIPT_FLE("script_reload.lua")

	}

	return true;
}

bool NFCLuaScriptModule::BeforeShut()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("ScriptModule.BeforeShut");

	return true;
}

int NFCLuaScriptModule::OnPropertyCommEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	try
	{
		LuaRef func(l, "ScriptModule.OnPropertyCommEvent");
		func.call<void>(self, strPropertyName, oldVar, newVar);
	}
	catch (LuaException& e) { cout << e.what() << endl; }

	//DoPropertyCommEvent(self, strPropertyName, oldVar, newVar);

	return 0;
}

int NFCLuaScriptModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	try
	{
		LuaRef func(l, "ScriptModule.OnRecordCommonEvent");
		func.call<void>(self, xEventData, oldVar, newVar);
	}
	catch (LuaException& e) { cout << e.what() << endl; }

	//DoRecordCommonEvent(self, xEventData, oldVar, newVar);

	return 0;
}

int NFCLuaScriptModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	try
	{
		LuaRef func(l, "ScriptModule.OnClassCommonEvent");
		func.call<void>(self, strClassName, (int)eClassEvent, (NFCDataList)var);
	}
	catch (LuaException& e) { cout << e.what() << endl; }



	//DoClassCommonEvent(m_pLogicClassModule, self, strClassName, eClassEvent, var);

	return 0;
}

int NFCLuaScriptModule::DoScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg)
{
	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, &arg);
		//luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, arg);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}

	return 1;
}

int NFCLuaScriptModule::DoEventScript(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg)
{
	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, nEventID, &arg);
		// luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, nEventID, arg);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 1;
}

int NFCLuaScriptModule::DoHeartBeatScript(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction)
{
	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, strHeartBeat.c_str(), fTime, nCount);
		//luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, strHeartBeat.c_str(), fTime, nCount);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 1;
}

int NFCLuaScriptModule::DoScriptPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, strPropertyName.c_str(), &oldVar, &newVar);
		//luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, strPropertyName.c_str(), oldVar, newVar);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 1;
}

int NFCLuaScriptModule::DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCDataList::TData& oldVar, const NFCDataList::TData& newVar)
{
	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, strRecordName.c_str(), nOpType, nRow, nCol, &oldVar, &newVar);
		//luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, strRecordName.c_str(), nOpType, nRow, nCol, oldVar, newVar);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 1;
}


bool NFCLuaScriptModule::AddPropertyCallBack(const NFGUID& self, std::string& strPropertyName, std::string& luaFunc)
{
	AddLuaFuncToMap(m_luaPropertyCallBackFuncMap, self, strPropertyName, luaFunc);
	m_pKernelModule->AddPropertyCallBack(self, strPropertyName, this, &NFCLuaScriptModule::OnLuaPropertyCB);
	return true;
}

int NFCLuaScriptModule::OnLuaPropertyCB(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	auto funcList = m_luaPropertyCallBackFuncMap.GetElement(strPropertyName);
	if (funcList)
	{
		auto funcNameList = funcList->GetElement(self);
		if (funcNameList)
		{
			string funcName;
			auto Ret = funcNameList->First(funcName);
			while (Ret)
			{
				LuaRef func(l, funcName.c_str());
				func.call(self, strPropertyName, oldVar, newVar);
				Ret = funcNameList->Next(funcName);
			}

		}
	}
	return 1;
}

bool NFCLuaScriptModule::AddRecordCallBack(const NFGUID& self, std::string& strRecordName, std::string& luaFunc)
{
	AddLuaFuncToMap(m_luaRecordCallBackFuncMap, self, strRecordName, luaFunc);
	m_pKernelModule->AddRecordCallBack(self, strRecordName, this, &NFCLuaScriptModule::OnLuaRecordCB);
	return true;
}

int NFCLuaScriptModule::OnLuaRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	auto funcList = m_luaRecordCallBackFuncMap.GetElement(xEventData.strRecordName);
	if (funcList)
	{
		auto funcNameList = funcList->GetElement(self);
		if (funcNameList)
		{
			string funcName;
			auto Ret = funcNameList->First(funcName);
			while (Ret)
			{
				LuaRef func(l, funcName.c_str());
				func.call(self, xEventData.strRecordName, xEventData.nOpType, xEventData.nRow, xEventData.nCol, oldVar, newVar);
				Ret = funcNameList->Next(funcName);
			}

		}
	}
	return 1;
}

bool NFCLuaScriptModule::AddEventCallBack(const NFGUID& self, const int nEventID, std::string& luaFunc)
{
	AddLuaFuncToMap(m_luaEventCallBackFuncMap, self, (int)nEventID, luaFunc);
	m_pKernelModule->AddEventCallBack(self, nEventID, this, &NFCLuaScriptModule::OnLuaEventCB);
	return true;
}

int NFCLuaScriptModule::OnLuaEventCB(const NFGUID& self, const int nEventID, const NFIDataList& argVar)
{
	auto funcList = m_luaEventCallBackFuncMap.GetElement(nEventID);
	if (funcList)
	{
		auto funcNameList = funcList->GetElement(self);
		if (funcNameList)
		{
			string funcName;
			auto Ret = funcNameList->First(funcName);
			while (Ret)
			{
				LuaRef func(l, funcName.c_str());
				func.call(self, nEventID, (NFCDataList&)argVar);
				Ret = funcNameList->Next(funcName);
			}

		}
	}
	return 1;
}

bool NFCLuaScriptModule::AddHeartBeat(const NFGUID& self, std::string& strHeartBeatName, std::string& luaFunc, const float fTime, const int nCount)
{
	AddLuaFuncToMap(m_luaHeartBeatCallBackFuncMap, self, strHeartBeatName, luaFunc);
	m_pKernelModule->AddHeartBeat(self, strHeartBeatName, this, &NFCLuaScriptModule::OnLuaHeartBeatCB, fTime, nCount);
	return true;
}

int NFCLuaScriptModule::OnLuaHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float fTime, const int nCount)
{
	auto funcList = m_luaHeartBeatCallBackFuncMap.GetElement(strHeartBeatName);
	if (funcList)
	{
		auto funcNameList = funcList->GetElement(self);
		if (funcNameList)
		{
			string funcName;
			auto Ret = funcNameList->First(funcName);
			while (Ret)
			{
				LuaRef func(l, funcName.c_str());
				func.call(self, strHeartBeatName, fTime, nCount);
				Ret = funcNameList->Next(funcName);
			}
		}
	}
	return 1;
}

int NFCLuaScriptModule::AddRow(const NFGUID& self, std::string& strRecordName, const NFCDataList& var)
{
	NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
	if (nullptr == pRecord)
	{
		return -1;
	}

	return pRecord->AddRow(-1, var);
}

template<typename KType>
bool NFCLuaScriptModule::AddLuaFuncToMap(NFMap<KType, NFMap<NFGUID, NFList<string>>>& funcMap, const NFGUID& self, KType &key, string& luaFunc)
{
	auto funcList = funcMap.GetElement(key);
	if (!funcList)
	{
		NFList<string> *funcNameList = new NFList<string>;
		funcNameList->Add(luaFunc);
		funcList = new NFMap<NFGUID, NFList<string>>;
		funcList->AddElement(self, funcNameList);
		funcMap.AddElement(key, funcList);
	}

	if (!funcList->GetElement(self))
	{
		NFList<string> *funcNameList = new NFList<string>;
		funcNameList->Add(luaFunc);
		funcList->AddElement(self, funcNameList);
	}
	else
	{
		auto funcNameList = funcList->GetElement(self);
		if (!funcNameList->Find(luaFunc))
		{
			funcNameList->Add(luaFunc);
		}
	}
	return true;
}

bool NFCLuaScriptModule::DoEvent(const NFGUID& self, const int nEventID, const NFCDataList& valueList)
{
	return m_pKernelModule->DoEvent(self, nEventID, valueList);
}

bool NFCLuaScriptModule::Regisger()
{
	LuaBinding(l).beginClass<RECORD_EVENT_DATA>("RECORD_EVENT_DATA")
		.endClass();

	LuaBinding(l).beginClass<NFIObject>("NFIObject")
		.addFunction("Self", &NFIObject::Self)
		.endClass();


	LuaBinding(l).beginClass<NFILogicClassModule>("NFILogicClassModule")
		.endClass();

	LuaBinding(l).beginClass<NFILuaScriptModule>("NFILuaScriptModule")
		.addFunction("AddPropertyCallBack", &NFILuaScriptModule::AddPropertyCallBack)
		.addFunction("AddRecordCallBack", &NFILuaScriptModule::AddRecordCallBack)
		.addFunction("AddEventCallBack", &NFILuaScriptModule::AddEventCallBack)
		.addFunction("AddHeartBeatCallBack", &NFILuaScriptModule::AddHeartBeatCallBack)
		.endClass();

	LuaBinding(l).beginClass<NFIPluginManager>("NFIPluginManager")
		.addFunction("FindLuaModule", &NFIPluginManager::FindModule<NFILuaScriptModule>)
		.addFunction("FindKernelModule", &NFIPluginManager::FindModule<NFIKernelModule>)
		.addFunction("FindLogicClassModule", &NFIPluginManager::FindModule<NFILogicClassModule>)
		.addFunction("FindElementInfoModule", &NFIPluginManager::FindModule<NFIElementInfoModule>)
		.endClass();

	LuaBinding(l).beginClass<NFIElementInfoModule>("NFIElementInfoModule")
		.addFunction("ExistElement", &NFIElementInfoModule::ExistElement)
		.addFunction("GetPropertyInt", &NFIElementInfoModule::GetPropertyInt)
		.addFunction("GetPropertyFloat", &NFIElementInfoModule::GetPropertyFloat)
		.addFunction("GetPropertyString", &NFIElementInfoModule::GetPropertyString)
		.endClass();

	LuaBinding(l).beginClass<NFIKernelModule>("NFIKernelModule")
		.addFunction("GetPluginManager", &NFIKernelModule::GetPluginManager)
		.addFunction("CreateScene", &NFIKernelModule::CreateScene)
		.addFunction("CreateObject", &NFIKernelModule::CreateObject)
		.addFunction("DoEvent", (bool (NFIKernelModule::*)(const NFGUID&, const int, const NFIDataList&))&NFIKernelModule::DoEvent)
		.addFunction("FindHeartBeat", &NFIKernelModule::FindHeartBeat)
		.addFunction("RemoveHeartBeat", &NFIKernelModule::RemoveHeartBeat)
		.addFunction("ExistContainer", &NFIKernelModule::ExistContainer)
		.addFunction("SetPropertyInt", &NFIKernelModule::SetPropertyInt)
		.addFunction("SetPropertyFloat", &NFIKernelModule::SetPropertyFloat)
		.addFunction("SetPropertyString", &NFIKernelModule::SetPropertyString)
		.addFunction("SetPropertyObject", &NFIKernelModule::SetPropertyObject)
		.addFunction("GetPropertyInt", &NFIKernelModule::GetPropertyInt)
		.addFunction("GetPropertyFloat", &NFIKernelModule::GetPropertyFloat)
		.addFunction("GetPropertyString", &NFIKernelModule::GetPropertyString)
		.addFunction("GetPropertyObject", &NFIKernelModule::GetPropertyObject)
		.addFunction("SetRecordInt", (bool (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int, const NFINT64))&NFIKernelModule::SetRecordInt)
		.addFunction("SetRecordFloat", (bool (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int, const double))&NFIKernelModule::SetRecordFloat)
		.addFunction("SetRecordString", (bool (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int, const string&))&NFIKernelModule::SetRecordString)
		.addFunction("SetRecordObject", (bool (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int, const NFGUID&))&NFIKernelModule::SetRecordObject)
		.addFunction("GetRecordInt", (NFINT64(NFIKernelModule::*)(const NFGUID&, const string&, const int, const int))&NFIKernelModule::GetRecordInt)
		.addFunction("GetRecordFloat", (double (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int))&NFIKernelModule::GetRecordFloat)
		.addFunction("GetRecordString", (const string& (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int))&NFIKernelModule::GetRecordString)
		.addFunction("GetRecordObject", (const NFGUID& (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int))&NFIKernelModule::GetRecordObject)
		.endClass();

	LuaBinding(l).beginClass<NFGUID>("NFGUID")
		.addConstructor(LUA_ARGS())
		//.addProperty("val", &NFGUID::val, &NFGUID::set_val)
		//.addProperty("fun", &NFGUID::callback, &NFGUID::setCallback)
		.addFunction("GetData", &NFGUID::GetData)
		.addFunction("SetData", &NFGUID::SetData)
		.addFunction("GetHead", &NFGUID::GetHead)
		.addFunction("SetHead", &NFGUID::SetHead)
		.endClass();

	LuaBinding(l).beginClass<NFIDataList>("NFIDataList")
		.endClass();

	LuaBinding(l).beginExtendClass<NFCDataList, NFIDataList>("NFCDataList")
		.addConstructor(LUA_ARGS())
		.addFunction("IsEmpty", &NFCDataList::IsEmpty)
		.addFunction("GetCount", &NFCDataList::GetCount)
		.addFunction("Type", &NFCDataList::Type)
		.addFunction("AddInt", &NFCDataList::AddInt)
		.addFunction("AddFloat", &NFCDataList::AddFloat)
		.addFunction("AddString", &NFCDataList::AddStringFromChar)
		.addFunction("AddObject", &NFCDataList::AddObject)
		.addFunction("SetInt", &NFCDataList::SetInt)
		.addFunction("SetFloat", &NFCDataList::SetFloat)
		.addFunction("SetString", &NFCDataList::SetString)
		.addFunction("SetObject", &NFCDataList::SetObject)
		.addFunction("Int", &NFCDataList::Int)
		.addFunction("Float", &NFCDataList::Float)
		.addFunction("String", &NFCDataList::String)
		.addFunction("Object", &NFCDataList::Object)
		.endClass();

	LuaBinding(l).beginClass<NFCDataList::TData>("TData")
		.addConstructor(LUA_ARGS())
		.addFunction("GetFloat", &NFCDataList::TData::GetFloat)
		.addFunction("GetInt", &NFCDataList::TData::GetInt)
		.addFunction("GetObject", &NFCDataList::TData::GetObject)
		.addFunction("GetString", &NFCDataList::TData::GetCharArr)
		.addFunction("GetType", &NFCDataList::TData::GetType)
		.addFunction("IsNullValue", &NFCDataList::TData::IsNullValue)
		.addFunction("SetFloat", &NFCDataList::TData::SetFloat)
		.addFunction("SetInt", &NFCDataList::TData::SetInt)
		.addFunction("SetObject", &NFCDataList::TData::SetObject)
		.addFunction("SetString", &NFCDataList::TData::SetString)
		.addFunction("StringValEx", &NFCDataList::TData::StringValEx)
		.endClass();

	LuaBinding(l).beginClass<NFCLuaScriptModule>("NFCLuaScriptModule")
		.addFunction("AddPropertyCallBack", &NFCLuaScriptModule::AddPropertyCallBack)
		.addFunction("AddRecordCallBack", &NFCLuaScriptModule::AddRecordCallBack)
		.addFunction("AddEventCallBack", &NFCLuaScriptModule::AddEventCallBack)
		.addFunction("AddHeartBeat", &NFCLuaScriptModule::AddHeartBeat)
		.addFunction("AddRow", &NFCLuaScriptModule::AddRow)
		.addFunction("DoEvent", &NFCLuaScriptModule::DoEvent)
		.endClass();

	LuaBinding(l).beginModule("KernelModule")
		.addFunction("DoEvent", static_cast<bool(*)(NFINT64, const NFGUID*, int, const NFCDataList*)>(&KernelModule_DoEvent))
		.addFunction("ExistElement", &KernelModule_ExistElement)
		.addFunction("GetElementPropertyInt", &KernelModule_GetElementPropertyInt)
		.addFunction("GetElementPropertyFloat", &KernelModule_GetElementPropertyFloat)
		.addFunction("GetElementPropertyString", &KernelModule_GetElementPropertyString)
		.addFunction("AddPropertyCallBack", &KernelModule_AddPropertyCallBack)
		.addFunction("AddRecordCallBack", &KernelModule_AddRecordCallBack)
		.addFunction("AddEventCallBack", &KernelModule_AddEventCallBack)
		.addFunction("AddHeartBeat", &KernelModule_AddHeartBeat)
		.addFunction("FindHeartBeat", &KernelModule_FindHeartBeat)
		.addFunction("RemoveHeartBeat", &KernelModule_RemoveHeartBeat)
		.addFunction("ExistContainer", &KernelModule_ExistContainer)
		.addFunction("SetPropertyInt", &KernelModule_SetPropertyInt)
		.addFunction("SetPropertyFloat", &KernelModule_SetPropertyFloat)
		.addFunction("SetPropertyString", &KernelModule_SetPropertyString)
		.addFunction("SetPropertyObject", &KernelModule_SetPropertyObject)
		.addFunction("GetPropertyInt", &KernelModule_GetPropertyInt)
		.addFunction("GetPropertyFloat", &KernelModule_GetPropertyFloat)
		.addFunction("GetPropertyString", &KernelModule_GetPropertyString)
		.addFunction("GetPropertyObject", &KernelModule_GetPropertyObject)
		.addFunction("SetRecordInt", &KernelModule_SetRecordInt)
		.addFunction("SetRecordFloat", &KernelModule_SetRecordFloat)
		.addFunction("SetRecordString", &KernelModule_SetRecordString)
		.addFunction("SetRecordObject", &KernelModule_SetRecordObject)
		.addFunction("GetRecordInt", &KernelModule_GetRecordInt)
		.addFunction("GetRecordFloat", &KernelModule_GetRecordFloat)
		.addFunction("GetRecordString", &KernelModule_GetRecordString)
		.addFunction("GetRecordObject", &KernelModule_GetRecordObject)
		.addFunction("AddRow", &KernelModule_AddRow)

		// this will bind string test(string), by using our LUA_FN macro
		// LUA_FN(RETURN_TYPE, FUNC_NAME, ARG_TYPES...)
		//.addFunction("test_1", test1)
		.endModule();

	return true;
}

int NFCLuaScriptModule::DoClassCommonScript(const NFGUID& self, const std::string& strComponentName, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent)
{
	try
	{
		LuaRef func(l, "OnClassCommonEvent");
		func.call<LuaRef>((NFINT64)m_pKernelModule, self, strComponentName, strClassName, (int)eClassEvent);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 0;
}
