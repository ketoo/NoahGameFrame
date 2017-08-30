//
// Created by LUSHENG HUANG on 30/08/17.
//

#ifndef NOAHFRAME_NFCSWITCHGAMESERVERMODULE_H
#define NOAHFRAME_NFCSWITCHGAMESERVERMODULE_H

#include "NFComm/NFPluginModule/NFISwitchGameServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"

class NFCSwitchGameServerModule
    : public NFISwitchGameServerModule
{
public:
    NFCSwitchGameServerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool SwitchServer(const NFGUID &self, const int nServer, const int nSceneID, const int nGroup);

protected:

    void OnClientReqSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnReqSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
    void OnAckSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

private:
    NFINetModule*				    m_pNetModule;
    NFIKernelModule*			    m_pKernelModule;
    NFILogModule*				    m_pLogModule;
    NFIElementModule*			    m_pElementModule;
    NFISceneProcessModule*          m_pSceneProcessModule;
    NFIPropertyModule*              m_pPropertyModule;
    NFINetClientModule*             m_pNetClientModule;
    NFIPackModule*                  m_pPackModule;
    NFISceneAOIModule*              m_pSceneAOIModule;
    NFIGameServerNet_ServerModule*  m_pGameServerNet_ServerModule;
};


#endif //NOAHFRAME_NFCSWITCHGAMESERVERMODULE_H
