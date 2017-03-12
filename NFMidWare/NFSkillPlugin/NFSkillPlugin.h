// -------------------------------------------------------------------------
//    @FileName			:    NFSkillPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFSkillPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_SKILL_PLUGIN_H
#define NF_SKILL_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFSkillPlugin : public NFIPlugin
{
public:
	NFSkillPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif