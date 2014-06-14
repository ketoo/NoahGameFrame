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
	NFCPacket(NFIMsgHead::NF_Head head)
	{
		munFD = 0;
        switch (head)
        {
        case NFIMsgHead::NF_HEAD_LENGTH:
            pHead = new NFCMsgHead();
            break;
//         case NFIMsgHead::NF_CS_HEAD_LENGTH:
//             pHead = new MsgCSHead();
//             break;
//         case NFIMsgHead::NF_SS_HEAD_LENGTH:
//             pHead = new MsgSSHead();
//             break;
        default:
            break;
        }
	}

    ~NFCPacket()
    {
        if (pHead)
        {
            delete pHead;
            pHead = NULL;
        }
    }

	virtual const NFIMsgHead* GetMsgHead() const
	{
		return pHead;
	}

    virtual const uint32_t GetMsgID() const
    {
        return pHead->GetMsgID();
    }

	virtual const char* GetPacketData() const
	{
		return strPackData;//include head
	}

	virtual const uint32_t GetPacketLen() const
	{
		return pHead->GetMsgLength();
	}

    virtual const uint32_t GetDataLen() const
    {
        return pHead->GetMsgLength() - pHead->GetHeadLength();
    }

	virtual const char* GetData() const
	{
		return strPackData + pHead->GetHeadLength();//not include head
	}

	virtual void Construction(const NFIPacket& packet)
	{
		memcpy(this->strPackData, packet.GetPacketData(), this->GetPacketLen());
	}

	virtual const uint32_t GetFd() const
	{
		return munFD;
	}

	virtual void SetFd(const uint32_t nFd)
	{
		munFD = nFd;
	}

    virtual uint32_t GetHeadSize()
    {
        return pHead->GetHeadLength();
    }

    //const uint32_t unLen: length of data, not include head
	virtual int EnCode(char* pHeadBuffer, const char* strData, const uint32_t unLen);

    //const uint32_t unLen: length of buff
	virtual int DeCode(const char* strData, const uint32_t unLen);

protected:
private:
	NFIMsgHead* pHead;
	char strPackData[NF_MAX_SERVER_PACKET_SIZE];//include head
	uint32_t munFD;
};


#pragma pack(pop)

#endif