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
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"

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
    virtual int AddBuilding(const NFIDENTID& self, const std::string& strBuilding, const float fX, const float fY, const float fZ);

    virtual int Upgrade(const NFIDENTID& self, const NFIDENTID& xBuilID);
    virtual int Boost(const NFIDENTID& self, const NFIDENTID& xBuilID);

    virtual int Produce(const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, const int nCount);
    virtual int Move(const NFIDENTID& self, const NFIDENTID nGUID, const float fX, const float fY, const float fZ);

protected:    
    int OnUpgradeHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var); 
    int OnBoostHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var);   
    int OnProduceHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var);   

    int CheckBuildingStatusEnd(const NFIDENTID& self);
    int OnClassObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    int CheckProduceData(const NFIDENTID& self);
    int AddProduceData(const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, const int nCount);
    bool GetProduceDataCount(const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, int& nCount);
    int CostProduceData(const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, const int nCount);

    std::string GetProduceHeartName(const NFIDENTID& self, const NFIDENTID& xBuild, const std::string& strItemID);

private:
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFIUUIDModule* m_pUUIDModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif // !__NFC_SLG_BUILDING_MODULE_H__