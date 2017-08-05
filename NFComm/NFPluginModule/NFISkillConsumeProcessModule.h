// -------------------------------------------------------------------------
//    @FileName			:   NFISkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFISkillConsumeProcessModule
// -------------------------------------------------------------------------

#ifndef NFI_SKILLCONSUME_PROCESS_MODULE_H
#define NFI_SKILLCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFISkillConsumeProcessModule
    : public NFIModule
{
public:

    virtual int ConsumeLegal(const NFGUID& self, const std::string& skillID,  const NFDataList& other) = 0;
    virtual int ConsumeProcess(const NFGUID& self, const std::string& strSkillName, const NFDataList& other, NFDataList& damageListValue, NFDataList& damageResultList) = 0;
};

#endif