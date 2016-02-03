// -------------------------------------------------------------------------
//    文件名      ：    NFIWarnModule.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2012-07-14 08:50
//    功能描述    ：    警戒对象管理模块
//
// -------------------------------------------------------------------------

#ifndef _NFI_WARN_MODULE_H_
#define _NFI_WARN_MODULE_H_

#include "NFComm/NFCore/NFIValueList.h"

class NFIWarnModule
{
public:

    //检查警戒对象，可以开火就开火
    virtual bool CheckWarnObject(const NFIDENTID& self) = 0;

    virtual void OnSpring(const NFIDENTID& self, const NFIDENTID& other) = 0;

    virtual void OnEndSpring(const NFIDENTID& self, const NFIDENTID& other) = 0;

    virtual void OnBeKilled(const NFIDENTID& self, const NFIDENTID& other) = 0;

    virtual bool OnCreateAIObject(const NFIDENTID& self) = 0;

    virtual bool OnDelAIObject(const NFIDENTID& self) = 0;

    virtual bool Empty(const NFIDENTID& self) = 0;

protected:
private:
};
#endif