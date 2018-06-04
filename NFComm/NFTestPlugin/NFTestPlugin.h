// -------------------------------------------------------------------------
//    @FileName			:    NFTestPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-27
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_TEST_PLUGIN_H
#define NF_TEST_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFTestPlugin : public NFIPlugin
{
public:
	NFTestPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif //NF_TASK_PLUGIN_H