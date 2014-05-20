// -------------------------------------------------------------------------
//    @FileName         :    NFIComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponent
//
// -------------------------------------------------------------------------

#ifndef _NFI_COMPONENT_H_
#define _NFI_COMPONENT_H_

class NFIComponent
{
public:

    virtual bool Enable() = 0;
    virtual bool SetEnable(bool bEnable) = 0;


private:
};

#endif
