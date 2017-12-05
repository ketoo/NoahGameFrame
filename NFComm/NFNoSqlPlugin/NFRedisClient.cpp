//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisClient::NFRedisClient()
{
    m_pRedisClientSocket = new NFRedisClientSocket();
    m_pRedisResult = new NFRedisResult(m_pRedisClientSocket);
}


bool NFRedisClient::ConnectTo(const std::string &ip, const int port, const std::string &auth)
{
    int64_t nFD = m_pRedisClientSocket->Connect(ip, port);
	if (nFD > 0)
	{
		if (!auth.empty())
		{
			NFRedisResult* pResult = AUTH(auth);
			return pResult->IsOKRespStatus();
		}

		return true;
	}

    return false;
}

bool NFRedisClient::SelectDB(int dbnum)
{
    return false;
}

bool NFRedisClient::KeepLive()
{
    return false;
}

bool NFRedisClient::Execute()
{
    m_pRedisClientSocket->Execute();
    return false;
}

bool NFRedisClient::GetStatusReply()
{
    if (m_pRedisResult->ReadReply())
    {
        if (m_pRedisResult->GetResultType() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK
                && m_pRedisResult->GetRespType() == NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS
                && m_pRedisResult->IsOKRespStatus())
        {
            return true;
        }
    }


    return false;
}

bool NFRedisClient::GetIntReply()
{
    if (m_pRedisResult->ReadReply())
    {
        if (m_pRedisResult->GetResultType() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK
            && m_pRedisResult->GetRespType() == NFREDIS_RESP_TYPE::NFREDIS_RESP_INT)
        {
            return true;
        }
    }


    return false;
}

bool NFRedisClient::GetBulkReply()
{
    if (m_pRedisResult->ReadReply())
    {
        if (m_pRedisResult->GetResultType() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK
            && m_pRedisResult->GetRespType() == NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK)
        {
            return true;
        }
    }


    return false;
}

bool NFRedisClient::GetArrayReply()
{
	if (m_pRedisResult->ReadReply())
	{
		if (m_pRedisResult->GetResultType() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK
			&& m_pRedisResult->GetRespType() == NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY)
		{
			return true;
		}
	}


	return false;

}

NFRedisResult *NFRedisClient::GetRedisResult()
{
    return m_pRedisResult;
}

NFRedisResult * NFRedisClient::AUTH(const std::string & auth)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("AUTH");
	cmd << auth;

	std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

	int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
	if (nRet != 0)
	{
		return m_pRedisResult;
	}

	GetStatusReply();


	return m_pRedisResult;
}

