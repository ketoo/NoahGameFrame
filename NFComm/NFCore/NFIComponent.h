// -------------------------------------------------------------------------
//    @FileName         :    NFIComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponent
//
// -----------------------------------------------------------------------

#ifndef _NFI_COMPONENT_H_
#define _NFI_COMPONENT_H_

#include "NFComm/NFPluginModule/NFIdentID.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIComponent : public NFILogicModule
{
public:
    virtual ~NFIComponent() {}
    virtual bool SetEnable(const bool bEnable)
    {
        return false;
    }

	virtual bool Enable()
    {
        return false;
    }

	virtual NFIDENTID Self()
    {
        return NFIDENTID();
    }

    virtual const std::string& ComponentName()
    {
        return NULL_STR;
    }
    virtual const std::string& LanguageName()
    {
        return NULL_STR;
    }
private:
};

#endif
