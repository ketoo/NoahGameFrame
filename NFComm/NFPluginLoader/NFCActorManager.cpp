// -------------------------------------------------------------------------
//    @FileName      :    NFCActorManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorManager
//
// -------------------------------------------------------------------------

#include "NFCActorManager.h"
#include "NFCPluginManager.h"

bool NFCActorManager::Init()
{
    framework = std::shared_ptr<Theron::Framework>(new Theron::Framework());

    //首个主线程
    mActorVec.push_back(new NFCPluginManager(*framework, this, EACTOR::EACTOR_MAIN));

	for (int i = EACTOR::EACTOR_LOGIC1; i < EACTOR::EACTOR_END; ++i)
	{
		mActorVec.push_back(new NFCPluginManager(*framework, this, (EACTOR)i));
	}

	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		Theron::Receiver receiver;

		NFIActorMessage message;
		message.eType = NFIActorMessage::EACTOR_INIT;
		framework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

		receiver.Wait();
	}

	return true;
}

bool NFCActorManager::AfterInit()
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		Theron::Receiver receiver;

		NFIActorMessage message;
		message.eType = NFIActorMessage::EACTOR_AFTER_INIT;
		framework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

		receiver.Wait();
	}
	return true;
}

bool NFCActorManager::BeforeShut()
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		Theron::Receiver receiver;

		NFIActorMessage message;
		message.eType = NFIActorMessage::EACTOR_BEFORE_SHUT;
		framework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

		receiver.Wait();
	}
	return true;
}

bool NFCActorManager::Shut()
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		Theron::Receiver receiver;

		NFIActorMessage message;
		message.eType = NFIActorMessage::EACTOR_SHUT;
		framework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

		receiver.Wait();
	}

	it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		delete *it;
	}
    framework.reset();

	return true;
}

bool NFCActorManager::Execute( const float fLasFrametime, const float fStartedTime )
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		NFIActorMessage message;
		message.eType = NFIActorMessage::EACTOR_EXCUTE;
		framework->Send(message, Theron::Address(), (*it)->GetAddress());
	}
	return true;
}

const Theron::Address NFCActorManager::GetAddress( NFIActorManager::EACTOR eActor )
{
	if (eActor >= NFIActorManager::EACTOR_LOGIC1
		&& eActor < NFIActorManager::EACTOR_END)
	{
		return mActorVec[eActor]->GetAddress();
// 		std::vector<NFIActor*>::iterator it = mActorVec.begin();
// 		std::advance(it, (int)eActor);
// 		return (*it)->GetAddress();
	}

	return Theron::Address();
}