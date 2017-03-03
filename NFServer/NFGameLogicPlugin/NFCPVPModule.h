// -------------------------------------------------------------------------
//    @FileName         :    NFCPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
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

	virtual int RandomTileScene();

protected:

	void OnSearchOppnent(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	void FindAllTileScene();
	void InitAllTileSceneRobot();

private:
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;

	std::vector<int> mxTileSceneIDList;
};


#endif
