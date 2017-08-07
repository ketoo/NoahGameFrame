// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-02
//    @Module           :    NFCPVPModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PVP_MODULE_H
#define NFC_PVP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPVPModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFITileModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCPVPModule
    : public NFIPVPModule
{
public:
    NFCPVPModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFCPVPModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();
	virtual bool ReadyExecute();

	virtual int RandomTileScene();

protected:

	void OnReqSearchOppnentProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqSwapHomeSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqStartPVPOppnentProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqEndPVPOppnentProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	void FindAllTileScene();
	void InitAllTileSceneRobot();

	int OnSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	int EnterSceneConditionEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	int BeforeEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int BeforeLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

private:
	NFITileModule* m_pTileModule;
	NFIPropertyModule* m_pPropertyModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFISceneAOIModule* m_pSceneAOIModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;

	std::vector<int> mxTileSceneIDList;
};


#endif
