// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCWorldLogicModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_WORLDLOGIC_MODULE_H_
#define _NFC_WORLDLOGIC_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"

class NFCWorldLogicModule
    : public NFIWorldLogicModule
{
public:
    NFCWorldLogicModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

//     virtual int GetWorldID();
//     virtual const char* GetWorldName();

protected:
// 
//     int mnWorldID;
//     std::string mstrWorldName;

protected:
    int mnWorldContranerID;

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif