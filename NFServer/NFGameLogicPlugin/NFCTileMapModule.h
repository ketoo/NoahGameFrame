// -------------------------------------------------------------------------
//    @FileName			:    NFCTileMapModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-25
//    @Module           :    NFCTileMapModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TILE_MAP_MODULE_H
#define NFC_TILE_MAP_MODULE_H

#include "NFComm/NFPluginModule/NFITileMapModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"

class NFCTileMapModule
    : public NFITileMapModule
{
public:
	NFCTileMapModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFCTileModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	/*
	 * if you stay at your home, than you can use your home scene data generate the map
	 * but, if you are attacking the others, you need to use that player's home scene to build the map for you
	 * the reason is now you are walking in that player's tile map
	 */
	virtual bool GenerateTileMap(const NFGUID self, const NFGUID other, const int nSceneID);
	virtual bool ReleaseTileMap(const NFGUID self);

protected:

	int BeforeEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int BeforeLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	void LoadStaticTileData(const std::string& strSceneIDName);


private:

	struct TilePosition
	{
		int x;
		int y;
	};

	NFMapEx<NFGUID, std::map<TilePosition, bool> > mxPlayerTileData;
	NFMapEx<int, std::map<TilePosition, bool> > mxStaticTileData;


private:
	NFINetModule* m_pNetModule;
	NFIBigMapRedisModule* m_pBigMapRedisModule;
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
	NFISceneAOIModule* m_pSceneAOIModule;
	NFIGuildRedisModule* m_pGuildRedisModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
