// -------------------------------------------------------------------------
//    @FileName         £º    NFCSLGBuildingModule.h
//    @Author           £º    eliteYang
//    @Date             £º    2014-09-12
//    @Module           £º    NFCSLGBuildingModule
//
// -------------------------------------------------------------------------
#ifndef NFC_SLG_BUILDING_MODULE_H
#define NFC_SLG_BUILDING_MODULE_H

#include "NFComm/NFPluginModule/NFISLGBuildingModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
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
    virtual bool Execute();

public:
    virtual int AddBuilding(const NFGUID& self, const std::string& strBuilding, const float fX, const float fY, const float fZ);

    virtual int Upgrade(const NFGUID& self, const NFGUID& xBuilID);
    virtual int Boost(const NFGUID& self, const NFGUID& xBuilID);

    virtual int Produce(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, const int nCount);
    virtual int Move(const NFGUID& self, const NFGUID nGUID, const float fX, const float fY, const float fZ);

protected:    
    int OnUpgradeHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount); 
    int OnBoostHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);   
    int OnProduceHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);   

    int CheckBuildingStatusEnd(const NFGUID& self);
    int OnClassObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    int CheckProduceData(const NFGUID& self);
    int AddProduceData(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, const int nCount);
    bool GetProduceDataCount(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, int& nCount);
    int CostProduceData(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, const int nCount);

    std::string GetProduceHeartName(const NFGUID& self, const NFGUID& xBuild, const std::string& strItemID);

private:
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFIUUIDModule* m_pUUIDModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif // !__NFC_SLG_BUILDING_MODULE_H_