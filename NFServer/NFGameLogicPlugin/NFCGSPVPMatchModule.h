// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPMatchModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2015-01-02
//    @Module           :    NFCPVPMatchModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GS_PVP_MATCH_MODULE_H
#define NFC_GS_PVP_MATCH_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIPVPMatchRedisModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFITeamModule.h"
#include "NFComm/NFPluginModule/NFIGSPVPMatchModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGSSwichServerModule.h"

class NFCGSPVPMatchModule
    : public NFIGSPVPMatchModule
{
public:
    NFCGSPVPMatchModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFCGSPVPMatchModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
	virtual bool AfterInit();

protected:
	void OnReqPVPMatchProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAckPVPMatchProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqCreatePVPEctyProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAckCreatePVPEctyProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIGameServerToWorldModule* m_pGameServerToWorldModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIGSSwichServerModule* m_pGSSwitchServerModule;
};

#endif
