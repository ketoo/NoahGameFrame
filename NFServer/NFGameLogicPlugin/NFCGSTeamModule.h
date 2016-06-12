// -------------------------------------------------------------------------
//    @FileName      :    NFCGSTeamModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2013-07-10
//    @Module           :    NFCGSTeamModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GSTEAM_MODULE_H
#define NFC_GSTEAM_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIGSSwichServerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"

class NFCGSTeamModule
    : public NFILogicModule
{
public:
    NFCGSTeamModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGSTeamModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
	virtual bool AfterInit();

protected:

    //from client
    void OnReqCreateTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqJoinTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqLeaveTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqInviteTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqOprmemberTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqTeamEnterEctypeFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //from world Server
    void OnAckCreateTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckJoinTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckLeaveTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckOprMemberTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqTeamEnterEctypeFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckTeamEnterEcypeFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
	NFIGameServerToWorldModule* m_pGameServerToWorldModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFILevelModule* m_pLevelModule;
    NFIPackModule* m_pPackModule;
    NFIHeroModule* m_pHeroModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIGSSwichServerModule* m_pGSSwitchServerModule;
};


#endif
