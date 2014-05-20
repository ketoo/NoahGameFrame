// -------------------------------------------------------------------------
//    @FileName         :   NFCNPCRefreshModule.h
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-10-17
//    @Module               :    NFCNPCRefreshModule
//    @Desc                 :    NPCËÀÍöË¢ÐÂ
// -------------------------------------------------------------------------

#ifndef _NFC_NPC_REFRESH_MODULE_H_
#define _NFC_NPC_REFRESH_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINPCRefreshModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"

class NFCNPCRefreshModule
    : public NFINPCRefreshModule
{
public:
    NFCNPCRefreshModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCNPCRefreshModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

protected:
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var );

    int OnObjectHPEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

    int OnDeadDestroyHeart( const NFIDENTID& self, const NFIValueList& var );
private:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIKernelModule* m_pKernelModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPackModule* m_pPackModule;

};


#endif
