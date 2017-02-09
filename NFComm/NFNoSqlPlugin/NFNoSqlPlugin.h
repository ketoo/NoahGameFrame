//------------------------------------------------------------------------ -
//    @FileName			:    NFNoSqlPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFNoSqlPlugin
//
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFDATABASEPLUGIN_EXPORTS
#define NFDATABASEPLUGIN_API __declspec(dllexport)
#else
#define NFDATABASEPLUGIN_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
class NFNoSqlPlugin : public NFIPlugin
{
public:
    NFNoSqlPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};