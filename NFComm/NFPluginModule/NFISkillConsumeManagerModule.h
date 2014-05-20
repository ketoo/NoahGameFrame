// -------------------------------------------------------------------------
//    @FileName      :   NFISkillConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFISkillConsumeManagerModule
//    @Desc             :   �������ѻ��ƹ�����,�������͵ļ����������;���ע���������
// -------------------------------------------------------------------------

#ifndef _NFI_SKILLCONSUME_MANAGER_MODULE_H_
#define _NFI_SKILLCONSUME_MANAGER_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFISkillConsumeProcessModule.h"

class NFISkillConsumeManagerModule
    : public NFILogicModule
{
public:
    virtual bool ResgisterConsumeModule(const int nModuleType, NFISkillConsumeProcessModule* pModule) = 0;

    virtual NFISkillConsumeProcessModule* GetConsumeModule(const int nModuleType) = 0;

};

#endif