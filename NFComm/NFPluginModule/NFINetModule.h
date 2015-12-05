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
#include <iosfwd>
#include "NFILogicModule.h"
#include "NFComm/NFNet/NFCNet.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#include "NFComm/NFCore/NFQueue.h"
#include "NFGUID.h"

enum NF_SERVER_TYPE
{
	NFST_NONE = 0,
	NFST_CIENT = 1,
	NFST_MAXTER_SERVER = 2,
	NFST_WORLD_SERVER = 3,
	NFST_GAME_SERVER = 4,
	NFST_PROXY_SERVER = 5,
	NFST_LOGIN_SERVER = 6,
	NFST_NOSQL_SERVER = 7,
};


#if NF_PLATFORM == NF_PLATFORM_WIN
class QueueEventPack
{
public:
	QueueEventPack()
	{
		eMsgType = ON_NET_NONE;
		bBC = false;
	}

	enum NetEventType
	{
		ON_NET_NONE = 0,
		ON_NET_RECIVE = 1,
		ON_NET_SEND = 2,
		ON_NET_FD_EVT = 3,
		ON_NET_TRANFORM = 4,
	};

	int nFD;//FD
	NetEventType eMsgType;//消息类型
	int nMsgID;//消息ID，如果是网络事件，则为事件ID
	std::string strData;//数据
	bool bBC;
};
#endif

////////////////////////////////////////////////////////////////////////////

// 客户端消息处理宏
#define CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msgData, nLen, msg)                 \
	NFGUID nPlayerID;                                \
	msg xMsg;                                           \
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msgData, nLen, xMsg, nPlayerID))             \
{                                                   \
	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(), "", "Parse msg error", __FUNCTION__, __LINE__); \
	return;                                         \
}                                                   \
	\
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(nPlayerID); \
	if ( NULL == pObject.get() )                        \
{                                                   \
	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "FromClient Object do not Exist", "", __FUNCTION__, __LINE__); \
	return;                                         \
}

#define CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msgData, nLen, msg)                 \
	NFGUID nPlayerID;                                \
	msg xMsg;                                           \
	if (!RecivePB(nSockIndex, nMsgID, msgData, nLen, xMsg, nPlayerID))             \
{                                                   \
	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(), "", "Parse msg error", __FUNCTION__, __LINE__); \
	return;                                         \
}

//////////////////////////////////////////////////////////////////////////

class NFINetModule
	: public NFILogicModule
{

public:
	NFINetModule()
	{
		mfLastHBTime = 0.0f;
		m_pNet = NULL;
	}

	virtual ~NFINetModule()
	{
		if (m_pNet)
		{
			m_pNet->Final();
		}

        delete m_pNet;
        m_pNet = NULL;
	}

	template<typename BaseType>
	void Initialization(BaseType* pBaseType, void (BaseType::*handleRecieve)(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen), void (BaseType::*handleEvent)(const int, const NF_NET_EVENT, NFINet*), const char* strIP, const unsigned short nPort)
	{
#if NF_PLATFORM == NF_PLATFORM_WIN
   //windows use queue
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		m_pNet = new NFCNet(this, &NFINetModule::OnRecivePack, &NFINetModule::OnSocketEvent);
#else
		m_pNet = new NFCNet(pBaseType, handleRecieve, handleEvent);
#endif
		m_pNet->Initialization(strIP, nPort);
	}

	template<typename BaseType>
	int Initialization(BaseType* pBaseType, void (BaseType::*handleRecieve)(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen), void (BaseType::*handleEvent)(const int, const NF_NET_EVENT, NFINet*), const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4)
	{
#if NF_PLATFORM == NF_PLATFORM_WIN
	 //windows use queue
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		m_pNet = new NFCNet(this, &NFINetModule::OnRecivePack, &NFINetModule::OnSocketEvent);
#else
		m_pNet = new NFCNet(pBaseType, handleRecieve, handleEvent);
#endif

		return m_pNet->Initialization(nMaxClient, nPort, nCpuCount);
	}

	virtual bool Execute(const float fLasFrametime, const float fStartedTime)
	{
		if (!m_pNet)
		{
			return false;
		}

		//把上次的数据处理了
		KeepAlive(fLasFrametime);

		//////////////////////////////////////////////////////////////////////////
#if NF_PLATFORM == NF_PLATFORM_WIN
		QueueEventPack xEventPack;
		while (mxQueue.Pop(xEventPack))
		{
			switch (xEventPack.eMsgType)
			{
			case QueueEventPack::ON_NET_RECIVE:
				{
					if(mRecvCB)
					{
						mRecvCB(xEventPack.nFD, xEventPack.nMsgID, xEventPack.strData.c_str(), xEventPack.strData.length());
					}
				}
				break;
			//case QueueEventPack::ON_NET_SEND:
			case QueueEventPack::ON_NET_TRANFORM:
				{
					if (xEventPack.bBC)
					{
						m_pNet->SendMsgToAllClientWithOutHead(xEventPack.nMsgID, xEventPack.strData.c_str(), xEventPack.strData.length());
					}
					else
					{
						m_pNet->SendMsgWithOutHead(xEventPack.nMsgID, xEventPack.strData.c_str(), xEventPack.strData.length(), xEventPack.nFD);
					}
				}
				break;
			case QueueEventPack::ON_NET_FD_EVT:
				{
					if(mEventCB)
					{
						mEventCB(xEventPack.nFD, (NF_NET_EVENT)xEventPack.nMsgID, m_pNet);
					}
				}
				break;
			}
		}
#endif
		//////////////////////////////////////////////////////////////////////////
		return m_pNet->Execute();
	}

	static NFGUID PBToNF(NFMsg::Ident xID)
	{
		NFGUID  xIdent;
		xIdent.nHead64 = xID.svrid();
		xIdent.nData64 = xID.index();

		return xIdent;
	}

	static NFMsg::Ident NFToPB(NFGUID xID)
	{
		NFMsg::Ident  xIdent;
		xIdent.set_svrid(xID.nHead64);
		xIdent.set_index(xID.nData64);

		return xIdent;
	}

    static bool RecivePB(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen, std::string& strMsg, NFGUID& nPlayer)
    {
        NFMsg::MsgBase xMsg;
        if(!xMsg.ParseFromArray(msg, nLen))
        {
            char szData[MAX_PATH] = { 0 };
            sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
            //LogRecive(szData);

            return false;
        }

        strMsg.assign(xMsg.msg_data().data(), xMsg.msg_data().length());

        nPlayer = PBToNF(xMsg.player_id());

        return true;
    }

	static bool RecivePB(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen, google::protobuf::Message& xData, NFGUID& nPlayer)
	{
		NFMsg::MsgBase xMsg;
		if(!xMsg.ParseFromArray(msg, nLen))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
			//LogRecive(szData);

			return false;
		}

		if (!xData.ParseFromString(xMsg.msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Parse Message Failed from MsgData to ProtocolData, MessageID: %d\n", nMsgID);
			//LogRecive(szData);

			return false;
		}

		nPlayer = PBToNF(xMsg.player_id());

		return true;
	}

	bool SendMsg(const int nMsgID, const std::string& msg, const int nSockIndex)
	{
#if NF_PLATFORM == NF_PLATFORM_WIN
		QueueEventPack xNetEventPack;
		xNetEventPack.eMsgType = QueueEventPack::ON_NET_TRANFORM;
		xNetEventPack.nMsgID = nMsgID;
		xNetEventPack.nFD = nSockIndex;
		xNetEventPack.strData = msg;//可以待优化,SerializeToString直接进来
		xNetEventPack.bBC = false;

		return mxQueue.Push(xNetEventPack);
#else
		return m_pNet->SendMsg(msg.c_str(), msg.length(), nSockIndex);
#endif
	}

	bool SendMsgToAllClient(const int nMsgID, const std::string& msg)
	{
#if NF_PLATFORM == NF_PLATFORM_WIN
		QueueEventPack xNetEventPack;
		xNetEventPack.eMsgType = QueueEventPack::ON_NET_TRANFORM;
		xNetEventPack.nMsgID = nMsgID;
		xNetEventPack.nFD = 0;
		xNetEventPack.strData = msg;//可以待优化,SerializeToString直接进来
		xNetEventPack.bBC = true;

		return mxQueue.Push(xNetEventPack);
#else
		return m_pNet->SendMsgToAllClient(msg.c_str(), msg.length());
#endif
	}

	bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex)
	{
		NFMsg::MsgBase xMsg;
		if (!xData.SerializeToString(xMsg.mutable_msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
		*pPlayerID = NFToPB(NFGUID());

		std::string strMsg;
		if(!xMsg.SerializeToString(&strMsg))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		SendMsg(nMsgID, strMsg, nSockIndex);

		return true;
	}

	bool SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message& xData)
	{
		NFMsg::MsgBase xMsg;
		if (!xData.SerializeToString(xMsg.mutable_msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nMsgID);

			return false;
		}

		NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
		*pPlayerID = NFToPB(NFGUID());

		std::string strMsg;
		if(!xMsg.SerializeToString(&strMsg))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nMsgID);

			return false;
		}

		return SendMsgToAllClient(nMsgID, strMsg);
	}

	bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList = NULL)
	{
		if (!m_pNet)
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		NFMsg::MsgBase xMsg;
		if (!xData.SerializeToString(xMsg.mutable_msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		//playerid主要是网关转发消息的时候做识别使用，其他使用不使用
		NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
		*pPlayerID = NFToPB(nPlayer);
		if (pClientIDList)
		{
			for (int i = 0; i < pClientIDList->size(); ++i)
			{
                const NFGUID& ClientID = (*pClientIDList)[i];

				NFMsg::Ident* pData = xMsg.add_player_client_list();
                if (pData)
                {
                   *pData = NFToPB(ClientID);
                }
			}
		}

		std::string strMsg;
		if(!xMsg.SerializeToString(&strMsg))
		{
			char szData[MAX_PATH] = { 0 };
			sprintf(szData, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		return SendMsg(nMsgID, strMsg, nSockIndex);
	}

    bool SendMsgPB(const uint16_t nMsgID, const std::string& strData, const uint32_t nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList = NULL)
    {
        if (!m_pNet)
        {
            char szData[MAX_PATH] = { 0 };
            sprintf(szData, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", nSockIndex, nMsgID);
            //LogSend(szData);

            return false;
        }

        NFMsg::MsgBase xMsg;
        xMsg.set_msg_data(strData.data(), strData.length());

        //playerid主要是网关转发消息的时候做识别使用，其他使用不使用
        NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
        *pPlayerID = NFToPB(nPlayer);
        if (pClientIDList)
        {
            for (int i = 0; i < pClientIDList->size(); ++i)
            {
                const NFGUID& ClientID = (*pClientIDList)[i];

                NFMsg::Ident* pData = xMsg.add_player_client_list();
                if (pData)
                {
                    *pData = NFToPB(ClientID);
                }
            }
        }

        std::string strMsg;
        if(!xMsg.SerializeToString(&strMsg))
        {
            char szData[MAX_PATH] = { 0 };
            sprintf(szData, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);
            //LogSend(szData);

            return false;
        }

		return SendMsg(nMsgID, strMsg, nSockIndex);
    }

	NFINet* GetNet()
	{
		return m_pNet;
	}

protected:

	void KeepAlive(float fLasFrametime)
	{
		if (!m_pNet)
		{
			return;
		}

		if (m_pNet->IsServer())
		{
			return;
		}

		if (mfLastHBTime < 10.0f)
		{
			mfLastHBTime += fLasFrametime;
			return;
		}

		mfLastHBTime = 0.0f;

		NFMsg::ServerHeartBeat xMsg;
		xMsg.set_count(0);

		SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_HEART_BEAT, xMsg, 0);

	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	void OnRecivePack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
	{
		QueueEventPack xNetEventPack;
		xNetEventPack.eMsgType = QueueEventPack::ON_NET_RECIVE;
		xNetEventPack.nMsgID = nMsgID;
		xNetEventPack.nFD = nSockIndex;
		xNetEventPack.strData.assign(msg, nLen);

		mxQueue.Push(xNetEventPack);
	}

	void OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
	{
		QueueEventPack xNetEventPack;
		xNetEventPack.eMsgType = QueueEventPack::ON_NET_FD_EVT;
		xNetEventPack.nMsgID = eEvent;
		xNetEventPack.nFD = nSockIndex;

		mxQueue.Push(xNetEventPack);
	}
#endif

private:

	NFINet* m_pNet;
	float mfLastHBTime;

#if NF_PLATFORM == NF_PLATFORM_WIN
	NET_RECIEVE_FUNCTOR mRecvCB;
	NET_EVENT_FUNCTOR mEventCB;

	NFQueue<QueueEventPack> mxQueue;
#endif

};

#endif
