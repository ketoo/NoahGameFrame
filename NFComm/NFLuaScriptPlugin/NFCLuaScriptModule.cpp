// -------------------------------------------------------------------------
//    @FileName			:    NFCLuaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include <assert.h>
#include "NFCLuaScriptModule.h"
#include "NFLuaScriptPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

#define TRY_RUN_GLOBAL_SCRIPT_FUN0(strFuncName)   try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(); }   catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN1(strFuncName, arg1)  try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(arg1); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }
#define TRY_RUN_GLOBAL_SCRIPT_FUN2(strFuncName, arg1, arg2)  try {LuaIntf::LuaRef func(mLuaContext, strFuncName);  func.call<LuaIntf::LuaRef>(arg1, arg2); }catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }

#define TRY_LOAD_SCRIPT_FLE(strFileName)  try{mLuaContext.doFile(strFileName);} catch (LuaIntf::LuaException& e) { cout << e.what() << endl; }

bool NFCLuaScriptModule::Awake()
{
	mnTime = pPluginManager->GetNowTime();

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();

	Regisger();

	std::string strRootFileh = pPluginManager->GetConfigPath() + "NFDataCfg/ScriptModule/NFScriptSystem.lua";

	TRY_LOAD_SCRIPT_FLE(strRootFileh.c_str());

	TRY_RUN_GLOBAL_SCRIPT_FUN1("init_script_system", this);

	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_awake");

	return true;
}

bool NFCLuaScriptModule::Init()
{
    TRY_RUN_GLOBAL_SCRIPT_FUN0("module_init");

    return true;
}

bool NFCLuaScriptModule::AfterInit()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_after_init");

    return true;
}

bool NFCLuaScriptModule::Shut()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_shut");

    return true;
}

bool NFCLuaScriptModule::ReadyExecute()
{
	TRY_RUN_GLOBAL_SCRIPT_FUN0("module_ready_execute");

	return true;
}

bool NFCLuaScriptModule::Execute()
{
    //10秒钟reload一次
    if (pPluginManager->GetNowTime() - mnTime > 10)
    {
        mnTime = pPluginManager->GetNowTime();

		TRY_RUN_GLOBAL_SCRIPT_FUN0("module_execute");
    }

    return true;
}

bool NFCLuaScriptModule::BeforeShut()
{
    TRY_RUN_GLOBAL_SCRIPT_FUN0("module_before_shut");

    return true;
}

bool NFCLuaScriptModule::FindProperty(const NFGUID & self, const std::string & strPropertyName)
{
	return false;
}

bool NFCLuaScriptModule::SetPropertyInt(const NFGUID & self, const std::string & strPropertyName, const NFINT64 nValue)
{
	return false;
}

bool NFCLuaScriptModule::SetPropertyFloat(const NFGUID & self, const std::string & strPropertyName, const double dValue)
{
	return false;
}

bool NFCLuaScriptModule::SetPropertyString(const NFGUID & self, const std::string & strPropertyName, const std::string & strValue)
{
	return false;
}

bool NFCLuaScriptModule::SetPropertyObject(const NFGUID & self, const std::string & strPropertyName, const NFGUID & objectValue)
{
	return false;
}

bool NFCLuaScriptModule::SetPropertyVector2(const NFGUID & self, const std::string & strPropertyName, const NFVector2 & value)
{
	return false;
}

bool NFCLuaScriptModule::SetPropertyVector3(const NFGUID & self, const std::string & strPropertyName, const NFVector3 & value)
{
	return false;
}

NFINT64 NFCLuaScriptModule::GetPropertyInt(const NFGUID & self, const std::string & strPropertyName)
{
	return NFINT64();
}

int NFCLuaScriptModule::GetPropertyInt32(const NFGUID & self, const std::string & strPropertyName)
{
	return 0;
}

double NFCLuaScriptModule::GetPropertyFloat(const NFGUID & self, const std::string & strPropertyName)
{
	return 0.0;
}

std::string NFCLuaScriptModule::GetPropertyString(const NFGUID & self, const std::string & strPropertyName)
{
	return "";
}

NFGUID NFCLuaScriptModule::GetPropertyObject(const NFGUID & self, const std::string & strPropertyName)
{
	return NFGUID();
}

NFVector2 NFCLuaScriptModule::GetPropertyVector2(const NFGUID & self, const std::string & strPropertyName)
{
	return NFVector2();
}

NFVector3 NFCLuaScriptModule::GetPropertyVector3(const NFGUID & self, const std::string & strPropertyName)
{
	return NFVector3();
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

int NFCLuaScriptModule::OnClassEventCB(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    auto funcName = m_ClassEventFuncMap.GetElement(strClassName);
    if (funcName)
    {
        try
        {
            LuaIntf::LuaRef func(mLuaContext, funcName->c_str());
            func.call(self, strClassName, (int)eClassEvent, (NFDataList)var);
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

int NFCLuaScriptModule::OnLuaPropertyCB(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
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

int NFCLuaScriptModule::OnLuaRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    return CallLuaFuncFromMap(m_luaRecordCallBackFuncMap, xEventData.strRecordName, self, xEventData.strRecordName, xEventData.nOpType, xEventData.nRow, xEventData.nCol, oldVar, newVar);
}

bool NFCLuaScriptModule::AddEventCallBack(const NFGUID& self, const NFEventDefine nEventID, std::string& luaFunc)
{
    if (AddLuaFuncToMap(m_luaEventCallBackFuncMap, self, (int)nEventID, luaFunc))
    {
		m_pEventModule->AddEventCallBack(self, nEventID, this, &NFCLuaScriptModule::OnLuaEventCB);
    }
    return true;
}

int NFCLuaScriptModule::OnLuaEventCB(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& argVar)
{
    return CallLuaFuncFromMap(m_luaEventCallBackFuncMap, (int)nEventID, self, nEventID, (NFDataList&)argVar);
}

bool NFCLuaScriptModule::AddSchedule(const NFGUID& self, std::string& strHeartBeatName, std::string& luaFunc, const float fTime, const int nCount)
{
    if (AddLuaFuncToMap(m_luaHeartBeatCallBackFuncMap, self, strHeartBeatName, luaFunc))
    {
		m_pScheduleModule->AddSchedule(self, strHeartBeatName, this, &NFCLuaScriptModule::OnLuaHeartBeatCB, fTime, nCount);
    }
    return true;
}
/*
bool NFCLuaScriptModule::DoEvent(const NFGUID & self, std::string & strHeartBeatName, std::string & luaFunc, const float fTime, const int nCount)
{
	return false;
}
*/

int NFCLuaScriptModule::OnLuaHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float fTime, const int nCount)
{
    return CallLuaFuncFromMap(m_luaHeartBeatCallBackFuncMap, strHeartBeatName, self, strHeartBeatName, fTime, nCount);
}

int NFCLuaScriptModule::AddRow(const NFGUID& self, std::string& strRecordName, const NFDataList& var)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (nullptr == pRecord)
    {
        return -1;
    }

    return pRecord->AddRow(-1, var);
}

bool NFCLuaScriptModule::RemRow(const NFGUID & self, std::string & strRecordName, const int nRow)
{
	return false;
}

bool NFCLuaScriptModule::SetRecordInt(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFINT64 value)
{
	return false;
}

bool NFCLuaScriptModule::SetRecordFloat(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const double value)
{
	return false;
}

bool NFCLuaScriptModule::SetRecordString(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const std::string & value)
{
	return false;
}

bool NFCLuaScriptModule::SetRecordObject(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFGUID & value)
{
	return false;
}

bool NFCLuaScriptModule::SetRecordVector2(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFVector2 & value)
{
	return false;
}

bool NFCLuaScriptModule::SetRecordVector3(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag, const NFVector3 & value)
{
	return false;
}

NFINT64 NFCLuaScriptModule::GetRecordInt(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return NFINT64();
}

double NFCLuaScriptModule::GetRecordFloat(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return 0.0;
}

std::string NFCLuaScriptModule::GetRecordString(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return "";
}

NFGUID NFCLuaScriptModule::GetRecordObject(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return NFGUID();
}

NFVector2 NFCLuaScriptModule::GetRecordVector2(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return NFVector2();
}

NFVector3 NFCLuaScriptModule::GetRecordVector3(const NFGUID & self, const std::string & strRecordName, const int nRow, const std::string & strColTag)
{
	return NFVector3();
}

NFINT64 NFCLuaScriptModule::GetNowTime()
{
	return pPluginManager->GetNowTime();
}

NFGUID NFCLuaScriptModule::CreateID()
{
	return m_pKernelModule->CreateGUID();
}

NFINT64 NFCLuaScriptModule::GetElePropertyInt(const std::string & strConfigName, const std::string & strPropertyName)
{
	return NFINT64();
}

double NFCLuaScriptModule::GetElePropertyFloat(const std::string & strConfigName, const std::string & strPropertyName)
{
	return 0.0;
}

std::string NFCLuaScriptModule::GetElePropertyString(const std::string & strConfigName, const std::string & strPropertyName)
{
	return "";
}

NFVector2 NFCLuaScriptModule::GetElePropertyVector2(const std::string & strConfigName, const std::string & strPropertyName)
{
	return NFVector2();
}

NFVector3 NFCLuaScriptModule::GetElePropertyVector3(const std::string & strConfigName, const std::string & strPropertyName)
{
	return NFVector3();
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
                    LuaIntf::LuaRef func(mLuaContext, funcName.c_str());
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
	LuaIntf::LuaBinding(mLuaContext).beginClass<NFGUID>("NFGUID")
		.addConstructor(LUA_ARGS())
		.addProperty("data", &NFGUID::GetData, &NFGUID::SetData)
		.addProperty("head", &NFGUID::GetHead, &NFGUID::SetHead)
		.addFunction("tostring", &NFGUID::ToString)
		.addFunction("fromstring", &NFGUID::FromString)
		.endClass();

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFDataList>("NFDataList")
		.endClass();

	LuaIntf::LuaBinding(mLuaContext).beginExtendClass<NFDataList, NFDataList>("NFDataList")
		.addConstructor(LUA_ARGS())
		.addFunction("empty", &NFDataList::IsEmpty)
		.addFunction("count", &NFDataList::GetCount)
		.addFunction("tye", &NFDataList::Type)
		.addFunction("add_int", &NFDataList::AddInt)
		.addFunction("add_float", &NFDataList::AddFloat)
		.addFunction("add_string", &NFDataList::AddStringFromChar)
		.addFunction("add_object", &NFDataList::AddObject)
		.addFunction("add_vector2", &NFDataList::AddVector2)
		.addFunction("add_vector3", &NFDataList::AddVector3)
		.addFunction("set_int", &NFDataList::SetInt)
		.addFunction("set_float", &NFDataList::SetFloat)
		.addFunction("set_string", &NFDataList::SetString)
		.addFunction("set_object", &NFDataList::SetObject)
		.addFunction("set_vector2", &NFDataList::SetVector2)
		.addFunction("set_vector3", &NFDataList::SetVector3)
		.addFunction("int", &NFDataList::Int)
		.addFunction("float", &NFDataList::Float)
		.addFunction("string", &NFDataList::String)
		.addFunction("object", &NFDataList::Object)
		.addFunction("vector2", &NFDataList::Vector2)
		.addFunction("vector3", &NFDataList::Vector3)
		.endClass();

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFData>("TData")
		.addConstructor(LUA_ARGS())
		.addFunction("float", &NFData::GetFloat)
		.addFunction("int", &NFData::GetInt)
		.addFunction("object", &NFData::GetObject)
		.addFunction("string", &NFData::GetString)
		.addFunction("vector2", &NFData::GetVector2)
		.addFunction("vector3", &NFData::GetVector3)
		.addFunction("type", &NFData::GetType)
		.addFunction("is_null", &NFData::IsNullValue)
		.addFunction("set_float", &NFData::SetFloat)
		.addFunction("set_int", &NFData::SetInt)
		.addFunction("set_object", &NFData::SetObject)
		.addFunction("set_string", &NFData::SetString)
		.addFunction("set_vector2", &NFData::SetVector2)
		.addFunction("set_vector3", &NFData::SetVector3)
		.endClass();

	LuaIntf::LuaBinding(mLuaContext).beginClass<NFCLuaScriptModule>("NFCLuaScriptModule")

		//for kernel module

		//create_object
		//exist_object
		//destroy_object
		//create_scene
		//exist_scene
		//destroy_scene
		//enter_scene
		//leave_scene

		//do_event


		.addFunction("set_prop_int", &NFCLuaScriptModule::SetPropertyInt)
		.addFunction("set_prop_float", &NFCLuaScriptModule::SetPropertyFloat)
		.addFunction("set_prop_string", &NFCLuaScriptModule::SetPropertyString)
		.addFunction("set_prop_object", &NFCLuaScriptModule::SetPropertyObject)
		.addFunction("set_prop_vector2", &NFCLuaScriptModule::SetPropertyVector2)
		.addFunction("set_prop_vector3", &NFCLuaScriptModule::SetPropertyVector3)

		.addFunction("get_prop_int", &NFCLuaScriptModule::GetPropertyInt)
		.addFunction("get_prop_float", &NFCLuaScriptModule::GetPropertyFloat)
		.addFunction("get_prop_string", &NFCLuaScriptModule::GetPropertyString)
		.addFunction("get_prop_object", &NFCLuaScriptModule::GetPropertyObject)
		.addFunction("get_prop_vector2", &NFCLuaScriptModule::GetPropertyVector2)
		.addFunction("get_prop_vector3", &NFCLuaScriptModule::GetPropertyVector3)

		.addFunction("set_record_int", &NFCLuaScriptModule::SetRecordInt)
		.addFunction("set_record_float",&NFCLuaScriptModule::SetRecordFloat)
		.addFunction("set_record_string", &NFCLuaScriptModule::SetRecordString)
		.addFunction("set_record_object", &NFCLuaScriptModule::SetRecordObject)
		.addFunction("set_record_vector2", &NFCLuaScriptModule::SetPropertyVector2)
		.addFunction("set_record_vector3", &NFCLuaScriptModule::SetPropertyVector3)
		
		.addFunction("get_record_int", &NFCLuaScriptModule::GetRecordInt)
		.addFunction("get_record_float", &NFCLuaScriptModule::GetRecordFloat)
		.addFunction("get_record_string", &NFCLuaScriptModule::GetRecordString)
		.addFunction("get_record_object", &NFCLuaScriptModule::GetRecordObject)
		.addFunction("get_record_vector2", &NFCLuaScriptModule::GetPropertyVector2)
		.addFunction("get_record_vector3", &NFCLuaScriptModule::GetPropertyVector3)

		.addFunction("add_prop_cb", &NFCLuaScriptModule::AddPropertyCallBack)
		.addFunction("add_record_cb", &NFCLuaScriptModule::AddRecordCallBack)
		.addFunction("add_event_cb", &NFCLuaScriptModule::AddEventCallBack)
		.addFunction("add_class_cb", &NFCLuaScriptModule::AddClassCallBack)
		.addFunction("add_schedule", &NFCLuaScriptModule::AddSchedule)
		.addFunction("do_event", &NFCLuaScriptModule::AddSchedule)
		//do_event
		.addFunction("add_row", &NFCLuaScriptModule::AddRow)
		.addFunction("rem_row", &NFCLuaScriptModule::RemRow)

		.addFunction("time", &NFCLuaScriptModule::GetNowTime)
		.addFunction("new_id", &NFCLuaScriptModule::CreateID)

		//for element module
		.addFunction("get_int", &NFCLuaScriptModule::GetElePropertyInt)
		.addFunction("get_float", &NFCLuaScriptModule::GetElePropertyFloat)
		.addFunction("get_string", &NFCLuaScriptModule::GetElePropertyString)
		.addFunction("get_vector2", &NFCLuaScriptModule::GetElePropertyVector2)
		.addFunction("get_vector3", &NFCLuaScriptModule::GetElePropertyVector3)

		//for class module


		//for net module
		/*
		.addFunction("add_msg_cb", &NFCLuaScriptModule::GetElePropertyVector3)
		.addFunction("send_msg", &NFCLuaScriptModule::GetElePropertyVector3)
		*/

		//for log module
		/*
		.addFunction("log_info", &NFCLuaScriptModule::GetElePropertyVector3)
		.addFunction("log_error", &NFCLuaScriptModule::GetElePropertyVector3)
		.addFunction("log_warning", &NFCLuaScriptModule::GetElePropertyVector3)
		.addFunction("log_debug", &NFCLuaScriptModule::GetElePropertyVector3)
		*/

		//for actor

		//for nosql
		/*
		.addFunction("save_data", &NFCLuaScriptModule::GetElePropertyVector3)
		*/


		.endClass();

    return true;
}
