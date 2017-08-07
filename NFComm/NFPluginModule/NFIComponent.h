// -------------------------------------------------------------------------
//    @FileName         :    NFIComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponent
//
// -----------------------------------------------------------------------

#ifndef NFI_COMPONENT_H
#define NFI_COMPONENT_H

#include "NFPlatform.h"
#include "NFGUID.h"
#include "NFIActor.h"

class NFIComponent
{
private:
    NFIComponent()
    {
    }

public:
    NFIComponent(const std::string& strName)
    {
        mbEnable = true;
        mstrName = strName;
    }

    virtual ~NFIComponent() {}

	virtual void SetActor(NFIActor* self)
	{
		mSelf = self;
	}
	virtual NFIActor* GetActor()
	{
		return mSelf;
	}
	virtual bool Awake()
	{
		return true;
	}

	virtual bool Init()
	{

		return true;
	}

	virtual bool AfterInit()
	{
		return true;
	}

	virtual bool CheckConfig()
	{
		return true;
	}

	virtual bool ReadyExecute()
	{
		return true;
	}

	virtual bool Execute()
	{
		return true;
	}

	virtual bool BeforeShut()
	{
		return true;
	}

	virtual bool Shut()
	{
		return true;
	}

	virtual bool Finalize()
	{
		return true;
	}

    virtual bool SetEnable(const bool bEnable)
    {
        mbEnable = bEnable;

		OnEnable();

        return mbEnable;
    }

    virtual bool Enable()
    {
        return mbEnable;
    }

	virtual void OnEnable()
	{

	}

    virtual const std::string& GetComponentName() const
    {
        return mstrName;
    };

    //for actor
    virtual int OnASyncEvent(const NFGUID& self, const int from, const int event, std::string& arg)
    {
        return 0;
    }

	template<typename BaseType>
	bool AddMsgObserver(const int nSubMessage, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, std::string&))
	{
		ACTOR_PROCESS_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr(new ACTOR_PROCESS_FUNCTOR(functor));

		return mSelf->AddBeginunc(nSubMessage, functorPtr);
	}

	virtual bool SendMsgPB(const uint16_t nMsgID, const std::string& strData, const NFSOCK nSockIndex)
	{
		return true;

	}

	virtual bool SendMsgPB(const uint16_t nMsgID, const std::string& strData, const NFSOCK nSockIndex, const NFGUID nPlayer, const std::vector<NFGUID>* pClientIDList = NULL)
	{
		return true;
	}

	virtual bool SendMsgPBToAllClient(const uint16_t nMsgID, const std::string& strData)
	{
		return true;
	}

	template <typename T>
	NF_SHARE_PTR<T> FindComponent(const std::string& strName)
	{
		if (!TIsDerived<T, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return NF_SHARE_PTR<T>();
		}

		return mSelf->FindComponent(strName);
	}

private:
    bool mbEnable;
	NFIActor* mSelf;
    std::string mstrName;
};

#endif