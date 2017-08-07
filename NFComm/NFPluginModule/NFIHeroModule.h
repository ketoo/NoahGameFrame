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
        ECONSTDEFINE_HERO_MAXSTAR = 100,
		ECONSTDEFINE_HERO_ONCELEVEEXP = 100,
		ECONSTDEFINE_HERO_MAXFIGHT_POS = 3,
    };

    virtual NFGUID AddHero(const NFGUID& self, const std::string& strID) = 0;
	virtual NFGUID ActiviteHero(const NFGUID& self, const string& strID) = 0;
	virtual bool ActiviteHero(const NFGUID& self, const NFGUID& hero) = 0;
    virtual bool AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int64_t nExp) = 0;
    virtual bool HeroStarUp(const NFGUID& self, const NFGUID& xHeroID) = 0;
    virtual bool HeroSkillUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex) = 0;
	virtual bool HeroTalentUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex) = 0;

	virtual bool HeroWearSkill(const NFGUID& self, const NFGUID& xHeroID, const std::string& xEquipID) = 0;
	virtual bool SetFightHero(const NFGUID& self, const NFGUID& xHeroID, const int nPos) = 0;
	virtual bool SwitchFightHero(const NFGUID& self, const NFGUID& xHeroID) = 0;

	virtual bool CreateHero(const NFGUID& self, const NFGUID& xHeroID) = 0;
	virtual bool DestroyHero(const NFGUID& self, const NFGUID& xHeroID) = 0;
	virtual NFGUID GetHeroGUID(const NFGUID& self, const std::string& strID) = 0;

};

#endif