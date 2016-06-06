// -------------------------------------------------------------------------
//    @FileName         :	NFIHeroModule.h
//    @Author           :	Lvsheng.Huang
//    @Date             :	2012-12-15
//    @Module           :	NFIHeroModule
// -------------------------------------------------------------------------
#ifndef NFI_HERO_MODULE_H
#define NFI_HERO_MODULE_H


#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIHeroModule
    : public NFILogicModule
{
public:
    enum EConstDefine_Hero
    {
        ECONSTDEFINE_HERO_MAXLEVEL = 100,
        ECONSTDEFINE_HERO_MAXSTAR = 100,
		ECONSTDEFINE_HERO_ONCELEVEEXP = 100,
		ECONSTDEFINE_HERO_MAX_FIGHT_COUNT = 5,

    };
    virtual bool AddHero(const NFGUID& self, const std::string& strID) = 0;
    virtual bool AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int nExp) = 0;
    virtual bool HeroStarUp(const NFGUID& self, const NFGUID& xHeroID) = 0;
    virtual bool HeroSkillUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex) = 0;
	virtual bool HeroTalentUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex) = 0;

	virtual bool HeroWearEquip(const NFGUID& self, const NFGUID& xHeroID, const NFGUID& xEquipID) = 0;
	virtual bool HeroUnEquip(const NFGUID& self, const NFGUID& xHeroID, const NFGUID& xEquipID) = 0;

	virtual bool HeroWearSkill(const NFGUID& self, const NFGUID& xHeroID, const std::string& xEquipID) = 0;
	virtual bool HeroUnSkill(const NFGUID& self, const NFGUID& xHeroID, const std::string& xEquipID) = 0;

	virtual bool SetFightHero(const NFGUID& self, const int nIndex, const NFGUID& xHeroID) = 0;
	virtual bool UnSetFightHero(const NFGUID& self, const int nIndex, const NFGUID& xHeroID) = 0;

};

#endif