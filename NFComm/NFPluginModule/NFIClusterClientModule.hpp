// -------------------------------------------------------------------------
//    @FileName         ：    NFIClusterClientModule.hpp
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2015-01-4
//    @Module           ：    NFIClusterClientModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_CLUSTER_CLIENT_MODULE_H_
#define _NFI_CLUSTER_CLIENT_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"
#include "NFComm/NFCore/NFCConsistentHash.hpp"

struct ServerData 
{
	ServerData()
	{
		nGameID = 0;
		nPort = 0;
		strName = "";
		strIP = "";
		eServerType = NFST_NONE;
		eState = NFMsg::EServerState::EST_MAINTEN;
		mfLastReportTime = 0;
	}

	int nGameID;
	NF_SERVER_TYPE eServerType;
	std::string strIP;
	int nPort;
	std::string strName;
	NFMsg::EServerState eState;
	float mfLastReportTime;

	NF_SHARE_PTR<NFINetModule> mxNetModule;
};

class NFIClusterClientModule : public NFILogicModule
{
public:
    enum EConstDefine
    {
        EConstDefine_DefaultWeith = 500, 
    };

public:

    void AddServer(const ServerData& xInfo)
    {
        mxTempNetList.push_back(xInfo);
    }

	//裸数据,发时组包
    void SendByServerID(const int nServerID, const int nMsgID, const std::string& strData)
    {
        NF_SHARE_PTR<ServerData> pServer = mxServerMap.GetElement(nServerID);
        if (pServer)
        {
            NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
            if (pNetModule.get())
            {
                pNetModule->SendMsgWithOutHead(nMsgID, strData, 0);
            }
        }
    }

	//裸数据,发时组包
	void SendToAllServer(const int nMsgID, const std::string& strData)
	{
		NF_SHARE_PTR<ServerData> pServer = mxServerMap.First();
		while (pServer)
		{
			NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
			if (pNetModule.get())
			{
				pNetModule->SendMsgWithOutHead(nMsgID, strData, 0);
			}

			pServer = mxServerMap.Next();
		}
	}

	void SendToServerByPB(const int nServerID, const uint16_t nMsgID, google::protobuf::Message& xData, const int nSockIndex = 0, const NFGUID nPlayer = NFGUID(), const std::vector<NFGUID>* pClientIDList = NULL, bool bBroadcast = false)
	{
		NF_SHARE_PTR<ServerData> pServer = mxServerMap.GetElement(nServerID);
		if (pServer)
		{
			NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
			if (pNetModule.get())
			{
				pNetModule->SendMsgPB(nMsgID, xData, nSockIndex, nPlayer, pClientIDList);
			}
		}
	}

    void SendByServerID(const int nServerID, const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
    {
        NF_SHARE_PTR<ServerData> pServer = mxServerMap.GetElement(nServerID);
        if (pServer)
        {
            NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
            if (pNetModule.get())
            {
                pNetModule->GetNet()->SendMsgWithOutHead(nMsgID, msg, nLen, 0);
            }
        }
    }

	void SendToAllServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData, const NFGUID nPlayer = NFGUID(), const std::vector<NFGUID>* pClientIDList = NULL)
	{
		NF_SHARE_PTR<ServerData> pServer = mxServerMap.First();
		while (pServer)
		{
			NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
			if (pNetModule.get())
			{
				pNetModule->SendMsgPB(nMsgID, xData, 0, nPlayer, pClientIDList);
			}

			pServer = mxServerMap.Next();
		}

	}

    void SendBySuit(const int& nHashKey, const int nMsgID, const std::string& strData)
    {
        if (mxConsistentHash.Size() <= 0)
        {
            return;
        }

        NFCMachineNode xNode;
        if (!GetServerMachineData(boost::lexical_cast<std::string> (nHashKey), xNode))
        {
            return ;
        }

        SendByServerID(nMsgID, xNode.nMachineID, strData);
    }

    void SendBySuit(const int& nHashKey, const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
    {
        if (mxConsistentHash.Size() <= 0)
        {
            return;
        }

        NFCMachineNode xNode;
        if (!GetServerMachineData(boost::lexical_cast<std::string>(nHashKey), xNode))
        {
            return ;
        }

        SendByServerID(xNode.nMachineID, nSockIndex, nMsgID, msg, nLen);
    }

	void SendSuitByPB(const int& nHashKey, const uint16_t nMsgID, google::protobuf::Message& xData, const int nSockIndex = 0, const NFGUID nPlayer = NFGUID(), const std::vector<NFGUID>* pClientIDList = NULL)
	{
		if (mxConsistentHash.Size() <= 0)
		{
			return;
		}

		NFCMachineNode xNode;
		if (!GetServerMachineData(boost::lexical_cast<std::string> (nHashKey), xNode))
		{
			return ;
		}

		SendToServerByPB(xNode.nMachineID, nMsgID, xData, nSockIndex, nPlayer, pClientIDList);
	}

	void SendSuitByPB(const uint16_t nMsgID, google::protobuf::Message& xData, const int nSockIndex = 0, const NFGUID nPlayer = NFGUID(), const std::vector<NFGUID>* pClientIDList = NULL)
	{
		if (mxConsistentHash.Size() <= 0)
		{
			return;
		}

		const int nHashKey = 0;

		SendSuitByPB(nHashKey, nMsgID, xData, nSockIndex, nPlayer, pClientIDList);
	}

	NF_SHARE_PTR<ServerData> GetServerNetInfo(const int nServerID)
	{
		return mxServerMap.GetElement(nServerID);
	}

	NFMapEx<int, ServerData>& GetServerList()
	{
		return mxServerMap;
	}

protected:  
	template<typename BaseType>
	void Bind(BaseType* pBaseType, void (BaseType::*handleRecieve)(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen), void (BaseType::*handleEvent)(const int, const NF_NET_EVENT, NFINet*))
	{
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

    bool Execute(const float fLastFrametime, const float fStartedTime)
    {
        ServerData* pServerData = mxServerMap.FirstNude();
        while (pServerData)
        {
            pServerData->mxNetModule->Execute(fLastFrametime, fStartedTime);

			KeepState(pServerData, fLastFrametime, fStartedTime);

            pServerData = mxServerMap.NextNude();
        }

        ProcessAddNetConnect();

        return true;
    }

	NF_SHARE_PTR<ServerData> GetServerNetInfo(const NFINet* pNet)
	{
		int nServerID = 0;
		for (NF_SHARE_PTR<ServerData> pData = mxServerMap.First(nServerID); pData != NULL; pData = mxServerMap.Next(nServerID))
		{
			if (pData->mxNetModule.get() && pNet == pData->mxNetModule->GetNet())
			{
				return pData;
			}
		}

		return NF_SHARE_PTR<ServerData> (NULL);
	}

	virtual void KeepReport(ServerData* pServerData){};
	virtual void LogServerInfo(const std::string& strServerInfo){};

private:
	virtual void LogServerInfo()
	{
		LogServerInfo("This is a client, begin to Printf Server Info----------------------------------");

		ServerData* pServerData = mxServerMap.FirstNude();
		while (pServerData)
		{
			std::ostringstream stream;
			stream << "Type: " << pServerData->eServerType << " ProxyServer ID: " << pServerData->nGameID << " State: " <<  NFMsg::EServerState_Name(pServerData->eState) << " IP: " << pServerData->strIP << " Port: " << pServerData->nPort;

			LogServerInfo(stream.str());

			pServerData = mxServerMap.NextNude();
		}

		LogServerInfo("This is a client, end to Printf Server Info----------------------------------");
	};

	void KeepState(ServerData* pServerData, const float fLastFrametime, const float fStartedTime)
	{
		if (pServerData->mfLastReportTime < 10.0f)
		{
			pServerData->mfLastReportTime += fLastFrametime;
			return;
		}

		pServerData->mfLastReportTime = 0.0f;

		KeepReport(pServerData);
		LogServerInfo();
	}

	void OnSocketEvent(const int fd, const NF_NET_EVENT eEvent, NFINet* pNet)
	{
		if (eEvent & BEV_EVENT_CONNECTED)
		{
			OnConnected(fd, pNet);
		}
		else
		{
			OnDisConnected(fd, pNet);
		}

		if (mEventCB)
		{
			mEventCB(fd, eEvent, pNet);
		}
	}

	void OnRecivePack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
	{
		if (mRecvCB)
		{
			mRecvCB(nSockIndex, nMsgID, msg, nLen);
		}

	}

    int OnConnected(const int fd, NFINet* pNet)
    {
        NF_SHARE_PTR<ServerData> pServerInfo = GetServerNetInfo(pNet);
        if (pServerInfo.get())
        {
            AddServerWeightData(pServerInfo);
            pServerInfo->eState = NFMsg::EServerState::EST_NARMAL;          
        }

        return 0;
    }

    int OnDisConnected(const int fd, NFINet* pNet)
    {
        NF_SHARE_PTR<ServerData> pServerInfo = GetServerNetInfo(pNet);
        if (pServerInfo.get())
        {
            RemoveServerWeightData(pServerInfo);
            pServerInfo->eState = NFMsg::EServerState::EST_CRASH;          
        }

        return 0;
    }

    void ProcessAddNetConnect()
    {
        std::list<ServerData>::iterator it = mxTempNetList.begin();
        for (; it != mxTempNetList.end(); ++it)
        {
            const ServerData& xInfo = *it;
            NF_SHARE_PTR<ServerData> xServerData = mxServerMap.GetElement(xInfo.nGameID);
            if (xServerData)
            {
                //新的信息
                if (xInfo.strIP != xServerData->strIP || xInfo.nPort != xServerData->nPort)
                {
                    //断旧，连新
                    RemoveServerWeightData(xServerData);

                    xServerData->nGameID = xInfo.nGameID;
                    xServerData->eServerType = xInfo.eServerType;
                    xServerData->strIP = xInfo.strIP;
                    xServerData->strName = xInfo.strName;
                    xServerData->eState = xInfo.eState;
					xServerData->nPort = xInfo.nPort;

                    //智能指针自己释放
                    xServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule());
					xServerData->mxNetModule->Initialization(this, &NFIClusterClientModule::OnRecivePack, &NFIClusterClientModule::OnSocketEvent, xServerData->strIP.c_str(), xServerData->nPort);

                }
            }
            else
            {
				//正常，添加新服务器
				xServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());

				xServerData->nGameID = xInfo.nGameID;
				xServerData->eServerType = xInfo.eServerType;
				xServerData->strIP = xInfo.strIP;
				xServerData->strName = xInfo.strName;
				xServerData->eState = xInfo.eState;
				xServerData->nPort = xInfo.nPort;

				xServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule());
				xServerData->mxNetModule->Initialization(this, &NFIClusterClientModule::OnRecivePack, &NFIClusterClientModule::OnSocketEvent, xServerData->strIP.c_str(), xServerData->nPort);


				mxServerMap.AddElement(xInfo.nGameID, xServerData);
            }
        }

        mxTempNetList.clear();
    }

    bool GetServerMachineData(const std::string& strServerID, NFCMachineNode& xMachineData)
    {
        boost::crc_32_type ret;
        ret.process_bytes(strServerID.c_str(), strServerID.length());
        uint32_t nCRC32 = ret.checksum();

        if (mxConsistentHash.GetSuitNode(nCRC32, xMachineData))
        {
            return true;
        }

        return false;
    }

    void AddServerWeightData(NF_SHARE_PTR<ServerData> xInfo)
    {
        //根据权重创建节点
        for (int j = 0; j < EConstDefine_DefaultWeith; ++j)
        {
            NFCMachineNode vNode(j);

            vNode.nMachineID = xInfo->nGameID;
            vNode.strIP = xInfo->strIP;
            vNode.nPort = xInfo->nPort;
            vNode.nWeight = EConstDefine_DefaultWeith;
            mxConsistentHash.Insert(vNode);
        }
    }

    void RemoveServerWeightData(NF_SHARE_PTR<ServerData> xInfo)
    {
        for (int j = 0; j < EConstDefine_DefaultWeith; ++j)
        {
            NFCMachineNode vNode(j);

            vNode.nMachineID = xInfo->nGameID;
            vNode.strIP = xInfo->strIP;
            vNode.nPort = xInfo->nPort;
            vNode.nWeight = EConstDefine_DefaultWeith;
            mxConsistentHash.Erase(vNode);
        }
    }

private:
	NET_RECIEVE_FUNCTOR mRecvCB;
	NET_EVENT_FUNCTOR mEventCB;

	NFMapEx<int, ServerData> mxServerMap;
	NFCConsistentHash mxConsistentHash;

    std::list<ServerData> mxTempNetList;
};

#endif