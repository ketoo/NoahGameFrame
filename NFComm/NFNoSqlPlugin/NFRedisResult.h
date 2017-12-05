//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#ifndef NFREDISPLUGIN_NFREDISRESULT_H
#define NFREDISPLUGIN_NFREDISRESULT_H

#include <iostream>
#include <vector>
#include <list>
#include "NFRedisProtocolDefine.h"
#include "NFRedisClientSocket.h"


class NFRedisResult
{
public:
    NFRedisResult(NFRedisClientSocket* pClientSocket);

    void Reset();

    bool ReadReply();

	void SetCommand(const std::string& str);
	const std::string& GetCommand();

    NFREDIS_RESULT_STATUS GetResultType();
    NFREDIS_RESP_TYPE GetRespType();

    //if not ok ...
    bool IsOKRespStatus();

    //get response value
    std::int64_t GetRespInt();
    std::string GetRespString();
    float GetRespFloat();
    double GetRespDouble();
    const std::vector<NFRedisResult>& GetRespArray();

private:
    NFREDIS_RESP_TYPE GetRespType(const char strRes);

    bool ReadForStatus();
    bool ReadForError();
    bool ReadForInt();
    bool ReadForBulk();
    bool ReadForArray();


    int DeSerializeForStatus(const std::string& strRes);
    int DeSerializeForError(const std::string& strRes);
    int DeSerializeForInt(const std::string& strRes);
    int DeSerializeForBulk(const std::string& strRes);

private:
    NFREDIS_RESULT_STATUS mxResultStatus;
    NFREDIS_RESP_TYPE mxRespType;
    std::string mstrRespValue;
	std::string mstrMsgValue;
	std::string mstrCommand;
    std::vector<NFRedisResult> mxRespList;

    NFRedisClientSocket*  m_pClientSocket;
};


#endif //NFREDISPLUGIN_NFREDISRESULT_H
