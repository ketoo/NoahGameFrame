/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#ifndef NF_BUFF_MODULE_H
#define NF_BUFF_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIBuffModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFBuffModule
    : public NFIBuffModule
{
public:
    enum RUNTIME_BUFF_INFO
    {
        RTBI_BUFFCONFIG,//string
        RTBI_RELEASER,//object
        RTBI_INTERVALTIME,//float
        RTBI_REMAINTIME,//int
        RTBI_EFFECTVALUE,//string
    };

    NFBuffModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFBuffModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();


    
    virtual bool AddBuff( const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent );

    
    virtual bool RemoveBuff( const NFGUID& self, const std::string& strConfig );

    
    virtual bool RemoveBuff( const NFGUID& self, NFIBuffConfigModule::BuffType eType );

    
    virtual bool SelfFree( const NFGUID& self );

    
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty);
    
    virtual bool HasBuff( const NFGUID& self, const std::string& strConfig );

    virtual int Attack( const NFGUID& self, const NFGUID& other );

    
    virtual int OnDead( const NFGUID& self );

protected:
    
    virtual bool ApplyRoleEffectValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    ///////////////////////////////////////////////////////////////////////////

    
    virtual int ReverseBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig );
    
    virtual int ReverseBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig );
    //////////////////////////////////////////////////////////////////////////

    
    virtual int ProcessDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessDeBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessDeBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ProcessDeBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    
    virtual int ProcessDeBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    
    virtual int ReverseDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig );
    
    virtual int ReverseDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig );

    
    virtual int ReverseControlProperty( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig );
    
    virtual int ReverseControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFBuffConfig* pBuffConfig );

    
    virtual std::string GetGroupBuffIndex( const NFGUID& self, int nGroupID );

    
    virtual void SetBuffTime( const NFGUID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime );

private:

    NFIKernelModule* m_pKernelModule;
    NFIBuffConfigModule* m_pBuffConfigModule;
};


#endif
