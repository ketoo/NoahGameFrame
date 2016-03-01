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
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#define REGISTER_MODULE(pManager, className)  NFILogicModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    pManager->AddModule( (#className), pRegisterModule##className );AddElement( (#className), pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, className) NFILogicModule* pUnRegisterModule##className =  \
        dynamic_cast<NFILogicModule*>( pManager->FindModule( (#className) ) ); pManager->RemoveModule( (#className) ); RemoveElement( (#className) ); delete pUnRegisterModule##className;

#define CREATE_PLUGIN(pManager, className)  NFIPlugin* pCreatePlugin##className = new className(pManager); pManager->Registered( pCreatePlugin##className );

#define DESTROY_PLUGIN(pManager, className) pManager->UnsRegistered( pManager->FindPlugin((#className)) );

#define GET_CLASS_NAME(className) (#className);


#define REGISTER_COMPONENT(pManager, className)  NFIComponent* pRegisterComponent##className= new className(pManager); \
    pRegisterComponent##className->strName = (#className); \
    pManager->AddComponent( (#className), pRegisterComponent##className );

#define UNREGISTER_COMPONENT(pManager, className) NFIComponent* pRegisterComponent##className =  \
        dynamic_cast<NFIComponent*>( pManager->FindComponent( (#className) ) ); pManager->RemoveComponent( (#className) ); delete pRegisterComponent##className;


class NFIPluginManager;

class NFIPlugin : public NFILogicModule,
    public NFMap<std::string, NFILogicModule>
{

public:

    virtual const int GetPluginVersion() = 0;

    virtual const std::string GetPluginName() = 0;

    virtual void Install() = 0;

    virtual bool Init()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->Init();
            if (!bRet)
            {
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool AfterInit()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->AfterInit();
            if (!bRet)
            {
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool CheckConfig()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->CheckConfig();

            pModule = Next();
        }

        return true;
    }

    virtual bool Execute()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->Execute();

            pModule = Next();
        }

        return true;
    }

    virtual bool BeforeShut()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->BeforeShut();

            pModule = Next();
        }
        return true;
    }

    virtual bool Shut()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->Shut();

            pModule = Next();
        }

        return true;
    }

    virtual void Uninstall() = 0;

protected:
    NFIPluginManager* pPluginManager;
};

#endif