// -------------------------------------------------------------------------
//    @FileName			:    NFCLuaScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_LUA_SCRIPT_MODULE_H
#define NFC_LUA_SCRIPT_MODULE_H

//just define it as 0 if you want to use luaintf with C
#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 1

#include "Dependencies/LuaIntf/LuaIntf.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"

class NFCLuaScriptModule
    : public NFILuaScriptModule
{
public:
    NFCLuaScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

    bool AddPropertyCallBack(const NFGUID& self, std::string& strPropertyName, std::string& luaFunc);
    bool AddRecordCallBack(const NFGUID& self, std::string& strRecordName, std::string& luaFunc);
    bool AddEventCallBack(const NFGUID& self, const NFEventDefine nEventID, std::string& luaFunc);
    bool AddHeartBeat(const NFGUID& self, std::string& strHeartBeatName, std::string& luaFunc, const float fTime, const int nCount);
    int AddRow(const NFGUID& self, std::string& strRecordName, const NFDataList& var);

    bool AddClassCallBack(std::string& className, std::string& funcName);

protected:
    template<typename T>
    bool AddLuaFuncToMap(NFMap<T, NFMap<NFGUID, NFList<string>>>& funcMap, const NFGUID& self, T key, std::string& luaFunc);
    template<typename T1, typename... T2>
    bool CallLuaFuncFromMap(NFMap<T1, NFMap<NFGUID, NFList<string>>>& funcMap, T1 key, const NFGUID& self, T2... arg);

    int OnLuaPropertyCB(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
    int OnLuaRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);
    int OnLuaHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float fTime, const int nCount);
    int OnLuaEventCB(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& argVar);

    int OnClassEventCB(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

protected:
    bool Regisger();

protected:
    NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pLogicClassModule;
	NFIEventModule* m_pEventModule;
	NFIScheduleModule* m_pScheduleModule;

protected:
    LuaIntf::LuaContext mLuaContext;
    int64_t mnTime;
    NFMap<std::string, NFMap<NFGUID, NFList<std::string>>> m_luaPropertyCallBackFuncMap;
    NFMap<std::string, NFMap<NFGUID, NFList<std::string>>> m_luaRecordCallBackFuncMap;
    NFMap<int, NFMap<NFGUID, NFList<std::string>>> m_luaEventCallBackFuncMap;
    NFMap<std::string, NFMap<NFGUID, NFList<std::string>>> m_luaHeartBeatCallBackFuncMap;

    NFMap<std::string, std::string> m_ClassEventFuncMap;
};

#endif
