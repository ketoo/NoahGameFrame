// -------------------------------------------------------------------------
//    @FileName      :    NFCClientRenderModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCClientRenderModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_CLIENTRENDER_MODULE_H_
#define _NFC_CLIENTRENDER_MODULE_H_

#include "NFComm/NFPluginModule/NFIClientRenderModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"

typedef std::map<std::string, void*> FunctionPointMap;

class NFCClientRenderModule
    : public NFIClientRenderModule
{
public:
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

    //virtual bool RegisterFunction( char* strFuncName, void* pFunc );

protected:

private:

    FunctionPointMap mtFuncPoint;

private:
    static NFIEventProcessModule* m_pEventProcessModule;
    static NFCClientRenderModule* m_pThis;
};

#endif