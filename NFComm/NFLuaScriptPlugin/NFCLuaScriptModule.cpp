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
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);

    m_pScriptKernelModule = new NFCScriptKernelModule(m_pKernelModule, this, m_pElementInfoModule, m_pEventProcessModule);

    bool ret = InstallLua("script_init.lua");
    assert( ret );

    std::shared_ptr<NFILogicClass> pClass = m_pLogicClassModule->First();
    while (pClass.get())
    {
        std::shared_ptr<NFIComponent> pComponent = pClass->GetComponentManager()->First();
        while (pComponent.get())
        {
            if (!CheckCompomentStatus(pComponent->ComponentName()))
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

bool NFCLuaScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCLuaScriptModule::BeforeShut()
{
    delete m_pScriptKernelModule;
    m_pScriptKernelModule = NULL;

    lua_close(mpLuaState);
    mpLuaState = NULL;

    return true;
}

int NFCLuaScriptModule::OnPropertyCommEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoPropertyCommEvent(m_pScriptKernelModule, self,strPropertyName, oldVar, newVar, arg);

    return 0;
}

int NFCLuaScriptModule::OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoRecordCommonEvent(m_pScriptKernelModule, self,strRecordName, nOpType, nRow, nCol, oldVar, newVar, arg);

    return 0;
}

int NFCLuaScriptModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
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

int NFCLuaScriptModule::DoScript( const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment[strFunction.c_str()];
    objFun(NFScriptIdent(NFIDENTID(0, (NFINT64)m_pScriptKernelModule)), NFScriptIdent(self), arg);

    return 1;
}

int NFCLuaScriptModule::DoEventScript( const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    objFun(NFScriptIdent(NFIDENTID(0, (NFINT64)m_pScriptKernelModule)), NFScriptIdent(self), nEventID, arg);

    return 1;
}

int NFCLuaScriptModule::DoHeartBeatScript( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    objFun(NFScriptIdent(NFIDENTID(0, (NFINT64)m_pScriptKernelModule)), NFScriptIdent(self), strHeartBeat.c_str(), fTime, nCount, arg);

    return 1;
}

int NFCLuaScriptModule::DoScriptPropertyCallBack( const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    objFun(NFScriptIdent(NFIDENTID(0, (NFINT64)m_pScriptKernelModule)), NFScriptIdent(self), strPropertyName.c_str(), oldVar, newVar, arg);

    return 1;
}

int NFCLuaScriptModule::DoScriptRecordCallBack( const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    if (!CheckCompomentStatus(strComponentName, strFunction))
    {
        return 0;
    }

    luabridge::LuaRef objCompoment = luabridge::getGlobal(mpLuaState, strComponentName.c_str());
    luabridge::LuaRef objFun = objCompoment.getGlobal(mpLuaState, strFunction.c_str());
    // TODO
    //objFun(NFScriptIdent(NFIDENTID(0, (NFINT64)m_pScriptKernelModule)), NFScriptIdent(self), strRecordName.c_str(), nOpType, nRow, nCol, oldVar, newVar, arg);

    return 1;
}


bool NFCLuaScriptModule::Regisger()
{
    luabridge::getGlobalNamespace(mpLuaState).addFunction("HasEventCallBack", KernelModule_HasEventCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("RemoveEvent", &KernelModule_RemoveEvent);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("RemoveEventCallBack", &KernelModule_RemoveEventCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("DoEvent", &KernelModule_DoEvent);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("ExistElement", &KernelModule_ExistElement);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyInt", (int (*)(const NFScriptIdent&, const std::string&, const std::string&))&KernelModule_GetPropertyInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyFloat", (float (*)(const NFScriptIdent&, const std::string&, const std::string&))&KernelModule_GetPropertyFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyString", (const std::string& (*)(const NFScriptIdent&, const std::string&, const std::string&))&KernelModule_GetPropertyString);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddPropertyCallBack", &KernelModule_AddPropertyCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddRecordCallBack", &KernelModule_AddRecordCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddEventCallBack", &KernelModule_AddEventCallBack);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("AddHeartBeat", &KernelModule_AddHeartBeat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("FindHeartBeat", &KernelModule_FindHeartBeat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("RemoveHeartBeat", &KernelModule_RemoveHeartBeat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetComponentEnable", &KernelModule_SetComponentEnable);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("QueryComponentEnable", &KernelModule_QueryComponentEnable);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("CreateContainer", &KernelModule_CreateContainer);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("ExistContainer", &KernelModule_ExistContainer);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("CreateObject", &KernelModule_CreateObject);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("DestroyObject", &KernelModule_DestroyObject);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("FindProperty", &KernelModule_FindProperty);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyInt", &KernelModule_SetPropertyInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyFloat", &KernelModule_SetPropertyFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyString", &KernelModule_SetPropertyString);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("SetPropertyObject", &KernelModule_SetPropertyObject);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyInt", (int (*)(const NFScriptIdent&, const NFScriptIdent&, const std::string&))&KernelModule_GetPropertyInt);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyFloat", (float (*)(const NFScriptIdent&, const NFScriptIdent&, const std::string&))&KernelModule_GetPropertyFloat);
    luabridge::getGlobalNamespace(mpLuaState).addFunction("GetPropertyString", (const std::string& (*)(const NFScriptIdent&, const NFScriptIdent&, const std::string&))&KernelModule_GetPropertyString);
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
        .beginClass<NFCScriptVarList>("NFCVarList")
            .addConstructor<void (*)(void)>()
            .addFunction("IsEmpty", &NFCScriptVarList::IsEmpty)
            .addFunction("GetCount", &NFCScriptVarList::GetCount)
            .addFunction("Type", &NFCScriptVarList::Type)
            .addFunction("AddInt", &NFCScriptVarList::AddInt)
            .addFunction("AddFloat", &NFCScriptVarList::AddFloat)
            .addFunction("AddString", &NFCScriptVarList::AddString)
            .addFunction("AddObject", &NFCScriptVarList::AddObject)
            .addFunction("SetInt", &NFCScriptVarList::SetInt)
            .addFunction("SetFloat", &NFCScriptVarList::SetFloat)
            .addFunction("SetString", &NFCScriptVarList::SetString)
            .addFunction("SetObject", &NFCScriptVarList::SetObject)
            .addFunction("Int", &NFCScriptVarList::Int)
            .addFunction("Float", &NFCScriptVarList::Float)
            .addFunction("String", &NFCScriptVarList::String)
            .addFunction("Object", &NFCScriptVarList::Object)
        .endClass();

    luabridge::getGlobalNamespace(mpLuaState)
        .beginClass<NFScriptIdent>("NFIDENTID")
            .addConstructor<void (*)(void)>()
            .addConstructor<void (*)(const NFScriptIdent&)>()
            .addFunction("GetIdent", &NFScriptIdent::GetIdent)
            .addFunction("GetData", &NFScriptIdent::GetData)
            .addFunction("SetData", &NFScriptIdent::SetData)
            .addFunction("GetHead", &NFScriptIdent::GetHead)
            .addFunction("SetHead", &NFScriptIdent::SetHead)
        .endClass();

    return true;
}