// -------------------------------------------------------------------------
//    @FileName         ��    NFIBuffModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIBuffModule
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

    //ɾ��
    virtual bool RemoveBuff(const NFIDENTID& self, const std::string& strConfig) = 0;

    //ɾ��ĳ��BUFF
    virtual bool RemoveBuff(const NFIDENTID& self, NFIBuffConfigModule::BuffType eType) = 0;

    //˲��ɾ�����еĿ�����BUFF
    virtual bool SelfFree(const NFIDENTID& self) = 0;

    //ɾ��Ӱ��ĳ�����Ե�ĳ��BUFF
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty) = 0;
    //�Ƿ������BUFF
    virtual bool HasBuff(const NFIDENTID& self, const std::string& strConfig) = 0;

    //����
    virtual bool Execute(const NFIDENTID& self, float fPassTime) = 0;

    //����ʱ
    virtual int Attack(const NFIDENTID& self, const NFIDENTID& other) = 0;

    //����ʱ
    virtual int OnDead(const NFIDENTID& self) = 0;

};

#endif