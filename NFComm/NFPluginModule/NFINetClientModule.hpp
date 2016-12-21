// -------------------------------------------------------------------------
//    @FileName         :    NFINetClientModule.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-4
//    @Module           :    NFINetClientModule
//
// -------------------------------------------------------------------------

#ifndef NFI_CLUSTER_CLIENT_MODULE_H
#define NFI_CLUSTER_CLIENT_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"
#include "NFComm/NFCore/NFCConsistentHash.hpp"

enum ConnectDataState
{
	DISCONNECT,
    CONNECTING,
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

class NFINetClientModule : public NFIModule
{
protected:
	NFINetClientModule()
	{
	}
public:
    enum EConstDefine
    {
        EConstDefine_DefaultWeith = 500,
    };

	NFINetClientModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init()
	{
		AddEventCallBack(this, &NFINetClientModule::OnSocketEvent);

		return true;
	}

	virtual bool AfterInit()
	{
		return true;
	}

	virtual bool BeforeShut()
	{
		return true;
	}

	virtual bool Shut()
	{
		return true;
	}

	virtual bool Execute()
	{
		ProcessExecute();
		ProcessAddNetConnect();

		return true;
	}

	template<typename BaseType>
	bool AddReceiveCallBack(const int nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxReceiveCallBack.find(nMsgID);
		if (mxReceiveCallBack.end() == it)
		{
			NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

			mxReceiveCallBack.insert(std::map<int, NET_RECEIVE_FUNCTOR_PTR>::value_type(nMsgID, functorPtr));

			return true;
		}

		assert(nMsgID);

		return false;
	}
	
	void RemoveReceiveCallBack(const int nMsgID)
	{
		std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxReceiveCallBack.find(nMsgID);
		if (mxReceiveCallBack.end() == it)
		{
			mxReceiveCallBack.erase(it);
		}
	}

	template<typename BaseType>
	int AddReceiveCallBack(BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		mxCallBackList.push_back(functorPtr);

		return false;
	}

	template<typename BaseType>
	bool AddEventCallBack(BaseType* pBase, void (BaseType::*handler)(const int, const NF_NET_EVENT, NFINet*))
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

    ////////////////////////////////////////////////////////////////////////////////
    void SendByServerID(const int nServerID, const int nMsgID, const std::string& strData)
    {
        SendByServerID(nServerID, nMsgID, strData.c_str(), strData.length());
    }

    void SendByServerID(const int nServerID, const int nMsgID, const char* msg, const uint32_t nLen)
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

	void SendToServerByPB(const int nServerID, const uint16_t nMsgID, google::protobuf::Message& xData)
	{
		NF_SHARE_PTR<ConnectData> pServer = mxServerMap.GetElement(nServerID);
		if (pServer)
		{
			NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
			if (pNetModule.get())
			{
				pNetModule->SendMsgPB(nMsgID, xData, 0, NFGUID(), NULL);
			}
		}
	}

	void SendToAllServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData)
	{
		NF_SHARE_PTR<ConnectData> pServer = mxServerMap.First();
		while (pServer)
		{
			NF_SHARE_PTR<NFINetModule> pNetModule = pServer->mxNetModule;
			if (pNetModule.get())
			{
				pNetModule->SendMsgPB(nMsgID, xData, 0);
			}

			pServer = mxServerMap.Next();
		}
	}

    ////////////////////////////////////////////////////////////////////////////////

    void SendBySuit(const std::string& strHashKey, const int nMsgID, const std::string& strData)
    {
        uint32_t nCRC32 = NFrame::CRC32(strHashKey);
        SendBySuit(nCRC32, nMsgID, strData);
    }

    void SendBySuit(const std::string& strHashKey, const int nMsgID, const char* msg, const uint32_t nLen)
    {
        uint32_t nCRC32 = NFrame::CRC32(strHashKey);
        SendBySuit(nCRC32, nMsgID, msg, nLen);
    }

    void SendBySuit(const int& nHashKey, const int nMsgID, const std::string& strData)
    {
        SendBySuit(nHashKey, nMsgID, strData.c_str(), strData.length());
    }

    void SendBySuit(const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen)
    {
		NF_SHARE_PTR<ConnectData> pConnectData = mxServerMap.GetElementBySuit(nHashKey);
		if (pConnectData)
		{

		}

        SendByServerID(pConnectData->nGameID, nMsgID, msg, nLen);
    }

    void SendSuitByPB(const std::string& strHashKey, const uint16_t nMsgID, google::protobuf::Message& xData)
    {
        uint32_t nCRC32 = NFrame::CRC32(strHashKey);
        SendSuitByPB(nCRC32, nMsgID, xData);
    }

	void SendSuitByPB(const int& nHashKey, const uint16_t nMsgID, google::protobuf::Message& xData)
	{
		NF_SHARE_PTR<ConnectData> pConnectData = mxServerMap.GetElementBySuit(nHashKey);
		if (pConnectData)
		{

		}
		SendToServerByPB(pConnectData->nGameID, nMsgID, xData);
	}

    ////////////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID)
	{
		return mxServerMap.GetElement(nServerID);
	}

	NFMapEx<int, ConnectData>& GetServerList()
	{
		return mxServerMap;
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

		return NF_SHARE_PTR<ConnectData>(NULL);
	}

	
protected:

	void InitCallBacks(NF_SHARE_PTR<ConnectData> pServerData)
	{
		//add msg callback
		std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator itReciveCB = mxReceiveCallBack.begin();
		for (; mxReceiveCallBack.end() != itReciveCB; ++itReciveCB)
		{
			pServerData->mxNetModule->AddReceiveCallBack(itReciveCB->first, itReciveCB->second);
		}

		//add event callback
		std::list<NET_EVENT_FUNCTOR_PTR>::iterator itEventCB = mxEventCallBack.begin();
		for (; mxEventCallBack.end() != itEventCB; ++itEventCB)
		{
			pServerData->mxNetModule->AddEventCallBack(*itEventCB);
		}

		std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator itCB = mxCallBackList.begin();
		for (; mxCallBackList.end() != itCB; ++itCB)
		{
			pServerData->mxNetModule->AddReceiveCallBack(*itCB);
		}
	}

	void ProcessExecute()
	{
		NF_SHARE_PTR<ConnectData> pServerData = mxServerMap.First();
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
				}
			}
			break;
			case ConnectDataState::CONNECTING:
			{
				if (pServerData->mxNetModule)
				{
					pServerData->mxNetModule->Execute();
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
				if ((pServerData->mnLastActionTime + 10) >= GetPluginManager()->GetNowTime())
				{
					break;
				}

				if (nullptr != pServerData->mxNetModule)
				{
					pServerData->mxNetModule = nullptr;
				}

				pServerData->eState = ConnectDataState::CONNECTING;
				pServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule(pPluginManager));
				pServerData->mxNetModule->Initialization(pServerData->strIP.c_str(), pServerData->nPort);

				InitCallBacks(pServerData);
			}
			break;
			default:
				break;
			}

			pServerData = mxServerMap.Next();
		}
	}

	void KeepReport(NF_SHARE_PTR<ConnectData> pServerData){};
	void LogServerInfo(const std::string& strServerInfo){};

private:
	virtual void LogServerInfo()
	{
		LogServerInfo("This is a client, begin to print Server Info----------------------------------");

		ConnectData* pServerData = mxServerMap.FirstNude();
		while (nullptr != pServerData)
		{
			std::ostringstream stream;
			stream << "Type: " << pServerData->eServerType << " ProxyServer ID: " << pServerData->nGameID << " State: " <<  pServerData->eState << " IP: " << pServerData->strIP << " Port: " << pServerData->nPort;

			LogServerInfo(stream.str());

			pServerData = mxServerMap.NextNude();
		}

		LogServerInfo("This is a client, end to print Server Info----------------------------------");
	};

	void KeepState(NF_SHARE_PTR<ConnectData> pServerData)
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
	}

    int OnConnected(const int fd, NFINet* pNet)
    {
        NF_SHARE_PTR<ConnectData> pServerInfo = GetServerNetInfo(pNet);
        if (pServerInfo.get())
        {
			/////////////////////////////////////////////////////////////////////////////////////
            //AddServerWeightData(pServerInfo);
            pServerInfo->eState = ConnectDataState::NORMAL;
        }

        return 0;
    }

    int OnDisConnected(const int fd, NFINet* pNet)
    {
        NF_SHARE_PTR<ConnectData> pServerInfo = GetServerNetInfo(pNet);
        if (nullptr != pServerInfo)
        {
			/////////////////////////////////////////////////////////////////////////////////////
			//RemoveServerWeightData(pServerInfo);
            pServerInfo->eState = ConnectDataState::DISCONNECT;
            pServerInfo->mnLastActionTime = GetPluginManager()->GetNowTime();
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
				xServerData = NF_SHARE_PTR<ConnectData>(NF_NEW ConnectData());

				xServerData->nGameID = xInfo.nGameID;
				xServerData->eServerType = xInfo.eServerType;
				xServerData->strIP = xInfo.strIP;
				xServerData->strName = xInfo.strName;
				xServerData->eState = ConnectDataState::CONNECTING;
				xServerData->nPort = xInfo.nPort;
				xServerData->mnLastActionTime = GetPluginManager()->GetNowTime();

				xServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule(pPluginManager));
				xServerData->mxNetModule->Initialization(xServerData->strIP.c_str(), xServerData->nPort);

				InitCallBacks(xServerData);

				mxServerMap.AddElement(xInfo.nGameID, xServerData);
            }
        }

        mxTempNetList.clear();
    }

private:
	NFCConsistentHashMapEx<int, ConnectData> mxServerMap;
	//NFMapEx<int, ConnectData> mxServerMap;
	//NFCConsistentHash mxConsistentHash;

    std::list<ConnectData> mxTempNetList;

	//call back
	std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
	std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBack;
	std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
};
#endif
