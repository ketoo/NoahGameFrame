// -------------------------------------------------------------------------
//    @FileName         :    NFIPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPlugin
//
// -------------------------------------------------------------------------

#ifndef _NFI_PLUGIN_H_
#define _NFI_PLUGIN_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFIActor.h"
#include "NFIPluginManager.h"

#define REGISTER_MODULE(pManager, className)  NFILogicModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    pManager->AddModule( (#className), pRegisterModule##className );AddElement( (#className), pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, className) NFILogicModule* pUnRegisterModule##className =  \
    dynamic_cast<NFILogicModule*>( pManager->FindModule( (#className) ) ); pManager->RemoveModule( (#className) ); delete pUnRegisterModule##className;

#define CREATE_PLUGIN(pManager, className)  NFIPlugin* pCreatePlugin##className= new className(pManager); pManager->Registered( pCreatePlugin##className );

#define DESTROY_PLUGIN(pManager, className) pManager->UnsRegistered( pManager->FindPlugin((#className)) );

#define GET_PLUGIN_NAME(className) return (#className);

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
        fLastTotal = 0.0f;

        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->Init();

            pModule = Next();
        }
        return true;
    }

    virtual bool AfterInit()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->AfterInit();
            pModule->OnCheckConfig();

            pModule = Next();
        }
        return true;
    }

    virtual bool Execute(const float fLastFrametime, const float fStartedTime)
    {

#ifdef NF_DYNAMIC_PLUGIN
        //主插件，时刻updata
        //次要插件，100毫秒一次updata
        if(!pPluginManager->GetActor() == NFIActorManager::EACTOR_MAIN)
        {
#endif
            if (fLastTotal < 0.1f)
            {
                fLastTotal += fLastFrametime;
                return false;
            }

#ifdef NF_DYNAMIC_PLUGIN
        }
#endif

        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->Execute(fLastTotal, fStartedTime);

            pModule = Next();
        }

        fLastTotal = 0.0f;

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
    float fLastTotal;
};

#endif