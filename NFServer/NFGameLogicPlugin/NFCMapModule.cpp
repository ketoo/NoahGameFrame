// -------------------------------------------------------------------------
//    @FileName				:    NFCMapModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCMapModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCMapModule.h"

bool NFCMapModule::Init()
{
    m_pNoSqlModule = pPluginManager->GetModule<NFINoSqlModule>("NFCNoSqlModule");
    m_pGameServerNet_ServerModule = pPluginManager->GetModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
    return true;
}

bool NFCMapModule::Shut()
{
    return true;
}

bool NFCMapModule::Execute()
{
    return true;
}

bool NFCMapModule::AfterInit()
{

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_BIG_MAP_GRID, this, &NFCMapModule::ReqBigMapsInfo)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_INFO, this, &NFCMapModule::ReqMapTitleInfo)) { return false; }

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_HOLD_MAP_GRID, this, &NFCMapModule::ReqStation)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_GET_MAP_GRID_AWARD, this, &NFCMapModule::ReqGetMapAward)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_LEAVE_MSG_MAP_GRID, this, &NFCMapModule::ReqLeaveMsgToMap)) { return false; }

	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_HUNTING, this, &NFCMapModule::ReqMapHunting)) { return false; }
	if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGameMsgID::EGMI_REQ_MAP_GRID_KING_WAR, this, &NFCMapModule::ReqMapKingWar)) { return false; }

    return true;
}

void NFCMapModule::ReqMapTitleInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqBigMapGridInfo);




}

void NFCMapModule::ReqBigMapsInfo(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqBigMapInfo);
}

void NFCMapModule::ReqStation(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqHoldMapGrid);
}

void NFCMapModule::ReqGetMapAward(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqGetMapAward);
}

void NFCMapModule::ReqLeaveMsgToMap(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqLeaveMapMsg);
}

void NFCMapModule::ReqMapHunting(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqMapHunting);
}

void NFCMapModule::ReqMapKingWar(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	//CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqMapKingWar);
}

std::string NFCMapModule::GetGridBaseKey(const std::string&strGridID)
{
    return "GridBaseKey_" + strGridID;
}

std::string NFCMapModule::GetGridLeaveMsgKey(const std::string&strGridID)
{
    return "GridLeaveMsgKey_" + strGridID;
}

std::string NFCMapModule::GetGridWarHistoryKey(const std::string&strGridID)
{
    return "GridWarHistoryKey_" + strGridID;
}

bool NFCMapModule::GetGridBaseInfo(const std::string&strGridID, NFMsg::BigMapGridBaseInfo& xBaseInfo)
{
    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
    if (pNoSqlDriver)
    {
        std::string strKey = GetGridBaseKey(strGridID);
        std::vector<std::pair<std::string, std::string>> xValue;
        int nRet = pNoSqlDriver->HGetAll(strKey, xValue);
        if(nRet > 0)
        {
            //xBaseInfo.set_id("");
            //xBaseInfo.set_owner();
            //xBaseInfo.set_level(1);
            //xBaseInfo.set_member_count(100);
            //xBaseInfo.set_resource(100);
            //xBaseInfo.set_icon("");

            return true;
        }
    }

    return false;
}

bool NFCMapModule::GetGridLeaveMsgInfo(const std::string&strGridID, std::vector<NFMsg::BigMapLeaveMsg>& xLeaveMsgList)
{
    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
    if (pNoSqlDriver)
    {
        std::string strKey = GetGridLeaveMsgKey(strGridID);
        std::vector<std::pair<std::string, std::string>> xValue;
        int nRet = pNoSqlDriver->HGetAll(strKey, xValue);
        if(nRet > 0)
        {
            for(int i = 0; i< xValue.size(); ++i)
            {
                //first is a guid
				NFMsg::BigMapLeaveMsg xLeaveMsg;
                if (!xLeaveMsg.ParseFromString(xValue[i].second))
                {
                    //log error
                    continue;
                }

                xLeaveMsgList.push_back(xLeaveMsg);
            }

            return true;
        }
    }

    return false;
}

bool NFCMapModule::GetGridWarHistoryInfo(const std::string&strGridID, std::vector<NFMsg::BigMapWarHistory>& xWarHistoryList)
{
    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
    if (pNoSqlDriver)
    {
        std::string strKey = GetGridWarHistoryKey(strGridID);
        std::vector<std::pair<std::string, std::string>> xValue;
        int nRet = pNoSqlDriver->HGetAll(strKey, xValue);
        if(nRet > 0)
        {
            for(int i = 0; i< xValue.size(); ++i)
            {
                //first is a guid
                 NFMsg::BigMapWarHistory xWarData;
                if (!xWarData.ParseFromString(xValue[i].second))
                {
                    //log error
                    continue;
                }

                xWarHistoryList.push_back(xWarData);
            }

            return true;
        }
    }

    return false;
}

bool NFCMapModule::SetGridBaseInfo(const std::string&strGridID, const NFMsg::BigMapGridBaseInfo& xBaseInfo)
{
    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
    if (pNoSqlDriver)
    {
        std::string strKey = GetGridBaseKey(strGridID);

        std::vector<std::string> keys;
        std::vector<std::string> values;

        //xBaseInfo.set_id("");
        //xBaseInfo.set_owner();
        //xBaseInfo.set_level(1);
        //xBaseInfo.set_member_count(100);
        //xBaseInfo.set_resource(100);
        //xBaseInfo.set_icon("");

        pNoSqlDriver->HMSet(strKey, keys, values);


        return true;
    }

    return false;
}

bool NFCMapModule::AddGridLeaveMsgInfo(const std::string&strGridID, const NFMsg::BigMapLeaveMsg& xLeaveMsg)
{
    //数量限制
    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
    if (pNoSqlDriver)
    {
        std::string strValue;
        std::string strKey = GetGridLeaveMsgKey(strGridID);
        pNoSqlDriver->ListPush(strKey, strValue);

        return true;
    }

    return false;
}

bool NFCMapModule::AddGridWarHistoryInfo(const std::string&strGridID, const NFMsg::BigMapWarHistory& xWarHistory)
{
    //数量限制
    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
    if (pNoSqlDriver)
    {
        std::string strValue;
        std::string strKey = GetGridWarHistoryKey(strGridID);

        pNoSqlDriver->ListPush(strKey, strValue);

        return true;
    }

    return false;
}
