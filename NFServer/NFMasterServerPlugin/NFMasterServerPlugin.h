// -------------------------------------------------------------------------
//    @FileName      :    NFCentreLogicPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    CentreLogicModule
//
// -------------------------------------------------------------------------

#ifndef _NF_MASTERSERVERPLUGIN_H_
#define _NF_MASTERSERVERPLUGIN_H_
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFMASTERSERVERPLUGIN_EXPORTS
#define NFMASTERSERVERPLUGIN_API __declspec(dllexport)
#else
#define NFMASTERSERVERPLUGIN_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
class NFMasterServerPlugin : public NFIPlugin
{
public:
    NFMasterServerPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif