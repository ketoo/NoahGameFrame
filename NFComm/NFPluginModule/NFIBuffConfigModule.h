// -------------------------------------------------------------------------
//    @FileName         ：    NFIBuffConfigModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-09-15
//    @Module           ：    NFIBuffConfigModule
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
    //大类型
    enum BuffType
    {
        BT_BUFF,
        BT_DEBUFF,
    };

    //子类型
    enum BuffSubType
    {
        //修改值属性的[BUFF为属性增值，DEBUFF为减值]
        BST_FIX_VALUE_PROPERTY,
        //修改控制属性的[免疫等开光,BUFF为开关增值，DEBUFF为减值，一般不允许配DEBUFF类型开光]
        BST_FIX_CONTROL_PROPERTY,
        //一定时间内攻击次数(BUFF为主动攻击次数，DEBUFF为被攻击次数)
        BST_FIX_ATK_COUNT,
    };

    //计算类型[绝对值计算，百分比计算]
    enum BuffEffectValueType
    {
        EVT_ABSOLUTEVALUE,
        EVT_RATIO,
    };

    //还原类型[不还原的基本就是HP MP之类的，其他基本要求还原;不需要还原的这些属性，必须是过程值，比如当前血量这些，而最终血量这些，则必须要还原]
    enum BuffReverseType
    {
        ERT_NEED_REVERSE,
        ERT_NO_REVERSE,
    };

    //死亡是否清除
    enum BuffEffectClearOnDead
    {
        ECOD_SAVE,
        ECOD_CLEAR,
    };

    //下线是否保存
    enum BuffDownSaveType
    {
        DST_NOSAVE,
        DST_SAVE,
    };

    class NFCBuffConfig
        : public NFMap<std::string, int>//响属性[property->value]
    {
    public:
        std::string strConfig;

        BuffType BuffTypeValue;

        BuffSubType BuffSubTypeValue;

        //Buff组，每个对象只能一个同样组号的BUFF
        int BuffGroupIDValue;

        //影响值类型 百分比 绝对值--如果是控制属性开关，则只能是绝对值 ------和影响属性必须对应[这个是否需要呢，用于百分比和绝对值属性的组合,先不做]
        BuffEffectValueType EffectValueTypeValue;

        //影响值参考类型[是基础值呢，还是装备的值还是所有的值，就是属性分层]
        //EffectValueReferType EffectValueReferTypeValue;
        int EffectValueReferTypeValue;

        //总共产生几次
        int EffectTimeValue;

        //每次间隔时间0.5f倍数
        float EffectTimeInterval;

        //还原类型
        BuffReverseType NeedReverseType;

        //死亡是否清除
        BuffEffectClearOnDead EffectClearOnDeadValue;

        //下线保存类别
        BuffDownSaveType DownSaveTypeValue;
    };

    //buffConfigManager，主要目的是作为buff对象的快速索引，因为有些数据是复合类型数据，如果每次不复发运算都解包的话太耗，因此这里做一个缓存
    virtual NFCBuffConfig* GetBuffConfig(const std::string& strConfig) = 0;

};

#endif