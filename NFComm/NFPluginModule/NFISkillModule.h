// -------------------------------------------------------------------------
//    @FileName         ��    NFISkillModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFISkillModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_SKILL_MODULE_H_
#define _NFI_SKILL_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFISkillModule
    : public NFILogicModule
{

public:

    //��Ӽ���
    virtual int AddSkill(const NFGUID& self, const std::string& strSkillName) = 0;

    //��ǰ�Ƿ���ڼ���
    virtual int ExistSkill(const NFGUID& self, const std::string& strSkillName) = 0;

    //���ü��ܵȼ�
    virtual int SetSkillLevel(const NFGUID& self, const std::string& strSkillName, const int nLevel) = 0;

    //���ü��ܱ�ʯ
    virtual int SetSkillGem(const NFGUID& self, const std::string& strSkillName, const std::string& strGemName) = 0;

    //���ü��ܱ�ʯ�ȼ�
    virtual int SetSkillGemLevel(const NFGUID& self, const std::string& strSkillName, const int nLevel) = 0;

    //�õ����ܵȼ�
    virtual int GetSkillLevel(const NFGUID& self, const std::string& strSkillName) = 0;
    \

    //�õ����ܱ�ʯ
    virtual std::string GetSkillGem(const NFGUID& self, const std::string& strSkillName) = 0;

    //�õ����ܱ�ʯ�ȼ�
    virtual int GetSkillGemLevel(const NFGUID& self, const std::string& strSkillName) = 0;

    //������ּ���
    virtual int AddNewerSkill(const NFGUID& self) = 0;

    virtual int OnUseSkill(const NFGUID& self, const NFIDataList& var) = 0;
};

#endif