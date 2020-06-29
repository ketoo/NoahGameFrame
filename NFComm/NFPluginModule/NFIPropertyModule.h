/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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


#ifndef NFI_PROPERTY_MODULE_H
#define NFI_PROPERTY_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIPropertyModule
    : public NFIModule
{
public:
    enum NFPropertyGroup
    {
		NPG_JOBLEVEL,
		NPG_FIGHTING_HERO,
		NPG_EFFECTVALUE,
		NPG_REBIRTH_ADD,
		NPG_EQUIP,
		NPG_EQUIP_AWARD,
		NPG_STATIC_BUFF,
		NPG_RUNTIME_BUFF,
        NPG_ALL ,
    };

	//返回100以内的百分比数值(比如10，就应该是10%), 用来计算armor值代来代伤害减免，幸运值代来代幸运概率等
	static int Cal(int value)
	{
		float x = value / 10.0f;
		float f = 0.06f * x / (1.0f + (0.06f * x));
		return (int)(f * 100);
	}

	//高斯求和, 返回等级带来代数值结果,比如  + 10等级, 应该就是55,
	static int CalLevelValue(int level)
	{
		int result = 0;
		if (level > 0)
		{
			result = (1 + level) * level / 2;
		}

		return result;
	}

    virtual int64_t GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType) = 0;
    virtual int SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue) = 0;

    virtual bool AddExp(const NFGUID& self, const int64_t nExp) = 0;
    virtual bool FullHPMP(const NFGUID& self) = 0;
    virtual bool AddHP(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeHP(const NFGUID& self, const int nValue) = 0;
    virtual bool EnoughHP(const NFGUID& self, const int nValue) = 0;
	virtual bool DamageHP(const NFGUID& self, const int nValue) = 0;

    virtual bool AddMP(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeMP(const NFGUID& self, const int nValue) = 0;
    virtual bool EnoughMP(const NFGUID& self, const int nValue) = 0;
	virtual bool DamageMP(const NFGUID& self, const int nValue) = 0;

    virtual bool FullSP(const NFGUID& self) = 0;
    virtual bool AddSP(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeSP(const NFGUID& self, const int nValue) = 0;
    virtual bool EnoughSP(const NFGUID& self, const int nValue) = 0;

    virtual bool AddGold(const NFGUID& self, const int64_t nValue) = 0;
    virtual bool ConsumeGold(const NFGUID& self, const int64_t nValue) = 0;
    virtual bool EnoughGold(const NFGUID& self, const int64_t nValue) = 0;

    virtual bool AddDiamond(const NFGUID& self, const int nValue) = 0;
    virtual bool ConsumeDiamond(const NFGUID& self, const int nValue) = 0;
	virtual bool EnoughDiamond(const NFGUID& self, const int nValue) = 0;

	virtual void ActiveExtraController() = 0;
};

#endif