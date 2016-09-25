#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFLogPlugin : public NFIPlugin
{
public:
    NFLogPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
