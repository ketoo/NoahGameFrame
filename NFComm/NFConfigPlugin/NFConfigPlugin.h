// -------------------------------------------------------------------------
//    @FileName         :    NFConfigPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-03-15
//    @Module           :    NFConfigPlugin
//
// -------------------------------------------------------------------------
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFConfigPlugin : public NFIPlugin
{
public:
    NFConfigPlugin(NFIPluginManager* p);

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
