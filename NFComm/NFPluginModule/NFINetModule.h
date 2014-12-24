// -------------------------------------------------------------------------
//    @FileName         ：    NFINetModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFINetModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_NET_MODULE_H_
#define _NFI_NET_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFNet/NFCNet.h"
#include "NFComm/NFNet/NFCPacket.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NetEventPack
{
public:
	enum NetEventType
	{
		ON_RECIVE = 0,
		ON_SEND = 1,
		ON_SOCKET_EVT = 2,
	};

	NetEventType eMsgType;
	NFCPacket xPacket;
};

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

    virtual void LogRecive(const char* str){};
    virtual void LogSend(const char* str){};

	static NFIDENTID PBToNF(NFMsg::Ident xID)
	{
		NFIDENTID  xIdent;
		xIdent.nSvrID = xID.svrid();
		xIdent.nData64 = xID.index();

		return xIdent;
	}

	static NFMsg::Ident NFToPB(NFIDENTID xID)
	{
		NFMsg::Ident  xIdent;
		xIdent.set_svrid(xID.nSvrID);
		xIdent.set_index(xID.nData64);

		return xIdent;
	}

	bool RecivePB(const NFIPacket& msg, google::protobuf::Message& xData, NFIDENTID& nPlayer)
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

        nPlayer = PBToNF(xMsg.player_id());

        return true;
	}

//     bool VerifyProtocol(const NFIPacket& msg, const int nFD)
//     {
//         NFMsg::MsgBase xMsg;
//         if(!xMsg.ParseFromArray(msg.GetData(), msg.GetDataLen()))
//         {
//             char szData[MAX_PATH] = { 0 };
//             sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", msg.GetMsgHead()->GetMsgID());
//             LogRecive(szData);
// 
//             return false;
//         }
// 
// 		NetObject* pObject = GetNet()->GetNetObject(nFD);
// 		if (pObject)
// 		{
// 			pObject->get
// 		}
//         if(nPlayer != PBToNF(xMsg.player_id()))
//         {
//             return false;
//         }
// 
//         return true;
//     }

	virtual bool Execute(const float fLasFrametime, const float fStartedTime)
	{
		return m_pNet->Execute(fLasFrametime, fStartedTime);
	}

	bool SendMsgPB(const uint16_t nMsgID, google::protobuf::Message& xData, const uint32_t nSockIndex = 0, const NFIDENTID nPlayer = NFIDENTID(), const std::vector<int>* pFdList = NULL, bool bBroadcast = false)
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

		//playerid主要是网关转发消息的时候做识别使用，其他使用不使用
        NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
		*pPlayerID = NFToPB(nPlayer);
        if (pFdList)
        {
            for (int i = 0; i < pFdList->size(); ++i)
            {
                xMsg.add_player_fd_list((*pFdList)[i]);
            }
        }


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

    //if you are a client
    virtual int GetFD(){return 0;}

	std::string mstrConfigIdent;

protected:
	NFINet* m_pNet;
	NFQueue<NetEventPack> mxQueue;

};

#endif