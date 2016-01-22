// -------------------------------------------------------------------------
//    @FileName         ：    NFINet.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-12-15
//    @Module           ：    NFINet
//    @Desc             :     INet
// -------------------------------------------------------------------------

#ifndef NFI_NET_H
#define NFI_NET_H

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <iostream>
#include <map>

#ifndef _MSC_VER
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/event_compat.h>
#include <assert.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../NFPluginModule/NFGUID.h"

#pragma pack(push, 1)

enum NF_NET_EVENT
{
    NF_NET_EVENT_EOF = 0x10,        //掉线
    NF_NET_EVENT_ERROR = 0x20,      //未知错误
    NF_NET_EVENT_TIMEOUT = 0x40,    //连接超时
    NF_NET_EVENT_CONNECTED = 0x80,  //连接成功(作为客户端)
};


struct  NFIMsgHead
{
	enum NF_Head
	{
		NF_HEAD_LENGTH = 6,
	};

	virtual int EnCode(char* strData) = 0;
	virtual int DeCode(const char* strData) = 0;

	virtual uint16_t GetMsgID() const = 0;
	virtual void SetMsgID(uint16_t nMsgID) = 0;

	virtual uint32_t GetBodyLength() const = 0;
	virtual void SetBodyLength(uint32_t nLength) = 0;

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

	// 内存结构[ MsgID(2) | MsgSize(4) ]
	virtual int EnCode(char* strData)
	{
		uint32_t nOffset = 0;

		uint16_t nMsgID = NF_HTONS(munMsgID);
		memcpy(strData + nOffset, (void*)(&nMsgID), sizeof(munMsgID));
		nOffset += sizeof(munMsgID);

		uint32_t nPackSize = munSize + NF_HEAD_LENGTH;
		uint32_t nSize = NF_HTONL(nPackSize);
		memcpy(strData + nOffset, (void*)(&nSize), sizeof(munSize));
		nOffset += sizeof(munSize);

		if (nOffset != NF_HEAD_LENGTH)
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
		munMsgID = NF_NTOHS(nMsgID);
		nOffset += sizeof(munMsgID);

		uint32_t nPackSize = 0;
		memcpy(&nPackSize, strData + nOffset, sizeof(munSize));
		munSize = NF_NTOHL(nPackSize) - NF_HEAD_LENGTH;
		nOffset += sizeof(munSize);

		if (nOffset != NF_HEAD_LENGTH)
		{
			assert(0);
		}

		return nOffset;
	}

	virtual uint16_t GetMsgID() const { return munMsgID; }
	virtual void SetMsgID(uint16_t nMsgID) { munMsgID = nMsgID; }

	virtual uint32_t GetBodyLength() const { return munSize; }
	virtual void SetBodyLength(uint32_t nLength){ munSize = nLength; }
protected:
	uint32_t munSize;
	uint16_t munMsgID;
};

class NFINet;

typedef std::function<void(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)> NET_RECIEVE_FUNCTOR;
typedef std::shared_ptr<NET_RECIEVE_FUNCTOR> NET_RECIEVE_FUNCTOR_PTR;

typedef std::function<void(const int nSockIndex, const NF_NET_EVENT nEvent, NFINet* pNet)> NET_EVENT_FUNCTOR;
typedef std::shared_ptr<NET_EVENT_FUNCTOR> NET_EVENT_FUNCTOR_PTR;

typedef std::function<void(int severity, const char *msg)> NET_EVENT_LOG_FUNCTOR;
typedef std::shared_ptr<NET_EVENT_LOG_FUNCTOR> NET_EVENT_LOG_FUNCTOR_PTR;

class NetObject
{
public:
    NetObject(NFINet* pNet, int32_t fd, sockaddr_in& addr, bufferevent* pBev)
    {
		mnLogicState = 0;
		mnGameID = 0;
		nFD = fd;
		bNeedRemove = false;

        m_pNet = pNet;

        bev = pBev;
        memset(&sin, 0, sizeof(sin));
        sin = addr;
    }

    virtual ~NetObject()
    {
    }

    int AddBuff(const char* str, uint32_t nLen)
    {
        mstrBuff.append(str, nLen);

        return (int)mstrBuff.length();
    }

    int CopyBuffTo(char* str, uint32_t nStart, uint32_t nLen)
    {
        if (nStart + nLen > mstrBuff.length())
        {
            return 0;
        }

        memcpy(str, mstrBuff.data() + nStart, nLen);

        return nLen;
    }

    int RemoveBuff(uint32_t nStart, uint32_t nLen)
    {
        if (nStart < 0)
        {
            return 0;
        }

        //移除前面，则后面跟上
        if (nStart + nLen > mstrBuff.length())
        {
            return 0;
        }

        //把后面的挪到前面来
        mstrBuff.erase(nStart, nLen);

        return mstrBuff.length();
    }

    const char* GetBuff()
    {
        return mstrBuff.data();
    }

    int GetBuffLen() const
    {
        return mstrBuff.length();
    }

	bufferevent * GetBuffEvent()
	{
		return bev;
	}

	NFINet* GetNet()
	{
		return m_pNet;
	}
	//////////////////////////////////////////////////////////////////////////
    int GetConnectKeyState() const
    {
        return mnLogicState;
    }

    void SetConnectKeyState(const int nState)
    {
        mnLogicState = nState;
    }
	bool NeedRemove()
	{
		return bNeedRemove;
	}
	void SetNeedRemove(bool b)
	{
		bNeedRemove = b;
	}
	const std::string& GetAccount() const
	{
		return mstrUserData;
	}

	void SetAccount(const std::string& strData)
	{
		mstrUserData = strData;
	}
    int GetGameID() const
    {
        return mnGameID;
    }

    void SetGameID(const int nData)
    {
        mnGameID = nData;
    }    
    const NFGUID& GetUserID()
    {
        return mnUserID;
    }

    void SetUserID(const NFGUID& nUserID)
    {
        mnUserID = nUserID;
    }

	const NFGUID& GetClientID()
	{
		return mnClientID;
	}

	void SetClientID(const NFGUID& xClientID)
	{
		mnClientID = xClientID;
	}

	int GetRealFD()
	{
		return nFD;
	}

private:
    sockaddr_in sin;
    bufferevent *bev;
	std::string mstrBuff;
	std::string mstrUserData;

    int32_t mnLogicState;
	int32_t mnGameID;
	NFGUID mnUserID;	NFGUID mnClientID;
    NFINet* m_pNet;
	//
	int nFD;
	bool bNeedRemove;
};

class NFINet
{
public:
	template<typename BaseType>
	bool AddReciveCallBack(const int nMsgID, BaseType* pBase, int (BaseType::*handler)(const int, const int, const char*, const uint32_t))
	{
		NET_RECIEVE_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECIEVE_FUNCTOR_PTR functorPtr(new NET_RECIEVE_FUNCTOR(functor));
		return AddReciveCallBack(nMsgID, functorPtr);
	}

	template<typename BaseType>
	bool AddEventCallBack(BaseType* pBase, int (BaseType::*handler)(const int, const NF_NET_EVENT, NFINet*))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr(new NET_EVENT_FUNCTOR(functor));
		return AddEventCallBack(functorPtr);
	}

	virtual bool AddReciveCallBack(const int nMsgID, const NET_RECIEVE_FUNCTOR_PTR& cb) = 0;
	virtual bool AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool Execute() = 0;

	virtual void Initialization(const char* strIP, const unsigned short nPort) = 0;
	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;

	virtual bool Final() = 0;

	//无包头，内部组装
	virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const int nSockIndex = 0) = 0;

	//已带上包头
	virtual bool SendMsgToAllClient(const char* msg, const uint32_t nLen) = 0;

	//无包头，内部组装
	virtual bool SendMsgToAllClientWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen) = 0;

	virtual bool CloseNetObject(const int nSockIndex) = 0;
    virtual NetObject* GetNetObject(const int nSockIndex) = 0;
    virtual bool AddNetObject(const int nSockIndex, NetObject* pObject) = 0;

	virtual bool IsServer() = 0;

	virtual bool Log(int severity, const char *msg) = 0;
};

#pragma pack(pop)

#endif
