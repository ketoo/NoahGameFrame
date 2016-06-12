// -------------------------------------------------------------------------
//    @FileName         £º    NFIGSSwichServerModule.h
//    @Author           £º    Chuanbo.Guo
//    @Date             £º    2013-07-10
//    @Module           £º    NFIGSSwichServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_GSSWICHSERVER_MODULE_H
#define _NFI_GSSWICHSERVER_MODULE_H

#include "NFILogicModule.h"

class NFIGSSwichServerModule : public NFILogicModule
{
public:
	virtual bool ChangeServer(const NFGUID& self, const int nServer, const int nSceneID, const int nGroup) = 0;
};

#endif