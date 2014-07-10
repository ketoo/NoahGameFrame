// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the NFEVENTTRIGGERPLUGIN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// NFEVENTTRIGGERPLUGIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFEVENTPROCESSPLUGIN_EXPORTS
#define NFEVENTPROCESSPLUGIN_API __declspec(dllexport)
#else
#define NFEVENTPROCESSPLUGIN_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
class NFEventProcessPlugin : public NFIPlugin
{
public:
    NFEventProcessPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
        bCanReload = false;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
