//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisResult.h"

NFRedisResult::NFRedisResult(NFRedisClientSocket *pClientSocket)
{
    m_pClientSocket = pClientSocket;
}

bool NFRedisResult::ReadRespType()
{
	char c = 0;
	m_pClientSocket->TryPredictType(c);

	mxRespType = GetRespType(c);

	return false;
}

bool NFRedisResult::ReadBuff()
{
	bool bRet = false;
	switch (mxRespType)
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		bRet = ReadForStatus();
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
		bRet = ReadForError();
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		bRet = ReadForInt();
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		bRet = ReadForBulk();
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		bRet = ReadForArray();
		break;
	default:
		break;
	}

	return true;
}

bool NFRedisResult::DeSerializeBuff()
{
	{
		bool bRet = false;
		/*
		switch (mxRespType)
		{
		case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
			bRet = DeSerializeForStatus(mstrMsgValue);
			break;
		case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
			bRet = DeSerializeForError(mstrMsgValue);
			break;
		case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
			bRet = DeSerializeForInt(mstrMsgValue);
			break;
		case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
			bRet = DeSerializeForBulk(mstrMsgValue);
			break;
		case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
			//bRet = DeSerializeForArray(mstrMsgValue);--to save memory
			break;
		default:
			bRet = false;
			break;
		}
		*/

		return true;
	}

	return false;
}
bool NFRedisResult::ReadReply()
{
    //1 get resptype---readN
    //2 get arg count
    //3 get data(for or wait---read line)

	ReadRespType();
	ReadBuff();
	DeSerializeBuff();



	return false;
}

void NFRedisResult::SetCommand(const std::string & str)
{
	mstrCommand = str;
}

const std::string & NFRedisResult::GetCommand() const
{
	return mstrCommand;
}

NFREDIS_RESP_TYPE NFRedisResult::GetRespType() const
{
    return mxRespType;
}

bool NFRedisResult::IsOKRespStatus() const
{
    if (NFREDIS_STATUS_OK == mstrRespValue)
    {
        return true;
    }

    return false;
}

std::int64_t NFRedisResult::GetRespInt() const
{
	return mnRespValue;
}

int NFRedisResult::GetRespBulkLen() const
{
	return mnRespBulkLen;
}

std::string NFRedisResult::GetRespString() const 
{
    return mstrRespValue;
}

float NFRedisResult::GetRespFloat() const
{
	return atof(mstrRespValue.data());
}

double NFRedisResult::GetRespDouble() const
{
	return atof(mstrRespValue.data());
}

const std::vector<NFRedisResult>& NFRedisResult::GetRespArray()
{
    return mxRespList;
}

NFREDIS_RESP_TYPE NFRedisResult::GetRespType(const char strRes)
{
    if (*NFREDIS_STATUS_REPLY == strRes)
    {
        return NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS;
    }
    else if (*NFREDIS_ERROR_REPLY == strRes)
    {
        return NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR;
    }
    else if (*NFREDIS_INT_REPLY == strRes)
    {
        return NFREDIS_RESP_TYPE::NFREDIS_RESP_INT;
    }
    else if (*NFREDIS_BULK_REPLY == strRes)
    {
        return NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK;
    }
    else if (*NFREDIS_ARRAY_REPLY == strRes)
    {
        return NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY;
    }

    return NFREDIS_RESP_UNKNOW;
}

bool NFRedisResult::DeSerializeForStatus(const std::string &strRes, std::string& status)
{
	char type = strRes[0];
	if (*NFREDIS_STATUS_REPLY == type)
	{
		int len = 0;
		const char* p = strRes.c_str();
		while (*p != '\r')
		{
			len++;
			p++;
		}

		if (len <= 1)
		{
			return false;
		}

		status = strRes.substr(1, len - 1);

		int nTotalLength = len + NFREDIS_SIZEOF_CRLF;
		if (nTotalLength == strRes.length())
		{
			return true;
		}
	}

	return false;
}

bool NFRedisResult::DeSerializeForInt(const std::string &strRes, int64_t& n)
{
	 char type = strRes[0];
	 if (*NFREDIS_INT_REPLY == type)
	 {
		 int len = 0;
		 const char* p = strRes.c_str();
		 while (*p != '\r')
		 {
			 len++;
			 p++;
		 }

		 if (len <= 1)
		 {
			 return false;
		 }

		 std::string strRespValue = strRes.substr(1, len - 1);
		 n = atoi(strRespValue.data());

		 int nTotalLength = len + NFREDIS_SIZEOF_CRLF;
		 if (nTotalLength == strRes.length())
		 {
			 return true;
		 }
	 }

	return false;
}

bool NFRedisResult::DeSerializeForBulkHead(const std::string & strRes, int & n)
{
	char type = strRes[0];
	if (*NFREDIS_BULK_REPLY == type)
	{
		int len = 0;
		const char* p = strRes.c_str();
		while (*p != '\r')
		{
			len++;
			p++;
		}

		if (len <= 1)
		{
			return false;
		}

		std::string strRespValue = strRes.substr(1, len - 1);
		n = atoi(strRespValue.data());

		int nTotalLength = len + NFREDIS_SIZEOF_CRLF;
		if (nTotalLength == strRes.length())
		{
			return true;
		}
	}
}

bool NFRedisResult::DeSerializeForError(const std::string &strRes, std::string& error)
{
	char type = strRes[0];
	if (*NFREDIS_ERROR_REPLY == type)
	{
		int len = 0;
		const char* p = strRes.c_str();
		while (*p != '\r')
		{
			len++;
			p++;
		}

		if (len <= 1)
		{
			return false;
		}

		error = strRes.substr(1, len - 1);

		int nTotalLength = len + NFREDIS_SIZEOF_CRLF;
		if (nTotalLength == strRes.length())
		{
			return true;
		}
	}

	return false;
}

bool NFRedisResult::DeSerializeForBulk(const std::string &strRes, std::string& bulk)
{
	char type = strRes[0];
	if (*NFREDIS_BULK_REPLY == type)
	{
		int len = 0;
		const char* p = strRes.c_str();
		while (*p != '\r')
		{
			len++;
			p++;
		}

		if (len <= 1)
		{
			return false;
		}

		std::string respValueLength = strRes.substr(1, len - 1);
		const int nValuelen = atoi(respValueLength.c_str());
		if (nValuelen > 0)
		{
			int nTotalLength = nValuelen + len + NFREDIS_SIZEOF_CRLF * 2;

			if (nTotalLength == strRes.length())
			{
				bulk = strRes.substr(len + NFREDIS_SIZEOF_CRLF, nValuelen);

				return true;
			}
		}
		else if (nValuelen == 0)
		{
			return true;
		}
		else if(nValuelen < 0)
		{
			return true;
		}

	}

	return false;
}

bool NFRedisResult::DeSerializeForArrayHead(const std::string& strRes, int& n)
{
	char type = strRes[0];
	if (*NFREDIS_ARRAY_REPLY == type)
	{
		int len = 0;
		const char* p = strRes.c_str();
		while (*p != '\r')
		{
			len++;
			p++;
		}

		if (len <= 1)
		{
			return false;
		}

		std::string strRespValue = strRes.substr(1, len - 1);
		n = atoi(strRespValue.data());

		int nTotalLength = len + NFREDIS_SIZEOF_CRLF;
		if (nTotalLength == strRes.length())
		{
			return true;
		}
	}
}

void NFRedisResult::Reset()
{
	mnRespValue = 0;
	mnRespBulkLen = 0;
    mxResultStatus = NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_UNKNOW;
    mxRespType = NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW;
    mstrRespValue.clear();
    mxRespList.clear();
    mstrMsgValue.clear();
}

NFREDIS_RESULT_STATUS NFRedisResult::GetResultStatus()
{
    return mxResultStatus;
}

bool NFRedisResult::ReadForBulk()
{
	std::string line1;
	if (m_pClientSocket->ReadLine(line1))
	{
		mstrMsgValue.append(line1);

		mnRespBulkLen = 0;
		if (DeSerializeForBulkHead(line1, mnRespBulkLen))
		{
			
			if (mnRespBulkLen > 0)
			{
				//$6\r\nfoobar\r\n"
				std::string line2;
				if (m_pClientSocket->ReadLine(line2))
				{
					mstrMsgValue.append(line2);
					if (DeSerializeForBulk(mstrMsgValue, mstrRespValue))
					{
						return true;
					}
				}
			}
			else if (mnRespBulkLen == 0)
			{
				std::string line2;
				m_pClientSocket->ReadLine(line2);

				return true;
			}
			else if (mnRespBulkLen < 0)
			{
				//"$-1\r\n"
				return true;
			}
		}
	}

    return false;
}

bool NFRedisResult::ReadForArray()
{
	//read the data length
	int nNowElementNum = 0;
	int nTotalElementNum = 0;

	std::string line1;
	if (m_pClientSocket->ReadLine(line1))
	{
		mstrMsgValue.append(line1);

		int nlen = 0;
		if (DeSerializeForArrayHead(mstrMsgValue, nlen))
		{
			nNowElementNum = nlen;
			nTotalElementNum = nlen;
			for (int i = 0; i < nlen; ++i)
			{

			}
		}

		return false;
	}

	/*
	switch (mxBulkStatus.mState)
	{
		case ReadBuffState::UNSTART:
		{
			//read the data length
			std::string line1;
			if (m_pClientSocket->ReadLine(line1))
			{
				mstrMsgValue.append(line1);
				mxArrayStatus.mState = ReadBuffState::WORKING;

				int64_t nlen = 0;
				if (DeSerializeForInt(mstrMsgValue, nlen))
				{
					mxArrayStatus.mnNowElementNum = nlen;
					mxArrayStatus.mnTotalElementNum = nlen;
				}

				return false;
			}
		}
		break;
		case ReadBuffState::WORKING:
		{
			if (mxArrayStatus.mnNowElementNum <= 0)
			{
				if (mxArrayStatus.mnTotalElementNum <= 0)
				{
					//"*0\r\n"
					mxArrayStatus.mState = ReadBuffState::FINISHED;
				}
				else
				{
					//ready


				}
				return true;
			}
			else if (mxArrayStatus.mnNowElementNum > 0)
			{
				std::string line1;
				if (m_pClientSocket->ReadLine(line1))
				{
					mstrMsgValue.append(line1);
					mxArrayStatus.mnNowElementNum--;
				}

				if (mxArrayStatus.mnNowElementNum <= 0)
				{

				}
			}
		}
		break;
		case ReadBuffState::FINISHED:
		{
			return true;
		}
		break;
		default:
			break;
	}

    std::string line1;
    if (m_pClientSocket->ReadLine(line1))
    {
        mstrMsgValue.append(line1);

        NFRedisResult xResultInt(m_pClientSocket);
        //if (xResultInt.DeSerializeForInt(mstrMsgValue))
        {
            int nArray = xResultInt.GetRespInt();
            if (nArray > 0)
            {
                for (int i = 0; i < nArray; ++i)
                {
                    std::string dataLen;
                    if (m_pClientSocket->ReadLine(dataLen))
                    {
                        mstrMsgValue.append(dataLen);

                        //判断头，是否与int类型
                        std::string data;
                        if (m_pClientSocket->ReadLine(data))
                        {
                            mstrMsgValue.append(data);

                            std::string bulkData;
                            bulkData.append(dataLen);
                            bulkData.append(data);

                            NFRedisResult xResultBulk(m_pClientSocket);
                            //if (xResultBulk.DeSerializeForBulk(bulkData))
                            {
                                mxRespList.push_back(xResultBulk);
                            }
                        }
                    }
                }

                return true;
            }
        }
    }
	*/
    return false;
}


bool NFRedisResult::ReadForStatus()
{
	std::string line;
	if (m_pClientSocket->ReadLine(line))
	{
		mstrMsgValue.append(line);

		if (DeSerializeForStatus(mstrMsgValue, mstrRespValue))
		{
			return true;
		}
	}

	return false;
}

bool NFRedisResult::ReadForError()
{
	std::string line;
	if (m_pClientSocket->ReadLine(line))
	{
		mstrMsgValue.append(line);

		if (DeSerializeForError(mstrMsgValue, mstrRespValue))
		{
			return true;
		}
	}

	return false;
}

bool NFRedisResult::ReadForInt()
{
	std::string line;
	if (m_pClientSocket->ReadLine(line))
	{
		mstrMsgValue.append(line);

		if (DeSerializeForInt(mstrMsgValue, mnRespValue))
		{
			return true;
		}
	}

	return false;
}