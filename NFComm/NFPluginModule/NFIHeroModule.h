// -------------------------------------------------------------------------
//    @FileName         ��    NFIHeroModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIHeroModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_HERO_MODULE_H
#define _NFI_HERO_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIHeroModule
    : public NFILogicModule
{
public:

    //  virtual NFGUID AddHero(const NFGUID& self, const std::string& strHeroCfgID) = 0;
    //  virtual NFGUID GetHero(const NFGUID& self, const std::string& strHeroCfgID) = 0;
    //  virtual bool DelHero(const NFGUID& self, const NFGUID xID) = 0;

    virtual bool AddHero(const NFGUID& self, const std::string& strID) = 0;
    virtual bool SetFightHero(const NFGUID& self, const NFGUID& xID) = 0;

};

#endif