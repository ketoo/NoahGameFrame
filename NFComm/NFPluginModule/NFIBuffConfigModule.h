// -------------------------------------------------------------------------
//    @FileName         ��    NFIBuffConfigModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2013-09-15
//    @Module           ��    NFIBuffConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_BUFFCONFIG_MODULE_H_
#define _NFI_BUFFCONFIG_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIBuffConfigModule
    : public NFILogicModule
{

public:
    //������
    enum BuffType
    {
        BT_BUFF,
        BT_DEBUFF,
    };

    //������
    enum BuffSubType
    {
        //�޸�ֵ���Ե�[BUFFΪ������ֵ��DEBUFFΪ��ֵ]
        BST_FIX_VALUE_PROPERTY,
        //�޸Ŀ������Ե�[���ߵȿ���,BUFFΪ������ֵ��DEBUFFΪ��ֵ��һ�㲻������DEBUFF���Ϳ���]
        BST_FIX_CONTROL_PROPERTY,
        //һ��ʱ���ڹ�������(BUFFΪ��������������DEBUFFΪ����������)
        BST_FIX_ATK_COUNT,
    };

    //��������[����ֵ���㣬�ٷֱȼ���]
    enum BuffEffectValueType
    {
        EVT_ABSOLUTEVALUE,
        EVT_RATIO,
    };

    //��ԭ����[����ԭ�Ļ�������HP MP֮��ģ���������Ҫ��ԭ;����Ҫ��ԭ����Щ���ԣ������ǹ���ֵ�����統ǰѪ����Щ��������Ѫ����Щ�������Ҫ��ԭ]
    enum BuffReverseType
    {
        ERT_NEED_REVERSE,
        ERT_NO_REVERSE,
    };

    //�����Ƿ����
    enum BuffEffectClearOnDead
    {
        ECOD_SAVE,
        ECOD_CLEAR,
    };

    //�����Ƿ񱣴�
    enum BuffDownSaveType
    {
        DST_NOSAVE,
        DST_SAVE,
    };

    class NFCBuffConfig
        : public NFMap<std::string, int>//������[property->value]
    {
    public:
        std::string strConfig;

        BuffType BuffTypeValue;

        BuffSubType BuffSubTypeValue;

        //Buff�飬ÿ������ֻ��һ��ͬ����ŵ�BUFF
        int BuffGroupIDValue;

        //Ӱ��ֵ���� �ٷֱ� ����ֵ--����ǿ������Կ��أ���ֻ���Ǿ���ֵ ------��Ӱ�����Ա����Ӧ[����Ƿ���Ҫ�أ����ڰٷֱȺ;���ֵ���Ե����,�Ȳ���]
        BuffEffectValueType EffectValueTypeValue;

        //Ӱ��ֵ�ο�����[�ǻ���ֵ�أ�����װ����ֵ�������е�ֵ���������Էֲ�]
        //EffectValueReferType EffectValueReferTypeValue;
        int EffectValueReferTypeValue;

        //�ܹ���������
        int EffectTimeValue;

        //ÿ�μ��ʱ��0.5f����
        float EffectTimeInterval;

        //��ԭ����
        BuffReverseType NeedReverseType;

        //�����Ƿ����
        BuffEffectClearOnDead EffectClearOnDeadValue;

        //���߱������
        BuffDownSaveType DownSaveTypeValue;
    };

    //buffConfigManager����ҪĿ������Ϊbuff����Ŀ�����������Ϊ��Щ�����Ǹ����������ݣ����ÿ�β��������㶼����Ļ�̫�ģ����������һ������
    virtual NFCBuffConfig* GetBuffConfig(const std::string& strConfig) = 0;

};

#endif