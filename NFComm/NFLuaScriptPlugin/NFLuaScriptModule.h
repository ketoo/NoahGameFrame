/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#ifndef NF_LUA_SCRIPT_MODULE_H
#define NF_LUA_SCRIPT_MODULE_H

#ifdef min
#undef min
#endif

//just define it as 0 if you want to use luaintf with C
//#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0

#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFLuaPBModule.h"
/*
void call0(lua_State* lua_state,const char* name)
{
	lua_getglobal(lua_state, name);
	if (lua_isfunction(lua_state, -1))
	{
		lua_pcall(lua_state, 0, 0, 0);
	}
}

template<typename T1>
void call1(lua_State* lua_state,const char* name, T1 arg1)
{
	lua_getglobal(lua_state, name);
	if (lua_isfunction(lua_state, -1))
	{
		luabridge::Stack<T1>::push(lua_state, arg1);
		lua_pcall(lua_state, 1, 0, 0);
	}
}

template<typename T1, typename T2>
void call2(lua_State* lua_state,const char* name, T1 arg1, T2 arg2)
{
	lua_getglobal(lua_state, name);
	if (lua_isfunction(lua_state, -1))
	{
		luabridge::Stack<T1>::push(lua_state, arg1);
		luabridge::Stack<T2>::push(lua_state, arg2);
		lua_pcall(lua_state, 2, 0, 0);
	}
}
*/

class NFILuaScriptModule
		: public NFIModule
{
public:

};

class NFLuaScriptModule
    : public NFILuaScriptModule
{
public:
    NFLuaScriptModule(NFIPluginManager* p)
    {
        m_bIsExecute = true;
        pPluginManager = p;
    }

	virtual bool Awake();
	virtual bool Init();
    virtual bool Shut();
	virtual bool ReadyExecute();
	virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();


protected:
	void RegisterModule(const std::string& tableName, const LuaIntf::LuaRef& luaTable);

	//FOR KERNEL MODULE
	NFGUID CreateObject(const NFGUID& self, const int sceneID, const int groupID, const std::string& className, const std::string& objectIndex, const NFDataList& arg);
	bool ExistObject(const NFGUID& self);
	bool DestroyObject(const NFGUID & self);

	//return the group id
	bool EnterScene(const int sceneID, const int groupID);
	bool DoEvent(const NFGUID& self, const int eventID, const NFDataList& arg);

	bool FindProperty(const NFGUID& self, const std::string& propertyName);

	bool SetPropertyInt(const NFGUID& self, const std::string& propertyName, const NFINT64 propValue);
	bool SetPropertyFloat(const NFGUID& self, const std::string& propertyName, const double propValue);
	bool SetPropertyString(const NFGUID& self, const std::string& propertyName, const std::string& propValue);
	bool SetPropertyObject(const NFGUID& self, const std::string& propertyName, const NFGUID& propValue);
	bool SetPropertyVector2(const NFGUID& self, const std::string& propertyName, const NFVector2& propValue);
	bool SetPropertyVector3(const NFGUID& self, const std::string& propertyName, const NFVector3& propValue);

	NFINT64 GetPropertyInt(const NFGUID& self, const std::string& propertyName);
	int GetPropertyInt32(const NFGUID& self, const std::string& propertyName);	//equal to (int)GetPropertyInt(...), to remove C4244 warning
	double GetPropertyFloat(const NFGUID& self, const std::string& propertyName);
	std::string GetPropertyString(const NFGUID& self, const std::string& propertyName);
	NFGUID GetPropertyObject(const NFGUID& self, const std::string& propertyName);
	NFVector2 GetPropertyVector2(const NFGUID& self, const std::string& propertyName);
	NFVector3 GetPropertyVector3(const NFGUID& self, const std::string& propertyName);

	bool AddPropertyCallBack(const NFGUID& self, std::string& propertyName, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);
    bool AddRecordCallBack(const NFGUID& self, std::string& recordName, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);
    bool AddEventCallBack(const NFGUID& self, const int eventID, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);
	bool AddSchedule(const NFGUID& self, std::string& strHeartBeatName, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc, const float time, const int count);
	bool AddModuleSchedule(std::string& strHeartBeatName, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc, const float time, const int count);

	int AddRow(const NFGUID& self, std::string& recordName, const NFDataList& var);
	bool RemRow(const NFGUID& self, std::string& recordName, const int row);

	bool SetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value);
	bool SetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const double value);
	bool SetRecordString(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const std::string& value);
	bool SetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value);
	bool SetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value);
	bool SetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value);

	NFINT64 GetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
	double GetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
	std::string GetRecordString(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
	NFGUID GetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
	NFVector2 GetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
	NFVector3 GetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);

	NFINT64 GetNowTime();
	NFGUID CreateId();
	NFINT64 APPId();
	NFINT64 APPType();

	//FOR ELEMENT MODULE
	bool ExistElementObject(const std::string& configName);
	std::vector<std::string> GetEleList(const std::string& className);

	NFINT64 GetElePropertyInt(const std::string& configName, const std::string& propertyName);
	double GetElePropertyFloat(const std::string& configName, const std::string& propertyName);
	std::string GetElePropertyString(const std::string& configName, const std::string& propertyName);
	NFVector2 GetElePropertyVector2(const std::string& configName, const std::string& propertyName);
	NFVector3 GetElePropertyVector3(const std::string& configName, const std::string& propertyName);

	//FOR NET MODULE
	//as server
	void RemoveCallBackAsServer(const int msgID);
	void AddMsgCallBackAsServer(const int msgID, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);

	//as client
	void RemoveMsgCallBackAsClient(const NF_SERVER_TYPES serverType, const int msgID);
	void AddMsgCallBackAsClient(const NF_SERVER_TYPES serverType, const int msgID, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);

/*
	void RemoveHttpCallBack(const std::string& path);
	void AddHttpCallBack(const std::string& path, const int httpType, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);
*/

    void ImportProtoFile(const std::string& fileName);
    const std::string Encode(const std::string& msgTypeName, const LuaIntf::LuaRef& luaTable);
	LuaIntf::LuaRef Decode(const std::string& msgTypeName, const std::string& data);

	void SendToServerByServerID(const int serverID, const uint16_t msgID, const std::string& data);
	void SendToServerBySuit(const NF_SERVER_TYPES eType, const uint16_t msgID, const std::string& data, const std::string& hash);
	void SendToAllServerByServerType(const NF_SERVER_TYPES eType, const uint16_t msgID, const std::string& data);

    //for net module
	void SendMsgToClientByFD(const NFSOCK fd, const uint16_t msgID, const std::string& data);

	void SendMsgToPlayer(const NFGUID player, const uint16_t msgID, const std::string& data);
	void SendToAllPlayer(const uint16_t msgID, const std::string& data);
    void SendToGroupPlayer(const uint16_t msgID, const std::string& data);

	//for log
	void LogInfo(const std::string& logData);
	void LogError(const std::string& logData);
	void LogWarning(const std::string& logData);
	void LogDebug(const std::string& logData);

    //hot fix
	void SetVersionCode(const std::string& logData);
	const std::string& GetVersionCode();

	//FOR CLASS MODULE
    bool AddClassCallBack(std::string& className, const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);

protected:
    template<typename T>
    bool AddLuaFuncToMap(NFMap<T, NFMap<NFGUID, NFList<string>>>& funcMap, const NFGUID& self, T key, std::string& luaFunc);

    template<typename T>
    bool AddLuaFuncToMap(NFMap<T, NFMap<NFGUID, NFList<string>>>& funcMap, T key, std::string& luaFunc);

    int OnLuaPropertyCB(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar, const NFINT64 reason);
    int OnLuaRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);
    int OnLuaHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float time, const int count);

    int OnLuaEventCB(const NFGUID& self, const int eventID, const NFDataList& argVar);

    int OnClassEventCB(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var);
    
	void OnScriptReload();

	void OnNetMsgCallBackAsServer(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnNetMsgCallBackAsClientForMasterServer(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnNetMsgCallBackAsClientForWorldServer(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnNetMsgCallBackAsClientForGameServer(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

protected:
    bool Register();
	std::string FindFuncName(const LuaIntf::LuaRef& luaTable, const LuaIntf::LuaRef& luaFunc);


protected:
    NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
	NFIEventModule* m_pEventModule;
    NFIScheduleModule* m_pScheduleModule;
    NFINetClientModule* m_pNetClientModule;
    NFINetModule* m_pNetModule;
    NFILogModule* m_pLogModule;
	NFILuaPBModule* m_pLuaPBModule;
	
protected:
    int64_t mnTime;
    std::string strVersionCode;
    LuaIntf::LuaContext mLuaContext;

	std::map<std::string, LuaIntf::LuaRef> mxTableName;

    NFMap<std::string, NFMap<NFGUID, NFList<std::string>>> mxLuaPropertyCallBackFuncMap;
    NFMap<std::string, NFMap<NFGUID, NFList<std::string>>> mxLuaRecordCallBackFuncMap;
    NFMap<int, NFMap<NFGUID, NFList<std::string>>> mxLuaEventCallBackFuncMap;
    NFMap<std::string, NFMap<NFGUID, NFList<std::string>>> mxLuaHeartBeatCallBackFuncMap;

    NFMap<std::string, NFList<std::string>> mxClassEventFuncMap;


	NFMap<int, NFList<std::string>> mxNetMsgCallBackFuncMapAsServer;
	NFMap<NF_SERVER_TYPES, NFMap<int, NFList<std::string>>> mxNetMsgCallBackFuncMapAsClient;
};

#endif
