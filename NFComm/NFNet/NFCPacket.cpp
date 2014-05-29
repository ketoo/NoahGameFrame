// -------------------------------------------------------------------------
//    @FileName         ：    NFCPacket.cpp
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-12-15
//    @Module           ：    NFIPacket
//    @Desc             :     Net Packet
// -------------------------------------------------------------------------

#include "NFCPacket.h"

int NFCPacket::DeCode( const char* strData, const uint32_t unLen )
{
	//解密--unLen为总长度
	if (unLen >= NF_MAX_SERVER_PACKET_SIZE)
	{
		//长度超越
		return -1;
	}

	//取包头
	char head[NF_HEAD_SIZE] = {0};	
	memcpy(head, strData, NF_HEAD_SIZE);

	MsgHead msgHead = ntohll(*(uint64_t*)head);
	if (msgHead.unDataLen + NF_HEAD_SIZE > unLen)
	{
		//总长度不够
		return 0;
	}

	mHead.unMsgID = msgHead.unMsgID;
	mHead.unDataLen = msgHead.unDataLen;
	munPacketLen = mHead.unDataLen + NF_HEAD_SIZE;
	//copy包头+包体
	memcpy(strPackData, strData, munPacketLen);
	//返回使用过的量
	return munPacketLen;
}
int NFCPacket::EnCode(uint32_t uMsgID, const char* strData, const uint32_t unLen)
{
	//加密
	//虽多一次copy，但是可以在外加解密而不耦合库
	if (unLen + NF_HEAD_SIZE > NF_MAX_SERVER_PACKET_SIZE)
	{
		return 0;
	}

	mHead.unMsgID = uMsgID;
	mHead.unDataLen = unLen;
	munPacketLen = mHead.unDataLen + NF_HEAD_SIZE;

	//////////////////////////////////////////////////////////////////////////
	uint64_t unHeadID = htonll(mHead.unHeadID);
	uint32_t nOffest = 0;

	memcpy(strPackData + nOffest, (void*)&unHeadID, NF_HEAD_SIZE);
	nOffest += NF_HEAD_SIZE;

	memcpy(strPackData + nOffest, strData, mHead.unDataLen);
	nOffest += unLen;

	if (munPacketLen != nOffest)
	{
		return 0;
	}

	return nOffest;
}
