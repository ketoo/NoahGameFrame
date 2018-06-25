// -------------------------------------------------------------------------
//    @FileName			:    NFLoginNet_HttpServerPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-05-01
//    @Module           :    NFLoginNet_HttpServerPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NF_LoginSERVERNET_HTTPSERVERPLUGIN_H
#define NF_LoginSERVERNET_HTTPSERVERPLUGIN_H
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFLoginNET_HTTPSERVERPLUGIN_EXPORTS
#define NFLoginNET_HTTPSERVERPLUGIN_API __declspec(dllexport)
#else
#define NFLoginNET_HTTPSERVERPLUGIN_API __declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
class NFLoginNet_HttpServerPlugin : public NFIPlugin
{
public:
	NFLoginNet_HttpServerPlugin(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual const int GetPluginVersion();

	virtual const std::string GetPluginName();

	virtual void Install();

	virtual void Uninstall();
};

#endif

