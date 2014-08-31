// -------------------------------------------------------------------------
//    @FileName      :   NFISkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFISkillConsumeProcessModule
//    @Desc             :   �������ѻ�����
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
    //��Ʒʹ���Ƿ�Ϸ�
    virtual int ConsumeLegal(const NFIDENTID& self, const std::string& skillID,  const NFIDataList& other) = 0;

    //ʹ����Ʒ������
    virtual int ConsumeSelf(const NFIDENTID& self, const std::string& skillID) = 0;

    //���صļ��ܽ��ֵ(�˺�)
    virtual int ConsumeProcess(const NFIDENTID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList) = 0;

    virtual int ConsumeProcessEx(const NFIDENTID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList) = 0;


};

#endif