// -------------------------------------------------------------------------
//    @FileName			:    NFAIPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFAIPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

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