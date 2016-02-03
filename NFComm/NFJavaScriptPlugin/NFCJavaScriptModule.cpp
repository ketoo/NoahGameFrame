// -------------------------------------------------------------------------
//    @FileName      :    NFCJavaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCJavaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCJavaScriptModule.h"
#include "NFJavaScriptPlugin.h"

bool NFCJavaScriptModule::Init()
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

    bool ret = InstallJS("script_init.js");
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
    m_pKernelModule->ResgisterCommonPropertyEvent(this, &NFCJavaScriptModule::OnPropertyCommEvent);
    m_pKernelModule->ResgisterCommonRecordEvent(this, &NFCJavaScriptModule::OnRecordCommonEvent);
    m_pKernelModule->ResgisterCommonClassEvent(this, &NFCJavaScriptModule::OnClassCommonEvent);


    return true;
}

bool NFCJavaScriptModule::AfterInit()
{
    return true;
}

bool NFCJavaScriptModule::BeforeShut()
{
    delete m_pScriptKernelModule;
    m_pScriptKernelModule = NULL;

    return true;
}

bool NFCJavaScriptModule::Shut()
{
    return true;
}

bool NFCJavaScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

int NFCJavaScriptModule::DoScript( const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoEventScript( const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoHeartBeatScript( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoScriptPropertyCallBack( const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& neVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoScriptRecordCallBack( const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::OnPropertyCommEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoPropertyCommEvent(m_pScriptKernelModule, self,strPropertyName, oldVar, newVar, arg);

    return 0;
}

int NFCJavaScriptModule::OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoRecordCommonEvent(m_pScriptKernelModule, self,strRecordName, nOpType, nRow, nCol, oldVar, newVar, arg);

    return 0;
}

int NFCJavaScriptModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    DoClassCommonEvent(m_pLogicClassModule, self, strClassName, eClassEvent, var);

    return 0;
}

bool NFCJavaScriptModule::InstallJS(const std::string& strComponentName)
{
    m_pIsolate = v8::Isolate::New();
    v8::HandleScope xHandleScope(m_pIsolate);
    mxContext = v8::Context::New(m_pIsolate);
    if (mxContext.IsEmpty())
    {
        assert(0);
        return false;
    }

    std::ifstream file(strComponentName.c_str());
    if (!file)
    {
        assert(0);
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string strJSContent = buffer.str();
    file.close();

    v8::Local<v8::Script> script = v8::Script::Compile(v8::String::NewFromUtf8(m_pIsolate, strJSContent.c_str()));
    if (script.IsEmpty())
    {
        return false;
    }

    script->Run();

    return true;
}
bool NFCJavaScriptModule::CheckCompomentStatus(const std::string& strComponentName, const std::string& strFuncName)
{
    if( !CheckCompomentStatus(strComponentName))
    {
        assert(0);

        return false;
    }

    //ÊÇ·ñÓÐº¯Êý
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

bool NFCJavaScriptModule::CheckCompomentStatus(const std::string& strComponentName)
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

bool NFCJavaScriptModule::Regisger()
{
    //using namespace luabind;

    //module(mpLuaState)
    //    [
    //        def ("HasEventCallBack", &KernelModule_HasEventCallBack),
    //        def ("RemoveEvent", &KernelModule_RemoveEvent),
    //        def ("RemoveEventCallBack", &KernelModule_RemoveEventCallBack),
    //        def ("DoEvent", &KernelModule_DoEvent),
    //        def ("ExistElement", &KernelModule_ExistElement),
    //        def ("GetPropertyInt", (int(*)(const NFScriptIdent&, const std::string&, const std::string& ))&KernelModule_GetPropertyInt),
    //        def ("GetPropertyFloat", (float (*)(const NFScriptIdent& ,const std::string& , const std::string& ))&KernelModule_GetPropertyFloat),
    //        def ("GetPropertyString", ( const std::string& (*)(const NFScriptIdent&, const std::string&, const std::string&))&KernelModule_GetPropertyString),
    //        def ("AddPropertyCallBack", &KernelModule_AddPropertyCallBack),
    //        def ("AddRecordCallBack", &KernelModule_AddRecordCallBack),
    //        def ("AddEventCallBack", &KernelModule_AddEventCallBack),
    //        def ("AddHeartBeat", &KernelModule_AddHeartBeat),
    //        def ("FindHeartBeat", &KernelModule_FindHeartBeat),
    //        def ("RemoveHeartBeat", &KernelModule_RemoveHeartBeat),
    //        def ("SetComponentEnable", &KernelModule_SetComponentEnable),
    //        def ("QueryComponentEnable", &KernelModule_QueryComponentEnable),
    //        def ("CreateContainer", &KernelModule_CreateContainer),
    //        def ("ExistContainer", &KernelModule_ExistContainer),
    //        def ("CreateObject", &KernelModule_CreateObject),
    //        def ("DestroyObject", &KernelModule_DestroyObject),
    //        def ("FindProperty", &KernelModule_FindProperty),
    //        def ("SetPropertyInt", &KernelModule_SetPropertyInt),
    //        def ("SetPropertyFloat", &KernelModule_SetPropertyFloat),
    //        def ("SetPropertyString", &KernelModule_SetPropertyString),
    //        def ("SetPropertyObject", &KernelModule_SetPropertyObject),

    //        def ("GetPropertyInt", (int (*)(const NFScriptIdent&, const NFScriptIdent&, const std::string&))&KernelModule_GetPropertyInt),
    //        def ("GetPropertyFloat", (float (*)(const NFScriptIdent&, const NFScriptIdent&,const std::string&))&KernelModule_GetPropertyFloat),
    //        def ("GetPropertyString", (const std::string& (*)(const NFScriptIdent&, const NFScriptIdent&,const std::string&))&KernelModule_GetPropertyString),
    //        def ("GetPropertyObject", &KernelModule_GetPropertyObject),
    //        def ("SetRecordInt", &KernelModule_SetRecordInt),
    //        def ("SetRecordFloat", &KernelModule_SetRecordFloat),
    //        def ("SetRecordString", &KernelModule_SetRecordString),
    //        def ("SetRecordObject", &KernelModule_SetRecordObject),
    //        def ("GetRecordInt", &KernelModule_GetRecordInt),
    //        def ("GetRecordFloat", &KernelModule_GetRecordFloat),
    //        def ("GetRecordString", &KernelModule_GetRecordString),
    //        def ("GetRecordObject", &KernelModule_GetRecordObject),
    //        def ("AddRow", &KernelModule_AddRow),
    //        def ("AddProperty", &KernelModule_AddProperty),
    //        //        def ("KernelModule_AddRecord", &KernelModule_AddRecord),

    v8::Handle<v8::FunctionTemplate> xScriptVarListClass = v8::FunctionTemplate::New(m_pIsolate);
    xScriptVarListClass->SetClassName(v8::String::NewFromUtf8(m_pIsolate, "NFCScriptVarList"));
    v8::Handle<v8::ObjectTemplate> xScriptVarListProto = xScriptVarListClass->PrototypeTemplate();
    xScriptVarListProto->Set(v8::String::NewFromUtf8(m_pIsolate, "IsEmpty"), v8::FunctionTemplate::New(m_pIsolate, &NFCScriptVarList::IsEmpty()));

        //    class_ <NFCScriptVarList> ("NFCVarList")
        //    .def(luabind::constructor<>())
        //    .def ("IsEmpty", &NFCScriptVarList::IsEmpty)
        //    .def ("GetCount", &NFCScriptVarList::GetCount)
        //    .def ("Type", &NFCScriptVarList::Type)
        //    .def ("AddInt", &NFCScriptVarList::AddInt)
        //    .def ("AddFloat", &NFCScriptVarList::AddFloat)
        //    .def ("AddString", &NFCScriptVarList::AddString)
        //    .def ("AddObject", &NFCScriptVarList::AddObject)
        //    .def ("SetInt", &NFCScriptVarList::SetInt)
        //    .def ("SetFloat", &NFCScriptVarList::SetFloat)
        //    .def ("SetString", &NFCScriptVarList::SetString)
        //    .def ("SetObject", &NFCScriptVarList::SetObject)
        //    .def ("Int", &NFCScriptVarList::Int)
        //    .def ("Float", &NFCScriptVarList::Float)
        //    .def("String", &NFCScriptVarList::String)
        //    .def("Object", &NFCScriptVarList::Object),

        //    class_ <NFScriptIdent> ("NFIDENTID")
        //    .def(luabind::constructor<>())
        //    .def ("GetIdent", &NFScriptIdent::GetIdent)
        //    .def ("GetData", &NFScriptIdent::GetData)
        //    .def ("SetData", &NFScriptIdent::SetData)
        //    .def ("GetHead", &NFScriptIdent::GetHead)
        //    .def ("SetHead", &NFScriptIdent::SetHead)
        //]; 

    return true;
}