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


#ifndef NFI_PLUGIN_H
#define NFI_PLUGIN_H

#include <iostream>
#include <assert.h>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#define REGISTER_MODULE(pManager, classBaseName, className)  \
	assert((TIsDerived<classBaseName, NFIModule>::Result));	\
	assert((TIsDerived<className, classBaseName>::Result));	\
	NFIModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    pManager->AddModule( #classBaseName, pRegisterModule##className );\
    AddElement( #classBaseName, pRegisterModule##className );

#define REGISTER_TEST_MODULE(pManager, classBaseName, className)  \
	assert((TIsDerived<classBaseName, NFIModule>::Result));	\
	assert((TIsDerived<className, NFIModule>::Result));	\
	NFIModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    pManager->AddTestModule( #classBaseName, pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, classBaseName, className) \
    NFIModule* pUnRegisterModule##className = dynamic_cast<NFIModule*>( pManager->FindModule( #classBaseName )); \
	pManager->RemoveModule( #classBaseName ); \
    RemoveElement( #classBaseName ); \
    delete pUnRegisterModule##className;

#define UNREGISTER_TEST_MODULE(pManager, classBaseName, className) \
    NFIModule* pUnRegisterModule##className = dynamic_cast<NFIModule*>( pManager->FindtESTModule( #classBaseName )); \
	pManager->RemoveTestModule( #classBaseName ); \
    delete pUnRegisterModule##className;

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
    virtual ~NFIPlugin()
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
			pPluginManager->SetCurrenModule(pModule);

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
			pPluginManager->SetCurrenModule(pModule);
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
			pPluginManager->SetCurrenModule(pModule);
            bool bRet = pModule->AfterInit();
            if (!bRet)
            {
				std::cout << pModule->strName << std::endl;
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
			pPluginManager->SetCurrenModule(pModule);
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
			pPluginManager->SetCurrenModule(pModule);
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
			pPluginManager->SetCurrenModule(pModule);
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
			pPluginManager->SetCurrenModule(pModule);
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
			pPluginManager->SetCurrenModule(pModule);
            pModule->Shut();

            pModule = Next();
        }

        return true;
    }

    virtual bool Finalize()
    {
        NFIModule* pModule = First();
        while (pModule)
        {
			pPluginManager->SetCurrenModule(pModule);
            pModule->Finalize();

            pModule = Next();
        }

        return true;
    }

	virtual bool OnReloadPlugin()
	{
		NFIModule* pModule = First();
		while (pModule)
		{
			pPluginManager->SetCurrenModule(pModule);
			pModule->OnReloadPlugin();

			pModule = Next();
		}

		return true;
	}
    virtual void Uninstall() = 0;
};

#endif
