// -------------------------------------------------------------------------
//    @FileName         £º    NFISLGBuildingModule.h
//    @Author           £º    eliteYang
//    @Date             £º    2014-09-12
//    @Module           £º    NFISLGBuildingModule
//
// -------------------------------------------------------------------------

#ifndef __NFI_SLG_BUILDING_MODULE_H__
#define __NFI_SLG_BUILDING_MODULE_H__

#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFISLGBuildingModule : public NFILogicModule
{
public:
    enum EBuildingListColType
    {
        BUILDING_CONFIG_ID  = 0 ,
        BUILDING_GUID           ,
        BUILDING_STATE          ,
        BUILDINNG_X             ,
        BUILDINNG_Y             ,
        BUILDINNG_Z             ,
    };

    virtual int AddBuilding(const NFIDENTID& self, const std::string& strBuilding, const float fX, const float fY, const float fZ) = 0;
    virtual int OnUpgrade(const NFIDENTID& self, const NFIDataList& var) = 0;
    virtual int OnBoost(const NFIDENTID& self, const NFIDataList& var) = 0;
    virtual int OnProduce(const NFIDENTID& self, const NFIDataList& var) = 0;
    virtual int OnMove(const NFIDENTID& self, const NFIDataList& var) = 0;
};

#endif // !__NFI_SLG_BUILDING_MODULE_H__