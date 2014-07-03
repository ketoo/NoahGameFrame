// -------------------------------------------------------------------------
//    @FileName      :    NFJavaScriptPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFJavaScriptPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFJAVA_SCRIPT_LUGIN_H_
#define _NFJAVA_SCRIPT_LUGIN_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFJavaScriptPlugin : public NFIPlugin
{
public:
    NFJavaScriptPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif