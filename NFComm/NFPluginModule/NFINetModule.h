// -------------------------------------------------------------------------
//    @FileName         ��    NFINetModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFINetModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_NET_MODULE_H_
#define _NFI_NET_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFNet/NFCNet.h"
#include "NFComm/NFNet/NFCPacket.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

class NFINetModule
	: public NFILogicModule
{

public:
	NFINetModule()
	{
		m_pNet = NULL;
	}

	virtual ~NFINetModule()
	{

	}

	void operator()() 
	{
	}

    virtual void LogRecive(const char* str){};
    virtual void LogSend(const char* str){};

	bool RecivePB(const NFIPacket& msg, google::protobuf::Message& xData, int32_t& nPlayer)
	{
        NFMsg::MsgBase xMsg;
        if(!xMsg.ParseFromArray(msg.GetData(), msg.GetDataLen()))
        {
            char szData[MAX_PATH] = { 0 };
            sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", msg.GetMsgHead()->GetMsgID());
            LogRecive(szData);

            return false;
        }

        if (!xData.ParseFromString(xMsg.msg_data()))
        {
            char szData[MAX_PATH] = { 0 };
            sprintf(szData, "Parse Message Failed from MsgData to ProtocolData, MessageID: %d\n", msg.GetMsgHead()->GetMsgID());
            LogRecive(szData);

            return false;
        }

        nPlayer = xMsg.player_id();

        return true;
	}

	virtual bool Execute(const float fLasFrametime, const float fStartedTime)
	{
		return m_pNet->Execute(fLasFrametime, fStartedTime);
	}

	bool SendMsgPB(const uint16_t nMsgID, google::protobuf::Message& xData, const uint32_t nSockIndex = 0, const int32_t nPlayer = 0, bool bBroadcast = false)
	{
		if (!m_pNet)
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", nSockIndex, nMsgID);
			LogSend(szData);

			return false;
		}

		NFMsg::MsgBase xMsg;
		if (!xData.SerializeToString(xMsg.mutable_msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nSockIndex, nMsgID);
			LogSend(szData);

			return false;
		}

		//playerid��Ҫ������ת����Ϣ��ʱ����ʶ��ʹ�ã�����ʹ�ò�ʹ��
		xMsg.set_player_id(nPlayer);

		std::string strMsg;
		if(!xMsg.SerializeToString(&strMsg))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);
			LogSend(szData);

			return false;
		}

		NFCPacket xPacket(m_pNet->GetHeadLen());
		if(!xPacket.EnCode(nMsgID, strMsg.c_str(), strMsg.length()))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Encode of MsgData, MessageID: %d, MessageLen: %d\n", nSockIndex, nMsgID, strMsg.length());
			LogSend(szData);

			return false;
		}

		return m_pNet->SendMsg(xPacket, nSockIndex, bBroadcast);

	}

	NFINet* GetNet()
	{
		return m_pNet;
	}


	std::string mstrConfigIdent;

protected:
	NFINet* m_pNet;

};

#endif