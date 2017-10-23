// -------------------------------------------------------------------------
//    @FileName			:    NFIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIModule
//
// -------------------------------------------------------------------------

#ifndef NFI_MODULE_H
#define NFI_MODULE_H

#include <string>
#include "NFIPluginManager.h"

class NFIModule
{

public:
    NFIModule()
    {
    }

    virtual ~NFIModule() {}

    virtual bool Awake()
    {
        return true;
    }

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

    virtual bool ReadyExecute()
    {
        return true;
    }

    virtual bool Execute()
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

    virtual bool Finalize()
    {
        return true;
    }

	virtual bool OnReloadPlugin()
	{
		return true;
	}

    virtual NFIPluginManager* GetPluginManager() const
    {
        return pPluginManager;
    }

	virtual void StartCoroutine()
	{
		pPluginManager->StartCoroutine();
	}

    virtual void StartCoroutine(CoroutineFunction func)
    {
        pPluginManager->StartCoroutine(func);
    }

	virtual void YieldCo()
	{
        pPluginManager->YieldCo();
    }

    std::string strName;

protected:
	NFIPluginManager* pPluginManager = NULL;
};
#endif
