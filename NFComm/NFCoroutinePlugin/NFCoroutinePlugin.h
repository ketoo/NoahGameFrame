// -------------------------------------------------------------------------
//    @FileName         :    NFCoroutinePlugin.h
//    @Author           :    GaoYi
//    @Date             :    2017-01-08
//    @Module           :    NFCoroutinePlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_OROUTINE_PLUGIN
#define NFC_OROUTINE_PLUGIN

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFCoroutinePlugin : public NFIPlugin
{
public:
	NFCoroutinePlugin(NFIPluginManager* p);

	virtual const int GetPluginVersion();

	virtual const std::string GetPluginName();

	virtual void Install();

	virtual void Uninstall();
};

#endif