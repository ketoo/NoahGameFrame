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

#define TRY_RUN_GLOBAL_SCRIPT_FUN0(strFuncName)  try{luacpp::call<void>(lw, strFuncName);} catch (string &err){printf("%s\n", err.c_str());}
#define TRY_RUN_GLOBAL_SCRIPT_FUN1(strFuncName, arg1)  try{luacpp::call<void>(lw, strFuncName, arg1);} catch (string &err){printf("%s\n", err.c_str());}

#define TRY_LOAD_SCRIPT_FLE(strFileName)  try{lw.dofile(strFileName);} catch (string &err){printf("%s\n", err.c_str());}
#define TRY_ADD_PACKAGE_PATH(strFilePath)  try{lw.addPackagePath(strFilePath);} catch (string &err){printf("%s\n", err.c_str());}

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
	TRY_RUN_GLOBAL_SCRIPT_FUN1("init_script_system", m_pKernelModule);
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
	DoClassCommonEvent(m_pLogicClassModule, self, strClassName, eClassEvent, var);

	return 0;
}

int NFCLuaScriptModule::DoScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg)
{
	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, &arg);
		luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, arg);
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
		luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, nEventID, arg);
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
		luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, strHeartBeat.c_str(), fTime, nCount);
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
		luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, strPropertyName.c_str(), oldVar, newVar);
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
		luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self, strRecordName.c_str(), nOpType, nRow, nCol, oldVar, newVar);
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
	luacpp::reg_cclass<NFGUID>::_reg(lw, "NFGUID")
		.constructor<void>()//ÎÞ²Î¹¹Ôì
		//.constructor<const test_class_A&>()//Ò»¸ö²ÎÊý¹¹Ôì
		.method("GetData", &NFGUID::GetData)
		.method("SetData", &NFGUID::SetData)
		.method("GetHead", &NFGUID::GetHead)
		.method("SetHead", &NFGUID::SetHead);

//	luacpp::reg_cclass<NFIKernelModule>::_reg(lw, "NFIKernelModule")
//		.property("strName", &NFIKernelModule::strName);

	luacpp::reg_cclass<NFIDataList>::_reg(lw, "NFIDataList");

	luacpp::reg_cclass<NFCDataList>::_reg(lw, "NFCDataList")
		.constructor<void>()
		.method("IsEmpty", &NFCDataList::IsEmpty)
		.method("GetCount", &NFCDataList::GetCount)
		.method("Type", &NFCDataList::Type)
		.method("AddInt", &NFCDataList::AddInt)
		.method("AddFloat", &NFCDataList::AddFloat)
		.method("AddString", &NFCDataList::AddStringFromChar)
		.method("AddObject", &NFCDataList::AddObject)
		.method("SetInt", &NFCDataList::SetInt)
		.method("SetFloat", &NFCDataList::SetFloat)
		.method("SetString", &NFCDataList::SetString)
		.method("SetObject", &NFCDataList::SetObject)
		.method("Int", &NFCDataList::Int)
		.method("Float", &NFCDataList::Float)
		.method("String", &NFCDataList::String)
		.method("Object", &NFCDataList::Object);

	luacpp::reg_cclass<NFCDataList::TData>::_reg(lw, "TData")
		.constructor<void>()//ÎÞ²Î¹¹Ôì
		.method("GetFloat", &NFCDataList::TData::GetFloat)
		.method("GetInt", &NFCDataList::TData::GetInt)
		.method("GetObject", &NFCDataList::TData::GetObject)
		.method("GetString", &NFCDataList::TData::GetCharArr)
		.method("GetType", &NFCDataList::TData::GetType)
		.method("IsNullValue", &NFCDataList::TData::IsNullValue)
		.method("SetFloat", &NFCDataList::TData::SetFloat)
		.method("SetInt", &NFCDataList::TData::SetInt)
		.method("SetObject", &NFCDataList::TData::SetObject)
		.method("SetString", &NFCDataList::TData::SetString)
		.method("StringValEx", &NFCDataList::TData::StringValEx)
		.property("GetData", &NFCDataList::TData::variantData);

	luacpp::reg_cfun(lw, "DoEvent", &KernelModule_DoEvent);
	luacpp::reg_cfun(lw, "ExistElement", &KernelModule_ExistElement);
	luacpp::reg_cfun(lw, "GetElementPropertyInt", &KernelModule_GetElementPropertyInt);
	luacpp::reg_cfun(lw, "GetElementPropertyFloat", &KernelModule_GetElementPropertyFloat);
	luacpp::reg_cfun(lw, "GetElementPropertyString", &KernelModule_GetElementPropertyString);
	luacpp::reg_cfun(lw, "AddPropertyCallBack", &KernelModule_AddPropertyCallBack);
	luacpp::reg_cfun(lw, "AddRecordCallBack", &KernelModule_AddRecordCallBack);
	luacpp::reg_cfun(lw, "AddEventCallBack", &KernelModule_AddEventCallBack);
	luacpp::reg_cfun(lw, "AddHeartBeat", &KernelModule_AddHeartBeat);
	luacpp::reg_cfun(lw, "FindHeartBeat", &KernelModule_FindHeartBeat);
	luacpp::reg_cfun(lw, "RemoveHeartBeat", &KernelModule_RemoveHeartBeat);
	luacpp::reg_cfun(lw, "ExistContainer", &KernelModule_ExistContainer);
	luacpp::reg_cfun(lw, "SetPropertyInt", &KernelModule_SetPropertyInt);
	luacpp::reg_cfun(lw, "SetPropertyFloat", &KernelModule_SetPropertyFloat);
	luacpp::reg_cfun(lw, "SetPropertyString", &KernelModule_SetPropertyString);
	luacpp::reg_cfun(lw, "SetPropertyObject", &KernelModule_SetPropertyObject);
	luacpp::reg_cfun(lw, "GetPropertyInt", &KernelModule_GetPropertyInt);
	luacpp::reg_cfun(lw, "GetPropertyFloat", &KernelModule_GetPropertyFloat);
	luacpp::reg_cfun(lw, "GetPropertyString", &KernelModule_GetPropertyString);
	luacpp::reg_cfun(lw, "GetPropertyObject", &KernelModule_GetPropertyObject);
	luacpp::reg_cfun(lw, "SetRecordInt", &KernelModule_SetRecordInt);
	luacpp::reg_cfun(lw, "SetRecordFloat", &KernelModule_SetRecordFloat);
	luacpp::reg_cfun(lw, "SetRecordString", &KernelModule_SetRecordString);
	luacpp::reg_cfun(lw, "SetRecordObject", &KernelModule_SetRecordObject);
	luacpp::reg_cfun(lw, "GetRecordInt", &KernelModule_GetRecordInt);
	luacpp::reg_cfun(lw, "GetRecordFloat", &KernelModule_GetRecordFloat);
	luacpp::reg_cfun(lw, "GetRecordString", &KernelModule_GetRecordString);
	luacpp::reg_cfun(lw, "GetRecordObject", &KernelModule_GetRecordObject);
	luacpp::reg_cfun(lw, "AddRow", &KernelModule_AddRow);

	return true;
}

int NFCLuaScriptModule::DoClassCommonScript(const NFGUID& self, const std::string& strComponentName, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent)
{
	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule);
		luacpp::call<void>(lw, "OnClassCommonEvent", (NFINT64)m_pKernelModule, self, strComponentName, strClassName, (int)eClassEvent);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 0;
}
