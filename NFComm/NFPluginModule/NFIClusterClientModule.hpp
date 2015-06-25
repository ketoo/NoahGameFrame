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
		eState = NFMsg::EServerState::EST_CRASH;
	}

	int nGameID;
	NF_SERVER_TYPE eServerType;
	std::string strIP;
	int nPort;
	std::string strName;
	NFMsg::EServerState eState;
	NF_SHARE_PTR<NFINetModule> mxNetModule;

};

class NFIClusterClientModule
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

    virtual void SendByServerID(const int nServerID, const std::string& strData)
    {
        NF_SHARE_PTR<ServerData> pServer = mxServerMap.GetElement(nServerID);
        if (pServer)
        {
            NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
            if (pNetModule.get())
            {
                pNetModule->SendMsg(strData);
            }
        }
    }

    virtual void SendBySuit(const int& nHashKey, const std::string& strData)
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

        SendByServerID(xNode.nMachineID, strData);
    }

protected:    
    virtual bool Execute(const float fLastFrametime, const float fStartedTime)
    {
        ServerData* pServerData = mxServerMap.FirstNude();
        while (pServerData)
        {
            pServerData->mxNetModule->Execute(fLastFrametime, fStartedTime);

            pServerData = mxServerMap.NextNude();
        }

        ProcessAddNetConnect();
        return true;
    }

    virtual int OnSocketEvent(const int fd, const NF_NET_EVENT eEvent, NFINet* pNet)
    {
        if (eEvent & BEV_EVENT_CONNECTED)
        {
            OnConnected(fd, pNet);
        }
        else
        {
            OnDisConnected(fd, pNet);
        }

        return 0;
    }

private:
    virtual void OnNetCreated(NF_SHARE_PTR<ServerData> xServerData) = 0;
    virtual int OnConnected(const int fd, NFINet* pNet)
    {
        NF_SHARE_PTR<ServerData> pServerInfo = GetServerNetInfo(pNet);
        if (pServerInfo.get())
        {
            AddServerWeightData(pServerInfo);
            pServerInfo->eState = NFMsg::EServerState::EST_NARMAL;          
        }

        return 0;
    }

    virtual int OnDisConnected(const int fd, NFINet* pNet)
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

                    //智能指针自己释放
                    xServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule());
                    OnNetCreated(xServerData);
                }
            }
            else
            {
                if (NFMsg::EServerState::EST_MAINTEN != xInfo.eState
                    && NFMsg::EServerState::EST_CRASH != xInfo.eState)
                {
                    //正常，添加新服务器
                    xServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());

                    xServerData->nGameID = xInfo.nGameID;
                    xServerData->eServerType = xInfo.eServerType;
                    xServerData->strIP = xInfo.strIP;
                    xServerData->strName = xInfo.strName;
                    xServerData->eState = xInfo.eState;
                    xServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule());

                    OnNetCreated(xServerData);

                    mxServerMap.AddElement(xInfo.nGameID, xServerData);
                }
            }
        }

        mxTempNetList.clear();
    }

    virtual bool GetServerMachineData(const std::string& strServerID, NFCMachineNode& xMachineData)
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

    NF_SHARE_PTR<ServerData> GetServerNetInfo(const NFINet* pNet)
    {
        int nServerID = 0;
        for (NF_SHARE_PTR<ServerData> pData = mxServerMap.First(nServerID); pData != NULL; pData = mxServerMap.Next(nServerID))
        {
            if (pData->mxNetModule.get() && pNet == pData->mxNetModule->GetNFINet())
            {
                return pData;
            }
        }

        return NF_SHARE_PTR<ServerData> (NULL);
    }

private:
	NFMapEx<int, ServerData> mxServerMap;
	NFCConsistentHash mxConsistentHash;
    std::list<ServerData> mxTempNetList;
};

#endif