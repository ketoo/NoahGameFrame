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
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
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

    std::string GetGridBaseKey(const std::string&strGridID);
    std::string GetGridLeaveMsgKey(const std::string&strGridID);
    std::string GetGridWarHistoryKey(const std::string&strGridID);

    bool GetGridBaseInfo(const std::string&strGridID, NFMsg::BigMapGridBaseInfo& xBaseInfo);
    bool GetGridLeaveMsgInfo(const std::string&strGridID, std::vector<NFMsg::BigMapLeaveMsg>& xLeaveMsgList);
    bool GetGridWarHistoryInfo(const std::string&strGridID, std::vector<NFMsg::BigMapWarHistory>& xWarHistoryList);

    bool SetGridBaseInfo(const std::string&strGridID, const NFMsg::BigMapGridBaseInfo& xBaseInfo);
    bool AddGridLeaveMsgInfo(const std::string&strGridID, const NFMsg::BigMapLeaveMsg& xLeaveMsg);
    bool AddGridWarHistoryInfo(const std::string&strGridID, const NFMsg::BigMapWarHistory& xWarHistory);



private:
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFINoSqlModule* m_pNoSqlModule;


};


#endif
