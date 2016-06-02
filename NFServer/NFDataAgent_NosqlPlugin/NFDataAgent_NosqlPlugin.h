// -------------------------------------------------------------------------
//    @FileName			:    NFDataAgent_NosqlPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFDataAgent_NosqlPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFDataAgent_NosqlPlugin : public NFIPlugin
{
public:
	NFDataAgent_NosqlPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};