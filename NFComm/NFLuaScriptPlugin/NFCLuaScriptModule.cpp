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
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);

    bool ret = InstallLua("script_init.lua");
    assert( ret );

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
    lua_close(mpLuaState);
    mpLuaState = NULL;

    return true;
}

int NFCLuaScriptModule::OnPropertyCommEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    DoPropertyCommEvent(self, strPropertyName, oldVar, newVar);

    return 0;
}

int NFCLuaScriptModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar)
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
    mpLuaState = luaL_newstate(); // lua 5.2已经废弃了lua_open， luaL_newstate是辅助库函数，采用默认的内存分配函数, lua_newstate需要传入自定义的内存分配函数
    luaL_openlibs(mpLuaState);

    if ( !Regisger() )
    {
        lua_close(mpLuaState);
        mpLuaState = NULL;
        return false;
    }    

    if(luaL_loadfile(mpLuaState, strComponentName.c_str()) != 0)
    {
        lua_close(mpLuaState);
        mpLuaState = NULL;
        return false;
    }

    lua_pcall(mpLuaState, 0, 0, 0);

    return true;
}

bool NFCLuaScriptModule::CheckCompomentStatus(const std::string& strComponentName, const std::string& strFuncName)
{
    if( !CheckCompomentStatus(strComponentName))
    {
        assert(0);

        return false;
    }

    //是否有函数
    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    if (!objCompoment.isTable())
    {
        return false;
    }

    luabridge::LuaRef objFun = objCompoment[strFuncName.c_str()];
    if (!objFun.isFunction())
    {
        assert(0);
        return false;
    }

    return true;
}

bool NFCLuaScriptModule::CheckCompomentStatus( const std::string& strComponentName )
{
    if (!mmCompomentStatus.GetElement(strComponentName))
    {
        luabridge::LuaRef func = luabridge::getGlobal(mpLuaState, "load_script_file");
        int nRet = func(strComponentName.c_str());
        if (nRet <= 0)
        {
            return false;
        }

        mmCompomentStatus.AddElement(strComponentName, new int(1));
    }

    return true;
}

int NFCLuaScriptModule::DoScript( const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment[strFunction.c_str()];
    objFun((NFINT64)m_pKernelModule, self, arg);

    return 1;
}

int NFCLuaScriptModule::DoEventScript( const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    objFun((NFINT64)m_pKernelModule, self, nEventID, arg);

    return 1;
}

int NFCLuaScriptModule::DoHeartBeatScript(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction)
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    objFun((NFINT64)m_pKernelModule, self, strHeartBeat.c_str(), fTime, nCount);

    return 1;
}

int NFCLuaScriptModule::DoScriptPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    objFun((NFINT64)m_pKernelModule, self, strPropertyName.c_str(), oldVar, newVar);

    return 1;
}

int NFCLuaScriptModule::DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCDataList& oldVar, const NFCDataList& newVar)
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    objFun((NFINT64)m_pKernelModule, self, strRecordName.c_str(), nOpType, nRow, nCol, oldVar, newVar);

    return 1;
}

bool NFCLuaScriptModule::Regisger()
{
    luabridge::getGlobalNamespace(mpLuaState).addFunction("DoEvent", &KernelModule_DoEvent);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("ExistElement", &KernelModule_ExistElement);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetElementPropertyInt", &KernelModule_GetElementPropertyInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetElementPropertyFloat", &KernelModule_GetElementPropertyFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetElementPropertyString", &KernelModule_GetElementPropertyString);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddPropertyCallBack", &KernelModule_AddPropertyCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddRecordCallBack", &KernelModule_AddRecordCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddEventCallBack", &KernelModule_AddEventCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddHeartBeat", &KernelModule_AddHeartBeat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("FindHeartBeat", &KernelModule_FindHeartBeat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("RemoveHeartBeat", &KernelModule_RemoveHeartBeat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("ExistContainer", &KernelModule_ExistContainer);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyInt", &KernelModule_SetPropertyInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyFloat", &KernelModule_SetPropertyFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyString", &KernelModule_SetPropertyString);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyObject", &KernelModule_SetPropertyObject);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyInt", &KernelModule_GetPropertyInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyFloat", &KernelModule_GetPropertyFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyString", &KernelModule_GetPropertyString);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyObject", &KernelModule_GetPropertyObject);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetRecordInt", &KernelModule_SetRecordInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetRecordFloat", &KernelModule_SetRecordFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetRecordString", &KernelModule_SetRecordString);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetRecordObject", &KernelModule_SetRecordObject);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetRecordInt", &KernelModule_GetRecordInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetRecordFloat", &KernelModule_GetRecordFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetRecordString", &KernelModule_GetRecordString);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetRecordObject", &KernelModule_GetRecordObject);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddRow", &KernelModule_AddRow);
    //luabridge::getGlobalNamespace(mpLuaState).addFunction("AddProperty", &KernelModule_AddProperty);

    luabridge::getGlobalNamespace(mpLuaState)
        .beginClass<NFCDataList>("NFCDataList")
            .addConstructor<void (*)(void)>()
            .addFunction("IsEmpty", &NFCDataList::IsEmpty)
            .addFunction("GetCount", &NFCDataList::GetCount)
            .addFunction("Type", &NFCDataList::Type)
            .addFunction("AddInt", &NFCDataList::AddInt)
            .addFunction("AddFloat", &NFCDataList::AddFloat)
            .addFunction("AddString", &NFCDataList::AddString)
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

    luabridge::getGlobalNamespace(mpLuaState)
        .beginClass<NFGUID>("NFGUID")
            .addConstructor<void (*)(void)>()
            .addConstructor<void (*)(const NFGUID&)>()
            .addConstructor<void (*)(NFINT64, NFINT64)>()
            .addFunction("GetData", &NFGUID::GetData)
            .addFunction("SetData", &NFGUID::SetData)
            .addFunction("GetHead", &NFGUID::GetHead)
            .addFunction("SetHead", &NFGUID::SetHead)
        .endClass();

    return true;
}

int NFCLuaScriptModule::DoClassCommonScript( const NFGUID& self, const std::string& strComponentName, const std::string& strFunction )
{
	if (!CheckCompomentStatus(strComponentName, strFunction))
	{
		return 0;
	}

	luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
	luabridge::LuaRef objFun = objCompoment[strFunction.c_str()];
	objFun((NFINT64)m_pKernelModule, self);

	return 0;
}
