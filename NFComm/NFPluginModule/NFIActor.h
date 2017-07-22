// -------------------------------------------------------------------------
//    @FileName			:    NFIActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActor
//
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_H
#define NFI_ACTOR_H

#define THERON_USE_STD_THREADS 1

#include <map>
#include <string>
#include <Theron/Theron.h>
#include "NFIModule.h"

class NFIComponent;
class NFIActorModule;

typedef std::function<int(const NFGUID&, const int, const int, std::string&)> ACTOR_PROCESS_FUNCTOR;
typedef NF_SHARE_PTR<ACTOR_PROCESS_FUNCTOR> ACTOR_PROCESS_FUNCTOR_PTR;


class NFIActorMessage
{
public:
	NFIActorMessage()
	{
		nMsgID = 0;
		nFormActor = 0;
		bComponentMsg = true;
	}

	int nMsgID;
	int nFormActor;
	std::string data;
	bool bComponentMsg;
	////////////////////event/////////////////////////////////////////////////
	NFGUID self;
	//////////////////////////////////////////////////////////////////////////
	ACTOR_PROCESS_FUNCTOR_PTR xEndFuncptr;
protected:
private:
};

struct NFAsyncEventFunc
{
	NFAsyncEventFunc()
	{
		nActorID = -1;
	}

	ACTOR_PROCESS_FUNCTOR_PTR xEndFuncptr;
	int nActorID;
};

class NFCObjectAsyncEventInfo
	: public NFMapEx<int, NFAsyncEventFunc>
{
};

class NFIActor : public Theron::Actor
{
public:
    NFIActor(Theron::Framework& framework) : Theron::Actor(framework)
    {
        RegisterHandler(this, &NFIActor::DefaultHandler);
    }

	virtual ~NFIActor() {}

	virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	
	virtual bool AddBeginunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor) = 0;
	virtual bool AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xEndFunctor) = 0;
    
	virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message) = 0;

protected:

    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from) {};
    virtual void Handler(const NFIActorMessage& message, const Theron::Address from) {};

private:
    void DefaultHandler(const NFIActorMessage& message, const Theron::Address from)
    {
        if (message.bComponentMsg > 0 )
        {
			Handler(message, from);
        }
        else
        {
            HandlerEx(message, from);
        }
    }

protected:
	NFIActorModule* m_pActorModule;

};

#endif // !NFI_ACTOR_H