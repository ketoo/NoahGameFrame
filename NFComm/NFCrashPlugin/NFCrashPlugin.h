
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFCRASHPLUGIN_EXPORTS
#define NFCRASHPLUGIN_API __declspec(dllexport)
#else
#define NFCRASHPLUGIN_API __declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
class NFCrashPlugin : public NFIPlugin
{
public:
    NFCrashPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
        bCanReload = false;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
