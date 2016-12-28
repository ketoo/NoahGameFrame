// -------------------------------------------------------------------------
//    @FileName			:    NFCScenePlugin.h
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    NFCScenePlugin
//
// -------------------------------------------------------------------------

#ifndef NFCScenePlugin_H
#define NFCScenePlugin_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


class NFCScenePlugin : public NFIPlugin
{
public:
    NFCScenePlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif