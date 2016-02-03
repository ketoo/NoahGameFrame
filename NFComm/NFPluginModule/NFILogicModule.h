// -------------------------------------------------------------------------
//    @FileName      :    NFILogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogicModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LOGIC_MODULE_H
#define _NFI_LOGIC_MODULE_H

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

template<typename DerivedType, typename BaseType> 
class TIsDerived
{
public:
	static int AnyFunction(BaseType* base)
	{
		return 1;
	}

	static  char AnyFunction(void* t2)
	{
		return 0;
	}

	enum 
	{
		Result = ( sizeof(int) == sizeof(AnyFunction( (DerivedType*)NULL) ) ), 
	};

};
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

    virtual bool Execute()
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

    virtual NFIPluginManager* GetPluginManager() const
    {
        return pPluginManager;
    }


    std::string strName;
    bool  bCanReload;

protected:
    NFIPluginManager* pPluginManager;
};
#endif