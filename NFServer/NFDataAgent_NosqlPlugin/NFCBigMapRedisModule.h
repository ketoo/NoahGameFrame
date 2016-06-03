// -------------------------------------------------------------------------
//    @FileName			:    NFCBigMapRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCBigMapRedisModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_BIGMAP_REDIS_MODULE_H
#define NFC_BIGMAP_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFCBigMapRedisModule : public NFIBigMapRedisModule
{
public:

	NFCBigMapRedisModule(NFIPluginManager* p);

	virtual ~NFCBigMapRedisModule();

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute(const float fLasFrametime, const float fStartedTime);
	virtual bool AfterInit();

	virtual bool GetGridBaseInfo(const std::string& strGridID, NFMsg::BigMapGridBaseInfo& xBaseInfo);
	virtual bool GetGridBaseInfo(std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfo);
	virtual bool GetGridBaseInfo(const std::vector<std::string>& strGridID, std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfo);
	virtual bool GetGridLeaveMsgInfo(const std::string& strGridID, std::vector<NFMsg::BigMapLeaveMsg>& xLeaveMsgList);
	virtual bool GetGridWarHistoryInfo(const std::string& strGridID, std::vector<NFMsg::BigMapWarHistory>& xWarHistoryList);

	virtual bool SetGridBaseInfo(const std::string& strGridID, const NFMsg::BigMapGridBaseInfo& xBaseInfo);
	virtual bool AddGridLeaveMsgInfo(const std::string& strGridID, const NFMsg::BigMapLeaveMsg& xLeaveMsg);
	virtual bool AddGridWarHistoryInfo(const std::string& strGridID, const NFMsg::BigMapWarHistory& xWarHistory);

private:
	std::string GetGridBaseKey();
	std::string GetGridLeaveMsgKey(const std::string&strGridID);
	std::string GetGridWarHistoryKey(const std::string&strGridID);

private:
	NFINoSqlModule* m_pNoSqlModule;
	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;

};


#endif
