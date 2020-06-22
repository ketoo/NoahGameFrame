/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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


#ifndef NF_PROPERTY_MODULE_H
#define NF_PROPERTY_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFPropertyModule : public NFIPropertyModule
{
public:
    NFPropertyModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual int64_t GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType);
    virtual int SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue);

    virtual bool AddExp(const NFGUID& self, const int64_t nExp);

    virtual bool FullHPMP(const NFGUID& self);
    virtual bool AddHP(const NFGUID& self, const int nValue);
    virtual bool ConsumeHP(const NFGUID& self, const int nValue);
	virtual bool EnoughHP(const NFGUID& self, const int nValue);
	virtual bool DamageHP(const NFGUID& self, const int nValue);

    virtual bool AddMP(const NFGUID& self, const int nValue);
    virtual bool ConsumeMP(const NFGUID& self, const int nValue);
	virtual bool EnoughMP(const NFGUID& self, const int nValue);
	virtual bool DamageMP(const NFGUID& self, const int nValue);

    virtual bool FullSP(const NFGUID& self);
    virtual bool AddSP(const NFGUID& self, const int nValue);
    virtual bool ConsumeSP(const NFGUID& self, const int nValue);
    virtual bool EnoughSP(const NFGUID& self, const int nValue);

    virtual bool AddGold(const NFGUID& self, const int64_t nValue);
    virtual bool ConsumeGold(const NFGUID& self, const int64_t nValue);
    virtual bool EnoughGold(const NFGUID& self, const int64_t nValue);

    virtual bool AddDiamond(const NFGUID& self, const int nValue);
    virtual bool ConsumeDiamond(const NFGUID& self, const int nValue);
    virtual bool EnoughDiamond(const NFGUID& self, const int nValue);

	virtual void ActiveExtraController();

protected:
    void RefreshBaseProperty(const NFGUID& self);

    void RefreshAllProperty(const NFGUID& self);

    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    int OnObjectLevelEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
    int OnObjectConfigIDEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

    int OnRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

private:
	bool activeExtraController = false;

    NFIKernelModule* m_pKernelModule;
    NFIPropertyConfigModule* m_pPropertyConfigModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
};


#endif
