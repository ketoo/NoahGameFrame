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
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
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

	//查看某地块信息
	void ReqMapTitleInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//查看整个大地图信息
	void ReqBigMapsInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	///////////////////////////
	//工会申请入驻
	void ReqStation(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


	//工会申请领取地图奖励
	void ReqGetMapAward(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//地图流言骂战
	void ReqLeaveMsgToMap(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	///////////////////////////

	//个人申请发动猎夺--资源互猎
	void ReqMapHunting(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//工会申请参与地图决战活动--决定地图归属权
	void ReqMapKingWar(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	void EndMapHunting(const std::string& strTitleID);
	void EndMapKingWar(const std::string& strTitleID);

	void SetKingForGrid(const std::string& strTitleID, const NFGUID& xGuildID);
	void LeaveStation(const std::string& strTitleID, const NFGUID& xGuildID);
private:
	NFIBigMapRedisModule* m_pBigMapRedisModule;
	NFIKernelModule* m_pKernelModule;
	NFILogicClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
	NFIGuildRedisModule* m_pGuildRedisModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;


};


#endif
