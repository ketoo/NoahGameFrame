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
#define TRY_RUN_GLOBAL_SCRIPT_FUN1(strFuncName, arg1)  try {LuaRef func(l, strFuncName);  func.call<LuaRef>(arg1); }	catch (LuaException& e) { cout << e.what() << endl; }

#define TRY_LOAD_SCRIPT_FLE(strFileName)  try{l.doFile(strFileName);} catch (LuaException& e) { cout << e.what() << endl; }
#define TRY_ADD_PACKAGE_PATH(strFilePath)  try{l.addPackagePath(strFilePath);} catch (LuaException& e) { cout << e.what() << endl; }

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
	TRY_LOAD_SCRIPT_FLE("script_init.lua");

	TRY_RUN_GLOBAL_SCRIPT_FUN1("init_script_system", pPluginManager);
	TRY_LOAD_SCRIPT_FLE("script_list.lua");

	TRY_RUN_GLOBAL_SCRIPT_FUN0("Init");

	return true;
}

bool NFCLuaScriptModule::AfterInit()
{
	//add all callback
	m_pKernelModule->ResgisterCommonPropertyEvent(this, &NFCLuaScriptModule::OnPropertyCommEvent);
	m_pKernelModule->ResgisterCommonRecordEvent(this, &NFCLuaScriptModule::OnRecordCommonEvent);
	m_pKernelModule->ResgisterCommonClassEvent(this, &NFCLuaScriptModule::OnClassCommonEvent);


	TRY_RUN_GLOBAL_SCRIPT_FUN0("AfterInit");

	return true;
}

bool NFCLuaScriptModule::Shut()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("Shut");

	return true;
}

bool NFCLuaScriptModule::Execute()
{
	//10秒钟reload一次
	if (pPluginManager->GetNowTime() - mnTime > 5)
	{
		mnTime = pPluginManager->GetNowTime();

		TRY_RUN_GLOBAL_SCRIPT_FUN0("Execute");

		TRY_LOAD_SCRIPT_FLE("script_reload.lua")

	}

	return true;
}

bool NFCLuaScriptModule::BeforeShut()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("BeforeShut");

	return true;
}

int NFCLuaScriptModule::OnPropertyCommEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	DoPropertyCommEvent(self, strPropertyName, oldVar, newVar);

	return 0;
}

int NFCLuaScriptModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	DoRecordCommonEvent(self, xEventData, oldVar, newVar);

	return 0;
}

int NFCLuaScriptModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{	
	try
	{
		LuaRef func(l, "TestModule.OnClassCommonEvent");
		func(m_pLogicClassModule, self, strClassName, (int)eClassEvent, (NFCDataList)var);
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

bool NFCLuaScriptModule::Regisger()
{
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

	LuaBinding(l).beginClass<NFCDataList>("NFCDataList")
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

	LuaBinding(l).beginModule("KernelModule")
		.addFunction("DoEvent", static_cast<bool(*)(NFINT64, const NFGUID*, int, const NFCDataList*)>(&KernelModule_DoEvent))
		.addFunction("ExistElement", static_cast<bool(*)(NFIKernelModule*, string&)>(&KernelModule_ExistElement))
		.addFunction("GetElementPropertyInt", static_cast<NFINT64(*)(NFINT64, string&, string&)>(&KernelModule_GetElementPropertyInt))
		.addFunction("GetElementPropertyFloat", static_cast<double(*)(NFINT64, string&, string&)>(&KernelModule_GetElementPropertyFloat))
		.addFunction("GetElementPropertyString", static_cast<const string&(*)(NFIKernelModule*, string&, string&)>(&KernelModule_GetElementPropertyString))
		.addFunction("AddPropertyCallBack", static_cast<bool(*)(NFINT64, const NFGUID*, string&, string&, string&)>(&KernelModule_AddPropertyCallBack))
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
		auto asdf = (NFINT64)m_pKernelModule;
		func.call<LuaRef>((NFINT64)m_pKernelModule, self, strComponentName, strClassName, (int)eClassEvent);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 0;
}
