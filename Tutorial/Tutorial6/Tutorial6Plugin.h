// -------------------------------------------------------------------------
//    @FileName      :    NFTutorial6.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   NFTutorial6
//
// -------------------------------------------------------------------------

#ifndef NF_TUTORIAL6_H
#define NF_TUTORIAL6_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial6Plugin : public NFIPlugin
{
public:
    Tutorial6Plugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif