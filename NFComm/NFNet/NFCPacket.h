// -------------------------------------------------------------------------
//    @FileName         £º    NFCPacket.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-12-15
//    @Module           £º    NFIPacket
//    @Desc             :     Net Packet
// -------------------------------------------------------------------------

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
		munFD = 0;
        mHead.unHead = 0;
	}

	virtual const MsgHead& GetMsgHead() const
	{
		return mHead;
	}

    virtual const uint32_t GetMsgID() const
    {
        return mHead.unMsgID;
    }

	virtual const char* GetPacketData() const
	{
		return strPackData;//include head
	}

	virtual const uint32_t GetPacketLen() const
	{
		return mHead.unDataLen;
	}

    virtual const uint32_t GetDataLen() const
    {
        return mHead.unDataLen - NF_HEAD_SIZE;
    }

	virtual const char* GetData() const
	{
		return strPackData + NF_HEAD_SIZE;//not include head
	}

	virtual void Construction(const NFIPacket& packet)
	{
		memcpy(this->strPackData, packet.GetPacketData(), this->GetPacketLen());
	}

	virtual const uint16_t GetFd() const
	{
		return munFD;
	}

	virtual void SetFd(const uint16_t nFd)
	{
		munFD = nFd;
	}

    virtual uint32_t GetHeadSize()
    {
        return sizeof(MsgHead);
    }

//     virtual int EnCodeHead(uint32_t uMsgID, const char* strData, const uint32_t unLen);
//     virtual int DeCodeHead(const char* strData, const uint32_t unLen);

    //const uint32_t unLen: length of data, not include head
	virtual int EnCode(uint32_t uMsgID, const char* strData, const uint32_t unLen);

    //const uint32_t unLen: length of buff
	virtual int DeCode(const char* strData, const uint32_t unLen);

protected:
private:
	MsgHead mHead;
	char strPackData[NF_MAX_SERVER_PACKET_SIZE];//include head
	uint16_t munFD;;
};


#pragma pack(pop)

#endif