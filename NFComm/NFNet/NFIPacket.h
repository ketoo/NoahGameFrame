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

#include <string>
#include "boost/math/special_functions/hypot.hpp"

#pragma pack(push, 1)



struct  NFIMsgHead
{
    enum NF_Head
    {
        NF_HEAD_LENGTH = 6,
    };

    virtual int EnCode(char* strData) = 0;
    virtual int DeCode(const char* strData) = 0;

    virtual uint32_t GetHeadLength() const = 0;

    virtual uint16_t GetMsgID() const = 0;
    virtual void SetMsgID(uint16_t nMsgID) = 0;

    virtual uint32_t GetMsgLength() const = 0;
    virtual void SetMsgLength(uint32_t nLength) = 0;

    int64_t NF_HTONLL(int64_t nData)
    {
#ifdef _MSC_VER
        return htonll(nData);
#else
        return htobe64(nData);
#endif
    }

    int64_t NF_NTOHLL(int64_t nData)
    {
#ifdef _MSC_VER
        return ntohll(nData);
#else
        return be64toh(nData);
#endif
    }

    int32_t NF_HTONL(int32_t nData)
    {
#ifdef _MSC_VER
        return htonl(nData);
#else
        return htobe32(nData);
#endif
    }

    int32_t NF_NTOHL(int32_t nData)
    {
#ifdef _MSC_VER
        return ntohl(nData);
#else
        return be32toh(nData);
#endif
    }

    int16_t NF_HTONS(int16_t nData)
    {
#ifdef _MSC_VER
        return htons(nData);
#else
        return htobe16(nData);
#endif
    }

    int16_t NF_NTOHS(int16_t nData)
    {
#ifdef _MSC_VER
        return ntohs(nData);
#else
        return be16toh(nData);
#endif
    }

};

class NFCMsgHead : public NFIMsgHead
{
public:
    NFCMsgHead()
    {
        munSize = 0;
        munMsgID = 0;
    }

    virtual uint32_t GetHeadLength() const { return NF_HEAD_LENGTH; }

    virtual int EnCode(char* strData)
    {
        uint32_t nOffset = 0;

        uint16_t nMsgID = htons(munMsgID);
        memcpy(strData + nOffset, (void*)(&nMsgID), sizeof(munMsgID));
        nOffset += sizeof(munMsgID);

        uint32_t nSize = htonl(munSize);
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
        memcpy(&nMsgID, strData + nOffset, sizeof(munMsgID));
        munMsgID = ntohs(nMsgID);
        nOffset += sizeof(munMsgID);

        uint32_t nSize = 0;
        memcpy(&nSize, strData + nOffset, sizeof(munSize));
        munSize = ntohl(nSize);
        nOffset += sizeof(munSize);

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

protected:
    uint32_t munSize;
    uint16_t munMsgID;
};

class NFIPacket
{
public:
	void operator = (const NFIPacket& packet)
	{
		this->Construction(packet);
	}

    virtual int EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unLen) = 0;
	virtual int DeCode(const char* strData, const uint32_t unLen) = 0;

	virtual void Construction(const NFIPacket& packet) = 0;
	virtual const NFIMsgHead* GetMsgHead() const = 0;
	virtual const char* GetPacketData() const = 0;
    virtual const std::string& GetPacketString() const = 0;
    virtual const uint32_t GetPacketLen() const = 0;
    virtual const uint32_t GetDataLen() const = 0;
	virtual const char* GetData() const = 0;
	virtual const int GetFd() const = 0;
	virtual void SetFd(const int nFd) = 0;
};

#pragma pack(pop)

#endif
