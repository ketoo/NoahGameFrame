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
	virtual void OnNetCreated(NF_SHARE_PTR<ServerData> xServerData) = 0;

	virtual bool Execute(const float fLastFrametime, const float fStartedTime)
	{
		ServerData* pServerData = mxServerMap.First();
		while (pServerData)
		{
			pServerData->mxNetModule->Execute(fLastFrametime, fStartedTime);

			pServerData = mxServerMap.Next();
		}

		return true;
	}


	void AddServer(const ServerData& xInfo)
	{
		NF_SHARE_PTR<ServerData> xServerData = mxServerMap.find(xInfo.nGameID);
		if (xServerData)
		{
			if (EServerState.EST_MAINTEN == xServerData->eState
				|| EServerState.EST_CRASH == xServerData->eState)
			{
				//宕了，进入重连队列
			}
			else
			{
				//正常，修改详细状态
			}
		}
		else
		{
			if (EServerState.EST_MAINTEN == xInfo->eState
				|| EServerState.EST_CRASH == xInfo->eState)
			{
				//宕了
			}
			else
			{
				//正常，添加新服务器
				xServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());

				xServerData->nGameID = xInfo.nGameID;
				xServerData->eServerType = xInfo->eState;
				xServerData->strIP = xInfo.strIP;
				xServerData->strName = xInfo.strName;
				xServerData->eState = xInfo.eState;
				xServerData->mxNetModule = NF_SHARE_PTR<NFINetModule>(NF_NEW NFINetModule());
				OnNetCreated(xServerData);
				//xServerData->m_pNetModule->Initialization(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCGameServerNet_ServerModule::OnRecivePack, &NFCGameServerNet_ServerModule::OnSocketEvent, nMaxConnect, nPort, nCpus);
					
				mxServerMap.AddElement(xInfo.nGameID, xServerData);
			}
		}
	}

	virtual void SendByServerID(const int nServerID, const std::string& strData)
	{

	}

	virtual void SendBySuit(const std::string& strData)
	{

	}

protected:
private:
	NFMapEx<int, ServerData> mxServerMap;
};