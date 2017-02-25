// -------------------------------------------------------------------------
//    @FileName			:    NFCTileModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-25
//    @Module           :    NFCTileModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TILE_MODULE_H
#define NFC_TILE_MODULE_H

#include "NFComm/NFPluginModule/NFITileModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCTileModule
    : public NFITileModule
{
public:
	NFCTileModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFCTileModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	void ReqMineTile(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	bool AddTile(const NFGUID& self, const int nX, const int nY);
	bool RemoveTile(const NFGUID& self, const int nX, const int nY);

private:
	NFINetModule* m_pNetModule;
	NFIBigMapRedisModule* m_pBigMapRedisModule;
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
	NFIGuildRedisModule* m_pGuildRedisModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;


};


#endif
