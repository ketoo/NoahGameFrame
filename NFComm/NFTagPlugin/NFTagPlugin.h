// -------------------------------------------------------------------------
//    @FileName      :    NFTagPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFTagPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFTAG_LUGIN_H_
#define _NFTAG_LUGIN_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFTagPlugin : public NFIPlugin
{
public:
    NFTagPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif