// -------------------------------------------------------------------------
//    @FileName      :    NFILogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogicModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LOGIC_MODULE_H_
#define _NFI_LOGIC_MODULE_H_

#include <string>
#include "NFComm/Define/NFObject_def.h"
#include "NFComm/Define/NFItem_def.h"
#include "NFComm/Define/NFSkill_def.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFDefine.h"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFCore/NFIRecord.h"
#include "NFComm/NFEventDefine/NFEventDefine.h"

class NFIPluginManager;

class NFILogicModule
{

public:
	NFILogicModule()
    {
        bCanReload = true;
    }

	virtual ~NFILogicModule(){}
	
	virtual bool Init()
    {
        
        return true;
    }

    virtual bool AfterInit()
    {
        return true;
    }

    virtual bool CheckConfig()
    {
        return true;
    }

    virtual bool BeforeShut()
    {
        return true;
    }

    virtual bool Shut()
    {
        return true;
    }

    virtual bool Execute(const float fLasFrametime, const float fStartedTime)
    {
        return true;
    }

    virtual void OnReload(const char* strModuleName, NFILogicModule* pModule)
    {
		BeforeShut();
		Shut();

		Init();
		AfterInit();
    }

#ifdef NF_USE_ACTOR
	virtual void Handler(const NFIActorMessage& message, const Theron::Address from){};
#endif

    std::string strName;
    bool  bCanReload;

protected:
    NFIPluginManager* pPluginManager;
};
#endif