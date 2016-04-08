// -------------------------------------------------------------------------
//    @FileName      :   NFISkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFISkillConsumeProcessModule
//    @Desc             :   技能消费机制类
// -------------------------------------------------------------------------

#ifndef _NFI_SKILLCONSUME_PROCESS_MODULE_H
#define _NFI_SKILLCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFISkillConsumeProcessModule
    : public NFILogicModule
{

public:
    //物品使用是否合法
    virtual int ConsumeLegal(const NFGUID& self, const std::string& skillID,  const NFIDataList& other) = 0;

    //使用物品的消耗
    virtual int ConsumeSelf(const NFGUID& self, const std::string& skillID) = 0;

    //返回的技能结果值(伤害)
    virtual int ConsumeProcess(const NFGUID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList) = 0;

    virtual int ConsumeProcessEx(const NFGUID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList) = 0;


};

#endif