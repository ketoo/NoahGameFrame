// -------------------------------------------------------------------------
//    @FileName      :    NFUrlPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFUrlPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFUrl_LUGIN_H_
#define _NFUrl_LUGIN_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFUrlPlugin : public NFIPlugin
{
public:
    NFUrlPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif