// -------------------------------------------------------------------------
//    @FileName      :    NFCMonoScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCMonoScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_MONO_SCRIPT_MODULE_H_
#define _NFC_MONO_SCRIPT_MODULE_H_

#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIScriptModule.h"

#include "mono/mini/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/metadata.h"
#include "mono/metadata/assembly.h"

class NFCMonoScriptModule
    : public NFIScriptModule
{
public:
    NFCMonoScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

    virtual int DoScript(const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg);
    virtual int DoEventScript(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg);
    virtual int DoHeartBeatScript(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg);


    virtual int DoScriptPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& neVar, const NFCScriptVarList& arg);
    virtual int DoScriptRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg);

    virtual int DoHeartBeatCommonCB(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var);
    virtual int DoEventCommonCB(const NFIDENTID& self, const int nEventID, const NFIDataList& var);

protected:


    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif