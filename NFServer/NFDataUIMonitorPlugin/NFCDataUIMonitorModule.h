// -------------------------------------------------------------------------
//    @FileName      :    NFCDataUIMonitorModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataUIMonitorModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_DATAUIMONITOR_MODULE_H_
#define _NFC_DATAUIMONITOR_MODULE_H_

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIDataUIMonitorModule.h"

class NFCDataUIMonitorModule
    : public NFIDataUIMonitorModule
{
public:
    virtual bool Init();
    virtual bool Shut();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);


    static NFIDataUIMonitorModule* m_pThis;
    static NFIKernelModule* m_pKernelModule;
    static NFIEventProcessModule* m_pEventProcessModule;
};

#endif