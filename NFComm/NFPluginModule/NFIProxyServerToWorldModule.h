// -------------------------------------------------------------------------
//    @FileName         £º    NFIProxyServerNet_ClientModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIProxyServerNet_ClientModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROXYNET_CLIENTMODULE_H_
#define _NFI_PROXYNET_CLIENTMODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIProxyServerToWorldModule
    : public  NFINetModule, public NFMap<int64_t, NFINetModule>
{
public:

    struct GameData 
    {
        GameData()
        {
            nGameID = 0;
            strIP = "";
            nPort = 0;
            strName = "";
            eState = NFMsg::EServerState::EST_CRASH;
        }

        int nGameID;
        std::string strIP;
        int nPort;
        std::string strName;
        NFMsg::EServerState eState;
    };

    
    typedef NFMap<int, GameData> GameDataMap;

public:
    virtual int Transpond(int nGameServerID, const NFIPacket& msg) = 0;

    virtual bool VerifyConnectData(const std::string& strAccount, const std::string& strKey) = 0;

    virtual GameData* GetGameData(int nGameID) = 0;
    virtual GameDataMap& GetGameDataMap() = 0;
};

#endif