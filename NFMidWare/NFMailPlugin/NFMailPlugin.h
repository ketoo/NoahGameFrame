// -------------------------------------------------------------------------
//    @FileName			:    NFMailPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFMailPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_MAIL_PLUGIN_H
#define NF_MAIL_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFMailPlugin : public NFIPlugin
{
public:
	NFMailPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

	virtual const int GetPluginVersion() override;

	virtual const std::string GetPluginName() override;

	virtual void Install() override;

	virtual void Uninstall() override;
};
#endif
