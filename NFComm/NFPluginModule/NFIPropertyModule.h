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

        NPG_PET             ,//��ǰ��ս�ĸ�����������--��ս��ò�ƻ�������ǻ�������
        NPG_ONAGER          ,//ŭ��Ӱ�������

        NPG_MOUNT           ,// ����

        NPG_BIRD_BUFF       ,//��ӥӰ��BUFF

        NPG_RUNTIME_BUFF    ,//��̬BUFF

        NPG_ALL = 100       ,
    };


    virtual int RefreshBaseProperty(const NFIDENTID& self) = 0;
    virtual int GetPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType) = 0;
    virtual int SetPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int AddPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;
    virtual int SubPropertyValue(const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue) = 0;

    virtual int FullHPMP(const NFIDENTID& self) = 0;

    virtual int AddHP(const NFIDENTID& self, int nValue)  = 0;
    virtual int ConsumeHP(const NFIDENTID& self, int nValue) = 0;

    virtual int AddMP(const NFIDENTID& self, int nValue) = 0;
    virtual int ConsumeMP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeSP(const NFIDENTID& self, int nValue) = 0;
    virtual int FullSP(const NFIDENTID& self) = 0;
    virtual int AddSP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeVP(const NFIDENTID& self, int nValue) = 0;
    virtual int FullVP(const NFIDENTID& self) = 0;
    virtual int RestoreVP( const NFIDENTID& self ) = 0;
    virtual int AddVP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeActivityVP(const NFIDENTID& self, int nValue) = 0;
    virtual int FullActivityVP(const NFIDENTID& self) = 0;
    virtual int AddActivityVP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeMoney(const NFIDENTID& self, int nValue) = 0;
    virtual int AddMoney(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeYBP(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeSoulStone(const NFIDENTID& self, int nValue) = 0;
    virtual int AddSoulStone(const NFIDENTID& self, int nValue) = 0;

    virtual int ConsumeEssence(const NFIDENTID& self, int nValue) = 0;
    virtual int AddEssence(const NFIDENTID& self, int nValue) = 0;

	virtual int AddHonour(const NFIDENTID& self, int nValue) = 0;
	virtual int ConsumeHonour(const NFIDENTID& self, int nValue) = 0;
};

#endif