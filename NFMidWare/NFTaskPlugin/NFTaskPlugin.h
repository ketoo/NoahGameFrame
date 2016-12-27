// -------------------------------------------------------------------------
//    @FileName			:    NFTaskPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-27
//    @Module           :    NFTaskPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_TASK_PLUGIN_H
#define NF_TASK_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFTaskPlugin : public NFIPlugin
{
public:
	NFTaskPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif //NF_TASK_PLUGIN_H