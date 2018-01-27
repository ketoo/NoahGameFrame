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
	const std::string& GetCommand() const;

    NFREDIS_RESULT_STATUS GetResultStatus();
    NFREDIS_RESP_TYPE GetRespType() const;

    //if not ok ...
    bool IsOKRespStatus() const;

    //get response value
	int64_t GetRespInt() const;
	int GetRespBulkLen() const;
    std::string GetRespString() const;
    float GetRespFloat() const;
    double GetRespDouble() const;
    const std::vector<NFRedisResult>& GetRespArray();

	static NFREDIS_RESP_TYPE GetRespType(const char strRes);

private:
	bool ReadRespType();
	bool ReadBuff();
	bool DeSerializeBuff();

private:
	bool ReadForStatus();
	bool ReadForInt();
	bool ReadForError();
    bool ReadForBulk();
    bool ReadForArray();

    static bool DeSerializeForStatus(const std::string& strRes, std::string& status);
	static bool DeSerializeForError(const std::string& strRes, std::string& error);
	static bool DeSerializeForInt(const std::string& strRes, int64_t& n);
	static bool DeSerializeForBulkHead(const std::string& strRes, int& nLenn);
	static bool DeSerializeForBulk(const std::string& strRes, std::string& bulk);

	static bool DeSerializeForArrayHead(const std::string& strRes, int& nLenn);
private:
	
    NFREDIS_RESULT_STATUS mxResultStatus;
    NFREDIS_RESP_TYPE mxRespType;

	int64_t mnRespValue;
	int mnRespBulkLen;
	std::string mstrRespValue;
	std::string mstrMsgValue;
	std::string mstrCommand;
    std::vector<NFRedisResult> mxRespList;

    NFRedisClientSocket*  m_pClientSocket;
};


#endif //NFREDISPLUGIN_NFREDISRESULT_H
