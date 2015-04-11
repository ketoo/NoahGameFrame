// -------------------------------------------------------------------------
//    @FileName      :   NFISkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFISkillConsumeProcessModule
//    @Desc             :   技能消费机制类
// -------------------------------------------------------------------------

#ifndef _NFI_SKILLCONSUME_PROCESS_MODULE_H_
#define _NFI_SKILLCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/Define/NFSkill_def.h"

class NFISkillConsumeProcessModule
    : public NFILogicModule
{

public:
    //物品使用是否合法
    virtual int ConsumeLegal(const NFIDENTID& self, const std::string& skillID,  const NFIDataList& other) = 0;

    //使用物品的消耗
    virtual int ConsumeSelf(const NFIDENTID& self, const std::string& skillID) = 0;

    //返回的技能结果值(伤害)
    virtual int ConsumeProcess(const NFIDENTID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList) = 0;

    virtual int ConsumeProcessEx(const NFIDENTID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList) = 0;


};

#endif