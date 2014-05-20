// -------------------------------------------------------------------------
//    文件名      ：    NFCWarnModule.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2012-07-14 08:50
//    功能描述    ：    警戒对象管理模块
//
// -------------------------------------------------------------------------

#ifndef _NFC_WARN_MODULE_H_
#define _NFC_WARN_MODULE_H_

#include <string>
#include <list>
#include <map>
#include <iostream>
#include "NFIWarnModule.h"

class NFCWarnModule
    : public NFIWarnModule
{
public:
    NFCWarnModule();
    virtual ~NFCWarnModule();

    //检查警戒对象，可以开火就开火
    virtual bool CheckWarnObject(const NFIDENTID& self);

    virtual void OnSpring(const NFIDENTID& self, const NFIDENTID& other);

    virtual void OnEndSpring(const NFIDENTID& self, const NFIDENTID& other);

    virtual void OnBeKilled(const NFIDENTID& self, const NFIDENTID& other);

    virtual bool OnCreateAIObject(const NFIDENTID& self);

    virtual bool OnDelAIObject(const NFIDENTID& self);

    virtual bool Empty(const NFIDENTID& self);

protected:

    bool Exist(const NFIDENTID& self);
    bool AddWarn(const NFIDENTID& self, const NFIDENTID& other);
    bool ClearWarn(const NFIDENTID& self, const NFIDENTID& other);
    bool ClearWarnAll(const NFIDENTID& self);
    bool ClearWarnAll();


private:

    //     任何逻辑类中不能使用map list等之类的信息记录，任何信息记录必须至表和属性中
    //     typedef std::list<NFIDENTID> TWARNLIST;
    //     typedef std::map<NFIDENTID, TWARNLIST*> TOBJECTWARN;
    //     TOBJECTWARN mtObjectWarn;
};

#endif