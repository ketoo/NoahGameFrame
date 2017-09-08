// -------------------------------------------------------------------------
//    @FileName			:    NFISwitchGameServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFISwitchGameServerModule
//
// -------------------------------------------------------------------------

#ifndef NFI_SWITCH_GAME_SERVER_MODULE_H
#define NFI_SWITCH_GAME_SERVER_MODULE_H

#include "NFIModule.h"

class NFISwitchGameServerModule
        : public NFIModule
{
public:
    virtual bool SwitchServer(const NFGUID &self, const int nServer, const int nSceneID, const int nGroup) = 0;
};

#endif