// -------------------------------------------------------------------------
//    @FileName         :	NFIHeroModule.h
//    @Author           :	Lvsheng.Huang
//    @Date             :	2012-12-15
//    @Module           :	NFIHeroModule
// -------------------------------------------------------------------------
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
		ECONSt_HERO_MAX = 4,
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