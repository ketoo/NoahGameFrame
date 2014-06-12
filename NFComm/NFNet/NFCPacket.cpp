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
	if (unLen >= NF_MAX_SERVER_PACKET_SIZE)
	{
		//���ȳ�Խ
		return -1;
	}

    if (unLen < pHead->GetHeadLength())
    {
        //���Ȳ���
        return -1;
    }

	//ȡ��ͷ
	pHead->DeCode(strData);
	if (pHead->GetMsgLength() > unLen)
	{
		//�ܳ��Ȳ���
		return -1;
	}

	//copy��ͷ+����
	memcpy(strPackData, strData, pHead->GetMsgLength());

	//����ʹ�ù�����
	return pHead->GetMsgLength();
}

int NFCPacket::EnCode(char* pHeadBuffer, const char* strData, const uint32_t unLen)
{
	//����
	//���һ��copy�����ǿ�������ӽ��ܶ�����Ͽ�
	if (unLen + pHead->GetHeadLength() > NF_MAX_SERVER_PACKET_SIZE)
	{
        // ���ȹ���
		return 0;
	}
    
    memcpy(strPackData, (void*)pHeadBuffer, pHead->GetHeadLength());

    if (unLen > 0)
    {
        memcpy(strPackData + pHead->GetHeadLength(), strData, unLen);
    }

	return unLen + pHead->GetHeadLength();
}
