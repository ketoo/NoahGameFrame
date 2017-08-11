// -------------------------------------------------------------------------
//    @FileName			:    NFTutorial6.h
//    @Author           :    bluesky
//    @Date             :    2017-08-07 08:51
//    @Module           :	 NFTutorial6
//
// -------------------------------------------------------------------------

#ifndef NFC_HELLO_WORLD_H
#define NFC_HELLO_WORLD_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINavigationModule.h"

class NFIHelloWorld
	: public NFIModule
{

};

class NFCHelloWorld
    : public NFIHelloWorld
{
public:
    NFCHelloWorld(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	NFINavigationModule* m_pNavigationModule;
};

#endif
