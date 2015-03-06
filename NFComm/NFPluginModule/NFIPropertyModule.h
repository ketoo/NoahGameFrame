// -------------------------------------------------------------------------
//    @FileName         ��    NFIPropertyModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIPropertyModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_MODULE_H_
#define _NFI_PROPERTY_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIPropertyModule
    : public NFILogicModule
{
public:
    enum NFPropertyGroup
    {
        NPG_JOBLEVEL        ,//ְҵ�ȼ�����
        NPG_EFFECTVALUE     ,//һ������Ӱ��������ԵĲ���(ֱ���ù�ʽ���㣬�������ֲ��ö�)
        NPG_REBIRTH_ADD     ,//��������
        NPG_EQUIP           ,//װ������Ӱ������֮��ĵ���2�μ���(ֱ�Ӳ�Ҫ��������ʲô��)
        NPG_EQUIP_AWARD     ,//��װ����
        NPG_STATIC_BUFF     ,//�������͵�BUFF���츳֮��ģ����߳Ե�����ҩˮ

        NPG_RUNTIME_BUFF    ,//��̬BUFF

        NPG_ALL ,
    };


    virtual int RefreshBaseProperty(const NFIDENTID& self) = 0;
    virtual int GetPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType) = 0;
    virtual int SetPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int AddPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int SubPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;

    virtual bool FullHPMP(const NFIDENTID& self) = 0;
    virtual bool AddHP(const NFIDENTID& self, int nValue)  = 0;
    virtual bool ConsumeHP(const NFIDENTID& self, int nValue) = 0;

    virtual bool AddMP(const NFIDENTID& self, int nValue) = 0;
    virtual bool ConsumeMP(const NFIDENTID& self, int nValue) = 0;

    virtual bool ConsumeSP(const NFIDENTID& self, int nValue) = 0;
    virtual bool FullSP(const NFIDENTID& self) = 0;
    virtual bool AddSP(const NFIDENTID& self, int nValue) = 0;

    virtual bool ConsumeMoney(const NFIDENTID& self, int nValue) = 0;
    virtual bool AddMoney(const NFIDENTID& self, int nValue) = 0;

};

#endif