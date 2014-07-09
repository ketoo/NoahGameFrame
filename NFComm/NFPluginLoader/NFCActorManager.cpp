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
    m_pFramework = new Theron::Framework();

    //首个主线程
    //mActorVec.push_back(new NFCPluginManager(*m_pFramework, this, EACTOR::EACTOR_MAIN));

	for (int i = EACTOR::EACTOR_MAIN; i < EACTOR::EACTOR_END; ++i)
	{
		mActorVec.push_back(new NFCPluginManager(*m_pFramework, this, (EACTOR)i));
	}

	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		Theron::Receiver receiver;

		NFIActorMessage message;
		message.eType = NFIActorMessage::EACTOR_INIT;
		m_pFramework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

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
		m_pFramework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

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
		m_pFramework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

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
		m_pFramework->Send(message, receiver.GetAddress(), (*it)->GetAddress());

		receiver.Wait();
	}

	it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		delete *it;
	}

    delete m_pFramework;
    m_pFramework = NULL;

	return true;
}

bool NFCActorManager::Execute( const float fLasFrametime, const float fStartedTime )
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
        NFIActor* pActor = *it;
        int nMsgCount = pActor->GetNumQueuedMessages();
        if (nMsgCount <= 1)
        {
            NFIActorMessage message;
            message.eType = NFIActorMessage::EACTOR_EXCUTE;
            m_pFramework->Send(message, Theron::Address(), pActor->GetAddress());
        }
	}

	return true;
}

const Theron::Address NFCActorManager::GetAddress( NFIActorManager::EACTOR eActor )
{
	if (eActor >= NFIActorManager::EACTOR_MAIN
		&& eActor < NFIActorManager::EACTOR_END)
	{
		return mActorVec[eActor]->GetAddress();
	}

	return Theron::Address();
}