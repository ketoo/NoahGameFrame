// -------------------------------------------------------------------------
//    @FileName         ��    NFCSLGBuildingModule.h
//    @Author           ��    eliteYang
//    @Date             ��    2014-09-12
//    @Module           ��    NFCSLGBuildingModule
//
// -------------------------------------------------------------------------
#ifndef __NFC_SLG_BUILDING_MODULE_H__
#define __NFC_SLG_BUILDING_MODULE_H__

#include "NFISLGBuildingModule.h"

class NFCSLGBuildingModule : public NFISLGBuildingModule
{
public:
    NFCSLGBuildingModule(NFIPluginManager* p);
    virtual ~NFCSLGBuildingModule(){}

    virtual bool Init();
    virtual bool AfterInit();
    virtual bool CheckConfig();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

public:
    virtual int AddBuilding(const NFIDENTID& self, const std::string& strBuilding);
private:

};

#endif // !__NFC_SLG_BUILDING_MODULE_H__