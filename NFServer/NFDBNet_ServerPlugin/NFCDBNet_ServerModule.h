// -------------------------------------------------------------------------
//    @FileName			:    NFCDBNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDBNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_WORLDNET_SERVER_MODULE_H
#define NFC_WORLDNET_SERVER_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIAccountRedisModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"

class NFIDBNet_ServerModule : public NFIModule
{

};

class NFCDBNet_ServerModule
    : public NFIDBNet_ServerModule
{
public:
    NFCDBNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

protected:

    void OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	void OnClientDisconnect(const NFSOCK nAddress);
	void OnClientConnected(const NFSOCK nAddress);


	void OnRequireRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnLoadRoleDataProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSaveRoleDataProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


protected:
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFIAccountRedisModule* m_pAccountRedisModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFINetClientModule* m_pNetClientModule;
};

#endif
