// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_BUFFCONFIG_MODULE_H
#define NFI_BUFFCONFIG_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIBuffConfigModule
    : public NFIModule
{

public:
    enum BuffType
    {
        BT_BUFF,
        BT_DEBUFF,
    };

    enum BuffSubType
    {
        BST_FIX_VALUE_PROPERTY,
        BST_FIX_CONTROL_PROPERTY,
        BST_FIX_ATK_COUNT,
    };

    enum BuffEffectValueType
    {
        EVT_ABSOLUTEVALUE,
        EVT_RATIO,
    };

    enum BuffReverseType
    {
        ERT_NEED_REVERSE,
        ERT_NO_REVERSE,
    };

    enum BuffEffectClearOnDead
    {
        ECOD_SAVE,
        ECOD_CLEAR,
    };

    enum BuffDownSaveType
    {
        DST_NOSAVE,
        DST_SAVE,
    };

    class NFCBuffConfig : public NFMap<std::string, int>
    {
    public:
        std::string strConfig;

        BuffType BuffTypeValue;

        BuffSubType BuffSubTypeValue;

        int BuffGroupIDValue;

        BuffEffectValueType EffectValueTypeValue;

        //EffectValueReferType EffectValueReferTypeValue;
        int EffectValueReferTypeValue;

        int EffectTimeValue;

        float EffectTimeInterval;

        BuffReverseType NeedReverseType;

        BuffEffectClearOnDead EffectClearOnDeadValue;

        BuffDownSaveType DownSaveTypeValue;
    };

    virtual NFCBuffConfig* GetBuffConfig(const std::string& strConfig) = 0;

};

#endif