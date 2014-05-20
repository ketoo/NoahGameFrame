// -------------------------------------------------------------------------
//    @FileName      :    NFCClientLogictModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCClientLogictModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_CLIENTLOGIN_MODULE_H_
#define _NFC_CLIENTLOGIN_MODULE_H_

#include "NFComm/NFPluginModule/NFIClientLogicModule.h"
#include "NFComm/NFPluginModule/NFIClientNetModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFIEventProcessModule;
class NFIClientNetModule;

class NFCClientLogictModule
    : public NFIClientLogicModule
{
public:
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);


private:
    static NFIKernelModule* m_pKernelModule;
    static NFIEventProcessModule* m_pEventProcessModule;
    static NFIClientNetModule* m_pClientNetModule;
    static NFCClientLogictModule* m_pThis;
};

#endif