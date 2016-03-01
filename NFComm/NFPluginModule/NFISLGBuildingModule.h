// -------------------------------------------------------------------------
//    @FileName         £º    NFISLGBuildingModule.h
//    @Author           £º    eliteYang
//    @Date             £º    2014-09-12
//    @Module           £º    NFISLGBuildingModule
//
// -------------------------------------------------------------------------

#ifndef __NFI_SLG_BUILDING_MODULE_H_
#define __NFI_SLG_BUILDING_MODULE_H_

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
        BUILDINNG_STATESTARTTIME,
        BUILDINNG_STATEENDTIME  ,
    };

    virtual int AddBuilding(const NFGUID& self, const std::string& strBuilding, const float fX, const float fY, const float fZ) = 0;
    virtual int Upgrade(const NFGUID& self, const NFGUID& xBuilID) = 0;
    virtual int Boost(const NFGUID& self, const NFGUID& xBuilID) = 0;
    virtual int Produce(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, const int nCount) = 0;
    virtual int Move(const NFGUID& self, const NFGUID nGUID, const float fX, const float fY, const float fZ) = 0;
};

#endif // !__NFI_SLG_BUILDING_MODULE_H_