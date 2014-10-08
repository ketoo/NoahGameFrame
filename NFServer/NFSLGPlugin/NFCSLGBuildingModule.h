// -------------------------------------------------------------------------
//    @FileName         £º    NFCSLGBuildingModule.h
//    @Author           £º    eliteYang
//    @Date             £º    2014-09-12
//    @Module           £º    NFCSLGBuildingModule
//
// -------------------------------------------------------------------------
#ifndef __NFC_SLG_BUILDING_MODULE_H__
#define __NFC_SLG_BUILDING_MODULE_H__

#include "NFComm/NFPluginModule/NFISLGBuildingModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

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
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
};

#endif // !__NFC_SLG_BUILDING_MODULE_H__