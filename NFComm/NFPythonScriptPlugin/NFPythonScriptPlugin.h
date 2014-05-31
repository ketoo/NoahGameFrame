// -------------------------------------------------------------------------
//    @FileName      :    NFPythonScriptPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFPythonScriptPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFPYTHON_SCRIPT_LUGIN_H_
#define _NFPYTHON_SCRIPT_LUGIN_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFPythonScriptPlugin : public NFIPlugin
{
public:
    NFPythonScriptPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif