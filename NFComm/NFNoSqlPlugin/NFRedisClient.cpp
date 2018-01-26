//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisClient::NFRedisClient()
{
    m_pRedisClientSocket = new NFRedisClientSocket();
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

	if (!mlCmdResultList.empty())
	{
		NF_SHARE_PTR<NFRedisResult> pRedisResult = mlCmdResultList.front();
		bool bRet = pRedisResult->ReadReply();
		if (bRet && pRedisResult->GetResultStatus() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK)
		{
			mlCmdResultList.pop_front();
		}
	}

    return false;
}

NF_SHARE_PTR<NFRedisResult> NFRedisClient::GetUnuseResult()
{
	NF_SHARE_PTR<NFRedisResult> p = nullptr;
	if (!mlUnusedResultList.empty())
	{
		p = mlUnusedResultList.front();
		mlUnusedResultList.pop_front();
	}
	else
	{
		p = NF_SHARE_PTR<NFRedisResult>(new NFRedisResult(m_pRedisClientSocket));
	}

	return p;
	
}

NF_SHARE_PTR<NFRedisResult> NFRedisClient::BuildSendCmd(const NFRedisCommand& cmd)
{
	std::string msg = cmd.Serialize();

	//m_pRedisResult->Reset();
	NF_SHARE_PTR<NFRedisResult> pRedisResult = GetUnuseResult();
	pRedisResult->Reset();
	pRedisResult->SetCommand(msg);

	int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
	if (nRet != 0)
	{
		//lost net
		//do some thing
		return pRedisResult;
	}

	mlCmdResultList.push_back(pRedisResult);

	return pRedisResult;
}

void NFRedisClient::WaitingResult(NF_SHARE_PTR<NFRedisResult> pRedisResult)
{
	while (!pRedisResult->GetResultStatus() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK)
	{
		if (YieldFunction)
		{
			YieldFunction();
		}
		else
		{
			Execute();
		}
	}
}

NFRedisResult * NFRedisClient::AUTH(const std::string & auth)
{
	NFRedisCommand cmd(GET_NAME(AUTH));
	cmd << auth;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);
	
	WaitingResult(pRedisResult);


	return pRedisResult.get();
}