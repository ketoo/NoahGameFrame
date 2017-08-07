// -------------------------------------------------------------------------
//    @FileName      :    NFCWSSwichServerModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2013-07-10
//    @Module           :    NFCWSSwichServerModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WSSWITCHSERVER_MODULE_H
#define NFC_WSSWITCHSERVER_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWSSwichServerModule.h"
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

class NFCWSSwichServerModule
    : public NFIWSSwichServerModule
{
public:
    NFCWSSwichServerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCWSSwichServerModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	void OnReqSwichServer( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );
	void OnAckSwichServer( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );

private:
	NFINetModule*				m_pNetModule;
    NFIKernelModule*			m_pKernelModule;
    NFILogModule*				m_pLogModule;
	NFIElementModule*			m_pElementModule;
	NFIWorldNet_ServerModule*	m_pWorlNet_ServerModule;
};


#endif
