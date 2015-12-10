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

////////////////////////////////////////////////////////////////////////////

// �ͻ�����Ϣ�����
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
		m_pNet = new NFCNet(pBaseType, handleRecieve, handleEvent);
		
		m_pNet->Initialization(strIP, nPort);
	}

	template<typename BaseType>
	int Initialization(BaseType* pBaseType, void (BaseType::*handleRecieve)(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen), void (BaseType::*handleEvent)(const int, const NF_NET_EVENT, NFINet*), const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4)
	{
		m_pNet = new NFCNet(pBaseType, handleRecieve, handleEvent);

		return m_pNet->Initialization(nMaxClient, nPort, nCpuCount);
	}

	virtual bool Execute(const float fLasFrametime, const float fStartedTime)
	{
		if (!m_pNet)
		{
			return false;
		}

		//���ϴε����ݴ�����
		KeepAlive(fLasFrametime);

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
			NFSPRINTF(szData, MAX_PATH, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
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
			NFSPRINTF(szData, MAX_PATH, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
			//LogRecive(szData);

			return false;
		}

		if (!xData.ParseFromString(xMsg.msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			NFSPRINTF(szData, MAX_PATH, "Parse Message Failed from MsgData to ProtocolData, MessageID: %d\n", nMsgID);
			//LogRecive(szData);

			return false;
		}

		nPlayer = PBToNF(xMsg.player_id());

		return true;
	}

	bool SendMsgWithOutHead(const int nMsgID, const std::string& msg, const int nSockIndex)
	{
		return m_pNet->SendMsgWithOutHead(nMsgID, msg.c_str(), msg.length(), nSockIndex);
	}

	bool SendMsgToAllClientWithOutHead(const int nMsgID, const std::string& msg)
	{
		return m_pNet->SendMsgToAllClient(msg.c_str(), msg.length());
	}

	bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex)
	{
		NFMsg::MsgBase xMsg;
		if (!xData.SerializeToString(xMsg.mutable_msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
		*pPlayerID = NFToPB(NFGUID());

		std::string strMsg;
		if(!xMsg.SerializeToString(&strMsg))
		{
			char szData[MAX_PATH] = { 0 };
			NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);

		return true;
	}

	bool SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message& xData)
	{
		NFMsg::MsgBase xMsg;
		if (!xData.SerializeToString(xMsg.mutable_msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nMsgID);

			return false;
		}

		NFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
		*pPlayerID = NFToPB(NFGUID());

		std::string strMsg;
		if(!xMsg.SerializeToString(&strMsg))
		{
			char szData[MAX_PATH] = { 0 };
			NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nMsgID);

			return false;
		}

		return SendMsgToAllClientWithOutHead(nMsgID, strMsg);
	}

	bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList = NULL)
	{
		if (!m_pNet)
		{
			char szData[MAX_PATH] = { 0 };
			NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		NFMsg::MsgBase xMsg;
		if (!xData.SerializeToString(xMsg.mutable_msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		//playerid��Ҫ������ת����Ϣ��ʱ����ʶ��ʹ�ã�����ʹ�ò�ʹ��
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
			NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);

			return false;
		}

		return SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);
	}

    bool SendMsgPB(const uint16_t nMsgID, const std::string& strData, const uint32_t nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList = NULL)
    {
        if (!m_pNet)
        {
            char szData[MAX_PATH] = { 0 };
            NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", nSockIndex, nMsgID);
            //LogSend(szData);

            return false;
        }

        NFMsg::MsgBase xMsg;
        xMsg.set_msg_data(strData.data(), strData.length());

        //playerid��Ҫ������ת����Ϣ��ʱ����ʶ��ʹ�ã�����ʹ�ò�ʹ��
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
            NFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);
            //LogSend(szData);

            return false;
        }

		return SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);
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

private:

	NFINet* m_pNet;
	float mfLastHBTime;
};

#endif
