// -------------------------------------------------------------------------
//    @FileName      :    NFCLevelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelModule
//
// -------------------------------------------------------------------------

#ifndef NFC_LEVEL_MODULE_H
#define NFC_LEVEL_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFCLevelModule
    : public NFILevelModule
{
public:
    NFCLevelModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCLevelModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual int AddExp( const NFGUID& self, const int nExp);

    virtual int OnKillObject( const NFGUID& self, const NFGUID& other );

    virtual int OnDead( const NFGUID& self, const NFGUID& other );

protected:

    int OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    int OnObjectLevelEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar );
    int OnObjectExpEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar );

    int OnObjectBeKilled( const NFGUID& object, const int nEventID, const NFIDataList& var );
    bool AddLevelUpAward( const NFGUID& self, const int nLevel );
private:
    NFIPropertyConfigModule* m_pPropertyConfigModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
};

#endif
