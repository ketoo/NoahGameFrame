// -------------------------------------------------------------------------
//    @FileName      :    NFCSLGModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCSLGModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_SLG_MODULE_H_
#define _NFC_SLG_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFISLGModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"

class NFCSLGModule
    : public NFISLGModule
{
public:
    NFCSLGModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCSLGModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );

    virtual bool AfterInit();
    virtual bool BeforeShut();


protected:


protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFIDataNoSqlModule* m_pNoSqlModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif
