// -------------------------------------------------------------------------
//    @FileName         :    NFDataBaseNet_ClientPlugin.h
//    @Author           :    eliteYang
//    @Date             :    2015/04/25
//    @Module           :    NFDataBaseNet_ClientPlugin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFDataBaseNet_ClientPlugin : public NFIPlugin
{
public:
    NFDataBaseNet_ClientPlugin( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};