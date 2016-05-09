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

class NFCMapModule
    : public NFIMapModule
{
public:

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();



protected:
	//工会申请入驻
	void ReqStation(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	
	//个人申请组队发动猎夺
	void ReqRaidWithMapGuild(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//工会申请地图决战
	void ReqFinestWithMapGuild(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//工会申请领取地图奖励
	void ReqGetMapAward(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//地图流言骂战
	void ReqLeaveMsgToMap(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//查看某地块信息
	void ReqMapTitleInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	//查看整个大地图信息
	void ReqBigMapsInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

};


#endif
