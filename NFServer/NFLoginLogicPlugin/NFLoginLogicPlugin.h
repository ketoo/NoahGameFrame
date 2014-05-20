// -------------------------------------------------------------------------
//    @FileName      :    NFLoginLogicPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    LoginLogicPluginModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_LOGINLOGICPLUGIN_MODULE_H_
#define _NFC_LOGINLOGICPLUGIN_MODULE_H_

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFLOGINLOGICPLUGIN_EXPORTS
#define NFLOGINLOGICPLUGIN_API __declspec(dllexport)
#else
#define NFLOGINLOGICPLUGIN_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
class NFLoginLogicPlugin : public NFIPlugin
{
public:
    NFLoginLogicPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif