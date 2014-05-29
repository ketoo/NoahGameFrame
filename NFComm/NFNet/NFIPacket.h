// -------------------------------------------------------------------------
//    @FileName         £º    NFIPacket.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-12-15
//    @Module           £º    NFIPacket
//    @Desc             :     Net Packet
// -------------------------------------------------------------------------

#ifndef __NFI_PACKET_H__
#define __NFI_PACKET_H__

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>

#pragma pack(push, 1)

const int NF_HEAD_SIZE = 8;
const int NF_MAX_SERVER_PACKET_SIZE = 65535;
const int NF_MAX_CLIENT_PACK_SIZE = 65535;

struct  MsgHead
{
	MsgHead()
	{
		unHeadID = 0;
	}

	MsgHead(uint64_t uID)
	{
		unHeadID = uID;
	}

	union
	{
		struct
		{
			uint32_t unDataLen;
			uint32_t unMsgID;
		};

		struct
		{
			uint64_t unHeadID;
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

	virtual int EnCode(uint32_t uMsgID, const char* strData, const uint32_t unLen) = 0;
	virtual int DeCode(const char* strData, const uint32_t unLen) = 0;

	virtual void Construction(const NFIPacket& packet) = 0;
	virtual const MsgHead& GetMsgHead() const = 0;
	virtual const char* GetPacketData() const = 0;
	virtual const uint32_t GetPacketLen() const = 0;
	virtual const uint32_t GetDataLen() const = 0;
	virtual const char* GetData() const = 0;
	virtual const uint16_t GetFd() const = 0;
	virtual void SetFd(const uint16_t nFd) = 0;
};

#pragma pack(pop)

#endif