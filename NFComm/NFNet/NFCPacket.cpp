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

    if (unLen < pHead->GetHeadLength())
    {
        //长度不够
        return -1;
    }

	//取包头
	pHead->DeCode(strData);
	if (pHead->GetMsgLength() > unLen)
	{
		//总长度不够
		return -1;
	}

	//copy包头+包体
	memcpy(strPackData, strData, pHead->GetMsgLength());

	//返回使用过的量
	return pHead->GetMsgLength();
}

int NFCPacket::EnCode(char* pHeadBuffer, const char* strData, const uint32_t unLen)
{
	//加密
	//虽多一次copy，但是可以在外加解密而不耦合库
	if (unLen + pHead->GetHeadLength() > NF_MAX_SERVER_PACKET_SIZE)
	{
        // 长度过大
		return 0;
	}
    
    memcpy(strPackData, (void*)pHeadBuffer, pHead->GetHeadLength());

    if (unLen > 0)
    {
        memcpy(strPackData + pHead->GetHeadLength(), strData, unLen);
    }

	return unLen + pHead->GetHeadLength();
}
