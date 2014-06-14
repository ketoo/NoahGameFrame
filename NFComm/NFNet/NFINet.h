// -------------------------------------------------------------------------
//    @FileName         £º    NFINet.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-12-15
//    @Module           £º    NFINet
//    @Desc             :     INet
// -------------------------------------------------------------------------

#ifndef __NFI_NET_H__
#define __NFI_NET_H__

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include <xtree>

#if NF_PLATFORM != NF_PLATFORM_WIN
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include "NFIPacket.h"

#include <event2/event.h> 
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/event_compat.h>
#include <event2/event.h>

#pragma pack(push, 1)

class NFINet
{
public:
	virtual  bool Execute(const float fLasFrametime, const float fStartedTime) = 0;

	virtual  int Initialization(const char* strIP, const unsigned short nPort) = 0;
	virtual  int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;

	virtual  bool Final() = 0;
    virtual  bool Reset() = 0;

	virtual bool SendMsg(const NFIPacket& msg, const uint32_t nSockIndex = 0, bool bBroadcast = false) = 0;
	virtual bool SendMsg(const char* msg, const uint32_t nLen, const uint32_t nSockIndex = 0, bool bBroadcast = false) = 0;
	
	virtual int OnRecivePacket(const uint32_t nSockIndex, const char* msg, const uint32_t nLen){return 1;};

	virtual bool CloseSocket(const uint32_t nSockIndex) = 0;
	virtual bool AddBan(const uint32_t nSockIndex, const int32_t nTime = -1) = 0;
	virtual bool RemoveBan(const uint32_t nSockIndex) = 0;

    virtual void HeartPack() = 0;
    virtual NFIMsgHead::NF_Head GetHeadLen() = 0;

protected:
private:
};

#pragma pack(pop)

#endif