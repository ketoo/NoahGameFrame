// -------------------------------------------------------------------------
//    @FileName			:    NFCLogicPlugin.h
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    NFCLogicPlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGIC_PLUGIN_H
#define NFC_LOGIC_PLUGIN_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


class NFCLogicPlugin : public NFIPlugin
{
public:
    NFCLogicPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif