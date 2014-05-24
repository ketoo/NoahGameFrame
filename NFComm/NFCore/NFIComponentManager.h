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

class NFIComponentManager : public NFMap<std::string, NFIComponent>
{
public:
    virtual NFIComponent* AddComponent(const std::string& strComponentName, const std::string& strScriptName) = 0;
    virtual NFIComponent* FindComponent(const std::string& strComponentName) = 0;
    virtual bool DestroyComponent(const std::string& strComponentName) = 0;

    virtual bool SetEnable(const std::string& strComponentName, const bool bEnable) = 0;

    virtual bool Enable(const std::string& strComponentName) = 0;

    virtual NFIDENTID Self() = 0;
private:
};

#endif