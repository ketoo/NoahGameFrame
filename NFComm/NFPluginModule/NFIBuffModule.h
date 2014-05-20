// -------------------------------------------------------------------------
//    @FileName         ：    NFIBuffModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIBuffModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_BUFF_MODULE_H_
#define _NFI_BUFF_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFIBuffConfigModule.h"


class NFIBuffModule
    : public NFILogicModule
{

public:

    virtual bool AddBuff(const NFIDENTID& self, const std::string& strConfig, const NFIDENTID& releaserIdent) = 0;

    //删除
    virtual bool RemoveBuff(const NFIDENTID& self, const std::string& strConfig) = 0;

    //删除某类BUFF
    virtual bool RemoveBuff(const NFIDENTID& self, NFIBuffConfigModule::BuffType eType) = 0;

    //瞬间删除所有的控制类BUFF
    virtual bool SelfFree(const NFIDENTID& self) = 0;

    //删除影响某个属性的某类BUFF
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty) = 0;
    //是否有这个BUFF
    virtual bool HasBuff(const NFIDENTID& self, const std::string& strConfig) = 0;

    //更新
    virtual bool Execute(const NFIDENTID& self, float fPassTime) = 0;

    //攻击时
    virtual int Attack(const NFIDENTID& self, const NFIDENTID& other) = 0;

    //死亡时
    virtual int OnDead(const NFIDENTID& self) = 0;

};

#endif