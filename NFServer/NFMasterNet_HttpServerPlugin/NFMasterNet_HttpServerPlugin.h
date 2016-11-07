#ifndef NF_MASTERSERVERNET_HTTPSERVERPLUGIN_H
#define NF_MASTERSERVERNET_HTTPSERVERPLUGIN_H
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFMASTERNET_HTTPSERVERPLUGIN_EXPORTS
#define NFMASTERNET_HTTPSERVERPLUGIN_API __declspec(dllexport)
#else
#define NFMASTERNET_HTTPSERVERPLUGIN_API __declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
class NFMasterNet_HttpServerPlugin : public NFIPlugin
{
public:
	NFMasterNet_HttpServerPlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual const int GetPluginVersion();

	virtual const std::string GetPluginName();

	virtual void Install();

	virtual void Uninstall();
};

#endif

