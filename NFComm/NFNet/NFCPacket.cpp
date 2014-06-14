// -------------------------------------------------------------------------
//    @FileName         ��    NFCPacket.cpp
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2013-12-15
//    @Module           ��    NFIPacket
//    @Desc             :     Net Packet
// -------------------------------------------------------------------------

#include "NFCPacket.h"

int NFCPacket::DeCode( const char* strData, const uint32_t unLen )
{
	//����--unLenΪbuff�ܳ���,���ʱ���ö����Ƕ���
    if (unLen < pHead->GetHeadLength())
    {
        //���Ȳ���
        return -1;
    }

	
	if(pHead->GetHeadLength() != pHead->DeCode(strData))
    {
        //ȡ��ͷʧ��
        return -1;
    }

	if (pHead->GetMsgLength() > unLen)
	{
		//�ܳ��Ȳ���
		return -1;
	}

	//copy��ͷ+����
    mstrPackData.append(strData, pHead->GetMsgLength());

	//����ʹ�ù�����
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
