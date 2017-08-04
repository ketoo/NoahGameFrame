// -------------------------------------------------------------------------
//    @FileName			:    NFCMapModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCMapModule
//
// -------------------------------------------------------------------------

#ifndef NFC_MAP_MODULE_H
#define NFC_MAP_MODULE_H

#include "NFComm/NFPluginModule/NFIMapModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCMapModule
    : public NFIMapModule
{
public:
	NFCMapModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFCMapModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:

	
	void ReqMapTitleInfo(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	
	void ReqBigMapsInfo(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	///////////////////////////
	
	void ReqStation(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


	
	void ReqGetMapAward(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	
	void ReqLeaveMsgToMap(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	///////////////////////////

	
	void ReqMapHunting(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	
	void ReqMapKingWar(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	void EndMapHunting(const std::string& strTitleID);
	void EndMapKingWar(const std::string& strTitleID);

	void SetKingForGrid(const std::string& strTitleID, const NFGUID& xGuildID);
	void LeaveStation(const std::string& strTitleID, const NFGUID& xGuildID);
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
