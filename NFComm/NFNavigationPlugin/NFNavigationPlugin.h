// -------------------------------------------------------------------------
//    @FileName         :    NFNavigationPlugin.h
//    @Author           :    Bluesky
//    @Date             :    2016-06-22
//    @Module           :    NFNavigationPlugin
//
// -------------------------------------------------------------------------
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFNavigationPlugin : public NFIPlugin
{
public:
	NFNavigationPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
