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
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

class NFIProxyServerNet_ClientModule
    : public NFILogicModule
{

public:
    //>0 gs_id, ==0 all gs, < 0 worldserver
    //     virtual int Transpond( int nGameServerID, NFIPacket* pMsgPacket ) = 0;
    //     virtual int Transpond( int nGameServerID, RakNet::BitStream* pBitStream ) = 0;

};

#endif