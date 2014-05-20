#ifndef __NFI_PACKET_H__
#define __NFI_PACKET_H__


#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>

#pragma pack(push, 1)

const int NF_HEAD_SIZE = 4;
const int NF_MAX_SERVER_PACKET_SIZE = 6553500;
const int NF_MAX_CLIENT_PACK_SIZE = 6553500;

struct  MsgHead
{
	MsgHead()
	{
		unHeadID = 0;
	}

	MsgHead(uint32_t uID)
	{
		unHeadID = uID;
	}

	union
	{
		struct
		{
			uint16_t unDataLen;
			uint16_t unMsgID;
		};

		struct
		{
			uint32_t unHeadID;
		};
	};	
};


class NFIPacket
{
public:
	void operator = (const NFIPacket& packet)
	{

		this->Construction(packet);
	}

	virtual int EnCode(uint16_t uMsgID, const char* strData, const uint16_t unLen) = 0;
	virtual int DeCode(const char* strData, const uint16_t unLen) = 0;

	virtual void Construction(const NFIPacket& packet) = 0;
	virtual const MsgHead& GetMsgHead() const = 0;
	virtual const char* GetPacketData() const = 0;
	virtual const uint16_t GetPacketLen() const = 0;
	virtual const uint16_t GetDataLen() const = 0;
	virtual const char* GetData() const = 0;
	virtual const uint16_t GetFd() const = 0;
	virtual void SetFd(const uint16_t nFd) = 0;
};

#pragma pack(pop)

#endif