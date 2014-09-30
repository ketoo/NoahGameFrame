
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFLOGPLUGIN_EXPORTS
#define NFLOGPLUGIN_API __declspec(dllexport)
#else
#define NFLOGPLUGIN_API __declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
class NFLogPlugin : public NFIPlugin
{
public:
    NFLogPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
        bCanReload = false;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
