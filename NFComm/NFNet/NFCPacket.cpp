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
	//解密--unLen为buff总长度,解包时能用多少是多少
	if (unLen >= NF_MAX_SERVER_PACKET_SIZE)
	{
		//长度超越
		return -1;
	}

    if (unLen < NF_HEAD_SIZE)
    {
        //长度不够
        return -1;
    }

	//取包头
	char head[NF_HEAD_SIZE] = {0};	
	memcpy(head, strData, NF_HEAD_SIZE);

	mHead.unHead = ntohll(*(uint64_t*)head);
	if (mHead.unDataLen > unLen)
	{
		//总长度不够
		return 0;
	}

	//copy包头+包体
	memcpy(strPackData, strData, mHead.unDataLen);

	//返回使用过的量
	return mHead.unDataLen;
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
	mHead.unDataLen = unLen + NF_HEAD_SIZE;

	//////////////////////////////////////////////////////////////////////////
	uint64_t unHeadID = htonll(mHead.unHead);

	memcpy(strPackData, (void*)&unHeadID, NF_HEAD_SIZE);
	memcpy(strPackData + NF_HEAD_SIZE, strData, mHead.unDataLen);

	return unLen + NF_HEAD_SIZE;
}
