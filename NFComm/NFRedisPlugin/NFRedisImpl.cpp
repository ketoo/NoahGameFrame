#include "NFRedisImpl.h"


NFCRedisImpl::NFCRedisImpl(std::thread::id threadID)
	:_threadID(threadID)
{
}

NFCRedisImpl::~NFCRedisImpl() 
{
}

int NFCRedisImpl::SetValueByBinary(const std::string& strKey, const char* pBuf, size_t nSize)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("SET %s %b", strKey.c_str(), pBuf, nSize);
		if (pReply == nullptr)
		{
			return -1;
		}

			/*std::string data(pBuf, nSize);
			std::vector<std::string> vecStrData;
			vecStrData.push_back("set");
			vecStrData.push_back(strKey);
			vecStrData.push_back(data);

			vector<const char*> argv(vecStrData.size());
			vector<size_t> argvlen(vecStrData.size());
			unsigned int j = 0;
			for (auto it = vecStrData.begin(); it != vecStrData.end(); ++it, ++j)
			{
				argv[j] = it->c_str();
				argvlen[j] = it->size();
			}
			RedisReplyPtr r = rc->exeCmd(argv.size(), &(argv[0]), &(argvlen[0]));
			if (r == nullptr)
			{
				return -1;
			}*/

			//const char *p = "set";
			//size_t nLength[3] = { 0 };
			//nLength[0] = strlen(p);
			//nLength[1] = strKey.length();
			//nLength[2] = nSize;
			//pBufMem = (char*)malloc(nLength[0] + nLength[1] + nLength[2]);//buf[8192];
			//char* pBuffs[3];
			//int n = 0;
			//pBuffs[0] = new(pBufMem + n)char[nLength[0]];
			//n += nLength[0];
			//pBuffs[1] = new(pBufMem + n)char[nLength[1]];
			//n += nLength[1];
			//pBuffs[2] = new(pBufMem + n)char[nLength[2]];
			//memcpy(pBuffs[1], strKey.c_str(), nLength[1]);
			//memcpy(pBuffs[2], pBuf, nLength[2]);
			//memcpy(pBuffs[0], "set", nLength[0]);

			//RedisReplyPtr r = rc->exeCmd(3, const_cast<const char**>(pBuffs), const_cast<const size_t*>(nLength));
			//if (r == nullptr)
			//{
			//	return -1;
			//}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::SetValueInt(const std::string& strKey, NFINT64 nValue)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)											
		{																						
			return -1;												
		}

		RedisReplyPtr pReply = rc->exeCmd("set %s %lld", strKey.c_str(), nValue);
		if (pReply == nullptr)
		{
			return -1;
		}
	
	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::GetValue(const std::string& strKey, char* buff, int buff_size, int& real_size)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			return -1;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("GET %s", strKey.c_str());
		if (pRedisReply == nullptr)
		{
			return -1;
		}

		if (pRedisReply->len > buff_size)
		{
			return -1;
		}

		if (pRedisReply->type == REDIS_REPLY_STRING)
		{
			memcpy(buff, pRedisReply->str, pRedisReply->len);
			real_size = pRedisReply->len;
		}
	
	_TRY_END_(strKey)

	return real_size;
}

long long NFCRedisImpl::GetValueInt(const std::string& strKey)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("GET %s", strKey.c_str());
		if (pRedisReply == nullptr)
		{
			
			return 0;
		}

		if (pRedisReply->type == REDIS_REPLY_STRING)
		{
			return lexical_cast<long long>(pRedisReply->str);
		}

	_TRY_END_(strKey)

	return 0;
}

bool NFCRedisImpl::Exist(const std::string& strKey)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return false;
		}

		RedisReplyPtr pReply = rc->exeCmd("EXISTS %s", strKey.c_str());
		if(pReply == nullptr)
			return false;

		if (REDIS_REPLY_INTEGER == pReply->type && 1 == pReply->integer)
		{
			return true;
		}

	_TRY_END_(strKey)

	return false;
}

bool NFCRedisImpl::RemoveKey(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
	if (nullptr == rc)
	{
		return false;
	}


	RedisReplyPtr pReply = rc->exeCmd("DEL %s", strKey.c_str());

	if (pReply == nullptr)
	{

		return false;
	}

	_TRY_END_(strKey)

		return true;
}

bool NFCRedisImpl::ZaddScore(const std::string& strKey, NFINT64 nScore, const std::string& member)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			return false;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZADD %s %lld %s", strKey.c_str(), nScore, member.c_str());
		if (pRedisReply == nullptr)
		{
			return false;
		}

	_TRY_END_(strKey)

	return true;
}

std::string NFCRedisImpl::ZgetValue(const std::string& strKey, NFINT64 nScore)
{
	std::string szResult = "";

	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			return szResult;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZRANGEBYSCORE %s %lld %lld", strKey.c_str(), nScore, nScore);
		if (pRedisReply == nullptr)
		{
			return szResult;
		}

		if (pRedisReply->type == REDIS_REPLY_ARRAY && pRedisReply->elements > 0)
		{
			szResult = pRedisReply->element[0]->str;
		}

	_TRY_END_(strKey)

	return szResult;
}

NFINT64 NFCRedisImpl::ZgetScore(const std::string& strKey, const std::string& member)
{
	NFINT64 nScore = 0;

	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZSCORE %s %s", strKey.c_str(), member.c_str());
		if (pRedisReply == nullptr)
		{			
			return 0;
		}

		if (pRedisReply->type == REDIS_REPLY_STRING)
		{
			nScore = (NFINT64)lexical_cast<double>(pRedisReply->str);
		}
	
	_TRY_END_(strKey)

	return nScore;
}

void NFCRedisImpl::ZCommonRange(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd, VecZRangeScoreValue& vecValue)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZRANGE %s %lld %lld WITHSCORES", strKey.c_str(), nStart, nEnd);
		if (pRedisReply == nullptr)
		{	
			return;
		}

		if (pRedisReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pRedisReply->elements; k = k + 2)
			{
				if (pRedisReply->element[k]->type == REDIS_REPLY_STRING)
				{
					std::pair<NFINT64, std::string> vecPair;
					vecPair.second = std::string(pRedisReply->element[k]->str, pRedisReply->element[k]->len);
					vecPair.first = lexical_cast<NFINT64>(pRedisReply->element[k + 1]->str);
					vecValue.push_back(vecPair);
				}
			}
		}

	_TRY_END_(strKey)
}

int NFCRedisImpl::ZgetCount(const std::string& strKey)
{
	int nCount = 0;

	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZCARD %s", strKey.c_str());
		if (pRedisReply == nullptr)
		{		
			return 0;
		}

		if (pRedisReply->type == REDIS_REPLY_INTEGER)
		{
			nCount = (int)pRedisReply->integer;
		}
	
	_TRY_END_(strKey)

	return nCount;
}

int NFCRedisImpl::ZMondifyValue(const std::string& strKey, const std::string& strOldName, const std::string& strNewName)
{
	_TRY_BEGIN_
		
		NFINT64 nScore = ZgetScore(strKey.c_str(), strOldName);
		if (nScore > 0)
		{
			ZDelValue(strKey.c_str(), strOldName);
			ZaddScore(strKey.c_str(), nScore, strNewName);
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZDelValue(const std::string& strKey, const std::string& strName)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return -1;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZREM %s %s", strKey.c_str(), strName.c_str());
		if (pRedisReply == nullptr)
		{		
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

void NFCRedisImpl::InsertToListByBinary(const std::string& strKey, const char* buf, size_t buf_size)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			throw("nullptr == rc");
		}

		RedisReplyPtr pReply = rc->exeCmd("LPUSH %s %b", strKey.c_str(), buf, buf_size);
		if (pReply == nullptr)
		{
			throw("pRedisReply == nullptr");
		}

	_TRY_END_(strKey)
}

bool NFCRedisImpl::GetRedisMsg(const std::string& strKey, std::string& strResult)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			
			return false;
		}

		RedisReplyPtr pReply = rc->exeCmd("RPOP %s", strKey.c_str());
		if (pReply != nullptr && pReply->type == REDIS_REPLY_STRING)
		{
			strResult = std::string(pReply->str, pReply->len);
			return true;
		}

	_TRY_END_(strKey)

	return false;
}

int NFCRedisImpl::GetListLen(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("LLEN  %s", strKey.c_str());
		if (pReply != nullptr&&pReply->type == REDIS_REPLY_INTEGER)
		{
			return (int)pReply->integer;
		}

	_TRY_END_(strKey)

	return 0;
}

bool NFCRedisImpl::DelListData(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return false;
		}

		RedisReplyPtr pReply = rc->exeCmd("LREM  %s 0 %s", strKey.c_str(), strContent.c_str());
		if (pReply != nullptr)
		{
			return true;
		}

	_TRY_END_(strKey)

	return false;
}

bool NFCRedisImpl::GetAllListData(const std::string& strKey, std::string* pszContent, int& size, int nBegin, int nCount)
{
	int nSizeTmp = size;
	size = 0;

	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return false;
		}

		if ( nSizeTmp <= 0 && pszContent == nullptr)
		{
			return false;
		}

		int nCounts = 0;
		RedisReplyPtr pReply = rc->exeCmd("LRANGE %s %d %d", strKey.c_str(), nBegin, nCount);
		if (pReply != nullptr&&pReply->type == REDIS_REPLY_ARRAY)
		{
			for (int k = 0; k < (int)pReply->elements && k < nSizeTmp; k++)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					pszContent[nCounts] = std::string(pReply->element[k]->str, pReply->element[k]->len);
					nCounts++;

					if (nCounts >= nSizeTmp)
					{
						break;
					}
				}
			}

			size = nCounts;
			return true;
		}

	_TRY_END_(strKey)

	size = 0;
	return false;
}

NFINT64 NFCRedisImpl::ZIncrScore(const std::string& strKey, const std::string& strName, NFINT64 nScore)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("ZINCRBY %s %lld %s", strKey.c_str(), nScore, strName.c_str());
		if (pReply == nullptr)
		{		
			return 0;
		}

		if (pReply->type == REDIS_REPLY_STRING)
			return lexical_cast<NFINT64>(pReply->str);

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::SetMutexVauleByStr(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
	if (nullptr == rc)
	{
		return 0;
	}

	RedisReplyPtr pReply = rc->exeCmd("SETNX %s %s", strKey.c_str(), strContent.c_str());
	if (pReply == nullptr)
		return 0;

	if (pReply->type == REDIS_REPLY_INTEGER)
		return (int)pReply->integer;

	_TRY_END_(strKey)

		return 0;
}

int NFCRedisImpl::SetMutexValueByBinary(const std::string& strKey, const char* buf, size_t buf_size)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("SETNX %s %b", strKey.c_str(), buf, buf_size);
		if (pReply == nullptr)
		{
			return -1;
		}

		if (pReply->type == REDIS_REPLY_INTEGER)
		{
			return (int)pReply->integer;
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::WatchKey(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("WATCH %s", strKey.c_str());
		if (pReply == nullptr)
		{		
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::Multi(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			
			return -1;
		}
		RedisReplyPtr pReply = rc->exeCmd("MULTI");
		if (pReply == nullptr)
		{
			
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::Exec(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -2;
		}

		RedisReplyPtr pReply = rc->exeCmd("EXEC");
		if (pReply == NULL)
		{
			return -2;
		}

		if (pReply->type == REDIS_REPLY_NIL)
		{
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::Discard(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -2;
		}
		RedisReplyPtr pReply = rc->exeCmd("DISCARD");
		if (pReply == NULL)
		{
			return -2;
		}
		if (pReply->type == REDIS_REPLY_NIL)
		{
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::Unwatch(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -2;
		}
		RedisReplyPtr pReply = rc->exeCmd("UNWATCH");
		if (pReply == NULL)
		{
			return -2;
		}
		if (pReply->type == REDIS_REPLY_NIL)
		{
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

bool NFCRedisImpl::TrimListData(const std::string& strKey, int nBegin /*= 0*/, int nCount /*= -1*/)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			
			return false;
		}
		RedisReplyPtr pReply = rc->exeCmd("LTRIM %s %d %d", strKey.c_str(), nBegin, nCount);

	_TRY_END_(strKey)

	return true;
}

int NFCRedisImpl::ZRemByRank(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("ZREMRANGEBYRANK %s %lld %lld", strKey.c_str(), nStart, nEnd);
		if (pReply == nullptr)
			return -2;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return 0;

	_TRY_END_(strKey)

	return -1;
}

int NFCRedisImpl::ZgetRangeByScoreByOpenInterval(const std::string& strKey, VecZRangeScoreValue& vecValue, NFINT64 nStart, int nCount, NFINT64 nEnd)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}
		
		RedisReplyPtr pReply = nullptr;
		if (nEnd < 0)
		{
			pReply = rc->exeCmd("ZRANGEBYSCORE %s %lld +inf WITHSCORES LIMIT 0 %d", strKey.c_str(), nStart, nCount);
			if (pReply == nullptr)
				return -2;
		}
		else if (nStart < 0)
		{
			pReply = rc->exeCmd("ZRANGEBYSCORE %s -inf %lld WITHSCORES LIMIT 0 %d", strKey.c_str(), nStart, nCount);
			if (pReply == nullptr)
				return -2;
		}
		else
		{
			pReply = rc->exeCmd("ZRANGEBYSCORE %s %lld %lld WITHSCORES LIMIT 0 %d", strKey.c_str(), nStart, nEnd, nCount);
			if (pReply == nullptr)
				return -2;
		}

		int nNum = 0;
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					std::pair<NFINT64, std::string> vecPair;
					vecPair.second = std::string(pReply->element[k]->str, pReply->element[k]->len);
					std::string szNum = pReply->element[k + 1]->str;
					vecPair.first = lexical_cast<NFINT64>(szNum);

					vecValue.push_back(vecPair);
					nNum++;
				}
			}
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZgetRangeByScore(const std::string& strKey, VecZRangeScoreValue& vecValue, NFINT64 nStart, int nCount, std::string min, std::string max)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("ZRANGEBYSCORE %s %s %s WITHSCORES LIMIT %d %d", strKey.c_str(), min.c_str(), max.c_str(), nStart, nCount);
		if (pReply == nullptr)
			return -2;

		int nNum = 0;
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					std::pair<NFINT64, std::string> vecPair;
					vecPair.second = std::string(pReply->element[k]->str, pReply->element[k]->len);
					std::string szNum = pReply->element[k + 1]->str;
					vecPair.first = lexical_cast<NFINT64>(szNum);

					vecValue.push_back(vecPair);
					nNum++;
				}
			}
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZgetRangeByScore(const std::string& strKey, VecZRangeScoreValue& vecValue, NFINT64 nStart, int nCount, NFINT64 nEnd)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{			
			return -1;
		}

		RedisReplyPtr pReply = nullptr;
		if (nEnd < 0)
		{
			pReply = rc->exeCmd("ZRANGEBYSCORE %s (%lld +inf WITHSCORES LIMIT 0 %d", strKey.c_str(), nStart, nCount);
			if (pReply == nullptr)
				return -2;
		}
		else if (nStart < 0)
		{
			pReply = rc->exeCmd("ZRANGEBYSCORE %s -inf (%lld WITHSCORES LIMIT 0 %d", strKey.c_str(), nStart, nCount);
			if (pReply == nullptr)
				return -2;
		}
		else
		{
			pReply = rc->exeCmd("ZRANGEBYSCORE %s (%lld (%lld WITHSCORES LIMIT 0 %d", strKey.c_str(), nStart, nEnd, nCount);
			if (pReply == nullptr)
				return -2;
		}

		int nNum = 0;
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					std::pair<NFINT64, std::string> vecPair;
					vecPair.second = std::string(pReply->element[k]->str, pReply->element[k]->len);
					std::string szNum = pReply->element[k + 1]->str;
					vecPair.first = lexical_cast<NFINT64>(szNum);

					vecValue.push_back(vecPair);
					nNum++;
				}
			}
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::PopPushListData(const std::string& strKey, std::string& vecContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("BRPOP %s 2", strKey.c_str());
		if (pReply == nullptr)
			return -2;

		if (pReply->type == REDIS_REPLY_NIL)
		{
			return -3;
		}

		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			vecContent = std::string(pReply->element[1]->str, pReply->element[1]->len);
		}

	_TRY_END_(strKey)

	return 0;
}

void NFCRedisImpl::InsertToRListByBinary(const std::string& strKey, const char* buf, size_t buf_size)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return;
		}

		RedisReplyPtr pReply = rc->exeCmd("RPUSH %s %b", strKey.c_str(), buf, buf_size);
		if (pReply == nullptr)
		{
			return;
		}

	_TRY_END_(strKey)
}

int NFCRedisImpl::ZReverRangeValue(const std::string& strKey, std::vector<NFZData>& vecContent, int nRankStart, int nRankEnd)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("ZREVRANGE %s %d %d WITHSCORES", strKey.c_str(), nRankStart, nRankEnd);
		if (pReply == nullptr)
			return -2;

		int nNum = 0;
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					NFZData data;
					data.nScore = lexical_cast<NFINT64>(pReply->element[k + 1]->str);
					data.strContent = std::string(pReply->element[k]->str, pReply->element[k]->len);
					vecContent.push_back(data);
					nNum++;
				}
			}
		}
		return nNum;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZReverRank(const std::string& strKey, const std::string& strName)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("ZREVRANK %s %s", strKey.c_str(), strName.c_str());
		if (pReply == nullptr)
			return -1;

		if (pReply->type == REDIS_REPLY_NIL)
			return -1;

		return (int)pReply->integer;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZModifyScore(const std::string& strKey, const std::string& strName, NFINT64 nScore)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{			
			return -3;
		}

		
		RedisReplyPtr pReply = rc->exeCmd("ZREM %s %s", strKey.c_str(), strName.c_str());
		if (pReply == nullptr)
			return -2;

		pReply = rc->exeCmd("ZADD %s %lld %s", strKey.c_str(), nScore, strName.c_str());
		if (pReply == nullptr)
			return -4;

	_TRY_END_(strKey)

	return 0;
}

NFUINT64 NFCRedisImpl::GetIncrNumber(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("INCR %s", strKey.c_str());
		if (pReply == nullptr)
			return 0;

		return pReply->integer;

	_TRY_END_(strKey)

	return -1;
}

NFUINT64 NFCRedisImpl::GetDecrNumber(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("DECR %s", strKey.c_str());
		if (pReply == nullptr)
			return 0;

		return pReply->integer;

	_TRY_END_(strKey)

	return -1;
}

int NFCRedisImpl::ZRemByScore(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("ZREMRANGEBYSCORE %s %lld %lld", strKey.c_str(), nStart, nEnd);
		if (pReply == nullptr)
			return -2;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return 0;

	_TRY_END_(strKey)

	return -1;
}

int NFCRedisImpl::RemoveListValue(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("LREM %s 0 %s", strKey.c_str(), strContent.c_str());
		if (pReply == nullptr)
			return -2;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::SetMutexKeyWithTime(const std::string& strKey, const std::string& strContent, NFUINT32 time)
{
	int ret = SetMutexVauleByStr(strKey, strContent);
	if (ret == 1)
	{
		SetMutexValueTimeOut(strKey, time);
	}

	return ret;
}

NFUINT64 NFCRedisImpl::SetMutexValueTimeOut(const std::string& strKey, int nTime)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("PEXPIRE %s %d", strKey.c_str(), nTime);
		if (pReply == nullptr)
			return 0;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return pReply->integer;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::HAddValue(const std::string& strKey , const std::string& szFiled , const std::string& strContent )
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str() , strKey.length());
		if( nullptr == rc )
		{
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd( "HSET %s %s %s" , strKey.c_str() , szFiled.c_str(),strContent.c_str() );
		if (pReply == nullptr)
		{
			return -1;
		}

		if(pReply->type == REDIS_REPLY_ERROR)
		{
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

std::string NFCRedisImpl::GetHValueByFiled( const std::string& strKey , const std::string& szFiled )
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if( nullptr == rc )
		{
			return "";
		}

		RedisReplyPtr pReply = rc->exeCmd( "HGET %s %s" , strKey.c_str() , szFiled.c_str() );
		if (pReply == nullptr)
		{
			return "";
		}

		if( pReply->type == REDIS_REPLY_ERROR || pReply->type == REDIS_REPLY_NIL)
		{
			throw pReply->str;
		}

		if( pReply->type == REDIS_REPLY_STRING )
		{
			return pReply->str;
		}

	_TRY_END_(strKey)

	return "";
}

int NFCRedisImpl::HDelValueByFiled( const std::string& strKey , const std::vector<std::string>& vecFiled )
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if( nullptr == rc )
		{
			return -1;
		}

		
		std::string szFiled = "";
		for(size_t k = 0; k < vecFiled.size(); k++)
		{
			szFiled += vecFiled[k];
			szFiled += " ";
		}
		
		RedisReplyPtr pReply = rc->exeCmd( "HDEL %s %s" , strKey.c_str() , szFiled.c_str() );
		if (pReply == nullptr)
		{
			throw "pReply == nullptr";
		}

		if( pReply->type == REDIS_REPLY_ERROR )
		{
			throw pReply->str;
		}

	_TRY_END_(strKey)

	return 0;

}

int NFCRedisImpl::HDelValueByFiled( const std::string& strKey , const std::string& szFiled )
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if( nullptr == rc )
		{
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd( "HDEL %s %s" , strKey.c_str() , szFiled.c_str() );
		if (pReply == nullptr)
		{
			throw "pReply == nullptr";
		}

		if( pReply->type == REDIS_REPLY_ERROR )
		{
			throw pReply->str;
		}

	_TRY_END_(strKey)

	return 0;
}

std::string NFCRedisImpl::GetKeyType( const std::string& strKey )
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if( nullptr == rc )
		{
			return "";
		}
	
		RedisReplyPtr pReply = rc->exeCmd( "TYPE %s" , strKey.c_str() );
		if (pReply == nullptr)
		{
			throw "pReply == nullptr";
		}

		if( pReply->type == REDIS_REPLY_ERROR )
		{
			throw pReply->str;
		}

		if( pReply->type == REDIS_REPLY_STATUS )
			return pReply->str;

	_TRY_END_(strKey)

	return "";
}

bool NFCRedisImpl::exeCmdPipe(std::string& strCMD, std::vector<std::pair<std::string, std::string>>& vecKeyValue)
{
	RedisClientPtr rs = nullptr;

	_TRY_BEGIN_

		std::map<RedisClientPtr, std::vector<std::string>> mapKey;
	for (auto &it : vecKeyValue)
	{
		std::string& strKey = it.second;

		rs = getRedisConnect(strKey.c_str(), strKey.length());

		if (strCMD == "ZSCORE")
		{
			strKey.append(" " + it.first);
		}

		mapKey[rs].push_back(it.first);
		rs->exeCmdInPipe(strCMD + " " + strKey);
	}

	vecKeyValue.clear();
	for (auto& it : mapKey)
	{
		rs = it.first;
		rs->getReplyPipe(it.second, vecKeyValue);
	}

	return true;

	_TRY_END_(strCMD)

		return false;
}

void NFCRedisImpl::GetAllClient(std::list<RedisClientPtr>& vecClient )
{
	for( auto& ref : _client )
	{
		bool bFind = false;
		for(auto& reflist :vecClient)
		{
			if( reflist.get() == ref.get() )
			{
				bFind = true;
				break;
			}
		}

		if (!bFind)
		{
			vecClient.push_back(ref);
		}
	}
}

void NFCRedisImpl::GetSingleAllKeys( RedisClientPtr ptrClient , const std::string& pattan , std::list<std::string>& szKeys )
{
	RedisReplyPtr pReply = ptrClient->exeCmd( "keys %s" , pattan.c_str() );
	if( pReply != nullptr&&pReply->type == REDIS_REPLY_ARRAY )
	{
		for(size_t k = 0; k < pReply->elements; k++ )
		{
			if( pReply->element[k]->type == REDIS_REPLY_STRING )
			{
				szKeys.push_back( std::string( pReply->element[k]->str , pReply->element[k]->len ) );
			}
		}
	}
}

int NFCRedisImpl::LPushEx(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return 0;
		}
	
		RedisReplyPtr pReply = rc->exeCmd("LPUSHX %s %s", strKey.c_str(), strContent.c_str());
		if (pReply == nullptr)
			return 0;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return (int)pReply->integer;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::LPushEx(const std::string& strKey, char* pBuff, int wSize)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("LPUSHX %s %b", strKey.c_str(), pBuff, wSize);
		if (pReply == nullptr)
		{
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

void NFCRedisImpl::ZsetValueByBinary(const std::string& strKey, NFINT64 nScore, const std::string& member)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return;
		}

		std::vector<std::string> vecStrData;
		vecStrData.push_back("ZADD");
		vecStrData.push_back(strKey);
		vecStrData.push_back(std::to_string(nScore));
		vecStrData.push_back(member);

		std::vector<const char*> argv(vecStrData.size());
		std::vector<size_t> argvlen(vecStrData.size());
		unsigned int j = 0;
		for (auto it = vecStrData.begin(); it != vecStrData.end(); ++it, ++j)
		{
			argv[j] = it->c_str();
			argvlen[j] = it->size();
		}
		RedisReplyPtr pReply = rc->exeCmd((int)argv.size(), &(argv[0]), &(argvlen[0]));
		if (pReply == nullptr)
		{
			return;
		}

	_TRY_END_(strKey)
}

void NFCRedisImpl::ZsetValue( const std::string& strKey, const NFINT64 nScore, const std::string& strName )
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if( nullptr == rc )
		{
			return;
		}
		
		RedisReplyPtr pReply = rc->exeCmd( "ZADD %s %lld %s" , strKey.c_str(), nScore,strName.c_str() );
		if( pReply == nullptr )
			return;

		if(pReply->type == REDIS_REPLY_ERROR)
		{
			throw pReply->str;
		}

		return;

	_TRY_END_(strKey)

	return;
}

int NFCRedisImpl::RemoveKeyTime(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("PERSIST %s", strKey.c_str());
		if (pReply == nullptr)
			return 0;
		
	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::SAddValueStr(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
	if (nullptr == rc)
	{
		return 0;
	}

	RedisReplyPtr pReply = rc->exeCmd("SADD %s %s", strKey.c_str(), strContent.c_str());
	if (pReply == nullptr)
		return 0;

	if (pReply->type == REDIS_REPLY_INTEGER)
		return (int)pReply->integer;

	_TRY_END_(strKey)

		return 0;
}

int NFCRedisImpl::SAddValueByBinary(const std::string& strKey, const char* buf, size_t buf_size)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("SADD %s %b", strKey.c_str(), buf, buf_size);
		if (pReply == nullptr)
		{
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::SMemberValue(const std::string& strKey, std::string *pszContent, int& nSize)
{
	_TRY_BEGIN_

		if (nSize == 0)
			return 0;

		int nSizeTmp = nSize;
		nSize = 0;
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return 0;
		}

		int nCount = 0;

		RedisReplyPtr pReply = rc->exeCmd("SMEMBERS %s", strKey.c_str());
		if (pReply == nullptr)
			return 0;

		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (int k = 0; k < (int)pReply->elements && k < nSizeTmp; k++)
			{
				pszContent[nCount++] = std::string(pReply->element[k]->str, pReply->element[k]->len);
				if (nCount >= nSizeTmp)
					break;
			}
		}

		nSize = nCount;
		return nCount;

	_TRY_END_(strKey)

	nSize = 0;

	return 0;
}

int NFCRedisImpl::SetValueByString(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_
		
		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}
	
		RedisReplyPtr pReply = rc->exeCmd("SET %s %s", strKey.c_str(), strContent.c_str());
		if (pReply == nullptr)
			return 0;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::Unsubscribe(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}
	
		RedisReplyPtr pReply = rc->exeCmd("UNSUBSCRIBE %s", strKey.c_str());
		if (pReply == nullptr)
			return 0;

	_TRY_END_(strKey)

	return 0;
}

NFUINT64 NFCRedisImpl::SetKeyTime(const std::string& strKey, NFUINT32 dwExpiredTime)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("EXPIRE %s %d", strKey.c_str(), dwExpiredTime);
		if (pReply == nullptr)
			return 0;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return pReply->integer;

	_TRY_END_(strKey)

	return 0;
}

NFUINT64 NFCRedisImpl::GetKeyTime(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{			
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("TTL %s", strKey.c_str());
		if (pReply == nullptr)
			return 0;

		if (pReply->type == REDIS_REPLY_INTEGER)
		{
			// 如果key不存在或者已过期,返回-2,如果key存在并且没有设置过期时间(永久有效),返回 -1 
			if (pReply->integer == -2)		
			{
				return 0;
			}
		}
		return pReply->integer;

	_TRY_END_(strKey)

	return 0;
}

bool NFCRedisImpl::SisMember(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return false;
		}

		RedisReplyPtr pReply = rc->exeCmd("SISMEMBER %s %s", strKey.c_str(), strContent.c_str());
		if (pReply == nullptr)
			return false;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return (int)pReply->integer > 0 ? true : false;

	_TRY_END_(strKey)

	return false;
}

int NFCRedisImpl::SremMember(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}
	
		RedisReplyPtr pReply = rc->exeCmd("SREM %s %s", strKey.c_str(), strContent.c_str());
		if (pReply == nullptr)
			return 0;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return (int)pReply->integer > 0 ? true : false;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::PublishStr(const std::string& strKey, const std::string& strContent)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("PUBLISH %s %s", strKey.c_str(), strContent.c_str());
		if (pReply == nullptr)
			return 0;

	_TRY_END_(strKey)

		return 0;
}

int NFCRedisImpl::PublishBinary(const std::string& strKey, const char* buf, size_t buf_size)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pReply = rc->exeCmd("PUBLISH %s %b", strKey.c_str(), buf, buf_size);
		if (pReply == nullptr)
		{
			return -1;
		}

	_TRY_END_(strKey)

	return 0;
}

NFINT64 NFCRedisImpl::GetIncrByNumber(const std::string& strKey, NFINT64 n)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("INCRBY %s %lld", strKey.c_str(), n);
		if (pReply == nullptr)
			return 0;

		return pReply->integer;

	_TRY_END_(strKey)

	return 0;
}

std::string NFCRedisImpl::GetSetValue(const std::string& strKey, const std::string& szValue)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("GETSET %s %s", strKey.c_str(), szValue.c_str());
		if (pReply == nullptr)
			return 0;

		return pReply->str;

	_TRY_END_(strKey)

	return "";
}

NFINT64 NFCRedisImpl::GetSetValueInt(const std::string& strKey, NFINT64 szValue)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("GETSET %s %lld", strKey.c_str(), szValue);
		if (pReply == nullptr)
			return 0;

		if (pReply->type == REDIS_REPLY_STRING)
		{
			return lexical_cast<NFINT64>(pReply->str);
		}

	_TRY_END_(strKey)

	return 0;
}

NFUINT64 NFCRedisImpl::GeoAdd(const std::string& strKey, double longitude, double latitude, const std::string& strName)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return 0;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("GEOADD %s %e %e %s", strKey.c_str(), (float)longitude, (float)latitude, strName.c_str());
		if (pReply == nullptr)
			return 0;

		return pReply->integer;

	_TRY_END_(strKey)

	return 0;
}

double NFCRedisImpl::GeoDist(const std::string& strKey, const std::string& strMember_1, const std::string& strMember_2)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return -1.0f;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("GEODIST %s %s %s m", strKey.c_str(), strMember_1.c_str(), strMember_2.c_str());
		if (pReply == nullptr)
			return -1.0f;

		if (pReply->type == REDIS_REPLY_NIL)
			return -1.0f;

		if (pReply->str[0] == '\0')
			return -1;

		return lexical_cast<double>(pReply->str);

	_TRY_END_(strKey)

	return -1.0f;
}

int NFCRedisImpl::GeoRedius(const std::string& strKey, double longitude, double latitude, int dist, std::vector<NFGeoRediusInfo>& vGeoInfo, int nMaxCount)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("GEORADIUS %s %e %e %d m WITHDIST WITHCOORD COUNT %d", strKey.c_str(), longitude, latitude, dist, nMaxCount);
		if (pReply == nullptr)
		{
			return -1;
		}

		if (pReply->type != REDIS_REPLY_ARRAY)
		{
			return -1;
		}

		for (size_t k = 0; k < pReply->elements; k++)
		{
			NFGeoRediusInfo info;
			redisReply *pRel = pReply->element[k];
			info.strMember = pRel->element[0]->str;
			info.fDistance = lexical_cast<double>(pRel->element[1]->str);
			info.vPosion.longitude = lexical_cast<double>(pRel->element[2]->element[0]->str);
			info.vPosion.latitude = lexical_cast<double>(pRel->element[2]->element[1]->str);
			vGeoInfo.push_back(info);
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::GeoPos(const std::string& strKey, const std::string& strMember, NFGeoRediusInfo::stPosion& vPos)
{
	_TRY_BEGIN_

		vPos.latitude = 10000.0f;
		vPos.longitude = 10000.0f;

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return -1;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("GEOPOS %s %s", strKey.c_str(), strMember.c_str());
		if (pReply == nullptr)
			return -1;

		if (pReply->type == REDIS_REPLY_NIL)
			return -1;

		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k++)
			{
				redisReply* pRedisReply = pReply->element[k];
				if (pRedisReply->type == REDIS_REPLY_ARRAY)
				{
					vPos.longitude = lexical_cast<double>(pRedisReply->element[0]->str);
					vPos.latitude = lexical_cast<double>(pRedisReply->element[1]->str);
				}
			}
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::GeoHashCode(const std::string& strKey, const std::string& strMember, std::string&sResult)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("GEOHASH %s %s", strKey.c_str(), strMember.c_str());
		if (pReply != nullptr && pReply->type == REDIS_REPLY_ARRAY && pReply->elements == 1)
		{
			if (pReply->element[0]->type == REDIS_REPLY_STRING)
			{
				sResult = pReply->element[0]->str;
				return 0;
			}
		}

	_TRY_END_(strKey)

	return -1;
}

int NFCRedisImpl::SetMutexValueInt(const std::string& strKey, NFINT64 nID)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("SETNX %s %lld", strKey.c_str(), nID);
		if (pReply == nullptr)
			return -1;

		if (pReply->type == REDIS_REPLY_INTEGER)
			return (int)pReply->integer;
	
	_TRY_END_(strKey)

	return -2;
}

bool NFCRedisImpl::SPopValue(const std::string& strKey, std::string& strResult)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return false;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("SPOP %s %d", strKey.c_str(), 1);
		if (pReply == nullptr)
			return false;

		if (pReply->type == REDIS_REPLY_NIL)
			return false;

		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			if (pReply->elements != 1)
				return false;

			strResult = pReply->element[0]->str;
		}

	_TRY_END_(strKey)

	return true;
}

int NFCRedisImpl::SAddValueInt(const std::string& strKey, NFINT64 nID)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("SADD %s %lld", strKey.c_str(), nID);
		if (pReply == nullptr)
			return -1;
		return (int)pReply->integer;
		
	_TRY_END_(strKey)

	return -1;
}

int NFCRedisImpl::SGetCount(const std::string& strKey)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}
		
		RedisReplyPtr pReply = rc->exeCmd("SCARD %s", strKey.c_str());
		if (pReply == nullptr)
			return -1;
		return (int)pReply->integer;

	_TRY_END_(strKey)

	return -1;
}

int NFCRedisImpl::ZRangeLex(const std::string& strKey, const std::string& strMin, const std::string& strMax, int nStart, int nCount, std::array<std::string, 200>& arrString)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZRANGEBYLEX %s [%s [%s LIMIT %d %d", strKey.c_str(), strMin.c_str(), strMax.c_str(), nStart, nCount);
		if (pRedisReply == nullptr)
		{		
			return 0;
		}

		if (pRedisReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pRedisReply->elements && k < 200; k++)
			{
				arrString[k] = pRedisReply->element[k]->str;
			}
		}

		return (int)pRedisReply->elements;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZLexCount(const std::string& strKey, const std::string& strMin, const std::string& strMax)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{
			
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZLEXCOUNT %s [%s [%s", strKey.c_str(), strMin.c_str(), strMax.c_str());
		if (pRedisReply == nullptr)
		{
			
			return 0;
		}

		return (int)pRedisReply->integer;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZRemLex(const std::string& strKey, const std::string& strMin, const std::string& strMax)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZREMRANGEBYLEX %s [%s [%s", strKey.c_str(), strMin.c_str(), strMax.c_str());
		if (pRedisReply == nullptr)
		{		
			return 0;
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZRank(const std::string& strKey, const std::string& strMember)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return -1;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZRANK %s %s", strKey.c_str(), strMember.c_str());
		if (pRedisReply == nullptr)
		{		
			return -1;
		}

		if (pRedisReply->type == REDIS_REPLY_NIL)
			return -1;
		return (int)pRedisReply->integer;

	_TRY_END_(strKey)

	return -1;
}

size_t NFCRedisImpl::ZCommonRangeWithoutScore(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd, std::string* szContent, size_t size)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZRANGE %s %lld %lld", strKey.c_str(), nStart, nEnd);
		if (pRedisReply == nullptr)
		{	
			return 0;
		}

		if (pRedisReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pRedisReply->elements && k < size; k++)
			{
				if (pRedisReply->element[k]->type == REDIS_REPLY_STRING)
				{
					szContent[k] = pRedisReply->element[k]->str;
				}
			}
		}

		return pRedisReply->elements > size ? size : pRedisReply->elements;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZReverRangeByScore(const std::string& strKey, VecZRangeScoreValue& vecValue, int offset /*= 0*/, int nCount /*= -1*/, std::string max /*= "+inf"*/, std::string min /*= "-inf"*/)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return 0;
		}

		RedisReplyPtr pRedisReply = rc->exeCmd("ZREVRANGEBYSCORE %s %s %s WITHSCORES LIMIT %d %d", strKey.c_str(), max.c_str(), min.c_str(), offset, nCount);
		if (pRedisReply == nullptr)
		{		
			return 0;
		}

		if (pRedisReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pRedisReply->elements; k = k + 2)
			{
				if (pRedisReply->element[k]->type == REDIS_REPLY_STRING)
				{
					std::pair<NFINT64, std::string> vecPair;
					vecPair.second = std::string(pRedisReply->element[k]->str, pRedisReply->element[k]->len);
					vecPair.first = lexical_cast<NFINT64>(pRedisReply->element[k + 1]->str);
					vecValue.push_back(vecPair);
				}
			}
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ZCount(const std::string& strKey, std::string min, std::string max)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{		
			return 0;
		}

		int nCount = 0;

		RedisReplyPtr pRedisReply = rc->exeCmd("ZCOUNT %s %s %s", strKey.c_str(), min.c_str(), max.c_str());
		if (pRedisReply == nullptr)
		{	
			return 0;
		}

		if (pRedisReply->type == REDIS_REPLY_INTEGER)
		{
			nCount = (int)pRedisReply->integer;
		}

		return nCount;

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ListGetByIndex(const std::string& strKey, std::string& strResult, int nIndex)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{			
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("LINDEX %s %d", strKey.c_str(), nIndex);
		if (pReply == nullptr)
			return -1;

		if (pReply->type == REDIS_REPLY_NIL)
			return -2;

		if(pReply->type == REDIS_REPLY_ERROR)
			return -3;

		if (pReply->type == REDIS_REPLY_STRING)
		{
			strResult = std::string(pReply->str,pReply->len);
		}

	_TRY_END_(strKey)

	return 0;
}

int NFCRedisImpl::ListModifyValueByIndex(const std::string& strKey, const std::string& strContent, int nIndex)
{
	_TRY_BEGIN_

		RedisClientPtr rc = getRedis(strKey.c_str(), strKey.length());
		if (nullptr == rc)
		{	
			return -1;
		}

		RedisReplyPtr pReply = rc->exeCmd("LSET %s %d %s", strKey.c_str(), nIndex, strContent.c_str());
		if (pReply == nullptr)
			return -1;

		if (pReply->type == REDIS_REPLY_ERROR)
			return -2;

	_TRY_END_(strKey)

	return 0;
}

