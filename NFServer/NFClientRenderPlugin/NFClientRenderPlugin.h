// -------------------------------------------------------------------------
//    @FileName      :    NFClientRenderPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFClientRenderPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFClientRenderPlugin : public NFIPlugin
{
public:

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
