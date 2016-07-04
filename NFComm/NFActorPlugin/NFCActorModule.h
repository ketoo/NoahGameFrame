// -------------------------------------------------------------------------
//    @FileName			:    NFCActorModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_ACTOR_MODULE_H
#define NFC_ACTOR_MODULE_H

#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCActorModule
    : public NFIActorModule
{
public:

	NFCActorModule(NFIPluginManager* p);
	virtual ~NFCActorModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute();

	virtual bool SendMsgToActor(const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg);

protected:
	virtual int RequireActor();
	virtual int AddBeginFunc(const int nActorIndex, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb);
	virtual int AddEndFunc(const int nActorIndex, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb);

private:
	bool InitActorPool(const int nSize);

private:
	class ActorData : public Theron::Actor
	{
	public:

		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR begin;
		EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR end;

		explicit ActorData(Theron::Framework &framework) : Theron::Actor(framework)
		{
			RegisterHandler(this, &ActorData::Handler);
		}

	private:

		void Handler(const int &message, const Theron::Address from)
		{
			Send(message, from);
		}

	};

	Theron::Framework* m_pFramework;
	NFMapEx<int, ActorData > mxActorMap;
};

#endif
