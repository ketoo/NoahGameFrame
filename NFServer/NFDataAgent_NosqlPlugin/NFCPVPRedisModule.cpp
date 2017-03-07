// -------------------------------------------------------------------------
//    @FileName			:    NFCPVPRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPVPRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCPVPRedisModule.h"

NFCPVPRedisModule::NFCPVPRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCPVPRedisModule::Init()
{
	return true;
}

bool NFCPVPRedisModule::Shut()
{
	return true;
}

bool NFCPVPRedisModule::Execute()
{
	return true;
}

bool NFCPVPRedisModule::AfterInit()
{
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();

	return true;
}

bool NFCPVPRedisModule::GetPlayerRoomID(const NFGUID& self, NFGUID& xRoomID)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePlayerRoomKey();
        std::string strData;
        if (pNoSqlDriver->HGet(strKey, self.ToString(), strData))
        {
            if (xRoomID.FromString(strData))
            {
                return true;
            }
        }
    }

    return false;
}

bool NFCPVPRedisModule::GetPlayerRoomIDList(const std::vector<NFGUID>& xPlayerList, std::vector<NFGUID>& vecRoomIDList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::vector<std::string> vFields;
        std::vector<std::string> vValues;
        std::string strKey = MakePlayerRoomKey();
        std::string strData;

        for (int i = 0; i < xPlayerList.size(); ++i)
        {
            vFields.push_back(xPlayerList[i].ToString());
        }

        if (pNoSqlDriver->HMGet(strKey, vFields, vValues))
        {
            for (int i = 0; i < vValues.size(); ++i)
            {
                NFGUID xIdent;
                xIdent.FromString(vValues[i]);
                vecRoomIDList.push_back(xIdent);
            }

            return true;
        }
    }

    return false;
}

bool NFCPVPRedisModule::SetPlayerRoomID(const NFGUID& self, const NFGUID& xRoomID)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePlayerRoomKey();
        return pNoSqlDriver->HSet(strKey, self.ToString(), xRoomID.ToString());
    }

    return false;
}

bool NFCPVPRedisModule::SetPlayerRoomIDList(const std::vector<NFGUID>& xPlayerList, std::vector<NFGUID>& vecRoomIDList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePlayerRoomKey();
        std::vector<std::string> vFields;
        std::vector<std::string> vValues;

        if (xPlayerList.size() != vecRoomIDList.size())
        {
            return false;
        }

        for (int i = 0; i < xPlayerList.size(); i ++)
        {
            vFields.push_back(xPlayerList[i].ToString());
            vValues.push_back(vecRoomIDList[i].ToString());
        }

        return pNoSqlDriver->HMSet(strKey, vFields, vValues);
    }

    return false;
}

bool NFCPVPRedisModule::GetRoomInfo(const NFGUID& xRoomID, NFMsg::PVPRoomInfo& xRoomInfo)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePvpRoomRedisKey();
        std::string strData;
        if (pNoSqlDriver->HGet(strKey, xRoomID.ToString(), strData))
        {
            if (xRoomInfo.ParseFromString(strData))
            {
                return true;
            }
        }
    }

    return false;
}

bool NFCPVPRedisModule::GetRoomInfoList(const std::vector<NFGUID>& xRoomIDList, std::vector<NFMsg::PVPRoomInfo>& vecRoomInfoList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePvpRoomRedisKey();
        std::vector<std::string> vFields;
        std::vector<std::string> vValues;

        for (int i = 0; i < xRoomIDList.size(); ++i)
        {
            vFields.push_back(xRoomIDList[i].ToString());
        }

		if (vFields.size()>0)
		{
			std::string strData;
			if (pNoSqlDriver->HMGet(strKey, vFields, vValues))
			{
				for (int i = 0; i < vValues.size(); ++i)
				{
					const std::string& strData = vValues[i];
					NFMsg::PVPRoomInfo xBaseInfo;
					if (xBaseInfo.ParseFromString(strData))
					{
						vecRoomInfoList.push_back(xBaseInfo);
					}
				}
				return true;
			}
		}
		else
		{
			return false;
		}
    }

    return false;
}

bool NFCPVPRedisModule::GetStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, std::vector<NFGUID>& xRoomIDList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeStatusRoomIDRedisKey(nPVPMode, nGrade, nStatus);
        std::string strData;

        std::vector<std::string> valueVec;
        if (pNoSqlDriver->HValues(strKey, valueVec))
        {
            for (int i = 0; i < valueVec.size(); i++)
            {
                NFGUID xIdent;
                xIdent.FromString(valueVec[i]);
                if (!xIdent.IsNull())
                {
                    xRoomIDList.push_back(xIdent);
                }
            }

            return true;
        }
    }

    return false;
}

bool NFCPVPRedisModule::SetRoomInfo(const NFGUID& xRoomID, NFMsg::PVPRoomInfo& xRoomInfo)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePvpRoomRedisKey();
        std::string strData;
        if (xRoomInfo.ParseFromString(strData))
        {
            if (pNoSqlDriver->HSet(strKey, xRoomID.ToString(), strData))
            {
                return true;
            }
        }
    }

    return false;
}

bool NFCPVPRedisModule::SetRoomInfoList(const std::vector<NFGUID>& xRoomIDList, std::vector<NFMsg::PVPRoomInfo>& vecRoomInfoList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::vector<std::string> vFields;
        std::vector<std::string> vValues;
        std::string strKey = MakePvpRoomRedisKey();

        if (xRoomIDList.size() != vecRoomInfoList.size())
        {
            return false;
        }

        for (int i = 0; i < xRoomIDList.size(); ++i)
        {
            std::string strData;
            if (vecRoomInfoList[i].SerializeToString(&strData))
            {
                vFields.push_back(xRoomIDList[i].ToString());
                vValues.push_back(strData);
            }
        }

        if (pNoSqlDriver->HMSet(strKey, vFields, vValues))
        {
            return true;
        }
    }

    return false;
}

bool NFCPVPRedisModule::SetStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, const NFGUID& xRoomID)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeStatusRoomIDRedisKey(nPVPMode, nGrade, nStatus);
        std::string strData;

        std::vector<std::string> valueVec;
        if (pNoSqlDriver->HSet(strKey, xRoomID.ToString(), xRoomID.ToString()))
        {
            return true;
        }
    }

    return false;
}

bool NFCPVPRedisModule::DeletePlayerRoomID(const NFGUID& self, const NFGUID& xRoomID)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePlayerRoomKey();
        return pNoSqlDriver->HDel(strKey, self.ToString());
    }

    return false;
}

bool NFCPVPRedisModule::DeletePlayerRoomIDList(const std::vector<NFGUID>& xPlayerList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePlayerRoomKey();
        std::vector<std::string> vFields;

        for (int i = 0; i < xPlayerList.size(); ++i)
        {
            pNoSqlDriver->HDel(strKey, xPlayerList[i].ToString());
        }

        return true;
    }

    return false;
}

bool NFCPVPRedisModule::DeleteRoomInfo(const NFGUID& xRoomID)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePvpRoomRedisKey();
        std::string strData;
        if (pNoSqlDriver->HDel(strKey, xRoomID.ToString()))
        {
            return true;
        }
    }

    return false;
}

bool NFCPVPRedisModule::DeleteRoomInfoList(const std::vector<NFGUID>& xRoomIDList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakePvpRoomRedisKey();
        std::vector<std::string> vFields;

        for (int i = 0; i < xRoomIDList.size(); ++i)
        {
            pNoSqlDriver->HDel(strKey, xRoomIDList[i].ToString());
        }

        return true;
    }

    return false;
}

bool NFCPVPRedisModule::DeleteStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, const NFGUID& xRoomID)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeStatusRoomIDRedisKey(nPVPMode, nGrade, nStatus);
        std::string strData;

        std::vector<std::string> valueVec;
        if (pNoSqlDriver->HDel(strKey, xRoomID.ToString()))
        {
            return true;
        }
    }

    return false;
}

bool NFCPVPRedisModule::DeletetStatusRoomID(const int nPVPMode, const int nGrade, const int nStatus, const std::vector<NFGUID>& xRoomIDList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeStatusRoomIDRedisKey(nPVPMode, nGrade, nStatus);
        std::string strData;

        for (int i = 0; i < xRoomIDList.size(); ++i)
        {
            pNoSqlDriver->HDel(strKey, xRoomIDList[i].ToString());
        }

        return true;
    }

    return false;
}

bool NFCPVPRedisModule::PushSinglePlayer(const NFGUID& self, const int nPVPMode, const int nGrade)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeSingleWaitRedisKey(nPVPMode, nGrade);
        std::string strData;

        return pNoSqlDriver->ListPush(strKey, self.ToString());
    }

    return false;
}

bool NFCPVPRedisModule::PopSinglePlayer(NFGUID& self, const int nPVPMode, const int nGrade)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeSingleWaitRedisKey(nPVPMode, nGrade);
        std::string strData;

        if (pNoSqlDriver->ListPop(strKey, strData))
        {
            return self.FromString(strData);
        }
    }

    return false;
}

bool NFCPVPRedisModule::PopSinglePlayerList(const int nPVPMode, const int nGrade, const int nCount, std::vector<NFGUID>& xPlayerList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeSingleWaitRedisKey(nPVPMode, nGrade);
        std::string strData;

        for (int i = 0; i < nCount; i++)
        {
            if (pNoSqlDriver->ListPop(strKey, strData))
            {
                NFGUID xIdent;
                xIdent.FromString(strData);

                xPlayerList.push_back(xIdent);
            }
        }
    }

    return false;
}

int NFCPVPRedisModule::GetSinglePlayerCount(NFGUID& self, const int nPVPMode, const int nGrade)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
    if (pNoSqlDriver)
    {
        std::string strKey = MakeSingleWaitRedisKey(nPVPMode, nGrade);
        std::string strData;

        int nLen = 0;
        if (pNoSqlDriver->ListLen(strKey, nLen))
        {
            return nLen;
        }
    }

    return 0;
}

std::string NFCPVPRedisModule::MakePlayerRoomKey()
{
    std::string szKey = "PvpPlayerRoom_";

    return szKey;
}

std::string NFCPVPRedisModule::MakePvpRoomRedisKey()
{
	std::string szKey = "PvpRoomInfo_";
	
	return szKey;
}

std::string NFCPVPRedisModule::MakeSingleWaitRedisKey(const int nPvpMode, const int nGrade)
{
    std::string szKey = "PvpSingleWait_" + lexical_cast<std::string>(nPvpMode) + "_" + lexical_cast<std::string>(nGrade);

    return szKey;
}

std::string NFCPVPRedisModule::MakeStatusRoomIDRedisKey(const int nPvpMode, const int nGrade, const int nStatus)
{
    std::string szKey = "WaitPlayerRoomID_" + lexical_cast<std::string>(nPvpMode) + "_" + lexical_cast<std::string>(nGrade) + "_" + lexical_cast<std::string>(nStatus);

    return szKey;

}
