// -------------------------------------------------------------------------
//    @FileName         :   NFCNPCRefreshModule.h
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-10-17
//    @Module               :    NFCNPCRefreshModule
//    @Desc                 :    NPCËÀÍöË¢ÐÂ
// -------------------------------------------------------------------------

#ifndef NFC_NPC_REFRESH_MODULE_H
#define NFC_NPC_REFRESH_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINPCRefreshModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIHeroPropertyModule.h"

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
    int OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    int OnObjectHPEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
	int OnObjectBeKilled(const NFGUID& self, const int nEventID, const NFIDataList& var);

    int OnDeadDestroyHeart( const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);
private:

    NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPackModule* m_pPackModule;
	NFILogModule* m_pLogModule;
	NFILevelModule* m_pLevelModule;
	NFIHeroPropertyModule* m_pHeroPropertyModule;

};


#endif
