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
	return true;
}

bool NFCLuaScriptModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pElementInfoModule);

	bool ret = InstallLua("script_init.lua");
	assert(ret);

	std::shared_ptr<NFILogicClass> pClass = m_pLogicClassModule->First();
	while (pClass.get())
	{
		std::shared_ptr<NFIComponent> pComponent = pClass->GetComponentManager()->First();
		while (pComponent.get())
		{
			if (!CheckCompomentStatus(pComponent->GetComponentName()))
			{
				assert(0);
			}

			pComponent = pClass->GetComponentManager()->Next();
		}

		pClass = m_pLogicClassModule->Next();
	}

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
	fflua.~fflua_t();
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

bool NFCLuaScriptModule::InstallLua(const std::string& strComponentName)
{
	fflua.reg(Regisger);
	fflua.add_package_path("./");
	try
	{
		fflua.load_file(strComponentName);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool NFCLuaScriptModule::CheckCompomentStatus(const std::string& strComponentName, const std::string& strFuncName)
{
	return true;
}

bool NFCLuaScriptModule::CheckCompomentStatus(const std::string& strComponentName)
{
	return true;
}

int NFCLuaScriptModule::DoScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg)
{
	try
	{
		fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, &arg);
	}
	catch (...)
	{
		
		return 0;
	}

	return 1;
}

int NFCLuaScriptModule::DoEventScript(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg)
{
	try
	{
		fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, nEventID, &arg);
	}
	catch (...)
	{
		
		return 0;
	}
	return 1;
}

int NFCLuaScriptModule::DoHeartBeatScript(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction)
{
	try
	{
		fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, strHeartBeat.c_str(), fTime, nCount);
	}
	catch (...)
	{
		
		return 0;
	}
	return 1;
}

int NFCLuaScriptModule::DoScriptPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	try
	{
		fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, strPropertyName.c_str(), &oldVar, &newVar);
	}
	catch (...)
	{
		
		return 0;
	}
	return 1;
}

int NFCLuaScriptModule::DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCDataList::TData& oldVar, const NFCDataList::TData& newVar)
{
	try
	{
		fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule, &self, strRecordName.c_str(), nOpType, nRow, nCol, &oldVar, &newVar);
	}
	catch (...)
	{
		
		return 0;
	}
	return 1;
}

bool NFCLuaScriptModule::Regisger(lua_State* ls)
{
	fflua_register_t<NFGUID, ctor()>(ls, "NFGUID")  //! 注册构造函数
		.def(&NFGUID::GetData, "GetData")
		.def(&NFGUID::SetData, "SetData")
		.def(&NFGUID::GetHead, "GetHead")
		.def(&NFGUID::SetHead, "SetHead");

	//! 注册基类函数, ctor() 为构造函数的类型
	fflua_register_t<NFCDataList, ctor()>(ls, "NFCDataList")  //! 注册构造函数
		.def(&NFCDataList::IsEmpty, "IsEmpty")
		.def(&NFCDataList::GetCount, "GetCount")
		.def(&NFCDataList::TypeInt, "Type")
		.def(&NFCDataList::AddInt, "AddInt")
		.def(&NFCDataList::AddFloat, "AddFloat")
		.def(&NFCDataList::AddString, "AddString")
		.def(&NFCDataList::AddObject, "AddObject")
		.def(&NFCDataList::SetInt, "SetInt")
		.def(&NFCDataList::SetFloat, "SetFloat")
		.def(&NFCDataList::SetString, "SetString")
		.def(&NFCDataList::SetObject, "SetObject")
		.def(&NFCDataList::Int, "Int")
		.def(&NFCDataList::Float, "Float")
		.def(&NFCDataList::String, "String")
		.def(&NFCDataList::Object, "Object");


	fflua_register_t<>(ls).def(&KernelModule_DoEvent, "DoEvent");
	fflua_register_t<>(ls).def(&KernelModule_ExistElement, "ExistElement");
	fflua_register_t<>(ls).def(&KernelModule_GetElementPropertyInt, "GetElementPropertyInt");
	fflua_register_t<>(ls).def(&KernelModule_GetElementPropertyFloat, "GetElementPropertyFloat");
	fflua_register_t<>(ls).def(&KernelModule_GetElementPropertyString, "GetElementPropertyString");
	fflua_register_t<>(ls).def(&KernelModule_AddPropertyCallBack, "AddPropertyCallBack");
	fflua_register_t<>(ls).def(&KernelModule_AddRecordCallBack, "AddRecordCallBack");
	fflua_register_t<>(ls).def(&KernelModule_AddEventCallBack, "AddEventCallBack");
	fflua_register_t<>(ls).def(&KernelModule_AddHeartBeat, "AddHeartBeat");
	fflua_register_t<>(ls).def(&KernelModule_FindHeartBeat, "FindHeartBeat");
	fflua_register_t<>(ls).def(&KernelModule_RemoveHeartBeat, "RemoveHeartBeat");
	fflua_register_t<>(ls).def(&KernelModule_ExistContainer, "ExistContainer");
	fflua_register_t<>(ls).def(&KernelModule_SetPropertyInt, "SetPropertyInt");
	fflua_register_t<>(ls).def(&KernelModule_SetPropertyFloat, "SetPropertyFloat");
	fflua_register_t<>(ls).def(&KernelModule_SetPropertyString, "SetPropertyString");
	fflua_register_t<>(ls).def(&KernelModule_SetPropertyObject, "SetPropertyObject");
	fflua_register_t<>(ls).def(&KernelModule_GetPropertyInt, "GetPropertyInt");
	fflua_register_t<>(ls).def(&KernelModule_GetPropertyFloat, "GetPropertyFloat");
	fflua_register_t<>(ls).def(&KernelModule_GetPropertyString, "GetPropertyString");
	fflua_register_t<>(ls).def(&KernelModule_GetPropertyObject, "GetPropertyObject");
	fflua_register_t<>(ls).def(&KernelModule_SetRecordInt, "SetRecordInt");
	fflua_register_t<>(ls).def(&KernelModule_SetRecordFloat, "SetRecordFloat");
	fflua_register_t<>(ls).def(&KernelModule_SetRecordString, "SetRecordString");
	fflua_register_t<>(ls).def(&KernelModule_SetRecordObject, "SetRecordObject");
	fflua_register_t<>(ls).def(&KernelModule_GetRecordInt, "GetRecordInt");
	fflua_register_t<>(ls).def(&KernelModule_GetRecordFloat, "GetRecordFloat");
	fflua_register_t<>(ls).def(&KernelModule_GetRecordString, "GetRecordString");
	fflua_register_t<>(ls).def(&KernelModule_GetRecordObject, "GetRecordObject");
	fflua_register_t<>(ls).def(&KernelModule_AddRow, "AddRow");
	//fflua_register_t<>(ls).def(&KernelModule_AddProperty, "AddProperty");

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
		fflua.call<void>(strFunction.c_str(), (NFINT64)m_pKernelModule);
	}
	catch (...)
	{
		
		return 0;
	}
	return 0;
}
