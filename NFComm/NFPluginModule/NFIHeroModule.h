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

#ifndef NFI_HERO_MODULE_H
#define NFI_HERO_MODULE_H


#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"

class NFIHeroModule
    : public NFIModule
{
public:
    enum EConstDefine_Hero
    {
        ECONSTDEFINE_HERO_MAXLEVEL = 100,
		ECONSTDEFINE_HERO_ONCELEVEEXP = 100,
		ECONSTDEFINE_HERO_MAX_STAR = 20,
    };
	
	enum EConsHero_Pos
	{
		ECONSt_HERO_UNKNOW = 0,
		ECONSt_HERO_POS1 = 1,
		ECONSt_HERO_POS2 = 2,
		ECONSt_HERO_POS3 = 3,
		ECONSt_HERO_POS4 = 4,
		ECONSt_HERO_MAX = 5,
	};

    virtual NFGUID AddHero(const NFGUID& self, const std::string& strID) = 0;
	virtual bool AddHeroExp(const NFGUID& self, const int64_t nExp) = 0;
    virtual bool AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int64_t nExp) = 0;

	virtual bool SetFightHero(const NFGUID& self, const NFGUID& xHeroID, const EConsHero_Pos nPos) = 0;
	virtual bool SwitchFightHero(const NFGUID& self, const NFGUID& xHeroID) = 0;

	virtual NFGUID GetHeroGUID(const NFGUID& self, const std::string& strID) = 0;

	virtual EConsHero_Pos GetFightPos(const NFGUID& self, const NFGUID& xHeroID) = 0;
};

#endif