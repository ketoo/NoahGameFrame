/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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