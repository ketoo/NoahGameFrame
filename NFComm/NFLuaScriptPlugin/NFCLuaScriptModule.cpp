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
    mpLuaState = luaL_newstate(); // lua 5.2已经废弃了lua_open， luaL_newstate是辅助库函数，采用默认的内存分配函数, lua_newstate需要传入自定义的内存分配函数
    luaL_openlibs(mpLuaState);
<<<<<<< HEAD
    luabind::open(mpLuaState);

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
    luabind::object objCompoment = luabind::globals(mpLuaState)[strComponentName.c_str()];
    if (luabind::type(objCompoment) != LUA_TTABLE)
    {
        return false;
    }

    luabind::object objFun = objCompoment[strFuncName];
    if (luabind::type(objFun) != LUA_TFUNCTION)
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
        int nRet = luabind::call_function<int>(mpLuaState, "load_script_file", strComponentName.c_str());
        if (nRet <= 0)
        {
            return false;
        }

        mmCompomentStatus.AddElement(strComponentName, new int(1));
    }

    return true;
}

int NFCLuaScriptModule::DoScript( const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabind::object objCompoment = luabind::globals(mpLuaState)[strComponentName.c_str()];
    luabind::call_member<int>(objCompoment, strFunction.c_str(), NFScriptInt64((int)m_pScriptKernelModule), NFScriptInt64(self), arg);

    return 1;
}

int NFCLuaScriptModule::DoEventScript( const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabind::object objCompoment = luabind::globals(mpLuaState)[strComponentName.c_str()];
    luabind::call_member<int>(objCompoment, strFunction.c_str(), NFScriptInt64((int)m_pScriptKernelModule), NFScriptInt64(self), nEventID, arg);

    return 1;
}

int NFCLuaScriptModule::DoHeartBeatScript( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabind::object objCompoment = luabind::globals(mpLuaState)[strComponentName.c_str()];
    luabind::call_member<int>(objCompoment, strFunction.c_str(), NFScriptInt64((int)m_pScriptKernelModule), NFScriptInt64(self), strHeartBeat.c_str(), fTime, nCount, arg);

    return 1;
}

int NFCLuaScriptModule::DoScriptPropertyCallBack( const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabind::object objCompoment = luabind::globals(mpLuaState)[strComponentName.c_str()];
    luabind::call_member<int>(objCompoment, strFunction.c_str(), NFScriptInt64((int)m_pScriptKernelModule), NFScriptInt64(self), strPropertyName.c_str(), oldVar, newVar, arg);

    return 1;
}

int NFCLuaScriptModule::DoScriptRecordCallBack( const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabind::object objCompoment = luabind::globals(mpLuaState)[strComponentName.c_str()];
    luabind::call_member<int>(objCompoment, strFunction.c_str(), NFScriptInt64((int)m_pScriptKernelModule), NFScriptInt64(self), strRecordName.c_str(), nOpType, nRow, nCol, oldVar, newVar, arg);

    return 1;
}


bool NFCLuaScriptModule::Regisger()
{
    using namespace luabind;

    module(mpLuaState)
        [  
//            def ("NFVarList_New", &NFVarList_New),
//            def ("NFVarList_Del", &NFVarList_Del),
//            def ("NFVarList_Clear", &NFVarList_Clear),//            def ("NFVarList_IsEmpty", &NFVarList_IsEmpty),//            def ("NFVarList_GetCount", &NFVarList_GetCount),//            def ("NFVarList_Type", &NFVarList_Type),//            def ("NFVarList_AddInt", &NFVarList_AddInt),//            def ("NFVarList_AddFloat", &NFVarList_AddFloat),//            def ("NFVarList_AddString", &NFVarList_AddString),//            def ("NFVarList_AddObject", &NFVarList_AddObject),//            def ("NFVarList_SetInt", &NFVarList_SetInt),//            def ("NFVarList_SetFloat", &NFVarList_SetFloat),//            def ("NFVarList_SetString", &NFVarList_SetString),//            def ("NFVarList_SetObject", &NFVarList_SetObject),//            def ("NFVarList_IntVal", &NFVarList_IntVal),//            def ("NFVarList_FloatVal", &NFVarList_FloatVal),//            def ("NFVarList_StringVal", &NFVarList_StringVal),//            def ("NFVarList_ObjectVal", &NFVarList_ObjectVal),

            def ("HasEventCallBack", &KernelModule_HasEventCallBack),
            def ("RemoveEvent", &KernelModule_RemoveEvent),
            def ("RemoveEventCallBack", &KernelModule_RemoveEventCallBack),
            def ("DoEvent", &KernelModule_DoEvent),
            def ("ExistElement", &KernelModule_ExistElement),
            def ("GetPropertyInt", (int(*)(const NFScriptInt64&, const std::string&, const std::string& ))&KernelModule_GetPropertyInt),
            def ("GetPropertyFloat", (float (*)(const NFScriptInt64& ,const std::string& , const std::string& ))&KernelModule_GetPropertyFloat),
            def ("GetPropertyString", ( const std::string& (*)(const NFScriptInt64&, const std::string&, const std::string&))&KernelModule_GetPropertyString),
            def ("AddPropertyCallBack", &KernelModule_AddPropertyCallBack),
            def ("AddRecordCallBack", &KernelModule_AddRecordCallBack),
            def ("AddEventCallBack", &KernelModule_AddEventCallBack),
            def ("AddHeartBeat", &KernelModule_AddHeartBeat),
            def ("FindHeartBeat", &KernelModule_FindHeartBeat),
            def ("RemoveHeartBeat", &KernelModule_RemoveHeartBeat),
            def ("SetComponentEnable", &KernelModule_SetComponentEnable),
            def ("QueryComponentEnable", &KernelModule_QueryComponentEnable),
            def ("CreateContainer", &KernelModule_CreateContainer),
            def ("ExistContainer", &KernelModule_ExistContainer),
            def ("CreateObject", &KernelModule_CreateObject),
            def ("DestroyObject", &KernelModule_DestroyObject),
            def ("FindProperty", &KernelModule_FindProperty),
            def ("SetPropertyInt", &KernelModule_SetPropertyInt),
            def ("SetPropertyFloat", &KernelModule_SetPropertyFloat),
            def ("SetPropertyString", &KernelModule_SetPropertyString),
            def ("SetPropertyObject", &KernelModule_SetPropertyObject),        

            def ("GetPropertyInt", (int (*)(const NFScriptInt64&, const NFScriptInt64&, const std::string&))&KernelModule_GetPropertyInt),
            def ("GetPropertyFloat", (float (*)(const NFScriptInt64&, const NFScriptInt64&,const std::string&))&KernelModule_GetPropertyFloat),
            def ("GetPropertyString", (const std::string& (*)(const NFScriptInt64&, const NFScriptInt64&,const std::string&))&KernelModule_GetPropertyString),
            def ("GetPropertyObject", &KernelModule_GetPropertyObject),
            def ("SetRecordInt", &KernelModule_SetRecordInt),
            def ("SetRecordFloat", &KernelModule_SetRecordFloat),
            def ("SetRecordString", &KernelModule_SetRecordString),
            def ("SetRecordObject", &KernelModule_SetRecordObject),
            def ("GetRecordInt", &KernelModule_GetRecordInt),
            def ("GetRecordFloat", &KernelModule_GetRecordFloat),
            def ("GetRecordString", &KernelModule_GetRecordString),
            def ("GetRecordObject", &KernelModule_GetRecordObject),
            def ("AddRow", &KernelModule_AddRow),
            def ("AddProperty", &KernelModule_AddProperty),
            //        def ("KernelModule_AddRecord", &KernelModule_AddRecord),

            class_ <NFCScriptVarList> ("NFCVarList")
            .def(luabind::constructor<>())
            .def ("IsEmpty", &NFCScriptVarList::IsEmpty)
            .def ("GetCount", &NFCScriptVarList::GetCount)
            .def ("Type", &NFCScriptVarList::Type)
            .def ("AddInt", &NFCScriptVarList::AddInt)
            .def ("AddFloat", &NFCScriptVarList::AddFloat)
            .def ("AddString", &NFCScriptVarList::AddString)
            .def ("AddObject", &NFCScriptVarList::AddObject)
            .def ("SetInt", &NFCScriptVarList::SetInt)
            .def ("SetFloat", &NFCScriptVarList::SetFloat)
            .def ("SetString", &NFCScriptVarList::SetString)
            .def ("SetObject", &NFCScriptVarList::SetObject)
            .def ("IntVal", &NFCScriptVarList::Int)
            .def ("FloatVal", &NFCScriptVarList::Float)
            .def("StringVal", &NFCScriptVarList::String)
            .def("ObjectVal", &NFCScriptVarList::Object),

            class_ <NFScriptInt64> ("NFINT64")
            .def(luabind::constructor<>())
            .def ("GetIdent", &NFScriptInt64::GetIdent)    
            .def ("GetnSerial", &NFScriptInt64::GetnSerial)    
            .def ("SetIdent", &NFScriptInt64::SetIdent)    
            .def ("SetnSerial", &NFScriptInt64::SetnSerial)    
        ]; 

    return true;
}
=======

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

int NFCLuaScriptModule::DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCDataList::TData& oldVar, const NFCDataList::TData& newVar)
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
>>>>>>> develop
