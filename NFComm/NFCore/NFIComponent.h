// -------------------------------------------------------------------------
//    @FileName         :    NFIComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponent
//
// -----------------------------------------------------------------------

#ifndef _NFI_COMPONENT_H_
#define _NFI_COMPONENT_H_

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIdentID.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIComponent : public NFILogicModule
{
private:
	NFIComponent()
	{
	}

public:
	NFIComponent(NFIDENTID self)
	{
		mbEnable = true;
		mSelf = self;
	}

    virtual ~NFIComponent() {}

	template <typename T>
	NF_SHARE_PTR<T> CreateNewInstance()
	{
		NFIComponent* pComponent = CreateNewInstance();
		if (pComponent)
		{
			if (TIsDerived<T, NFIComponent>::Result)
			{
				T* pT = dynamic_cast<T*>(pComponent);
				if (pT)
				{
					return NF_SHARE_PTR<pT>();
				}				
			}

			delete pComponent;
			pComponent = NULL;
		}

		return NF_SHARE_PTR<T>();
	}

	virtual NFIComponent* CreateNewInstance()
	{
		return NULL;
	}

    virtual bool SetEnable(const bool bEnable)
    {
        return mbEnable;
    }

	virtual bool Enable()
    {
        return mbEnable;
    }

	virtual NFIDENTID Self()
    {
        return NFIDENTID();
    }

    virtual const std::string& GetComponentName() const
    {
        return mstrName;
    }

	virtual void SetComponentName(const std::string& strName)
	{
		mstrName = strName;
	}

	virtual int OnASyncEvent(const NFIDENTID& self, const int event, std::string& arg) = 0;

private:
	bool mbEnable;
	NFIDENTID mSelf;
	std::string mstrName;
};

#endif
