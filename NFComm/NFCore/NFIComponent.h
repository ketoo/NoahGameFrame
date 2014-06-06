// -------------------------------------------------------------------------
//    @FileName         :    NFIComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponent
//
// -----------------------------------------------------------------------

#ifndef _NFI_COMPONENT_H_
#define _NFI_COMPONENT_H_

#include "NFIdentID.h"
#include "NFComm\NFPluginModule\NFILogicModule.h"

class NFIComponent : public NFILogicModule
{
public:
    virtual bool SetEnable(const bool bEnable) = 0;

    virtual bool Enable() = 0;

    virtual NFIDENTID Self() = 0;

    virtual const std::string& ComponentName() = 0;
    virtual const std::string& LanguageName() = 0;
private:
};

#endif
