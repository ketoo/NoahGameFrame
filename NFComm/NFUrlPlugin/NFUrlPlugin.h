// -------------------------------------------------------------------------
//    @FileName			:    NFUrlPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFUrlPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_URL_LUGIN_H
#define NF_URL_LUGIN_H

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