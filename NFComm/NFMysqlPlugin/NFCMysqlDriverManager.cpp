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

#include "NFCMysqlDriver.h"
#include "NFCMysqlDriverManager.h"
#include "NFComm/NFCore/NFDataList.hpp"

NFCMysqlDriverManager::~NFCMysqlDriverManager()
{
	for (NFIMysqlDriver* pMysqlDriver = mvMysql.First(); nullptr != pMysqlDriver; pMysqlDriver = mvMysql.Next())
	{
		delete pMysqlDriver;
		pMysqlDriver = nullptr;
	}
	mvMysql.ClearAll();
	for (NFIMysqlDriver* pMysqlDriver = mvInvalidMsyql.First(); nullptr != pMysqlDriver; pMysqlDriver = mvInvalidMsyql.Next())
	{
		delete pMysqlDriver;
		pMysqlDriver = nullptr;
	}
	mvInvalidMsyql.ClearAll();
}

NFIMysqlDriver* NFCMysqlDriverManager::GetMysqlDriver()
{
    return mvMysql.First(); // ÔÝÊ±ÏÈ¸øfirst
}

void NFCMysqlDriverManager::CheckMysql()
{
    //////////////////////////////////////////////////////////////////////////
    int nServerID = 0;
    std::vector<int> xIntVec;
    for (NFIMysqlDriver* pMysqlDriver = mvMysql.First(nServerID); pMysqlDriver != NULL; pMysqlDriver = mvMysql.Next(nServerID))
    {
        if (!pMysqlDriver->Enable())
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
        if (!pMysqlDriver->Enable() && pMysqlDriver->CanReconnect())
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

}
bool NFCMysqlDriverManager::AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/)
{
    std::string  strDnsIp;
    if (!strDns.empty())
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
    if (pMysqlDriver->Connect(strDBName, strDnsIp, nPort, strDBUser, strDBPwd))
    {
        mvMysql.AddElement(nServerID, pMysqlDriver);
    }
    else
    {
        mvInvalidMsyql.AddElement(nServerID, pMysqlDriver);
    }

    return true;
}

std::string NFCMysqlDriverManager::GetIPByHostName(const std::string& strHostName)
{
    if (strHostName.empty())
    {
        return NULL_STR;
    }

    hostent* pHost = gethostbyname(strHostName.c_str());
    if (pHost == NULL)
    {
        return NULL_STR;
    }

    if (pHost->h_addr_list[0] != NULL)
    {
        char strIp[32] = {0};
        inet_ntop(pHost->h_addrtype, pHost->h_addr_list[0], strIp, sizeof(strIp));
        return std::string(strIp);
    }

    return NULL_STR;
}
