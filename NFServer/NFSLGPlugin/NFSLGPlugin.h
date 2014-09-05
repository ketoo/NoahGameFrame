// -------------------------------------------------------------------------
//    @FileName      :    NFZoneLogicPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFZoneLogicPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVERPLUGIN_H_
#define _NFC_GAMESERVERPLUGIN_H_

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFGameServerPlugin : public NFIPlugin
{
public:
    NFGameServerPlugin( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif
