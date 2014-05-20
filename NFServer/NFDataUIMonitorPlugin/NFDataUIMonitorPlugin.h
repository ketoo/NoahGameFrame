
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef  NFDATAUIMONITOR_EXPORTS
#define NFDATAUIMONITOR_API __declspec(dllexport)
#else
#define NFDATAUIMONITOR_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
class NFDataUIMonitorPlugin : public NFIPlugin
{
public:

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
