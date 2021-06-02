/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFRedisClient.h"

NFRedisClient::NFRedisClient()
{
	mnPort = 0;
	mbAuthed = false;
	mbEnable = mbEnable;
    m_pRedisClientSocket = new NFRedisClientSocket();
}

bool NFRedisClient::Enable()
{
	return mbEnable && m_pRedisClientSocket->Enable();
}

bool NFRedisClient::Authenticated()
{
	return mbAuthed;
}

bool NFRedisClient::Connect(const std::string &ip, const int port, const std::string &auth, const bool sync)
{
    int64_t nFD = m_pRedisClientSocket->Connect(ip, port);
	if (nFD > 0)
	{
		synchronization = sync;
		mstrIP = ip;
		mnPort = port;
		mstrAuthKey = auth;
		mbEnable = true;

		return true;
	}

	return false;
}

bool NFRedisClient::KeepLive()
{
    return false;
}


void NFRedisClient::UpdateSlot(int start, int end)
{
	startSlot = start;
	endSlot = end;
}

bool NFRedisClient::IsConnected()
{
	if (m_pRedisClientSocket)
	{
		return m_pRedisClientSocket->IsConnect();
	}

	return false;
}

bool NFRedisClient::Execute()
{
    m_pRedisClientSocket->Execute();

    return false;
}

NF_SHARE_PTR<NFRedisReply> NFRedisClient::BuildSendCmd(const NFRedisCommand& cmd)
{
	if (!IsConnected())
	{
		mbEnable = false;

		return nullptr;
	}
	else
	{
		std::string msg = cmd.Serialize();
		if (msg.empty())
		{
			return nullptr;
		}
		int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
		if (nRet != 0)
		{
			return nullptr;
		}
	}


	lastReply = ParseForReply();

	return lastReply;
}

NF_SHARE_PTR<NFRedisReply> NFRedisClient::ParseForReply()
{
	struct redisReply* reply = nullptr;
	while (true)
	{
		// When the buffer is empty, reply will be null
		int ret = redisReaderGetReply(m_pRedisClientSocket->GetRedisReader(), (void**)&reply);
		if (ret == REDIS_OK && reply != nullptr)
		{
			break;
		}

		Execute();

		if (!IsConnected())
		{
			mbEnable = false;
			break;
		}
	}


	if (reply == nullptr)
	{
		return nullptr;
	}

	NF_SHARE_PTR<NFRedisReply> fakeReply = NF_SHARE_PTR<NFRedisReply>(new NFRedisReply);
	if (REDIS_REPLY_ERROR == reply->type)
	{
		// write error log
		std::cout << reply->str << std::endl;
		fakeReply->error = reply->str;

		freeReplyObject(reply);
		return fakeReply;
	}

	fakeReply->reply = NF_SHARE_PTR<redisReply>(reply, [](redisReply* r) { if (r) freeReplyObject(r); });

	return fakeReply;
}

bool NFRedisClient::AUTH(const std::string& auth)
{
	if (auth.empty())
	{
		mbAuthed = true;
		return true;
	}

	NFRedisCommand cmd(GET_NAME(AUTH));
	cmd << auth;

	// if password error, redis will return REDIS_REPLY_ERROR
	// pReply will be null
	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_STATUS)
	{
		if (std::string("OK") == std::string(pReply->reply->str, pReply->reply->len) ||
			std::string("ok") == std::string(pReply->reply->str, pReply->reply->len))
		{
			mbAuthed = true;
			return true;
		}
	}
	
	return false;
}

bool NFRedisClient::INFO(std::string& info)
{
	NFRedisCommand cmd(GET_NAME(INFO));

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_STRING)
	{
		info.append(pReply->reply->str, pReply->reply->len);
		return true;
	}

	return false;
}

void StringToVector(const std::string& data, const std::string& split, std::vector<std::string>& out)
{
	if (data.empty())
	{
		return;
	}
	std::string strData(data);
	if (strData.empty())
	{
		return;
	}

	std::string::size_type pos;
	strData += split;

	std::string::size_type size = strData.length();

	for (std::string::size_type i = 0; i < size; i++)
	{
		pos = int(strData.find(split, i));
		if (pos < size)
		{
			std::string strSub = strData.substr(i, pos - i);
			out.push_back(strSub);

			i = pos + split.size() - 1;
		}
	}
}
