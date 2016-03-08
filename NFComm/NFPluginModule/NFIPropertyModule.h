// -------------------------------------------------------------------------
//    @FileName         ��    NFIPropertyModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIPropertyModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTY_MODULE_H
#define _NFI_PROPERTY_MODULE_H

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


    virtual int RefreshBaseProperty(const NFGUID& self) = 0;
    virtual int GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType) = 0;
    virtual int SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int AddPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int SubPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;

    virtual bool FullHPMP(const NFGUID& self) = 0;
    virtual bool AddHP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeHP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughHP(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool AddMP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeMP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughMP(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool FullSP(const NFGUID& self) = 0;
    virtual bool AddSP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeSP(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughSP(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool AddMoney(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeMoney(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughMoney(const NFGUID& self, const NFINT64& nValue) = 0;

    virtual bool AddDiamond(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool ConsumeDiamond(const NFGUID& self, const NFINT64& nValue) = 0;
    virtual bool EnoughDiamond(const NFGUID& self, const NFINT64& nValue) = 0;
};

#endif