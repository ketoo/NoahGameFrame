#ifndef NFC_GUILD_COMPONENT_H
#define NFC_GUILD_COMPONENT_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCGuildComponent : public NFIComponent
{

public:
	NFCGuildComponent(NFIPluginManager* pPluginManager) : NFIComponent(NFGUID(), "")
	{
	}

	NFCGuildComponent(NFGUID self, const std::string& strName) : NFIComponent(self, strName)
	{
	}

	virtual ~NFCGuildComponent()
	{

	}

	virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg)
	{
		std::cout << "Hello, welcome to actor thread: " << self.ToString() << " MsgID: " << event << " Data:" << arg << std::endl;

		return 0;
	}

protected:
	virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance()
	{
		return  NF_SHARE_PTR<NFIComponent>(NF_NEW  NFCHttpComponent(NFGUID(1, 2), ""));
	}
};

#endif