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

class NFIPluginManager;

typedef void (* CoroutineFunction)(void* arg);

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
        NFIModule* p = (NFIModule*)pPluginManager;
        p->StartCoroutine();
    }

    virtual void StartCoroutine(CoroutineFunction func)
    {
        NFIModule* p = (NFIModule*)pPluginManager;
        p->StartCoroutine(func);
    }

	virtual void Yield(){
        NFIModule* p = (NFIModule*)pPluginManager;
        p->Yield();
    }

    std::string strName;

protected:
	NFIPluginManager* pPluginManager = NULL;
};
#endif
