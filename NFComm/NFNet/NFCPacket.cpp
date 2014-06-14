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
    if (unLen < pHead->GetHeadLength())
    {
        //长度不够
        return -1;
    }

	
	if(pHead->GetHeadLength() != pHead->DeCode(strData))
    {
        //取包头失败
        return -1;
    }

	if (pHead->GetMsgLength() > unLen)
	{
		//总长度不够
		return -1;
	}

	//copy包头+包体
    mstrPackData.append(strData, pHead->GetMsgLength());

	//返回使用过的量
	return pHead->GetMsgLength();
}

int NFCPacket::EnCode( const uint16_t unMsgID, const char* strData, const uint32_t unLen )
{

    pHead->SetMsgID(unMsgID);
    pHead->SetMsgLength(unLen + pHead->GetHeadLength());

    char szHead[64] = { 0 };
    pHead->EnCode(szHead);

    mstrPackData.clear();
    mstrPackData.append(szHead, pHead->GetHeadLength());
    mstrPackData.append(strData, unLen);

    return pHead->GetMsgLength();
}
