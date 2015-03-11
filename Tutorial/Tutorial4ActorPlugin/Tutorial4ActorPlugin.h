// -------------------------------------------------------------------------
//    @FileName      :    NFTutorial4Actor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   NFTutorial4Actor
//
// -------------------------------------------------------------------------

#ifndef _NF_Tutorial4Actor_H_
#define _NF_Tutorial4Actor_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial4ActorPlugin : public NFIPlugin
{
public:
    Tutorial4ActorPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif