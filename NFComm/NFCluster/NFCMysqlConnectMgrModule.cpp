#include "NFCMysqlConnectMgrModule.h"
#include "NFComm/NFCluster/NFCMysqlDriver.h"

#ifdef _MSC_VER
#include <WinSock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#endif

NFCMysqlConnectMgrModule::NFCMysqlConnectMgrModule( NFIPluginManager* p )
{
    pPluginManager = p;
	mfLastCheckTime = 0.0f;
    bCanReload = false;
}

NFCMysqlConnectMgrModule::~NFCMysqlConnectMgrModule()
{

}

bool NFCMysqlConnectMgrModule::Init()
{
    return true;
}

bool NFCMysqlConnectMgrModule::Shut()
{
	for (NFIMysqlDriver* pMysqlDriver = mvMysql.First(); pMysqlDriver != NULL; pMysqlDriver = mvMysql.Next())
	{
		if (pMysqlDriver)
		{
			delete pMysqlDriver;
			pMysqlDriver = NULL;
		}
	}

	for (NFIMysqlDriver* pMysqlDriver = mvInvalidMsyql.First(); pMysqlDriver != NULL; pMysqlDriver = mvInvalidMsyql.Next())
	{
		if (pMysqlDriver)
		{
			delete pMysqlDriver;
			pMysqlDriver = NULL;
		}
	}

    return true;
}

bool NFCMysqlConnectMgrModule::BeforeShut()
{
    return true;
}

bool NFCMysqlConnectMgrModule::AfterInit()
{  
    return true;
}

bool NFCMysqlConnectMgrModule::Execute( const float fLasFrametime, const float fStartedTime )
{
	CheckMysql(fLasFrametime, fStartedTime);

    return true;
}

NFIMysqlDriver* NFCMysqlConnectMgrModule::GetMysqlDriver()
{
	return mvMysql.First(); // ÔÝÊ±ÏÈ¸øfirst
}

void NFCMysqlConnectMgrModule::CheckMysql(float fLastFrameTime, const float fStartedTime)
{
	if (mfLastCheckTime < 5.0f)
	{
		mfLastCheckTime += fLastFrameTime;
		return;
	}


	//////////////////////////////////////////////////////////////////////////
	int nServerID = 0;
	std::vector<int> xIntVec;
	for (NFIMysqlDriver* pMysqlDriver = mvMysql.First(nServerID); pMysqlDriver != NULL; pMysqlDriver = mvMysql.Next(nServerID))
	{ 
		//if (!pMysqlDriver->Enable())
		{
			xIntVec.push_back(nServerID);
			mvInvalidMsyql.AddElement(nServerID, pMysqlDriver);
		}
	}

	for (int i = 0; i < xIntVec.size(); ++i)
	{
		mvMysql.RemoveElement(xIntVec[i]);
	}
	//////////////////////////////////////////////////////////////////////////
	xIntVec.clear();
	nServerID = 0;

	for (NFIMysqlDriver* pMysqlDriver = mvInvalidMsyql.First(nServerID); pMysqlDriver != NULL; pMysqlDriver = mvInvalidMsyql.Next(nServerID))
	{
		if (!pMysqlDriver->Enable() && pMysqlDriver->CanReconnect(mfLastCheckTime))
		{
			pMysqlDriver->Reconnect();
			if (pMysqlDriver->Enable())
			{
				xIntVec.push_back(nServerID);
				mvMysql.AddElement(nServerID, pMysqlDriver);
			}
		}
	}

	for (int i = 0; i < xIntVec.size(); ++i)
	{
		mvInvalidMsyql.RemoveElement(xIntVec[i]);
	}

    mfLastCheckTime = 0.0f;
}
bool NFCMysqlConnectMgrModule::AddMysqlServer( const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd)
{
	std::string  strDnsIp;
	if(!strDns.empty())
	{
		strDnsIp = GetIPByHostName(strDns);   
	}

	if (strDnsIp.empty())
	{
		strDnsIp = strIP;
	}

	if (strDnsIp.empty())
	{
		return false;
	}

    NFIMysqlDriver* pMysqlDriver = mvMysql.GetElement(nServerID);
    if (pMysqlDriver)
    {
		return false;
    }

	NFIMysqlDriver* pInvalidRedisDriver = mvInvalidMsyql.GetElement(nServerID);
	if (pInvalidRedisDriver)
	{
		return false;
	}


    pMysqlDriver = NF_NEW NFCMysqlDriver(nRconnectTime, nRconneCount);
    pMysqlDriver->Connect(strDBName, strDnsIp, nPort, strDBUser, strDBPwd);

    mvMysql.AddElement(nServerID, pMysqlDriver);

    return true;
}

std::string NFCMysqlConnectMgrModule::GetIPByHostName( const std::string& strHostName )
{
    if (strHostName.empty())
    {
        return NULL_STR;
    }

    hostent* pHost = gethostbyname(strHostName.c_str());
    if(pHost == NULL)
    {
        return NULL_STR;
    }

   if(pHost->h_addr_list[0] != NULL)
    {
        char strIp[32] = {0};
        inet_ntop(pHost->h_addrtype,pHost->h_addr_list[0], strIp, sizeof(strIp));
        return std::string(strIp);
    }

    return NULL_STR;
}
