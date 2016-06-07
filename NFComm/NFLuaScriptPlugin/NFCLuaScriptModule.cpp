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
#include "NFComm/NFCore/NFTimer.h"

#define TRY_RUN_GLOBAL_SCRIPT_FUN0(strFuncName)   try {LuaIntf::LuaRef func(l, strFuncName);  func.call<LuaIntf::LuaRef>(); }   catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN1(strFuncName, arg1)  try {LuaIntf::LuaRef func(l, strFuncName);  func.call<LuaIntf::LuaRef>(arg1); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN2(strFuncName, arg1, arg2)  try {LuaIntf::LuaRef func(l, strFuncName);  func.call<LuaIntf::LuaRef>(arg1, arg2); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }

#define TRY_LOAD_SCRIPT_FLE(strFileName)  try{l.doFile(strFileName);} catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_ADD_PACKAGE_PATH(strFilePath)  try{l.addPackagePath(strFilePath);} catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
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

    TRY_ADD_PACKAGE_PATH(pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule"); //Add Search Path to Lua

    TRY_LOAD_SCRIPT_FLE("script_init.lua");

    TRY_RUN_GLOBAL_SCRIPT_FUN2("init_script_system", pPluginManager, this);

    TRY_LOAD_SCRIPT_FLE("script_list.lua");
    TRY_LOAD_SCRIPT_FLE("script_module.lua");

    TRY_RUN_GLOBAL_SCRIPT_FUN0("ScriptModule.Init");

    return true;
}

bool NFCLuaScriptModule::AfterInit()
{
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
    if (pPluginManager->GetNowTime() - mnTime > 10)
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

bool NFCLuaScriptModule::AddClassCallBack(std::string& className, std::string& funcName)
{
    auto newFuncName = m_ClassEventFuncMap.GetElement(className);
    if (!newFuncName)
    {
        newFuncName = new std::string(funcName);
        m_ClassEventFuncMap.AddElement(className, newFuncName);
        m_pKernelModule->AddClassCallBack(className, this, &NFCLuaScriptModule::OnClassEventCB);
        return true;
    }
    else
    {
        return false;
    }
}

int NFCLuaScriptModule::OnClassEventCB(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    auto funcName = m_ClassEventFuncMap.GetElement(strClassName);
    if (funcName)
    {
        try
        {
            LuaIntf::LuaRef func(l, funcName->c_str());
            func.call(self, strClassName, (int)eClassEvent, (NFCDataList)var);
        }
        catch (LuaIntf::LuaException& e)
        {
            cout << e.what() << endl;
            return false;
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

bool NFCLuaScriptModule::AddPropertyCallBack(const NFGUID& self, std::string& strPropertyName, std::string& luaFunc)
{
    if (AddLuaFuncToMap(m_luaPropertyCallBackFuncMap, self, strPropertyName, luaFunc))
    {
        m_pKernelModule->AddPropertyCallBack(self, strPropertyName, this, &NFCLuaScriptModule::OnLuaPropertyCB);
    }

    return true;
}

int NFCLuaScriptModule::OnLuaPropertyCB(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    return CallLuaFuncFromMap(m_luaPropertyCallBackFuncMap, strPropertyName, self, strPropertyName, oldVar, newVar);
}

bool NFCLuaScriptModule::AddRecordCallBack(const NFGUID& self, std::string& strRecordName, std::string& luaFunc)
{
    if (AddLuaFuncToMap(m_luaRecordCallBackFuncMap, self, strRecordName, luaFunc))
    {
        m_pKernelModule->AddRecordCallBack(self, strRecordName, this, &NFCLuaScriptModule::OnLuaRecordCB);
    }
    return true;
}

int NFCLuaScriptModule::OnLuaRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    return CallLuaFuncFromMap(m_luaRecordCallBackFuncMap, xEventData.strRecordName, self, xEventData.strRecordName, xEventData.nOpType, xEventData.nRow, xEventData.nCol, oldVar, newVar);
}

bool NFCLuaScriptModule::AddEventCallBack(const NFGUID& self, const int nEventID, std::string& luaFunc)
{
    if (AddLuaFuncToMap(m_luaEventCallBackFuncMap, self, (int)nEventID, luaFunc))
    {
        m_pKernelModule->AddEventCallBack(self, nEventID, this, &NFCLuaScriptModule::OnLuaEventCB);
    }
    return true;
}

int NFCLuaScriptModule::OnLuaEventCB(const NFGUID& self, const int nEventID, const NFIDataList& argVar)
{
    return CallLuaFuncFromMap(m_luaEventCallBackFuncMap, nEventID, self, nEventID, (NFCDataList&)argVar);
}

bool NFCLuaScriptModule::AddHeartBeat(const NFGUID& self, std::string& strHeartBeatName, std::string& luaFunc, const float fTime, const int nCount)
{
    if (AddLuaFuncToMap(m_luaHeartBeatCallBackFuncMap, self, strHeartBeatName, luaFunc))
    {
        m_pKernelModule->AddHeartBeat(self, strHeartBeatName, this, &NFCLuaScriptModule::OnLuaHeartBeatCB, fTime, nCount);
    }
    return true;
}

int NFCLuaScriptModule::OnLuaHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float fTime, const int nCount)
{
    return CallLuaFuncFromMap(m_luaHeartBeatCallBackFuncMap, strHeartBeatName, self, strHeartBeatName, fTime, nCount);
}

int NFCLuaScriptModule::AddRow(const NFGUID& self, std::string& strRecordName, const NFIDataList& var)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (nullptr == pRecord)
    {
        return -1;
    }

    return pRecord->AddRow(-1, var);
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
                    LuaIntf::LuaRef func(l, funcName.c_str());
                    func.call(self, arg...);
                }
                catch (LuaIntf::LuaException& e)
                {
                    cout << e.what() << endl;
                    return false;
                }
                Ret = funcNameList->Next(funcName);
            }
        }
    }
    return true;
}

bool NFCLuaScriptModule::Regisger()
{
    LuaIntf::LuaBinding(l).beginClass<RECORD_EVENT_DATA>("RECORD_EVENT_DATA")
    .endClass();

    LuaIntf::LuaBinding(l).beginClass<NFIObject>("NFIObject")
    .addFunction("Self", &NFIObject::Self)
    .endClass();

    LuaIntf::LuaBinding(l).beginClass<NFILogicClassModule>("NFILogicClassModule")
    .endClass();

    LuaIntf::LuaBinding(l).beginClass<NFIPluginManager>("NFIPluginManager")
    .addFunction("FindLuaModule", &NFIPluginManager::FindModule<NFILuaScriptModule>)
    .addFunction("FindKernelModule", &NFIPluginManager::FindModule<NFIKernelModule>)
    .addFunction("FindLogicClassModule", &NFIPluginManager::FindModule<NFILogicClassModule>)
    .addFunction("FindElementInfoModule", &NFIPluginManager::FindModule<NFIElementInfoModule>)
	.addFunction("GetNowTime", &NFIPluginManager::GetNowTime)
    .endClass();

    LuaIntf::LuaBinding(l).beginClass<NFIElementInfoModule>("NFIElementInfoModule")
    .addFunction("ExistElement", &NFIElementInfoModule::ExistElement)
    .addFunction("GetPropertyInt", &NFIElementInfoModule::GetPropertyInt)
    .addFunction("GetPropertyFloat", &NFIElementInfoModule::GetPropertyFloat)
    .addFunction("GetPropertyString", &NFIElementInfoModule::GetPropertyString)
    .endClass();

    LuaIntf::LuaBinding(l).beginClass<NFIKernelModule>("NFIKernelModule")
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
    .addFunction("GetRecordFloat", (double(NFIKernelModule::*)(const NFGUID&, const string&, const int, const int))&NFIKernelModule::GetRecordFloat)
    .addFunction("GetRecordString", (const string & (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int))&NFIKernelModule::GetRecordString)
    .addFunction("GetRecordObject", (const NFGUID & (NFIKernelModule::*)(const NFGUID&, const string&, const int, const int))&NFIKernelModule::GetRecordObject)
    .endClass();

    LuaIntf::LuaBinding(l).beginClass<NFGUID>("NFGUID")
    .addConstructor(LUA_ARGS())
    .addFunction("GetData", &NFGUID::GetData)
    .addFunction("SetData", &NFGUID::SetData)
    .addFunction("GetHead", &NFGUID::GetHead)
    .addFunction("SetHead", &NFGUID::SetHead)
    .endClass();

    LuaIntf::LuaBinding(l).beginClass<NFIDataList>("NFIDataList")
    .endClass();

    LuaIntf::LuaBinding(l).beginExtendClass<NFCDataList, NFIDataList>("NFCDataList")
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

    LuaIntf::LuaBinding(l).beginClass<NFCDataList::TData>("TData")
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

    LuaIntf::LuaBinding(l).beginClass<NFCLuaScriptModule>("NFCLuaScriptModule")
    .addFunction("AddPropertyCallBack", &NFCLuaScriptModule::AddPropertyCallBack)
    .addFunction("AddRecordCallBack", &NFCLuaScriptModule::AddRecordCallBack)
    .addFunction("AddEventCallBack", &NFCLuaScriptModule::AddEventCallBack)
    .addFunction("AddHeartBeat", &NFCLuaScriptModule::AddHeartBeat)
    .addFunction("AddRow", &NFCLuaScriptModule::AddRow)
    .addFunction("AddClassCallBack", &NFCLuaScriptModule::AddClassCallBack)
    .endClass();

    return true;
}
