// -------------------------------------------------------------------------
//    @FileName			:    NFIActorManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActorManager
//
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_MANAGER_H
#define NFI_ACTOR_MANAGER_H

#include "NFIModule.h"
#include "NFComm/NFCore/NFIComponent.h"

///////////////////////////////////////////////////

typedef std::function<int(const NFGUID&, const int, std::string&)> EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR;
typedef std::function<int(const NFGUID&, const int, const int, const std::string&)> EVENT_ASYNC_PROCESS_END_FUNCTOR;


typedef NF_SHARE_PTR<EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR> EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR;
typedef NF_SHARE_PTR<EVENT_ASYNC_PROCESS_END_FUNCTOR> EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR;

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
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR xEndFuncptr;
protected:
private:
};

struct NFAsyncEventFunc
{
    NFAsyncEventFunc()
    {
        nActorID = -1;
    }

    EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR xEndFuncptr;
    int nActorID;
};

class NFCObjectAsyncEventInfo
    : public NFMapEx<int, NFAsyncEventFunc>
{
};

class NFIActorModule : public NFIModule
{
public:
    template<typename TypeComponent, typename BaseType>
    int RequireActor(BaseType* pBase, int (BaseType::*handler_end)(const NFGUID&, const int, const int, const std::string&))
    {
        if (!TIsDerived<TypeComponent, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return 0;
        }

        int nActorID = RequireActor();
        if (nActorID >= 0)
        {
            AddComponent<TypeComponent>(nActorID);
            AddEndFunc<BaseType>(nActorID, pBase, handler_end);

            return nActorID;
        }

        return -1;
    }

    virtual bool SendMsgToActor(const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg) = 0;
	virtual bool HandlerEx(const NFIActorMessage& message, const int from) = 0;
	virtual bool ReleaseActor(const int nActorIndex) = 0;

protected:
    virtual int RequireActor() = 0;

    virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent) = 0;
    virtual bool AddEndFunc(const int nActorIndex, EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end) = 0;

private:
	template<typename TypeComponent, typename BaseType>
	int AddEndFunc(const int nActorIndex, BaseType* pBase, int (BaseType::*handler_end)(const NFGUID&, const int, const int, const std::string&))
	{
		EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));

		return AddEndFunc(nActorIndex, functorPtr_end);
	}

	template<typename TypeComponent>
	bool AddComponent(const int nActorIndex)
	{
		if (!TIsDerived<TypeComponent, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return false;
		}

		//use CreateNewInstance to replace this line to create a new component script
		NF_SHARE_PTR<TypeComponent> pComponent = NF_SHARE_PTR<TypeComponent>(NF_NEW TypeComponent(pPluginManager));
		return AddComponent(nActorIndex, pComponent);
	}

};

#endif
