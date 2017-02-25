// -------------------------------------------------------------------------
//    @FileName				:   NFCNPCRefreshModule.h
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-10-17
//    @Module               :    NFCNPCRefreshModule

// -------------------------------------------------------------------------

#ifndef NFC_NPC_REFRESH_MODULE_H
#define NFC_NPC_REFRESH_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINPCRefreshModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIHeroPropertyModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

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
    virtual bool Execute();
    virtual bool AfterInit();

protected:
    int OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var );

    int OnObjectHPEvent( const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	int OnObjectBeKilled(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& var);

    int OnDeadDestroyHeart( const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);
private:
	NFIEventModule* m_pEventModule;
	NFIScheduleModule* m_pScheduleModule;
    NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
    NFISceneProcessModule* m_pSceneProcessModule;
	NFILogModule* m_pLogModule;
	NFILevelModule* m_pLevelModule;

};


#endif
