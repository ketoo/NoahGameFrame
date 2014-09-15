// -------------------------------------------------------------------------
//    @FileName      :    NFSLGPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFSLGPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFC_SLG_PLUGIN_H_
#define _NFC_SLG_PLUGIN_H_

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFSLGPlugin : public NFIPlugin
{
public:
    NFSLGPlugin( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif
