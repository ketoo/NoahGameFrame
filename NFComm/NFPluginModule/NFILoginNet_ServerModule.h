// -------------------------------------------------------------------------
//    @FileName         ��    NFILoginNet_ServerModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFILoginNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LOGINNET_SERVERMODULE_H_
#define _NFI_LOGINNET_SERVERMODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFILoginNet_ServerModule
    : public NFINetModule
{

public:
	virtual void SynWorldToClient() = 0;;

};

#endif