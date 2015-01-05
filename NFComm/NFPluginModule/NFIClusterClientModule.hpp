// -------------------------------------------------------------------------
//    @FileName         £∫    NFIClusterClientModule.hpp
//    @Author           £∫    LvSheng.Huang
//    @Date             £∫    2015-01-4
//    @Module           £∫    NFIClusterClientModule
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
		mnFD = 0;
		strName = "";
		strIP = "";
		m_pNetModule = NULL;
		eServerType = NFST_NONE;
		eState = NFMsg::EServerState::EST_CRASH;
	}

	int mnFD;
	int nGameID;
	NF_SERVER_TYPE eServerType;
	std::string strIP;
	int nPort;
	std::string strName;
	NFMsg::EServerState eState;
	NFINetModule* m_pNetModule;

};

class NFIClusterClientModule
{
public:
	virtual bool Execute(const float fLastFrametime, const float fStartedTime)
	{
		ServerData* pServerData = mxServerMap.First();
		while (pServerData)
		{
			pServerData->m_pNetModule.Execute(fLastFrametime, fStartedTime);

			pServerData = mxServerMap.Next();
		}

		return true;
	}


	void AddServer(const ServerData& xInfo)
	{
		ServerData* pServerData = mxServerMap.find(xInfo.nGameID);
		if (pServerData)
		{
			if (EServerState.EST_MAINTEN == pServerData->eState
				|| EServerState.EST_CRASH == pServerData->eState)
			{
				//Â¥¡À
			}
			else
			{

			}
		}
		else
		{
			if (EServerState.EST_MAINTEN == pServerData->eState
				|| EServerState.EST_CRASH == pServerData->eState)
			{
				//Â¥¡À
			}
			else
			{

			}
		}
	}

	void RemovServer(const int nID)
	{
	}

	void AddFD(const int nFD, const int nID)
	{
	}

	void RemoveFD(const int nFD)
	{
	}

	virtual void SendByServerID(const int nServerID, const std::string& strData)
	{

	}

	virtual void SendByFD(const int nFD, const std::string& strData)
	{

	}

	virtual void SendBySuit(const std::string& strData)
	{

	}

protected:
private:
	NFMap<int, ServerData> mxServerMap;
	NFMap<int, int> mxFDMap;
};