#ifndef __NFC_PACKET_H__
#define __NFC_PACKET_H__


#include "NFIPacket.h"
#include <winsock2.h>

#pragma pack(push, 1)

class NFCPacket : public NFIPacket
{
public:
	NFCPacket()
	{
		munPacketLen = 0;
		munFD = 0;
	}



	virtual const MsgHead& GetMsgHead() const
	{
		return mHead;
	}

	virtual const char* GetPacketData() const
	{
		return strPackData;
	}

	virtual const uint16_t GetPacketLen() const
	{
		return munPacketLen;
	}

	virtual const uint16_t GetDataLen() const
	{
		return mHead.unDataLen;
	}

	virtual const char* GetData() const
	{
		return strPackData + NF_HEAD_SIZE;
	}

	virtual void Construction(const NFIPacket& packet)
	{
		this->mHead = packet.GetMsgHead();
		this->munPacketLen = packet.GetPacketLen();
		memcpy(this->strPackData, packet.GetPacketData(), this->munPacketLen);
	}

	virtual const uint16_t GetFd() const
	{
		return munFD;
	}

	virtual void SetFd(const uint16_t nFd)
	{
		munFD = nFd;
	}

	virtual int EnCode(uint16_t uMsgID, const char* strData, const uint16_t unLen);

	virtual int DeCode(const char* strData, const uint16_t unLen);

protected:
private:
	MsgHead mHead;
	uint16_t munPacketLen;
	char strPackData[NF_MAX_SERVER_PACKET_SIZE];
	uint16_t munFD;;
};


#pragma pack(pop)

#endif