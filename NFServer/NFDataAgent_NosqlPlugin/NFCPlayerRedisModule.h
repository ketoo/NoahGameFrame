// -------------------------------------------------------------------------
//    @FileName			:    NFCPlayerRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPlayerRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_PLAYER_REDIS_MODULE_H
#define NFC_PLAYER_REDIS_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"

class NFCPlayerRedisModule : public NFIPlayerRedisModule
{
public:

	NFCPlayerRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool AfterInit();

	virtual int64_t GetPlayerCacheGameID(const NFGUID& self);
	virtual int64_t GetPlayerCacheProxyID(const NFGUID& self);

	virtual bool GetPlayerCacheGameID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList);
	virtual bool GetPlayerCacheProxyID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList);
	virtual int GetPlayerHomeSceneID(const NFGUID& self);

	virtual bool LoadPlayerData(const NFGUID& self);
	virtual bool SavePlayerData(const NFGUID& self);

	virtual bool SavePlayerTile(const int nSceneID, const NFGUID& self, const std::string& strTileData);
	virtual bool LoadPlayerTile(const int nSceneID, const NFGUID& self, std::string& strTileData);
	virtual bool LoadPlayerTileRandom(const int nSceneID, NFGUID& xPlayer, std::string& strTileData);
	virtual bool LoadPlayerTileRandomCache(const NFGUID& xPlayer, std::string& strTileData);
protected:
	std::string GetOnlineGameServerKey();
	std::string GetOnlineProxyServerKey();

	const bool AttachData(const NFGUID& self);

	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

	void OnOnline(const NFGUID& self);
	void OnOffline(const NFGUID& self);


private:
	struct PlayerDataCache
	{
		PlayerDataCache()
		{
			nLoadTime = NFDateTime::Now().GetSecond();
			nHomeSceneID = 0;
		}
		int nLoadTime;
		int nHomeSceneID;

		NFMsg::ObjectPropertyList xPbPropertyCacheList;
		NFMsg::ObjectPropertyList xPbPropertyStorageList;
		NFMsg::ObjectRecordList xPbRecordCacheList;
		NFMsg::ObjectRecordList xPbRecordStorageList;
	};

	NFMapEx<NFGUID, PlayerDataCache> mxObjectDataCache;
	NFMapEx<NFGUID, std::string> mxObjectTileCache;

private:
	NFIClassModule* m_pLogicClassModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFICommonRedisModule* m_pCommonRedisModule;
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
};


#endif
