/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NFC_SLG_BUILDING_MODULE_H
#define NFC_SLG_BUILDING_MODULE_H

#include "NFComm/NFPluginModule/NFISLGBuildingModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

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
    int OnClassObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    int CheckProduceData(const NFGUID& self);
    int AddProduceData(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, const int nCount);
    bool GetProduceDataCount(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, int& nCount);
    int CostProduceData(const NFGUID& self, const NFGUID& xBuilID, const std::string& strItemID, const int nCount);

    std::string GetProduceHeartName(const NFGUID& self, const NFGUID& xBuild, const std::string& strItemID);

protected:
	void OnSLGClienMoveObject(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSLGClienUpgradeBuilding(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSLGClienCreateItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIScheduleModule* m_pScheduleModule;
	NFIEventModule* m_pEventModule;
};

#endif // !__NFC_SLG_BUILDING_MODULE_H_