// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-07-21
//    @Module           :    NFCGuildComponent
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_COMPONENT_H
#define NFC_GUILD_COMPONENT_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCGuildComponent : public NFIComponent
{
public:
	NFCGuildComponent() : NFIComponent(GET_CLASS_NAME(NFCGuildComponent))
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

};

#endif