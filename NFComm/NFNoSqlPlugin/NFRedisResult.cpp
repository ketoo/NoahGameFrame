//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisResult.h"

NFRedisResult::NFRedisResult(NFRedisClientSocket *pClientSocket)
{
    m_pClientSocket = pClientSocket;
}

bool NFRedisResult::ReadReply()
{
    Reset();
    //1 get resptype---readN
    //2 get arg count
    //3 get data(for or wait---read line)

    mxResultStatus = NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_UNKNOW;

	char chType[1] = {0};
    m_pClientSocket->ReadN(chType, 1);
    mstrMsgValue.append(chType, 1);
    mxRespType = GetRespType(chType[0]);



    mxResultStatus = NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_IMCOMPLETE;

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

    if (!bRet)
    {
        return false;
    }

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

    mxResultStatus = NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK;

    if (!bRet)
    {
        return false;
    }

    return true;
}

void NFRedisResult::SetCommand(const std::string & str)
{
	mstrCommand = str;
}

const std::string & NFRedisResult::GetCommand()
{
	return mstrCommand;
}

NFREDIS_RESP_TYPE NFRedisResult::GetRespType()
{
    return mxRespType;
}

bool NFRedisResult::IsOKRespStatus()
{
    if (NFREDIS_STATUS_OK == mstrRespValue)
    {
        return true;
    }

    return false;
}

std::int64_t NFRedisResult::GetRespInt()
{
    return atoi(mstrRespValue.data());
}

std::string NFRedisResult::GetRespString()
{
    return mstrRespValue;
}

float NFRedisResult::GetRespFloat()
{
    return 0;
}

double NFRedisResult::GetRespDouble()
{
    return 0;
}

const std::vector<NFRedisResult>& NFRedisResult::GetRespArray()
{
    return mxRespList;
}

int NFRedisResult::DeSerializeForBulk(const std::string &strRes)
{
    char type = strRes[0];
    if (*NFREDIS_INT_REPLY == type)
    {
        return DeSerializeForInt(strRes);
    }
    else if (*NFREDIS_BULK_REPLY == type)
    {
        int len = 0;
        const char* p = strRes.c_str();
        while(*p != '\r')
        {
            len++;
            p++;
        }

        if (len <= 1)
        {
            return 0;
        }

        std::string respValueLength = strRes.substr(1, len-1);
        int nValuelen = atoi(respValueLength.c_str());
        if (nValuelen > 0)
        {
            int nTotalLength = nValuelen + len + NFREDIS_SIZEOF_CRLF * 2;

            if (nTotalLength == strRes.length())
            {
                mstrRespValue = strRes.substr(len+ NFREDIS_SIZEOF_CRLF, nValuelen);

                mxRespType = NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK;
            }
            //start,length

            return nTotalLength;
        }

    }

    return 0;
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

int NFRedisResult::DeSerializeForStatus(const std::string &strRes)
{
    int len = 0;
    const char* p = strRes.c_str();
    while(*p != '\r')
    {
        len++;
        p++;
    }

    if (len <= 1)
    {
        return 0;
    }

    mstrRespValue = strRes.substr(1, len-1);

    int nTotalLength = len + NFREDIS_SIZEOF_CRLF;
    if (nTotalLength == strRes.length())
    {
        mxRespType = NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS;
    }

    return nTotalLength;
}

int NFRedisResult::DeSerializeForInt(const std::string &strRes)
{
    int len = 0;
    const char* p = strRes.c_str();
    while(*p != '\r')
    {
        len++;
        p++;
    }

    if (len <= 1)
    {
        return 0;
    }

    mstrRespValue = strRes.substr(1, len-1);

    int nTotalLength = len + NFREDIS_SIZEOF_CRLF;
    if (nTotalLength == strRes.length())
    {
        mxRespType = NFREDIS_RESP_TYPE::NFREDIS_RESP_INT;
    }

    return nTotalLength;
}

int NFRedisResult::DeSerializeForError(const std::string &strRes)
{
    mstrRespValue = strRes.substr(1);
    mxRespType = NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR;

    return 1;
}

void NFRedisResult::Reset()
{
    mxResultStatus = NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_UNKNOW;
    mxRespType = NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW;
    mstrRespValue.clear();
    mxRespList.clear();
    mstrMsgValue.clear();
}

NFREDIS_RESULT_STATUS NFRedisResult::GetResultType()
{
    return mxResultStatus;
}

bool NFRedisResult::ReadForStatus()
{
    std::string line;
    if (m_pClientSocket->ReadLine(line))
    {
        mstrMsgValue.append(line);

        return true;
    }

    return false;
}

bool NFRedisResult::ReadForError()
{
    std::string line;
    if (m_pClientSocket->ReadLine(line))
    {
        mstrMsgValue.append(line);

        return true;
    }

    return false;
}

bool NFRedisResult::ReadForInt()
{
    std::string line;
    if (m_pClientSocket->ReadLine(line))
    {
        mstrMsgValue.append(line);

        return true;
    }

    return false;
}

bool NFRedisResult::ReadForBulk()
{
    std::string line1;
    if (m_pClientSocket->ReadLine(line1))
    {
        mstrMsgValue.append(line1);

        NFRedisResult xResultInt(m_pClientSocket);
        if (xResultInt.DeSerializeForInt(mstrMsgValue))
        {
            if (xResultInt.GetRespInt() > 0)
            {
                std::string line2;
                if (m_pClientSocket->ReadLine(line2))
                {
                    mstrMsgValue.append(line2);

                }
            }

            return true;
        }
    }

    return false;
}

bool NFRedisResult::ReadForArray()
{
    std::string line1;
    if (m_pClientSocket->ReadLine(line1))
    {
        mstrMsgValue.append(line1);

        NFRedisResult xResultInt(m_pClientSocket);
        if (xResultInt.DeSerializeForInt(mstrMsgValue))
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
                            if (xResultBulk.DeSerializeForBulk(bulkData))
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

    return false;
}

