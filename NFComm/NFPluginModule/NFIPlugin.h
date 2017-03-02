// -------------------------------------------------------------------------
//    @FileName         :    NFIPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPlugin
//
// -------------------------------------------------------------------------

#ifndef NFI_PLUGIN_H
#define NFI_PLUGIN_H

#include <iostream>
#include <assert.h>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#define REGISTER_MODULE(pManager, classBaseName, className)  \
	assert((TIsDerived<classBaseName, NFIModule>::Result));	\
	assert((TIsDerived<className, classBaseName>::Result));	\
	NFIModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    pManager->AddModule( #classBaseName, pRegisterModule##className );AddElement( #classBaseName, pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, classBaseName, className) NFIModule* pUnRegisterModule##className =  \
	dynamic_cast<NFIModule*>( pManager->FindModule( #classBaseName )); \
	pManager->RemoveModule( #classBaseName ); RemoveElement( #classBaseName ); delete pUnRegisterModule##className;


#define CREATE_PLUGIN(pManager, className)  NFIPlugin* pCreatePlugin##className = new className(pManager); pManager->Registered( pCreatePlugin##className );

#define DESTROY_PLUGIN(pManager, className) pManager->UnRegistered( pManager->FindPlugin((#className)) );

/*
#define REGISTER_COMPONENT(pManager, className)  NFIComponent* pRegisterComponent##className= new className(pManager); \
    pRegisterComponent##className->strName = (#className); \
    pManager->AddComponent( (#className), pRegisterComponent##className );

#define UNREGISTER_COMPONENT(pManager, className) NFIComponent* pRegisterComponent##className =  \
        dynamic_cast<NFIComponent*>( pManager->FindComponent( (#className) ) ); pManager->RemoveComponent( (#className) ); delete pRegisterComponent##className;
*/

class NFIPluginManager;

class NFIPlugin : public NFIModule,
    public NFMap<std::string, NFIModule>
{

public:
	NFIPlugin()
	{
	}

    virtual const int GetPluginVersion() = 0;

    virtual const std::string GetPluginName() = 0;

    virtual void Install() = 0;

	virtual bool Awake()
	{
		NFIModule* pModule = First();
		while (pModule)
		{
			bool bRet = pModule->Awake();
			if (!bRet)
			{
				std::cout << pModule->strName << std::endl;
				assert(0);
			}

			pModule = Next();
		}
		return true;
	}

    virtual bool Init()
    {
        NFIModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->Init();
            if (!bRet)
            {
				std::cout << pModule->strName << std::endl;
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool AfterInit()
    {
        NFIModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->AfterInit();
            if (!bRet)
            {
				std::cout << pModule ->strName << std::endl;
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool CheckConfig()
    {
        NFIModule* pModule = First();
        while (pModule)
        {
            pModule->CheckConfig();

            pModule = Next();
        }

        return true;
    }

	virtual bool ReadyExecute()
	{
		NFIModule* pModule = First();
		while (pModule)
		{
			pModule->ReadyExecute();

			pModule = Next();
		}

		return true;
	}

    virtual bool Execute()
    {
        NFIModule* pModule = First();
        while (pModule)
        {
            pModule->Execute();

            pModule = Next();
        }

        return true;
    }

    virtual bool BeforeShut()
    {
        NFIModule* pModule = First();
        while (pModule)
        {
            pModule->BeforeShut();

            pModule = Next();
        }
        return true;
    }

    virtual bool Shut()
    {
        NFIModule* pModule = First();
        while (pModule)
        {
            pModule->Shut();

            pModule = Next();
        }

        return true;
    }

	virtual bool OnReloadPlugin()
	{
		NFIModule* pModule = First();
		while (pModule)
		{
			pModule->OnReloadPlugin();

			pModule = Next();
		}

		return true;
	}
    virtual void Uninstall() = 0;
};

#endif
