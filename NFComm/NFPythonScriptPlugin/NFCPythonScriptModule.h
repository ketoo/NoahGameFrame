// -------------------------------------------------------------------------
//    @FileName         :    NFCPythonScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCPythonScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_PYTHON_SCRIPT_MODULE_H_
#define _NFC_PYTHON_SCRIPT_MODULE_H_

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIPythonScriptModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCPythonScriptModule
    : public NFIPythonScriptModule
{
public:
    NFCPythonScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();
    
    //call script
    virtual int DoScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg);
    virtual int DoEventScript(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg);
    virtual int DoHeartBeatScript(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction);
    virtual int DoScriptPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFIDataList::TData& oldVar, const NFIDataList::TData& neVar);
    virtual int DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

    virtual int DoClassCommonScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction);
protected:
    int OnPropertyCommEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnRecordCommonEvent(const NFGUID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

protected:
    NFIElementInfoModule* m_pElementInfoModule;
    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
};

#endif