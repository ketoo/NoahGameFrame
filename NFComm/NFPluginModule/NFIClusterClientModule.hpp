// -------------------------------------------------------------------------
//    @FileName         £º    NFIClusterClientModule.hpp
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2015-01-4
//    @Module           £º    NFIClusterClientModule
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
		strIP = "";
		nPort = 0;
		strName = "";
		eServerType = NFST_NONE;
		eState = NFMsg::EServerState::EST_CRASH;
	}

	int nGameID;
	NF_SERVER_TYPE eServerType;
	std::string strIP;
	int nPort;
	std::string strName;
	NFMsg::EServerState eState;
};

class NFIClusterClientModule
{
public:
protected:
private:
};