// -------------------------------------------------------------------------
//    @FileName			:    NFIActorManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActorManager
//
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_MANAGER_H
#define NFI_ACTOR_MANAGER_H

#include "NFILogicModule.h"
#include "NFComm/NFCore/NFIComponent.h"

///////////////////////////////////////////////////
class NFIActor;

struct NFAsyncEventFunc
{
    NFAsyncEventFunc()
    {
        nActorID = -1;
    }

    //EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR xBeginFuncptr;
    EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR xEndFuncptr;
    int nActorID;
};

class NFCObjectAsyncEventInfo
    : public NFMapEx<int, NFAsyncEventFunc>
{
};

class NFIActorManager : public NFILogicModule
{
public:
    template<typename BaseTypeComponent, typename BaseType>
    int RequireActor(BaseType* pBase, int (BaseType::*handler_end)(const NFGUID&, const int, const int, const std::string&))
    {
        if (!TIsDerived<BaseTypeComponent, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return 0;
        }

        int nActorID = RequireActor();
        if (nActorID >= 0)
        {
            AddComponent<BaseTypeComponent>(nActorID);
            AddEndFunc<BaseType>(nActorID, pBase, handler_end);

            return nActorID;
        }

        return -1;
    }

    virtual NF_SHARE_PTR<NFIActor> GetActor(const int nActorIndex) = 0;
    virtual bool ReleaseActor(const int nActorIndex) = 0;
    virtual void ReloadPlugin() = 0;

    virtual bool SendMsgToActor(const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg) = 0;

protected:
    template<typename BaseTypeComponent, typename BaseType>
    int AddEndFunc(const int nActorIndex, BaseType* pBase, int (BaseType::*handler_end)(const NFGUID&, const int, const int, const std::string&))
    {
        EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));

        return AddEndFunc(nActorIndex, functorPtr_end);
    }

    template<typename BaseTypeComponent>
    bool AddComponent(const int nActorIndex)
    {
        if (!TIsDerived<BaseTypeComponent, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return false;
        }

        NF_SHARE_PTR<BaseTypeComponent> pComponent = NF_SHARE_PTR<BaseTypeComponent>(NF_NEW BaseTypeComponent(pPluginManager));
        return AddComponent(nActorIndex, pComponent);
    }

protected:
    virtual int RequireActor() = 0;

    virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent) = 0;
    virtual bool AddEndFunc(const int nActorIndex, EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end) = 0;

};

#endif
