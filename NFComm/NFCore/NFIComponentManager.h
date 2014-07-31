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
#include "NFComm\NFPluginModule\NFILogicModule.h"

class NFIComponentManager : public NFILogicModule, public NFMap<std::string, NFIComponent>
{
public:
    virtual ~NFIComponentManager() {}
    virtual NFIComponent* AddComponent(const std::string& strComponentName, const std::string& strLanguageName) = 0;
    virtual NFIComponent* FindComponent(const std::string& strComponentName) = 0;

    virtual bool SetEnable(const std::string& strComponentName, const bool bEnable) = 0;

    virtual bool Enable(const std::string& strComponentName) = 0;

    virtual NFIDENTID Self() = 0;

    //     template<typename BaseType>
    //     virtual bool SenbdMessage(const std::string& strComponentName, const bool bEnable);

private:
};

#endif