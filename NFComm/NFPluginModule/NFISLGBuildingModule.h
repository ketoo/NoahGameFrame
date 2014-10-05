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
    virtual int AddBuilding(const NFIDENTID& self, const std::string& strBuilding) = 0;
};

#endif // !__NFI_SLG_BUILDING_MODULE_H__