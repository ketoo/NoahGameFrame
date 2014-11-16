// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the NFAIPLUGIN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// NFAIPLUGIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

// #pragma warning(disable : 4482)
// #pragma warning(disable : 4305)
// #pragma warning(disable : 4244)
// #pragma warning(disable : 4018)
// #pragma warning(disable : 4009)
// #pragma warning(disable : 4099)
// #pragma warning(disable : 4251)
// #pragma warning(disable : 4819)
// #pragma warning(disable : 4996)


#ifdef NFAIPLUGIN_EXPORTS
#define NFAIPLUGIN_API __declspec(dllexport)
#else
#define NFAIPLUGIN_API __declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
class NFAIPlugin : public NFIPlugin
{
public:
    NFAIPlugin(NFIPluginManager* pm)
    {
        pPluginManager = pm;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};