// -------------------------------------------------------------------------
//    @FileName			:    NFCScalePlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-10-23 12:22
//    @Module           :    NFCScalePlugin
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NF_SCALE_PLUGIN_H
#define NF_SCALE_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFScalePlugin : public NFIPlugin
{
public:
	NFScalePlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif