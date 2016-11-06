// -------------------------------------------------------------------------
//    @FileName			:    NFIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOGIC_MODULE_H
#define NFI_LOGIC_MODULE_H

#include <string>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFCore/NFIRecord.h"

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
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType*)NULL))),
    };
};

class NFIPluginManager;

class NFIModule
{

public:
    NFIModule()
    {
        mbReloading = false;
    }

    virtual ~NFIModule() {}

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

    virtual bool StartReLoadState()
    {
        mbReloading = true;
        return true;
    }

    virtual bool EndReLoadState()
    {
        mbReloading = false;
        return true;
    }

    virtual NFIPluginManager* GetPluginManager() const
    {
        return pPluginManager;
    }

	bool Loading() const
    {
        return mbReloading;
    }
public:
    std::string strName;


protected:
    NFIPluginManager* pPluginManager;


private:

    bool  mbReloading;
};
#endif
