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

    if (unLen < NF_HEAD_SIZE)
    {
        //���Ȳ���
        return -1;
    }

	//ȡ��ͷ
	char head[NF_HEAD_SIZE] = {0};	
	memcpy(head, strData, NF_HEAD_SIZE);

	mHead.unHead = ntohll(*(uint64_t*)head);
	if (mHead.unDataLen > unLen)
	{
		//�ܳ��Ȳ���
		return 0;
	}

	//copy��ͷ+����
	memcpy(strPackData, strData, mHead.unDataLen);

	//����ʹ�ù�����
	return mHead.unDataLen;
}

int NFCPacket::EnCode(uint32_t uMsgID, const char* strData, const uint32_t unLen)
{
	//����
	//���һ��copy�����ǿ�������ӽ��ܶ�����Ͽ�
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
