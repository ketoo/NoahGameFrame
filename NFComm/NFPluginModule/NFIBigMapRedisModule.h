// -------------------------------------------------------------------------
//    @FileName         :    NFIBigMapRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIBigMapRedisModule
//
// -------------------------------------------------------------------------

#ifndef NFI_BIGMAP_REDIS_MODULE_H
#define NFI_BIGMAP_REDIS_MODULE_H

#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

class NFIBigMapRedisModule
    : public NFIModule
{
public:

	virtual bool GetGridBaseInfo(const std::string& strGridID, NFMsg::BigMapGridBaseInfo& xBaseInfo) = 0;
	virtual bool GetGridBaseInfo(std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfo) = 0;
	virtual bool GetGridBaseInfo(const std::vector<std::string>& strGridID, std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfo) = 0;

	virtual bool GetGridLeaveMsgInfo(const std::string& strGridID, std::vector<NFMsg::BigMapLeaveMsg>& xLeaveMsgList) = 0;
	virtual bool GetGridWarHistoryInfo(const std::string& strGridID, std::vector<NFMsg::BigMapWarHistory>& xWarHistoryList) = 0;
	virtual bool GetGridStationInfo(const std::string& strGridID, std::vector<NFMsg::GridGuildBaseInfo>& xWarHistoryList) = 0;

	virtual bool SetGridBaseInfo(const std::string& strGridID, const NFMsg::BigMapGridBaseInfo& xBaseInfo) = 0;

	virtual bool AddGridLeaveMsgInfo(const std::string& strGridID, const NFMsg::BigMapLeaveMsg& xLeaveMsg) = 0;
	virtual bool AddGridWarHistoryInfo(const std::string& strGridID, const NFMsg::BigMapWarHistory& xWarHistory) = 0;
	
	virtual bool AddGridStationInfo(const std::string& strGridID, const NFGUID& self, const int nResource) = 0;
	virtual bool RemoveGridStationInfo(const std::string& strGridID, const NFGUID& self, const int nResource) = 0;


};

#endif