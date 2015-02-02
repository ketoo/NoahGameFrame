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

#pragma pack(push, 1)

class NFCPacket : public NFIPacket
{
public:
	NFCPacket(NFIMsgHead::NF_Head eHeadType)
	{
		munFD = 0;
        switch (eHeadType)
        {
        case NFIMsgHead::NF_HEAD_LENGTH:
            pHead = new NFCMsgHead();
            break;
        default:
            break;
        }
	}

    virtual ~NFCPacket()
    {
        if (pHead)
        {
            delete pHead;
            pHead = NULL;
        }
    }

	void operator = (const NFIPacket& id)
	{

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
		return this->mstrPackData.data();//include head
	}

    virtual const std::string& GetPacketString() const
    {
        return this->mstrPackData;
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
		return this->mstrPackData.data() + pHead->GetHeadLength();//not include head
	}

	virtual void Construction(const NFIPacket& packet)
	{
        this->mstrPackData.assign(packet.GetPacketData(), this->GetPacketLen());
	}

	virtual const int GetFd() const
	{
		return munFD;
	}

	virtual void SetFd(const int nFd)
	{
		munFD = nFd;
	}

    virtual uint32_t GetHeadSize()
    {
        return pHead->GetHeadLength();
    }

    //const uint32_t unLen: length of data, not include head
	virtual int EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unLen);

    //const uint32_t unLen: length of buff
	virtual int DeCode(const char* strData, const uint32_t unLen);

protected:
private:
	NFIMsgHead* pHead;
	//char strPackData[NF_MAX_SERVER_PACKET_SIZE];//include head
    std::string mstrPackData;//include head
	int munFD;
};


#pragma pack(pop)

#endif
