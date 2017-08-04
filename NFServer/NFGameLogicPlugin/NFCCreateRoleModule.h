// -------------------------------------------------------------------------
//    @FileName			:    NFCCreateRoleModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-03-29
//    @Module           :    NFCCreateRoleModule
//
// -------------------------------------------------------------------------

#ifndef NFC_CREATEROLE_MODULE_H
#define NFC_CREATEROLE_MODULE_H

#include "NFComm/NFCore/NFConsistentHash.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFICreateRoleModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFIAccountRedisModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPVPModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCCreateRoleModule
    : public NFICreateRoleModule
{
public:
    NFCCreateRoleModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCCreateRoleModule() {};

    virtual bool Init();
    virtual bool Shut();
	virtual bool ReadyExecute();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	void OnReqiureRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClienEnterGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFIPVPModule* m_pPVPModule;
	NFINetModule* m_pNetModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFIAccountRedisModule* m_pAccountRedisModule;
	NFISceneAOIModule* m_pSceneAOIModule;
	NFISceneProcessModule* m_pSceneProcessModule;
};

#endif