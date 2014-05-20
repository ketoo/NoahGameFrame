// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROXYSERVERPLUGIN_H_
#define _NFC_PROXYSERVERPLUGIN_H_

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


class NFProxyPlugin : public NFIPlugin
{
public:
    NFProxyPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif