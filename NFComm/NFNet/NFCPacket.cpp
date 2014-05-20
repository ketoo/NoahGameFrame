#include "NFCPacket.h"

int NFCPacket::DeCode( const char* strData, const uint16_t unLen )
{
	//����--unLenΪ�ܳ���
	if (unLen >= NF_MAX_SERVER_PACKET_SIZE)
	{
		//���ȳ�Խ
		return -1;
	}

	//ȡ��ͷ
	char head[NF_HEAD_SIZE] = {0};	
	memcpy(head, strData, NF_HEAD_SIZE);

	MsgHead msgHead = ntohl(*(uint32_t*)head);
	if (msgHead.unDataLen + NF_HEAD_SIZE > unLen)
	{
		//�ܳ��Ȳ���
		return 0;
	}

	mHead.unMsgID = msgHead.unMsgID;
	mHead.unDataLen = msgHead.unDataLen;
	munPacketLen = mHead.unDataLen + NF_HEAD_SIZE;
	//copy��ͷ+����
	memcpy(strPackData, strData, munPacketLen);
	//����ʹ�ù�����
	return munPacketLen;
}
int NFCPacket::EnCode(uint16_t uMsgID, const char* strData, const uint16_t unLen)
{
	//����
	//���һ��copy�����ǿ�������ӽ��ܶ�����Ͽ�
	if (unLen + NF_HEAD_SIZE > NF_MAX_SERVER_PACKET_SIZE)
	{
		return 0;
	}

	mHead.unMsgID = uMsgID;
	mHead.unDataLen = unLen;
	munPacketLen = mHead.unDataLen + NF_HEAD_SIZE;

	//////////////////////////////////////////////////////////////////////////
	uint32_t unHeadID = htonl(mHead.unHeadID);
	uint16_t nOffest = 0;

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
