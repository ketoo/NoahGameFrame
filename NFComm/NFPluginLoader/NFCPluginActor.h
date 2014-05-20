// -------------------------------------------------------------------------
//    @FileName      :    NFCPluginActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginActor
//
// -------------------------------------------------------------------------

#ifndef _NFC_PLUGINACTOR_H_
#define _NFC_PLUGINACTOR_H_

#include <map>
#include <string>
#include "NFCPluginManager.h"
#include "NFComm/NFPluginModule/NFIActorManager.h"
#include "NFComm/NFPluginModule/NFIActor.h"

class NFIActor : public Theron::Actor
{
public:

	// Constructor, passes the framework to the baseclass.
	NFIActor(Theron::Framework &framework) : Theron::Actor(framework)
	{
		// Register the message handler.
		RegisterHandler(this, &NFIActor::Handler);
	}

	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from) = 0;
	virtual void Init() = 0;
	virtual void Excute() = 0;
	virtual void Final() = 0;

private:
	virtual void Handler(const NFIActorMessage& message, const Theron::Address from)
	{
		switch (message.eType)
		{
		case EACTOR_MESSAGE_ID::EACTOR_INIT:
			Init();
			break;

		case EACTOR_MESSAGE_ID::EACTOR_EXCUTE:
			Excute();
			break;

		case EACTOR_MESSAGE_ID::EACTOR_FINAL:
			Final();
			break;

		default:
			HandlerEx(message, from);
			break;
		}
	}

protected:
	NFIActorManager* m_pManager;

};

#endif