// -------------------------------------------------------------------------
//    @FileName      :    NFCLuaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCLuaScriptModule.h"
#include "NFLuaScriptPlugin.h"
#include <assert.h>


bool NFCLuaScriptModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);

	Regisger();

	try
	{
		lw.dofile("script_init.lua");
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		assert(0);
	}

	//std::shared_ptr<NFILogicClass> pClass = m_pLogicClassModule->First();
	//while (pClass.get())
	//{
	//	std::shared_ptr<NFIComponent> pComponent = pClass->GetComponentManager()->First();
	//	while (pComponent.get())
	//	{
	//		if (!CheckCompomentStatus(pComponent->GetComponentName()))
	//		{
	//			assert(0);
	//		}

	//		pComponent = pClass->GetComponentManager()->Next();
	//	}

	//	pClass = m_pLogicClassModule->Next();
	//}
	if (!CheckCompomentStatus("Test"))
	{
		assert(0);
	}

	//call Test:Init()
	luacpp::call<void>(lw, "Init");

	return true;
}

bool NFCLuaScriptModule::AfterInit()
{
	luacpp::call<void>(lw, "AfterInit");

	//add all callback
	m_pKernelModule->ResgisterCommonPropertyEvent(this, &NFCLuaScriptModule::OnPropertyCommEvent);
	m_pKernelModule->ResgisterCommonRecordEvent(this, &NFCLuaScriptModule::OnRecordCommonEvent);
	m_pKernelModule->ResgisterCommonClassEvent(this, &NFCLuaScriptModule::OnClassCommonEvent);

	return true;
}

bool NFCLuaScriptModule::Shut()
{

	return true;
}

bool NFCLuaScriptModule::Execute()
{
	return true;
}

bool NFCLuaScriptModule::BeforeShut()
{
	//fflua.~fflua_t();
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

bool NFCLuaScriptModule::CheckCompomentStatus(const std::string& strComponentName, const std::string& strFuncName)
{
	return true;
}

bool NFCLuaScriptModule::CheckCompomentStatus(const std::string& strComponentName)
{
	try
	{
		luacpp::call<void>(lw, "load_script_file", strComponentName);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return false;
	}

	return true;
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
		.constructor<void>()//无参构造
		//.constructor<const test_class_A&>()//一个参数构造
		.method("GetData", &NFGUID::GetData)
		.method("SetData", &NFGUID::SetData)
		.method("GetHead", &NFGUID::GetHead)
		.method("SetHead", &NFGUID::SetHead);

	luacpp::reg_cclass<NFIDataList>::_reg(lw, "NFIDataList");

	luacpp::reg_cclass<NFCDataList>::_reg(lw, "NFCDataList")
		.constructor<void>()//无参构造
		//.constructor<const test_class_A&>()//一个参数构造
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
		.constructor<void>()//无参构造
		.method("GetFloat", &NFCDataList::TData::GetFloat)
		.method("GetInt", &NFCDataList::TData::GetInt)
		.method("GetObjectA", &NFCDataList::TData::GetObjectA)
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

int NFCLuaScriptModule::DoClassCommonScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction)
{
	if (!CheckCompomentStatus(strComponentName, strFunction))
	{
		return 0;
	}

	try
	{
		//fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule);
		luacpp::call<void>(lw, strFunction.c_str(), (NFINT64)m_pKernelModule, self);
	}
	catch (string &err)
	{
		printf("%s\n", err.c_str());
		return 0;
	}
	return 0;
}
