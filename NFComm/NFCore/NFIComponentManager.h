// -------------------------------------------------------------------------
//    @FileName         :    NFIComponentManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponentManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_COMPONENT_MANAGER_H_
#define _NFI_COMPONENT_MANAGER_H_

#include "NFMap.h"
#include "NFIComponent.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

///////////////////////////////////////////////////



class NFIComponentManager : public NFILogicModule, public NFMapEx<std::string, NFIComponent>
{
public:
    virtual ~NFIComponentManager() {}

	template <typename T>
	NF_SHARE_PTR<T> AddComponent()
	{
		NF_SHARE_PTR<T> pComponent = NF_SHARE_PTR<T>(NF_NEW T());
		std::string strComponentName = GET_CLASS_NAME(T)

		return AddComponent(strComponentName, pComponent);
	}

	template <typename T>
	NF_SHARE_PTR<T> FindComponent()
	{
		return NF_SHARE_PTR<T>();
	}

    virtual NF_SHARE_PTR<NFIComponent> AddComponent(const std::string& strComponentName, NF_SHARE_PTR<NFIComponent> pNewComponent) = 0;

    virtual bool SetEnable(const std::string& strComponentName, const bool bEnable) = 0;

    virtual bool Enable(const std::string& strComponentName) = 0;

    virtual NFIDENTID Self() = 0;

protected:

	virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName) = 0;

private:
};

#endif
