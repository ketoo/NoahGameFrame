// -------------------------------------------------------------------------
//    @FileName      :    NFCPythonScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCPythonScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_PYTHON_SCRIPT_MODULE_H_
#define _NFC_PYTHON_SCRIPT_MODULE_H_

//#include <boost/python.hpp>
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIPythonScriptModule.h"

class NFCPythonScriptModule
    : public NFIPythonScriptModule
{
public:
    NFCPythonScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
        m_pScriptKernelModule = NULL;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();
    
    //call script
    virtual int DoScript(const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg);
    virtual int DoEventScript(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg);
    virtual int DoHeartBeatScript(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg);


    virtual int DoScriptPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& neVar, const NFCScriptVarList& arg);
    virtual int DoScriptRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg);

protected:

    virtual int OnPropertyCommEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& arg);
    virtual int OnRecordCommonEvent(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& arg);
    virtual int OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);
    
    virtual int OnHeartBeatCommonCB(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var);
    virtual int OnEventCommonCB(const NFIDENTID& self, const int nEventID, const NFIValueList& var);


protected:

    NFCScriptKernelModule* m_pScriptKernelModule;

    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif