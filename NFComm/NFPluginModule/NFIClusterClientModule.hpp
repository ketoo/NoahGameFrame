// -------------------------------------------------------------------------
//    @FileName         ：    NFIClusterClientModule.hpp
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2015-01-4
//    @Module           ：    NFIClusterClientModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_CLUSTER_CLIENT_MODULE_H
#define _NFI_CLUSTER_CLIENT_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"
#include "NFComm/NFCore/NFCConsistentHash.hpp"

enum ConnectDataState
{
	DISCONNECT,
	NORMAL,
	RECONNECT,

};
struct ConnectData 
{
	ConnectData()
	{
		nGameID = 0;
		nPort = 0;
		strName = "";
		strIP = "";
		eServerType = NF_ST_NONE;
		eState = ConnectDataState::DISCONNECT;
		mnLastActionTime = 0;
	}

	int nGameID;
	NF_SERVER_TYPES eServerType;
	std::string strIP;
	int nPort;
	std::string strName;
	ConnectDataState eState;
	NFINT64 mnLastActionTime;

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
	template<typename BaseType>
	int AddReciveCallBack(const int nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		std::map<int, NET_RECIEVE_FUNCTOR_PTR>::iterator it = mxReciveCallBack.find(nMsgID);
		if (mxReciveCallBack.end() == it)
		{
			NET_RECIEVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			NET_RECIEVE_FUNCTOR_PTR functorPtr(new NET_RECIEVE_FUNCTOR(functor));

			mxReciveCallBack.insert(std::map<int, NET_RECIEVE_FUNCTOR_PTR>::value_type(nMsgID, functorPtr));

			return true;
		}

		assert(nMsgID);

		return false;
	}

	template<typename BaseType>
	bool AddEventCallBack(BaseType* pBase, int (BaseType::*handler)(const int, const NF_NET_EVENT, NFINet*))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr(new NET_EVENT_FUNCTOR(functor));

		mxEventCallBack.push_back(functorPtr);

		return true;
	}

    void AddServer(const ConnectData& xInfo)
    {
        mxTempNetList.push_back(xInfo);
    }

	//裸数据,发时组包
    void SendByServerID(const int nServerID, const int nMsgID, const std::string& strData)
    {
        NF_SHARE_PTR<ConnectData> pServer = mxServerMap.GetElement(nServerID);
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
		NF_SHARE_PTR<ConnectData> pServer = mxServerMap.First();
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
		NF_SHARE_PTR<ConnectData> pServer = mxServerMap.GetElement(nServerID);
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
        NF_SHARE_PTR<ConnectData> pServer = mxServerMap.GetElement(nServerID);
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
		NF_SHARE_PTR<ConnectData> pServer = mxServerMap.First();
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

	NF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID)
	{
		return mxServerMap.GetElement(nServerID);
	}

	NFMapEx<int, ConnectData>& GetServerList()
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

    bool Execute()
    {
        ConnectData* pServerData = mxServerMap.FirstNude();
        while (pServerData)
        {
			switch (pServerData->eState)
			{
			case ConnectDataState::DISCONNECT:
				{
					if (NULL != pServerData->mxNetModule)
					{
						pServerData->mxNetModule = nullptr;
						pServerData->eState = ConnectDataState::RECONNECT;
						pServerData->mnLastActionTime = GetPluginManager()->GetNowTime();
					}
				}
				break;
			case ConnectDataState::NORMAL:
				{
					if (pServerData->mxNetModule)
					{
						pServerData->mxNetModule->Execute();

						KeepState(pServerData);
					}
				}
				break;
			case ConnectDataState::RECONNECT:
				{
					//计算时间
					if (pServerData->mnLastActionTime + 30.0f > GetPluginManager()->GetNowTime())
					{
						break;
					}

                    if (nullptr != pServerData->mxNetModule)
                    {
                        pServerData->mxNetModule = nullptr;
                    }

					pServerData->mnLastActionTime = GetPluginManager()->GetNowTime();

					pServerData->eState = ConnectDataState::NORMAL;
                    pServerData->mxNetModule = NF_SHARE_PTR<NFINetModule> (NF_NEW NFINetModule(pPluginManager));
					pServerData->mxNetModule->Initialization(this, &NFIClusterClientModule::OnRecivePack, &NFIClusterClientModule::OnSocketEvent, pServerData->strIP.c_str(), pServerData->nPort);
				}
				break;
			default:
				break;
			}

            pServerData = mxServerMap.NextNude();
        }

        ProcessAddNetConnect();

        return true;
    }

	NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NFINet* pNet)
	{
		int nServerID = 0;
		for (NF_SHARE_PTR<ConnectData> pData = mxServerMap.First(nServerID); pData != NULL; pData = mxServerMap.Next(nServerID))
		{
			if (pData->mxNetModule.get() && pNet == pData->mxNetModule->GetNet())
			{
				return pData;
			}
		}

		return NF_SHARE_PTR<ConnectData> (NULL);
	}

	virtual void KeepReport(ConnectData* pServerData){};
	virtual void LogServerInfo(const std::string& strServerInfo){};

private:
	virtual void LogServerInfo()
	{
		LogServerInfo("This is a client, begin to Printf Server Info----------------------------------");

		ConnectData* pServerData = mxServerMap.FirstNude();
		while (pServerData)
		{
			std::ostringstream stream;
			stream << "Type: " << pServerData->eServerType << " ProxyServer ID: " << pServerData->nGameID << " State: " <<  pServerData->eState << " IP: " << pServerData->strIP << " Port: " << pServerData->nPort;

			LogServerInfo(stream.str());

			pServerData = mxServerMap.NextNude();
		}

		LogServerInfo("This is a client, end to Printf Server Info----------------------------------");
	};

	void KeepState(ConnectData* pServerData)
	{

		if (pServerData->mnLastActionTime + 10 > GetPluginManager()->GetNowTime())
		{
			return;
		}

		pServerData->mnLastActionTime = GetPluginManager()->GetNowTime();

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
        NF_SHARE_PTR<ConnectData> pServerInfo = GetServerNetInfo(pNet);
        if (pServerInfo.get())
        {
            AddServerWeightData(pServerInfo);
            pServerInfo->eState = ConnectDataState::NORMAL;
        }

        return 0;
    }

    int OnDisConnected(const int fd, NFINet* pNet)
    {
        NF_SHARE_PTR<ConnectData> pServerInfo = GetServerNetInfo(pNet);
        if (pServerInfo.get())
        {
            RemoveServerWeightData(pServerInfo);
            pServerInfo->eState = ConnectDataState::DISCONNECT;
        }

        return 0;
    }

    void ProcessAddNetConnect()
    {
        std::list<ConnectData>::iterator it = mxTempNetList.begin();
        for (; it != mxTempNetList.end(); ++it)
        {
            const ConnectData& xInfo = *it;
            NF_SHARE_PTR<ConnectData> xServerData = mxServerMap.GetElement(xInfo.nGameID);
            if (nullptr == xServerData)
            {
				//正常，添加新服务器
				xServerData = NF_SHARE_PTR<ConnectData>(NF_NEW ConnectData());

				xServerData->nGameID = xInfo.nGameID;
				xServerData->eServerType = xInfo.eServerType;
				xServerData->strIP = xInfo.strIP;
				xServerData->strName = xInfo.strName;
				xServerData->eState = ConnectDataState::NORMAL;
				xServerData->nPort = xInfo.nPort;
				xServerData->mnLastActionTime = GetPluginManager()->GetNowTime();

				xServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule(pPluginManager));
				xServerData->mxNetModule->Initialization(this, &NFIClusterClientModule::OnRecivePack, &NFIClusterClientModule::OnSocketEvent, xServerData->strIP.c_str(), xServerData->nPort);

				//add msg callback
				std::map<int, NET_RECIEVE_FUNCTOR_PTR>::iterator itReciveCB = mxReciveCallBack.begin();
				for ( ; mxReciveCallBack.end() != itReciveCB; ++itReciveCB)
				{
					xServerData->mxNetModule->GetNet()->AddReciveCallBack(itReciveCB->first, itReciveCB->second);
				}

				//add event callback
				std::list<NET_EVENT_FUNCTOR_PTR>::iterator itEventCB = mxEventCallBack.begin();
				for ( ; mxEventCallBack.end() != itEventCB; ++itEventCB)
				{
					xServerData->mxNetModule->GetNet()->AddEventCallBack(*itEventCB);
				}

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

    void AddServerWeightData(NF_SHARE_PTR<ConnectData> xInfo)
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

    void RemoveServerWeightData(NF_SHARE_PTR<ConnectData> xInfo)
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

	NFMapEx<int, ConnectData> mxServerMap;
	NFCConsistentHash mxConsistentHash;

    std::list<ConnectData> mxTempNetList;

	//call back
	std::map<int, NET_RECIEVE_FUNCTOR_PTR> mxReciveCallBack;
	std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBack;
};

#endif