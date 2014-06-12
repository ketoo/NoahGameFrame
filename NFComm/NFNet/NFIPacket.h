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
#include <assert.h>

#ifdef _MSC_VER
#include <WinSock2.h>
#else

#endif

#pragma pack(push, 1)

//const int NF_HEAD_SIZE = 8;
const int NF_MAX_SERVER_PACKET_SIZE = 65535;

struct  NFIMsgHead
{
    enum NF_Head
    {
        NF_HEAD_LENGTH = 6,
        NF_CS_HEAD_LENGTH = 10,
        NF_SS_HEAD_LENGTH = 32,
    };

    virtual int EnCode(char* strData) = 0;
    virtual int DeCode(const char* strData) = 0;

    virtual uint32_t GetHeadLength() const = 0;

    virtual uint16_t GetMsgID() const = 0;
    virtual void SetMsgID(uint16_t nMsgID) = 0;

    virtual uint32_t GetMsgLength() const = 0;
    virtual void SetMsgLength(uint32_t nLength) = 0;
};

class NFCMsgHead : public NFIMsgHead
{
public:
    NFCMsgHead()
    {
        munSize = 0;
        munMsgID = 0;
    }

    uint32_t munSize;
    uint16_t munMsgID;           // Ã¶¾Ù¼ûMG_PTOTOCOL_TYPES

    virtual uint32_t GetHeadLength() const { return NF_HEAD_LENGTH; }
    virtual int EnCode(char* strData)
    {
        uint32_t nOffset = 0;

        uint16_t nMsgID = htons(munMsgID);
        memcpy(strData + nOffset, (void*)(&nMsgID), sizeof(munMsgID));
        nOffset += sizeof(munMsgID);

        uint32_t nSize = htons(munSize);
        memcpy(strData + nOffset, (void*)(&nSize), sizeof(munSize));
        nOffset += sizeof(munSize);

        if (nOffset != GetHeadLength())
        {
            assert(0);
        }

        return nOffset;
    }

    virtual int DeCode(const char* strData)
    {
        uint32_t nOffset = 0;

        uint16_t nMsgID = 0;
        memcpy(&nMsgID, strData + nOffset, sizeof(nMsgID));
        munMsgID = ntohs(nMsgID);
        nOffset += sizeof(munMsgID);

        uint32_t nSize = 0;
        memcpy(&nSize, strData + nOffset, sizeof(nSize));
        munMsgID = ntohs(nSize);
        nOffset += sizeof(munMsgID);


        if (nOffset != GetHeadLength())
        {
            assert(0);
        }

        return nOffset;
    }

    virtual uint16_t GetMsgID() const { return munMsgID; }
    virtual void SetMsgID(uint16_t nMsgID) { munMsgID = nMsgID; }

    virtual uint32_t GetMsgLength() const { return munSize; }
    virtual void SetMsgLength(uint32_t nLength){ munSize = nLength; }
};

class NFIPacket
{
public:
	void operator = (const NFIPacket& packet)
	{
		this->Construction(packet);
	}

	virtual int EnCode(char* pHeadBuffer, const char* strData, const uint32_t unLen) = 0;
	virtual int DeCode(const char* strData, const uint32_t unLen) = 0;

	virtual void Construction(const NFIPacket& packet) = 0;
	virtual const NFIMsgHead* GetMsgHead() const = 0;
	virtual const char* GetPacketData() const = 0;
    virtual const uint32_t GetPacketLen() const = 0;
    virtual const uint32_t GetDataLen() const = 0;
	virtual const char* GetData() const = 0;
	virtual const uint32_t GetFd() const = 0;
	virtual void SetFd(const uint32_t nFd) = 0;
};

#pragma pack(pop)

#endif