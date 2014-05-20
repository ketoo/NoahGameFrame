// -------------------------------------------------------------------------
//    @FileName         ��    NFIProxyServerNet_ServerModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIProxyServerNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROXYNET_SERVERMODULE_H_
#define _NFI_PROXYNET_SERVERMODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIProxyServerNet_ServerModule
    : public NFILogicModule
{

public:
    virtual int GetServerPort() = 0;
    //objectList:need to broadcast
    //valueMessage:message infomation
    //virtual int Transpond( int nMsgID, int nSubMsgID, const NFIValueList& objectList, const NFIValueList& valueMessage ) = 0;
};

#endif