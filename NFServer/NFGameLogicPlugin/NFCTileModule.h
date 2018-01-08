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
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIPVPModule.h"

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

	virtual bool GetOnlinePlayerTileData(const NFGUID& self, std::string& strData);

protected:

	void OnReqMineTileProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

	bool AddTile(const NFGUID& self, const int nX, const int nY, const int nOpr);
	bool AddBuilding(const NFGUID& self, const int nX, const int nY, const NFGUID& id, const std::string& strCnfID);
	bool AddNPC(const NFGUID& self, const int nX, const int nY, const NFGUID& id, const std::string& strCnfID);

	bool RemoveTile(const NFGUID& self, const int nX, const int nY);
	bool RemoveBuilding(const NFGUID& self, const int nX, const int nY, const NFGUID& id);
	bool RemoveNPC(const NFGUID& self, const int nX, const int nY, const NFGUID& id);

	bool SaveTileData(const NFGUID& self);
	bool LoadTileData(const NFGUID& self, const int nSceneID);

	bool SendTileData(const NFGUID& self);
	bool CreateTileBuilding(const NFGUID& self);

	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

	int BeforeEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int BeforeLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);


protected:
	struct TileState
	{
		int x;
		int y;
		int state;
	};
	struct TileBuilding
	{
		int x;
		int y;
		std::string configID;
		NFGUID ID;
	};
	struct TileNPC
	{
		int x;
		int y;
		std::string configID;
		NFGUID ID;
	};
	struct TileData
	{
		//x,y,state
		NFMapEx<int, NFMapEx<int, TileState>> mxTileState;

		//x,y,building
		NFMapEx<int, NFMapEx<int, TileBuilding>> mxTileBuilding;

		//x,y,npc
		NFMapEx<int, NFMapEx<int, TileNPC>> mxTileNPC;
	};

	NFMapEx<NFGUID, TileData> mxTileData;

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
