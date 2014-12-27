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
#ifdef NF_USE_ACTOR
		mActorVec.push_back(new NFCPluginManager(*m_pFramework, this, (EACTOR)i));
#else
		mActorVec.push_back(new NFCPluginManager(this));
#endif
	}

	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		NFIActor* pActor = *it;

#ifdef NF_USE_ACTOR
		Theron::Receiver receiver;
		NFIActorMessage xMessage;
		xMessage.eType = NFIActorMessage::EACTOR_INIT;
		m_pFramework->Send(xMessage, receiver.GetAddress(), pActor->GetAddress());

		receiver.Wait();
#else
		pActor->Init();
#endif

	}

	return true;
}

bool NFCActorManager::AfterInit()
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		NFIActor* pActor = *it;

#ifdef NF_USE_ACTOR

		Theron::Receiver xReceiver;
		NFIActorMessage xMessage;
		xMessage.eType = NFIActorMessage::EACTOR_AFTER_INIT;
		m_pFramework->Send(xMessage, xReceiver.GetAddress(), pActor->GetAddress());

		xReceiver.Wait();
#else
		pActor->AfterInit();
#endif
	}
	return true;
}

bool NFCActorManager::CheckConfig()
{
    std::vector<NFIActor*>::iterator it = mActorVec.begin();
    for (; it != mActorVec.end(); ++it)
    {
		NFIActor* pActor = *it;

#ifdef NF_USE_ACTOR

        Theron::Receiver xReceiver;
        NFIActorMessage xMessage;
        xMessage.eType = NFIActorMessage::EACTOR_CHECKCONFIG;
        m_pFramework->Send(xMessage, xReceiver.GetAddress(), (*it)->GetAddress());

        xReceiver.Wait();
#else
		pActor->CheckConfig();
#endif
    }
    return true;
}

bool NFCActorManager::BeforeShut()
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		NFIActor* pActor = *it;

#ifdef NF_USE_ACTOR

		Theron::Receiver xReceiver;
		NFIActorMessage xMessage;
		xMessage.eType = NFIActorMessage::EACTOR_BEFORE_SHUT;
		m_pFramework->Send(xMessage, xReceiver.GetAddress(), (*it)->GetAddress());

		xReceiver.Wait();
#else
		pActor->BeforeShut();
#endif
	}
	return true;
}

bool NFCActorManager::Shut()
{
	std::vector<NFIActor*>::iterator it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		NFIActor* pActor = *it;

#ifdef NF_USE_ACTOR

		Theron::Receiver xReceiver;
		NFIActorMessage xMessage;
		xMessage.eType = NFIActorMessage::EACTOR_SHUT;
		m_pFramework->Send(xMessage, xReceiver.GetAddress(), (*it)->GetAddress());

		xReceiver.Wait();
#else
		pActor->Shut();
#endif
	}

	it = mActorVec.begin();
	for (; it != mActorVec.end(); ++it)
	{
		delete *it;
	}

	mActorVec.clear();

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

#ifdef NF_USE_ACTOR

        const int nMsgCount = pActor->GetNumQueuedMessages();
        if (nMsgCount <= 10)
        {
            NFIActorMessage xMessage;
            xMessage.eType = NFIActorMessage::EACTOR_EXCUTE;

			//if only one actor, then use synchronous mode, other use  asynchronous mode.
			//at the same,it's use synchronous mode in the same actor.
			if (NFIActorManager::EACTOR_END - NFIActorManager::EACTOR_MAIN == 1)
			{
// 				Theron::Receiver xReceiver;
// 				m_pFramework->Send(xMessage, xReceiver.GetAddress(), pActor->GetAddress());
// 				xReceiver.Wait();
				m_pFramework->Send(xMessage, Theron::Address(), pActor->GetAddress());
			}
			else if (NFIActorManager::EACTOR_END - NFIActorManager::EACTOR_MAIN > 1)
			{
				m_pFramework->Send(xMessage, Theron::Address(), pActor->GetAddress());
			}
        }
#else
		pActor->Execute(fLasFrametime, fStartedTime);
#endif
	}

	return true;
}

#ifdef NF_USE_ACTOR
const Theron::Address NFCActorManager::GetAddress( NFIActorManager::EACTOR eActor )
{
	if (eActor >= NFIActorManager::EACTOR_MAIN
		&& eActor < NFIActorManager::EACTOR_END)
	{
		return mActorVec[eActor]->GetAddress();
	}

	return Theron::Address();
}
#else
#endif
