// -------------------------------------------------------------------------
//    @FileName      :    NFTutorial3.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   NFTutorial3
//
// -------------------------------------------------------------------------

#ifndef _NF_TUTORIAL3_H_
#define _NF_TUTORIAL3_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial3Plugin : public NFIPlugin
{
public:
    Tutorial3Plugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif