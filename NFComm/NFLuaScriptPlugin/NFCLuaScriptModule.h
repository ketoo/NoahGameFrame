// -------------------------------------------------------------------------
//    @FileName      :    NFCLuaScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_LUA_SCRIPT_MODULE_H_
#define _NFC_LUA_SCRIPT_MODULE_H_

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIScriptModule.h"

class NFCLuaScriptModule
    : public NFILuaScriptModule
{
public:
    NFCLuaScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
        mpLuaState = NULL;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();

    //call script
    virtual int DoScript(const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg);
    virtual int DoEventScript(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg);
    virtual int DoHeartBeatScript(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction);

    virtual int DoScriptPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& oldVar, const NFCDataList& neVar);
    virtual int DoScriptRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCDataList& oldVar, const NFCDataList& newVar);

protected:

    int OnPropertyCommEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar);
    int OnRecordCommonEvent(const NFIDENTID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar);
    int OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);    

protected:
    bool InstallLua(const std::string& strComponentName);
    bool Regisger(); 
    bool CheckCompomentStatus(const std::string& strComponentName, const std::string& strFuncName);
    bool CheckCompomentStatus(const std::string& strComponentName);

    void LuaInit(NFIKernelModule* pKernel, const NFIDENTID& self,const NFIDataList& arg);

protected:

    //NFIScriptKernelModule* m_pScriptKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;

protected:
    NFMap<std::string, int> mmCompomentStatus;//Ω≈±æ «∑Òº”‘ÿ
    lua_State *mpLuaState;
};

#endif